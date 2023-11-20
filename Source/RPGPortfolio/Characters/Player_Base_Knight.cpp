// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Base_Knight.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../CharacterAnim/AnimInstance_Knight.h"
#include "../System/Component/LockOnTargetComponent.h"
#include "../Header/Enum.h"
#include "Player_CameraArm.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayer_Base_Knight::APlayer_Base_Knight()
	: bEnableJump(true)
	, bEnableMove(true)
	, bIsAttack(false)
	, bAttackToggle(false)
	, bLockOn(false)
	, bAutoLockOnMode(false)
	, CurrentCombo(1)
	, MaxCombo(4)
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
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.2f;

	m_Arm = CreateDefaultSubobject<UPlayer_CameraArm>(TEXT("SpringArm"));
	m_Arm->SetupAttachment(GetRootComponent());
	m_Arm->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Camera->SetupAttachment(m_Arm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	m_Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	fAccTime = 0.f;

	LockonControlRotationRate = 10.f;
	TargetSwitchMouseDelta = 3.f;
	TargetSwitchMinDelaySeconds = .5f;
	BreakLockMouseDelta = 10.f;
	BrokeLockAimingCooldown = .5f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AtkMontage(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Player/Animation/AM_Knight_Attack.AM_Knight_Attack'"));
	if (AtkMontage.Succeeded())
	{
		m_AttackMontage = AtkMontage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PrimaryAtkMontage(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Player/Animation/AM_Knight_PrimaryAttack.AM_Knight_PrimaryAttack'"));
	if (PrimaryAtkMontage.Succeeded())
	{
		m_PrimaryAttackMontage = PrimaryAtkMontage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeMontage(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Player/Animation/AM_Knight_Dodge.AM_Knight_Dodge'"));
	if (DodgeMontage.Succeeded())
	{
		m_DodgeMontage = DodgeMontage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeBWMontage(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Player/Animation/AM_Knight_Dodge_Backward.AM_Knight_Dodge_Backward'"));
	if (DodgeMontage.Succeeded())
	{
		m_DodgeBWMontage = DodgeBWMontage.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ParryMontage(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Player/Animation/AM_Knight_Parry.AM_Knight_Parry'"));
	if (DodgeMontage.Succeeded())
	{
		m_ParryMontage = ParryMontage.Object;
	}
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

	m_AnimInst = Cast<UAnimInstance_Knight>(GetMesh()->GetAnimInstance());
	if (IsValid(m_AnimInst))
	{
		m_AnimInst->OnNextAttackCheck.AddUObject(this, &APlayer_Base_Knight::NextAttackCheck);
		m_AnimInst->OnAttackHitCheck.AddUObject(this, &APlayer_Base_Knight::AttackHitCheck);
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
		bEnableMove = false;
		fAccTime += DeltaTime;

		if (fAccTime > 0.5f)
		{
			bEnableJump = true;
			bEnableMove = true;
			fAccTime = 0.f;
		}
	}

	// 록온 상태일 때 카메라가 록온대상에게 고정되도록
	if (m_Arm->IsCameraLockedToTarget())
	{
		// Vector from player to target
		FVector TargetVect = m_Arm->m_Target->GetComponentLocation() - (m_Arm->GetComponentLocation() + FVector(0.f, 0.f, 100.f));
		FRotator TargetRot = TargetVect.GetSafeNormal().Rotation();
		FRotator CurrentRot = GetControlRotation();
		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, LockonControlRotationRate);

		// Update control rotation to face target
		GetController()->SetControlRotation(NewRot);
	}

	if (!m_AnimInst->Montage_IsPlaying(m_AttackMontage.LoadSynchronous()))
	{
		bIsAttack = false;
	}
	bAttackToggle = false;

	if (m_AnimInst->Montage_IsPlaying(m_DodgeBWMontage.LoadSynchronous()))
	{
		GetCharacterMovement()->AddInputVector(vDodgeVector * -100.f * DeltaTime);
		SetActorRotation(rDodgeRotation);
	}
	else if (m_AnimInst->Montage_IsPlaying(m_DodgeMontage.LoadSynchronous()))
	{
		AddMovementInput(vDodgeVector, 120.f * DeltaTime);
		SetActorRotation(rDodgeRotation);
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
			case EInputActionType::DODGE:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::DodgeAction);
				break;
			case EInputActionType::ATTACK:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::AttackAction);
				break;
			case EInputActionType::PRIMARYATTACK:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::PrimaryAttackAction);
				break;
			case EInputActionType::PARRY:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::ParryAction);
				break;
			case EInputActionType::LOCKON:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, m_Arm, &UPlayer_CameraArm::ToggleCameraLockOn);
				break;
			case EInputActionType::SWITCHLOCKON:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::SwitchLockOnTarget);
				break;
			default:
				break;
			}
		}
	}

}

void APlayer_Base_Knight::SetOrientRotation(bool _Val)
{
	GetCharacterMovement()->bOrientRotationToMovement = _Val;
}

void APlayer_Base_Knight::MoveAction(const FInputActionInstance& _Instance)
{
	FVector2D vInput = _Instance.GetValue().Get<FVector2D>();

	if (CheckMontagePlaying() && bEnableMove)
	{
		if ((Controller != NULL) && (vInput.X != 0.0f))
		{
			const FRotator Rotation = m_Arm->m_Target == nullptr ? Controller->GetControlRotation() : (m_Arm->m_Target->GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, vInput.X);

			//GetCharacterMovement()->AddInputVector(GetActorForwardVector() * vInput.X);
		}

		if ((Controller != NULL) && (vInput.Y != 0.0f))
		{
			const FRotator Rotation = m_Arm->m_Target == nullptr ? Controller->GetControlRotation() : (m_Arm->m_Target->GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, vInput.Y);

			// GetCharacterMovement()->AddInputVector(GetActorRightVector() * vInput.Y);
		}
	}
}

void APlayer_Base_Knight::RotateAction(const FInputActionInstance& _Instance)
{
	FVector2D vInput = _Instance.GetValue().Get<FVector2D>();

	/*AddControllerYawInput(vInput.X);
	AddControllerPitchInput(-vInput.Y);*/

	float TimeSinceLastTargetSwitch = GetWorld()->GetRealTimeSeconds() - LastTargetSwitchTime;

	if (m_Arm->IsCameraLockedToTarget())
	{
		// Should try switch target?
		if (bAutoLockOnMode)
		{
			if (FMath::Abs(vInput.X) > TargetSwitchMouseDelta && TimeSinceLastTargetSwitch > TargetSwitchMinDelaySeconds)	// Prevent switching multiple times using a single movement
			{
				if (vInput.X < 0)
				{
					m_Arm->SwitchTarget(ELockOnDirection::Left);
				}
				else
				{
					m_Arm->SwitchTarget(ELockOnDirection::Right);
				}

				LastTargetSwitchTime = GetWorld()->GetRealTimeSeconds();
			}
		}
	}
	else
	{
		if (!m_Arm->bToggleLockOn)
		{
			// If camera lock was recently broken by a large mouse delta, allow a cooldown time to prevent erratic camera movement
			bool bRecentlyBrokeLock = (GetWorld()->GetRealTimeSeconds() - BrokeLockTime) < BrokeLockAimingCooldown;
			if (!bRecentlyBrokeLock)
			{
				AddControllerYawInput(vInput.X);
			}
			AddControllerPitchInput(-vInput.Y);
		}
	}
}

void APlayer_Base_Knight::JumpAction(const FInputActionInstance& _Instance)
{
	if (CheckMontagePlaying() && !m_AnimInst->bIsGuard)
	{
		ACharacter::Jump();
	}
}

void APlayer_Base_Knight::SprintToggleAction(const FInputActionInstance& _Instance)
{
	bool bToggle = _Instance.GetValue().Get<bool>();

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
	if (CheckMontagePlaying())
	{
		m_AnimInst->bIsGuard = _Instance.GetValue().Get<bool>();
	}
}

void APlayer_Base_Knight::AttackAction(const FInputActionInstance& _Instance)
{
	bAttackToggle = _Instance.GetValue().Get<bool>();

	if (!IsValid(m_AnimInst))
	{
		UE_LOG(LogTemp, Warning, TEXT("애님인스턴스를 찾을 수 없음"));
		return;
	}

	if (CheckMontagePlaying() && !m_AnimInst->bIsGuard)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackStart"));
		m_AnimInst->Montage_Play(m_AttackMontage.LoadSynchronous());
		SetAttackMontage(m_AttackMontage);
		bIsAttack = true;
		CurrentCombo = 1;
	}
}

void APlayer_Base_Knight::PrimaryAttackAction(const FInputActionInstance& _Instance)
{
	bAttackToggle = _Instance.GetValue().Get<bool>();

	if (!IsValid(m_AnimInst))
	{
		UE_LOG(LogTemp, Warning, TEXT("애님인스턴스를 찾을 수 없음"));
		return;
	}

	if (CheckMontagePlaying() && !m_AnimInst->bIsGuard)
	{
		m_AnimInst->Montage_Play(m_PrimaryAttackMontage.LoadSynchronous());
		SetAttackMontage(m_PrimaryAttackMontage);
		bIsAttack = true;
		CurrentCombo = 1;
	}
}

void APlayer_Base_Knight::DodgeAction(const FInputActionInstance& _Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Dodge"));

	if (CheckMontagePlaying() && !m_AnimInst->bIsGuard)
	{
		if(GetCharacterMovement()->GetLastInputVector().IsZero())
		{
			m_AnimInst->Montage_Play(m_DodgeBWMontage.LoadSynchronous());
			vDodgeVector = GetActorForwardVector();
			rDodgeRotation = GetActorRotation();
		}
		else
		{
			m_AnimInst->Montage_Play(m_DodgeMontage.LoadSynchronous());
			vDodgeVector = GetCharacterMovement()->GetLastInputVector();
			rDodgeRotation = UKismetMathLibrary::MakeRotFromX(vDodgeVector);
		}
		
	}
}

void APlayer_Base_Knight::ParryAction(const FInputActionInstance& _Instance)
{
	m_AnimInst->Montage_Play(m_ParryMontage.LoadSynchronous());
}

void APlayer_Base_Knight::SwitchLockOnTarget(const FInputActionInstance& _Instance)
{
	float SwitchDirection = _Instance.GetValue().Get<float>();

	UE_LOG(LogTemp, Warning, TEXT("SwitchDirection : %f"), SwitchDirection);

	if (SwitchDirection > 0.f)
	{
		m_Arm->SwitchTarget(ELockOnDirection::Left);
	}
	else if (SwitchDirection < 0.f)
	{
		m_Arm->SwitchTarget(ELockOnDirection::Right);
	}

}

bool APlayer_Base_Knight::CheckMontagePlaying()
{
	if (m_AnimInst->Montage_IsPlaying(m_AttackMontage.LoadSynchronous()) ||
		m_AnimInst->Montage_IsPlaying(m_PrimaryAttackMontage.LoadSynchronous()) ||
		m_AnimInst->Montage_IsPlaying(m_DodgeBWMontage.LoadSynchronous()) ||
		m_AnimInst->Montage_IsPlaying(m_DodgeMontage.LoadSynchronous()) ||
		GetCharacterMovement()->IsFalling()
		)
	{
		return false;
	}

	return true;
}

void APlayer_Base_Knight::NextAttackCheck()
{
	if (bAttackToggle)
	{
		if (CurrentCombo == MaxCombo)
		{
			CurrentCombo = 1;
		}
		else
		{
			CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
		}

		FName NextComboCount = FName(*FString::Printf(TEXT("Combo%d"), CurrentCombo));
		m_AnimInst->Montage_JumpToSection(NextComboCount, GetAttackMontage().LoadSynchronous());
	}
}

void APlayer_Base_Knight::AttackHitCheck()
{
	float AtkRange = 200.f;
	float AtkRadius = 50.f;
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel
	(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AtkRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeCapsule(AtkRadius, AtkRange * 0.5f + AtkRadius),
		Params
	);
	FColor color;
	bResult ? color = FColor::Red : color = FColor::Green;
	DrawDebugCapsule(GetWorld(), GetActorLocation() + GetActorForwardVector() * 100.f * 0.5f, AtkRange * 0.5f + AtkRadius, AtkRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector() * AtkRange).ToQuat(), color, false, 1.f);

	if (bResult)
	{
		if (HitResult.GetActor()->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit!!!"));
		}
	}
}
