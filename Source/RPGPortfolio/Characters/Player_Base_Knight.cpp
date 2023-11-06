// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Base_Knight.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../CharacterAnim/AnimInstance_Knight.h"

// Sets default values
APlayer_Base_Knight::APlayer_Base_Knight()
	: bEnableJump(true)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	m_Arm = CreateDefaultSubobject<UPlayer_CameraArm>(TEXT("SpringArm"));
	m_Arm->SetupAttachment(GetRootComponent());
	m_Arm->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Camera->SetupAttachment(m_Arm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	m_Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	/*ConstructorHelpers::FObjectFinder<USkeletalMesh> Skel_Mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/GKnight/Meshes/SK_GothicKnight_VA.SK_GothicKnight_VA'"));
	if (Skel_Mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(Skel_Mesh.Object);
	}*/

	AccTime = 0.f;
}

// Called when the game starts or when spawned
void APlayer_Base_Knight::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* pController = Cast<APlayerController>(GetController());

	if (pController)
	{
		ULocalPlayer* pLocalPlayer = pController->GetLocalPlayer();

		if (pLocalPlayer && !m_IMC.IsNull())
		{
			UEnhancedInputLocalPlayerSubsystem* pSubsystem = pLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			pSubsystem->AddMappingContext(m_IMC.LoadSynchronous(), 0);
		}
	}
}

// Called every frame
void APlayer_Base_Knight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFalling())
	{
		bEnableJump = false;
	}

	if (!GetCharacterMovement()->IsFalling() && !bEnableJump)
	{
		AccTime += DeltaTime;

		if (AccTime > 0.5f)
		{
			bEnableJump = true;
			AccTime = 0.f;
		}
	}
}

// Called to bind functionality to input
void APlayer_Base_Knight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (nullptr == InputComp)
	{
		return;
	}

	if (!m_IA_Setting.IsNull())
	{
		UDA_InputAction* pIADA = m_IA_Setting.LoadSynchronous();
	
		for (int32 i = 0; i < pIADA->IADataArr.Num(); ++i)
		{
			switch (pIADA->IADataArr[i].Type)
			{
			case EInputActionType::MOVE:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::MoveAction);
				break;
			case EInputActionType::ROTATION:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::RotateAction);
				break;
			case EInputActionType::JUMP:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::JumpAction);
				break;
			case EInputActionType::SPRINT:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::SprintToggleAction);
				break;
			case EInputActionType::GUARD:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::GuardAction);
				break;

			default:
				break;
			}
		}

	}

}

void APlayer_Base_Knight::MoveAction(const FInputActionInstance& _Instance)
{
	FVector2D vInput = _Instance.GetValue().Get<FVector2D>();

	if (bEnableJump)
	{
		if ((Controller != NULL) && (vInput.X != 0.0f))
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, vInput.X);

			//AddMovementInput(GetActorForwardVector(), vInput.X);
			//GetCharacterMovement()->AddInputVector(GetActorForwardVector() * vInput.X);
		}

		if ((Controller != NULL) && (vInput.Y != 0.0f))
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, vInput.Y);

			//AddMovementInput(GetActorRightVector(), vInput.Y);
			//GetCharacterMovement()->AddInputVector(GetActorRightVector() * vInput.Y);
		}
	}
}

void APlayer_Base_Knight::RotateAction(const FInputActionInstance& _Instance)
{
	FVector2D vInput = _Instance.GetValue().Get<FVector2D>();

	UE_LOG(LogTemp, Warning, TEXT("X : %f"), vInput.X);
	UE_LOG(LogTemp, Warning, TEXT("Y : %f"), vInput.Y);

	AddControllerYawInput(vInput.X);
	AddControllerPitchInput(-vInput.Y);

	/*float DT = GetWorld()->GetDeltaSeconds();

	FRotator rCameraRotation = m_Arm->GetRelativeRotation();

	rCameraRotation.Yaw += vInput.X * 100.f * DT;

	rCameraRotation.Pitch += vInput.Y * 100.f * DT;

	if (rCameraRotation.Pitch > 40.f)
	{
		rCameraRotation.Pitch = 40.f;
	}
	else if (rCameraRotation.Pitch < -40.f)
	{
		rCameraRotation.Pitch = -40.f;
	}

	m_Arm->SetRelativeRotation(rCameraRotation);*/
}

void APlayer_Base_Knight::JumpAction(const FInputActionInstance& _Instance)
{
	if (bEnableJump)
	{
		ACharacter::Jump();
	}
}

void APlayer_Base_Knight::SprintToggleAction(const FInputActionInstance& _Instance)
{
	bool bToggle = _Instance.GetValue().Get<bool>();

	UE_LOG(LogTemp, Warning, TEXT("Sprint"));

	if (bToggle)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}

}

void APlayer_Base_Knight::GuardAction(const FInputActionInstance& _Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Guard"));
	m_Anim = Cast<UAnimInstance_Knight>(GetMesh()->GetAnimInstance());

	m_Anim->bIsGuard = _Instance.GetValue().Get<bool>();
}

