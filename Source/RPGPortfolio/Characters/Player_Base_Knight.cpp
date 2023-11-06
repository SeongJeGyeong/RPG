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

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	m_Arm->SetupAttachment(GetRootComponent());
	m_Camera->SetupAttachment(m_Arm);

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
		if (vInput.X != 0.f)
		{
			GetCharacterMovement()->AddInputVector(GetActorForwardVector() * vInput.X);
		}

		if (vInput.Y != 0.f)
		{
			GetCharacterMovement()->AddInputVector(GetActorRightVector() * vInput.Y);
		}
	}
}

void APlayer_Base_Knight::RotateAction(const FInputActionInstance& _Instance)
{
	FVector2D vInput = _Instance.GetValue().Get<FVector2D>();

	AddControllerYawInput(vInput.X);

	float DT = GetWorld()->GetDeltaSeconds();

	FRotator rCameraRotation = m_Arm->GetRelativeRotation();
	rCameraRotation.Pitch += vInput.Y * 100.f * DT;

	if (rCameraRotation.Pitch > 45)
	{
		rCameraRotation.Pitch = 45.f;
	}
	else if (rCameraRotation.Pitch < -45.f)
	{
		rCameraRotation.Pitch = -45.f;
	}

	m_Arm->SetRelativeRotation(rCameraRotation);
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

