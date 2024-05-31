// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Base_Knight.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../CharacterAnim/AnimInstance_Knight.h"
#include "../System/Component/LockOnTargetComponent.h"
#include "../Header/Enum.h"
#include "Player_CameraArm.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../RPGPortfolioGameModeBase.h"
#include "../UI/UI_Base.h"
#include "../UI/UI_Player_Main.h"
#include "../UI/UI_Message_Main.h"
#include "../UI/UI_Message_Item.h"
#include "../UI/UI_EquipMain.h"
#include "../UI/UI_EquipItemList.h"
#include "../UI/UI_Player_QuickSlot.h"
#include "../UI/UI_Player_Soul.h"
#include "Components/CapsuleComponent.h"
#include "../Item/Item_Dropped_Base.h"
#include "../Manager/Inventory_Mgr.h"
#include "../System/PlayerState_Base.h"
#include "../Monsters/Monster_Base.h"
#include "../Manager/Equip_Mgr.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "../System/DamageType_Base.h"
#include "Engine/DamageEvents.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "NiagaraFunctionLibrary.h"
#include "../Projectiles/Proj_Player_Cutter.h"

// Sets default values
APlayer_Base_Knight::APlayer_Base_Knight()
	: fFrontBack(0.f)
	, fLeftRight(0.f)
	, bEnableJump(true)
	, bEnableMove(true)
	, bAttackToggle(false)
	, bHeavyToggle(false)
	, bNextAtkCheckOn(false)
	, bItemDelay(false)
	, bToggleInvinc(false)
	, fInvincTime(0.f)
	, CurrentCombo(1)
	, MaxCombo(3)
	, bShowMenu(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1500.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.2f;

	m_Arm = CreateDefaultSubobject<UPlayer_CameraArm>(TEXT("SpringArm"));
	m_Arm->SetupAttachment(GetRootComponent());
	m_Arm->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Camera->SetupAttachment(m_Arm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	m_Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	LockonControlRotationRate = 10.f;

	ConstructorHelpers::FClassFinder<UUserWidget> MarkerUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/Monster/BPC_UI_LockOnMarker.BPC_UI_LockOnMarker_C'"));
	if (MarkerUI.Succeeded())
	{
		m_MarkerClass = MarkerUI.Class;
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
		m_AnimInst->OnInvincibleTimeCheck.AddUObject(this, &APlayer_Base_Knight::InvincibleTimeCheck);
		m_AnimInst->OnAttackMove.AddUObject(this, &APlayer_Base_Knight::AttackMoveStart);
	}

	ARPGPortfolioGameModeBase* pGameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if ( !IsValid(pGameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode Not Found"));
		return;
	}
	m_MainUI = pGameMode->GetMainHUD();
	m_PlayerUI = m_MainUI->GetMainUIWidget();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayer_Base_Knight::ActionTriggerBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayer_Base_Knight::ActionTriggerEndOverlap);

	if ( !IsValid(m_PlayerMontage.LoadSynchronous()) )
	{
		UE_LOG(LogTemp, Error, TEXT("플레이어 몽타주 데이터에셋 로드 실패"));
	}

	if ( !IsValid(m_PlayerSound.LoadSynchronous()) )
	{
		UE_LOG(LogTemp, Error, TEXT("플레이어 사운드 데이터에셋 로드 실패"));
	}
	
	if ( !IsValid(m_MenuSound.LoadSynchronous()) )
	{
		UE_LOG(LogTemp, Error, TEXT("플레이어 메뉴사운드 데이터에셋 로드 실패"));
	}

	if ( IsValid(m_MarkerClass) )
	{
		m_Marker = CreateWidget(GetWorld(), m_MarkerClass);

		if ( IsValid(m_Marker) )
		{
			m_Marker->AddToViewport();
			m_Marker->SetPositionInViewport(FVector2D(0.f, 0.f));
			m_Marker->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("락온 마커 생성 실패"));
		}
	}

	LockOnDelegate.BindUFunction(this, FName("TargetLockOn"));
}

// Called every frame
void APlayer_Base_Knight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	fFrontBack = 0.f;
	fLeftRight = 0.f;
	
	// 회피 애니메이션 재생중일 때
	if (bDodging)
	{
		if (bDodgeMove)
		{
			if ( m_AnimInst->Montage_IsPlaying(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::DODGE_BW)) )
			{
				GetCharacterMovement()->AddInputVector(vDodgeVector * -100.f * DeltaTime);
			}
			else if ( m_AnimInst->Montage_IsPlaying(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::DODGE_FW)) )
			{
				AddMovementInput(vDodgeVector, 2000.f * DeltaTime);
			}
		}
		SetActorRotation(rDodgeRotation);
	}

	// 점프공격
	if (bJumpAtk)
	{
		float CurTime = m_AnimInst->Montage_GetPosition(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::JUMPATTACK));

		// 바닥에 착지하기 전까지는 공격 준비 자세에서 정지한다.
		if (CurTime > 0.3f)
		{
			m_AnimInst->Montage_Pause(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::JUMPATTACK));
		}

		// 바닥에 착지하면 애니메이션을 다시 재생
		if (!GetCharacterMovement()->IsFalling())
		{
			m_AnimInst->Montage_Resume(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::JUMPATTACK));
			bJumpAtk = false;
		}
	}

	if (bAtkMove)
	{
		AddMovementInput(vAtkMoveVec, 40.f * DeltaTime);
	}

	if (bAtkTrace)
	{
		AttackHitCheck(EATTACK_TYPE::PHYSIC_MELEE);
	}

	// 다음 공격 입력 체크기간
	if (bNextAtkCheckOn)
	{
		if (bAttackToggle)
		{
			NextAttackCheck();
		}
	}

	if (bSprintToggle)
	{
		// 스테미너가 0일 경우 달리기 불가
		APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());
		if ( pState->GetPlayerBasePower().CurStamina <= 0.f )
		{
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
			bSprintToggle = false;
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
			case EInputActionType::DODGE:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::DodgeAction);
				break;
			case EInputActionType::ATTACK:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::AttackAction);
				break;
			case EInputActionType::HEAVYTOGGLE:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::HeavyAttackToggle);
				break;
			case EInputActionType::PARRY:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::ParryAction);
				break;
			case EInputActionType::LOCKON:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::LockOnToggleAction);
				break;
			case EInputActionType::SWITCHLOCKON:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::SwitchLockOnTarget);
				break;
			case EInputActionType::OPENMENU:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::OpenMenu);
				break;
			case EInputActionType::ACTION:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::ActionCommand);
				break;
			case EInputActionType::QUICKSLOTCHANGE:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::QuickSlotChange);
				break;
			case EInputActionType::USELOWERQUICKSLOT:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::UseLowerQuickSlot);
				break;
			case EInputActionType::USESKILL_1:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::UseSkill_1);
				break;
			default:
				break;
			}
		}
	}
}

void APlayer_Base_Knight::SetOrientRotation(const bool& _Val)
{
	GetCharacterMovement()->bOrientRotationToMovement = _Val;
}

////////////////////////////// 인풋액션 함수 //////////////////////////////
//////////////////////////////////////////////////////////////////////////
void APlayer_Base_Knight::MoveAction(const FInputActionInstance& _Instance)
{
	FVector vInput = _Instance.GetValue().Get<FVector>();

	if (!CheckMontagePlaying() && bEnableMove)
	{
		if (bSprintToggle)
		{
			APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());
			pState->SetPlayerCurrentStamina(pState->GetPlayerBasePower().CurStamina - 10.f * GetWorld()->GetDeltaSeconds());
		}

		if ((Controller != NULL) && (vInput.X != 0.0f))
		{
			const FRotator Rotation = m_Arm->m_Target == nullptr ? Controller->GetControlRotation() : (m_Arm->m_Target->GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, vInput.X);

			if (m_Arm->IsCameraLockedToTarget())
			{
				if (bSprintToggle)
				{
					fFrontBack = (vInput.X > 0.f) ? 2.f : -2.f;
				}
				else
				{
					fFrontBack = (vInput.X > 0.f) ? 1.f : -1.f;
				}
			}
			else
			{
				// 락온 중이 아닐때는 캐릭터의 앞으로만 이동하므로
				fFrontBack = bSprintToggle ? 2.f : 1.f;
			}
		}
		if ((Controller != NULL) && (vInput.Y != 0.0f))
		{
			const FRotator Rotation = m_Arm->m_Target == nullptr ? Controller->GetControlRotation() : (m_Arm->m_Target->GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, vInput.Y);

			if (m_Arm->IsCameraLockedToTarget())
			{
				if (bSprintToggle)
				{
					fLeftRight = ( vInput.Y > 0.f ) ? 2.f : -2.f;
				}
				else
				{
					fLeftRight = ( vInput.Y > 0.f ) ? 1.f : -1.f;
				}
			}
			else
			{
				// 락온 중이 아닐때는 캐릭터의 앞으로만 이동하므로
				fFrontBack = bSprintToggle ? 2.f : 1.f;
			}
		}
	}
}

void APlayer_Base_Knight::RotateAction(const FInputActionInstance& _Instance)
{
	FVector2D vInput = _Instance.GetValue().Get<FVector2D>();

	if (!m_Arm->bToggleLockOn)
	{
		AddControllerYawInput(vInput.X);
		AddControllerPitchInput(-vInput.Y);
	}
}

void APlayer_Base_Knight::JumpAction(const FInputActionInstance& _Instance)
{
	if (!CheckMontagePlaying() && !m_AnimInst->GetbIsGuard() && bEnableMove)
	{
		ACharacter::Jump();
	}
}

void APlayer_Base_Knight::SprintToggleAction(const FInputActionInstance& _Instance)
{
	bSprintToggle = _Instance.GetValue().Get<bool>();

	if (m_AnimInst->GetbIsGuard() || !bEnableMove)
	{
		bSprintToggle = false;
		return;
	}

	APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());
	// 스테미너가 0일 경우 달리기 불가
	if (pState->GetPlayerBasePower().CurStamina <= 0.f)
	{
		bSprintToggle = false;
		return;
	}

	if (bSprintToggle)
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
	if (!CheckMontagePlaying())
	{
		bool bGuard = _Instance.GetValue().Get<bool>();
		m_AnimInst->SetbIsGaurd(bGuard);
		if (bGuard)
		{
			APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());
			pState->SetbSTRecovSlowly(true);
		}
		else
		{
			bToggleGuard = false;
			APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());
			pState->SetbSTRecovSlowly(false);
		}
	}
}

void APlayer_Base_Knight::AttackAction(const FInputActionInstance& _Instance)
{
	if (!IsValid(m_AnimInst))
	{
		UE_LOG(LogTemp, Warning, TEXT("애님인스턴스를 찾을 수 없음"));
		return;
	}

	APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());

	// 스테미너가 0일 경우 공격 불가
	if (pState->GetPlayerBasePower().CurStamina <= 0.f)
	{
		return;
	}
	
	bAttackToggle = _Instance.GetValue().Get<bool>();
	
	if (GetCharacterMovement()->IsFalling())
	{
		m_AnimInst->Montage_Play(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::JUMPATTACK));
		ConsumeStaminaForMontage(EPlayerMontage::JUMPATTACK);
		bAttackToggle = false;
		bJumpAtk = true;
		return;
	}

	if (!CheckMontagePlaying() && !m_AnimInst->GetbIsGuard())
	{
		if (bHeavyToggle)
		{
			// 강공격
			m_AnimInst->Montage_Play(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::HEAVYATTACK));
			SetAttackMontage(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::HEAVYATTACK));
			CurrentCombo = 2;
			ConsumeStaminaForMontage(EPlayerMontage::HEAVYATTACK);
		}
		else
		{
			// 약공격
			m_AnimInst->Montage_Play(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::ATTACK));
			SetAttackMontage(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::ATTACK));
			CurrentCombo = 1;
			ConsumeStaminaForMontage(EPlayerMontage::ATTACK);
		}
		bAttackToggle = false;
	}
}

void APlayer_Base_Knight::HeavyAttackToggle(const FInputActionInstance& _Instance)
{
	bHeavyToggle = _Instance.GetValue().Get<bool>();
}

void APlayer_Base_Knight::DodgeAction(const FInputActionInstance& _Instance)
{
	if (!CheckMontagePlaying() && !m_AnimInst->GetbIsGuard())
	{
		APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());

		// 스테미너가 0일 경우 회피 불가
		if ( pState->GetPlayerBasePower().CurStamina <= 0.f )
		{
			return;
		}

		if(GetCharacterMovement()->GetLastInputVector().IsZero())
		{
			m_AnimInst->Montage_Play(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::DODGE_BW));
			vDodgeVector = GetActorForwardVector();
			//rDodgeRotation = UKismetMathLibrary::MakeRotFromX(vDodgeVector);
			rDodgeRotation = GetActorRotation();
			ConsumeStaminaForMontage(EPlayerMontage::DODGE_BW);
		}
		else
		{
			m_AnimInst->Montage_Play(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::DODGE_FW));
			vDodgeVector = GetCharacterMovement()->GetLastInputVector();
			rDodgeRotation = UKismetMathLibrary::MakeRotFromX(vDodgeVector);
			ConsumeStaminaForMontage(EPlayerMontage::DODGE_FW);
		}
		bDodging = true;
	}
}

void APlayer_Base_Knight::ParryAction(const FInputActionInstance& _Instance)
{
	m_AnimInst->Montage_Play(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::PARRY));
}

void APlayer_Base_Knight::LockOnToggleAction(const FInputActionInstance& _Instance)
{
	bToggleLockOn = _Instance.GetValue().Get<bool>();
	bool bTargetLocked = m_Arm->ToggleCameraLockOn(bToggleLockOn);
	if (bTargetLocked)
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetWorld()->GetTimerManager().SetTimerForNextTick(LockOnDelegate);
		m_Marker->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		m_Marker->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APlayer_Base_Knight::SwitchLockOnTarget(const FInputActionInstance& _Instance)
{
	float SwitchDirection = _Instance.GetValue().Get<float>();

	if (SwitchDirection > 0.f)
	{
		m_Arm->SwitchTarget(ELockOnDirection::Left);
	}
	else if (SwitchDirection < 0.f)
	{
		m_Arm->SwitchTarget(ELockOnDirection::Right);
	}

}

void APlayer_Base_Knight::OpenMenu(const FInputActionInstance& _Instance)
{
	ARPGPortfolioGameModeBase* pGameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if ( !IsValid(pGameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode Not Found"));
		return;
	}
	APlayerController* pController = Cast<APlayerController>(GetController());
	if ( !IsValid(pController) )
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController Not Found"));
		return;
	}

	// 세부메뉴가 열려있을 경우
	if (pGameMode->IsSubMenuUIOpened())
	{
		if (UInventory_Mgr::GetInst(GetWorld())->CheckInventoryOpened())
		{
			UInventory_Mgr::GetInst(GetWorld())->CloseInventoryUI();
			UGameplayStatics::PlaySound2D(GetWorld(), m_MenuSound.LoadSynchronous()->GetMenuSound(EMenuSound::MENU_CLOSE));
			return;
		}
		if (pGameMode->IsStatusOpened())
		{
			pGameMode->CloseStatus();
			UGameplayStatics::PlaySound2D(GetWorld(), m_MenuSound.LoadSynchronous()->GetMenuSound(EMenuSound::MENU_CLOSE));
			return;
		}
		if (pGameMode->IsManualOpened())
		{
			pGameMode->CloseManual();
			UGameplayStatics::PlaySound2D(GetWorld(), m_MenuSound.LoadSynchronous()->GetMenuSound(EMenuSound::MENU_CLOSE));
			return;
		}

		UUI_EquipMain* EquipUI = pGameMode->GetEquipUI();
		if (EquipUI->GetVisibility() == ESlateVisibility::Visible)
		{
			if (EquipUI->GetItemList()->GetVisibility() == ESlateVisibility::Visible)
			{
				EquipUI->GetItemList()->SetVisibility(ESlateVisibility::Hidden);
				UGameplayStatics::PlaySound2D(GetWorld(), m_MenuSound.LoadSynchronous()->GetMenuSound(EMenuSound::MENU_CLOSE));
				return;
			}

			EquipUI->SetVisibility(ESlateVisibility::Hidden);
			UGameplayStatics::PlaySound2D(GetWorld(), m_MenuSound.LoadSynchronous()->GetMenuSound(EMenuSound::MENU_CLOSE));
			return;
		}

		return;
	}

	bShowMenu = (bShowMenu != _Instance.GetValue().Get<bool>());

	if (bShowMenu)
	{
		FInputModeGameAndUI GAU;
		GAU.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
		GAU.SetHideCursorDuringCapture(false);
		pController->SetInputMode(GAU);

		UGameplayStatics::PlaySound2D(GetWorld(), m_MenuSound.LoadSynchronous()->GetMenuSound(EMenuSound::MENU_OPEN));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("menu close"));
		FInputModeGameOnly GameOnly;
		pController->SetInputMode(GameOnly);

		UGameplayStatics::PlaySound2D(GetWorld(), m_MenuSound.LoadSynchronous()->GetMenuSound(EMenuSound::MENU_CLOSE));
	}
		
	pController->bShowMouseCursor = bShowMenu;
	pController->SetPause(bShowMenu);

	m_MainUI->ShowMenu(bShowMenu);
}

void APlayer_Base_Knight::ActionCommand(const FInputActionInstance& _Instance)
{
	if (!OverlapInteractionArr.IsEmpty())
	{
		OverlapInteractionArr[OverlapInteractionArr.Num() - 1]->Interaction();
	}
	// 주변에 아이템이 없고 아이템 획득 메시지 표시된 상태일 때
	else if (m_MainUI->GetRootMessageDisplayed())
	{
		m_MainUI->ShowItemMessageUI(false);
		m_MainUI->ShowMainMessageUI(false);
	}
}

void APlayer_Base_Knight::QuickSlotChange(const FInputActionInstance& _Instance)
{
	if (UEquip_Mgr::GetInst(GetWorld())->QuickSlotValidForArr())
	{
		int32 Idx = UEquip_Mgr::GetInst(GetWorld())->GetNextArrayIndex();
		UE_LOG(LogTemp, Warning, TEXT("퀵슬롯 인덱스 : %d"), Idx);
		m_MainUI->GetQuickSlotUI()->RenewLowerQuickSlot(Idx);
	}
}

void APlayer_Base_Knight::UseLowerQuickSlot(const FInputActionInstance& _Instance)
{
	if (!bItemDelay)
	{
		int32 iCurIdx = UEquip_Mgr::GetInst(GetWorld())->GetCurrentIndex();
		if (UEquip_Mgr::GetInst(GetWorld())->QuickSlotValidForIdx(iCurIdx))
		{
			FInvenItemRow* pItem = UEquip_Mgr::GetInst(GetWorld())->GetQSItemForIndex(iCurIdx);
			UseItem(pItem->ItemInfo->ID, pItem->EquipedSlot);

			// 아이템 사용후 대기시간 on
			bItemDelay = true;
			ItemDelaytime(1.f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("퀵슬롯에 지정된 아이템 없음"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템 딜레이 대기중"));
		return;
	}
}
void APlayer_Base_Knight::UseSkill_1(const FInputActionInstance& _Instance)
{
	if ( !IsValid(m_AnimInst) )
	{
		UE_LOG(LogTemp, Warning, TEXT("애님인스턴스를 찾을 수 없음"));
		return;
	}

	APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());
	// 스테미너가 0일 경우 공격 불가
	if (pState->GetPlayerBasePower().CurStamina <= 0.f)
	{
		return;
	}

	// 마나가 0일 경우 공격 불가
	if (pState->GetPlayerBasePower().CurMP <= 0.f)
	{
		return;
	}

	if (!CheckMontagePlaying() && !m_AnimInst->GetbIsGuard() && bEnableMove)
	{
		m_AnimInst->Montage_Play(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::SLASH_CUTTER));
		ConsumeStaminaForMontage(EPlayerMontage::SLASH_CUTTER);
		pState->SetPlayerCurrentMP(pState->GetPlayerBasePower().CurMP - 20.f);
		// ShotProjectile로
	}
}
//////////////////////////////////////////////////////////////////////////
////////////////////////////// 인풋액션 함수 //////////////////////////////

// 재생중일 때 이동입력안되는 몽타주 목록
bool APlayer_Base_Knight::CheckMontagePlaying()
{
	// true일 경우 이동 입력이 되지않도록 판단하기 위한 함수
	if (m_AnimInst->Montage_IsPlaying(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::ATTACK))		||
		m_AnimInst->Montage_IsPlaying(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::HEAVYATTACK))	||
		m_AnimInst->Montage_IsPlaying(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::JUMPATTACK))	||
		//m_AnimInst->Montage_IsPlaying(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::DODGE_BW))	||
		//m_AnimInst->Montage_IsPlaying(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::DODGE_FW))	||
		m_AnimInst->Montage_IsPlaying(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::HIT))			||
		m_AnimInst->Montage_IsPlaying(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::GUARDBREAK))	||
		m_AnimInst->Montage_IsPlaying(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::SLASH_CUTTER))||
		m_AnimInst->Montage_IsPlaying(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::USEITEM))		||
		GetCharacterMovement()->IsFalling() ||
		bDodging == true
		)
	{
		//UE_LOG(LogTemp, Display, TEXT("몽타주를 재생 불가능한 상태입니다."));
		return true;
	}
	else
	{
		if (m_AnimInst->Montage_IsPlaying(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::DODGE_BW)))
		{
			m_AnimInst->Montage_Stop(1.f);
		}
	}

	return false;
}

// 연속공격 다음 모션 체크함수
void APlayer_Base_Knight::NextAttackCheck()
{
	if (CurrentCombo == MaxCombo)
	{
		CurrentCombo = 2;
	}
	else
	{
		CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
	}

	FName NextComboCount = FName(*FString::Printf(TEXT("Combo%d"), CurrentCombo));
	m_AnimInst->Montage_JumpToSection(NextComboCount, GetAttackMontage().LoadSynchronous());

	APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());
	if ( m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::ATTACK) == GetAttackMontage().LoadSynchronous() )
	{
		pState->SetPlayerCurrentStamina(pState->GetPlayerBasePower().CurStamina - 10.f);
	}
	else
	{
		pState->SetPlayerCurrentStamina(pState->GetPlayerBasePower().CurStamina - 15.f);
	}

	bNextAtkCheckOn = false;
	bAttackToggle = false;
}

void APlayer_Base_Knight::AttackHitCheck(EATTACK_TYPE _AtkType)
{
	float AtkRadius = 10.f;
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	FVector vSwordBottom = GetMesh()->GetSocketLocation("FX_Sword_Bottom");
	FVector vSwordTop = GetMesh()->GetSocketLocation("FX_Sword_Top");
	bool bResult = GetWorld()->SweepSingleByChannel
	(
		HitResult,
		vSwordBottom,
		vSwordTop,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,	// 트레이스 채널 설정은 Block으로 해놔야 HitResult에 걸림(Block으로 해도 밀려나진 않음)
		FCollisionShape::MakeCapsule(AtkRadius, (vSwordTop - vSwordBottom).Size() * 0.5f),
		Params
	);

	FColor color;
	bResult ? color = FColor::Red : color = FColor::Green;
	FVector vMidpoint = FMath::Lerp(vSwordTop, vSwordBottom, 0.5f);
	DrawDebugCapsule(GetWorld(), vMidpoint, (vSwordTop - vSwordBottom).Size() * 0.5f, AtkRadius, FRotationMatrix::MakeFromZ(vSwordTop - vSwordBottom).ToQuat(), color, false, 0.5f);
	if (bResult)
	{
		if (HitResult.GetActor()->IsValidLowLevel())
		{
			for (AActor* HitActor : HitActorArr)
			{
				if (HitResult.GetActor() == HitActor)
				{
					return;
				}
			}
			ApplyPointDamage(HitResult, EATTACK_TYPE::PHYSIC_MELEE);
			HitActorArr.Add(HitResult.GetActor());
		}
	}
}

void APlayer_Base_Knight::ApplyPointDamage(FHitResult const& HitInfo, EATTACK_TYPE _AtkType)
{
	float iDamage;
	APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());

	switch ( _AtkType )
	{
	case EATTACK_TYPE::PHYSIC_MELEE:
	case EATTACK_TYPE::PHYSIC_RANGE:
		iDamage = pState->GetPlayerBasePower().PhysicAtk;
		if ( GetAttackMontage() == m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::HEAVYATTACK) )
		{
			iDamage = iDamage * 1.5f;
		}
		break;
	case EATTACK_TYPE::MAGIC_MELEE:
	case EATTACK_TYPE::MAGIC_RANGE:
		iDamage = pState->GetPlayerBasePower().MagicAtk;
		break;
	default:
		break;
	}

	TSubclassOf<UDamageType_Base> DamageTypeBase = UDamageType_Base::StaticClass();
	DamageTypeBase.GetDefaultObject()->SetAtkType(_AtkType);

	UGameplayStatics::ApplyPointDamage(HitInfo.GetActor(), iDamage, HitInfo.Normal, HitInfo, GetController(), this, DamageTypeBase);
}

float APlayer_Base_Knight::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UDamageType_Base* pDamageType = Cast<UDamageType_Base>(DamageEvent.DamageTypeClass->GetDefaultObject());
	APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());

	// 받은 공격타입에 따라 몬스터의 방어력 설정
	float fPlayerDef;
	switch (pDamageType->GetAtkType())
	{
	case EATTACK_TYPE::PHYSIC_MELEE:
	case EATTACK_TYPE::PHYSIC_RANGE:
		fPlayerDef = pState->GetPlayerBasePower().PhysicDef;
		break;
	case EATTACK_TYPE::MAGIC_MELEE:
	case EATTACK_TYPE::MAGIC_RANGE:
		fPlayerDef = pState->GetPlayerBasePower().MagicDef;
		break;
	default:
		break;
	}

	FinalDamage = FMath::Clamp(FinalDamage - fPlayerDef, 0.f, FinalDamage);
	int32 iCurHP = pState->GetPlayerBasePower().CurHP;
	iCurHP = FMath::Clamp(iCurHP - FinalDamage, 0.f, pState->GetPlayerBasePower().MaxHP);

	pState->SetPlayerCurrentHP(iCurHP);

	if ( iCurHP <= 0.f && GetController() )
	{
		// 사망처리
		return 0.f;
	}
	
	// 방어 상태 해제
	m_AnimInst->SetbIsGaurd(false);
	bToggleGuard = false;
	pState->SetbSTRecovSlowly(false);

	// 현재 행동상태 해제
	bJumpAtk = false;
	bAtkTrace = false;
	bAtkMove = false;
	bNextAtkCheckOn = false;
	bAttackToggle = false;

	AMonster_Base* pMonster = Cast<AMonster_Base>(DamageCauser);

	// 피격 시 피격지점 반대방향으로 밀려나도록
	FVector LaunchVec = GetActorLocation() - pMonster->GetActorLocation();
	FVector LaunchForce = LaunchVec.GetSafeNormal() * 300.f;
	LaunchCharacter(LaunchForce, false, false);

	// 피격 애니메이션 재생
	if ( CheckMontagePlaying() )
	{
		m_AnimInst->Montage_Stop(1.f);
		bAtkMove = false;
	}
	m_AnimInst->Montage_Play(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::HIT));
	// 피격 사운드 재생
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_PlayerSound.LoadSynchronous()->GetPlayerSound(EPlayerSound::HIT), GetActorLocation());

	return 0.0f;
}

void APlayer_Base_Knight::AttackMoveStart(bool _AtkMove)
{
	bAtkMove = _AtkMove;
	if (bAtkMove)
	{
		vAtkMoveVec = GetActorForwardVector();
	}
}

bool APlayer_Base_Knight::BlockEnemyAttack(float _Damage, FVector _MonDir)
{
	// 플레이어와 몬스터가 바라보는 방향 사이의 각도 구하기
	// fDegree가 180도에 가까울수록 서로 마주보고 있음(몬스터가 플레이어의 정면에 있음)
	// fDegree가 0도에 가까울수록 서로 같은 방향을 보고 있음(몬스터가 플레이어의 뒤를 노림)
	// 플레이어 정면 기준으로 160도 각도 안에서 공격했을 경우 막히도록
	FVector vPlayerDir = GetActorForwardVector().GetSafeNormal();
	float fDot = FVector::DotProduct(vPlayerDir, _MonDir);
	float fAcosAngle = FMath::Acos(fDot);
	float fDegree = FMath::RadiansToDegrees(fAcosAngle);

	// 가드 범위 밖에서 공격받을 경우 가드 실패
	if (fDegree < 100.f)
	{
		return false;
	}

	APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());
	// 몬스터의 공격력의 10분의 1 만큼 스태미나 감소
	pState->SetPlayerCurrentStamina(pState->GetPlayerBasePower().CurStamina - _Damage * 0.1f);
	if (pState->GetPlayerBasePower().CurStamina < 0.f)
	{
		// 방어 풀리고 경직상태 되도록
		m_AnimInst->SetbIsGaurd(false);
		bToggleGuard = false;
		pState->SetbSTRecovSlowly(false);
		m_AnimInst->Montage_Play(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::GUARDBREAK));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Block"));
		GetMesh()->SetAllBodiesBelowSimulatePhysics(FName(TEXT("clavicle_l")), true);
		GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(FName(TEXT("clavicle_l")), 0.2f);
		GetMesh()->AddImpulseToAllBodiesBelow(_MonDir * 500.f, FName(TEXT("clavicle_l")), true);

		GetWorld()->GetTimerManager().SetTimer(BlockReactTimer, this, &APlayer_Base_Knight::StopBlockPhysics, 0.1f, false);
	}

	return true;
}

void APlayer_Base_Knight::StopBlockPhysics()
{
	GetMesh()->SetAllBodiesBelowSimulatePhysics(FName(TEXT("clavicle_l")), false);
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(FName(TEXT("clavicle_l")), 0.0f);
}

void APlayer_Base_Knight::TargetLockOn()
{
	// 록온 상태일 때 카메라가 록온대상에게 고정되도록
	if ( m_Arm->IsCameraLockedToTarget() )
	{
		// 플레이어에서 타겟으로의 벡터
		FVector TargetVect = m_Arm->m_Target->GetComponentLocation() - ( m_Arm->GetComponentLocation() + FVector(0.f, 0.f, 100.f) );
		FRotator TargetRot = TargetVect.GetSafeNormal().Rotation();
		FRotator CurrentRot = GetControlRotation();
		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, GetWorld()->GetDeltaSeconds(), LockonControlRotationRate);

		// 타겟을 바라보도록 로테이션 수정
		GetController()->SetControlRotation(NewRot);
		// 락온 타겟 컴포넌트의 위치를 스크린 좌표로 변환해서 해당 좌표에 락온 마커를 표시
		APlayerController* pController = Cast<APlayerController>(GetController());
		FVector2D ScreenPos;
		bool bConverted = UGameplayStatics::ProjectWorldToScreen(pController, m_Arm->m_Target->GetComponentLocation(), ScreenPos);
		if (bConverted)
		{
			m_Marker->SetPositionInViewport(ScreenPos);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("락온 스크린 좌표 계산 실패"));
		}

		if (bToggleLockOn)
		{
			GetWorld()->GetTimerManager().SetTimerForNextTick(LockOnDelegate);
		}
	}
	else
	{
		m_Marker->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APlayer_Base_Knight::BreakLockOn()
{
	m_Marker->SetVisibility(ESlateVisibility::Hidden);
	m_Arm->BreakLockOnTarget();
}

void APlayer_Base_Knight::ShotProjectile()
{
	FActorSpawnParameters param = {};
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	param.OverrideLevel = GetLevel();
	param.bDeferConstruction = false;	// 지연생성(BeginPlay 호출 X)
	param.Owner = this;

	// 투사체 생성위치	
	FVector ProjectileLocation = GetActorLocation() + FVector(0.f, 0.f, 10.f) + GetActorForwardVector() * 200.f;

	// 투사체 발사 방향
	FVector vDir = GetActorForwardVector() * 1000.f;

	TSubclassOf<AProj_Player_Cutter> ProjClass = LoadClass<AProj_Player_Cutter>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Projectile/BPC_SlashCutter.BPC_SlashCutter_C'"));
	AProj_Player_Cutter* pProjectile = GetWorld()->SpawnActor<AProj_Player_Cutter>(ProjClass, ProjectileLocation, GetActorRotation(), param);
	APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());
	pProjectile->SetProjDamage(EATTACK_TYPE::MAGIC_RANGE, pState->GetPlayerBasePower().MagicAtk);
	pProjectile->SetShotInstigator(this);
	pProjectile->LaunchMotion(vDir);
}

void APlayer_Base_Knight::UseItem(EITEM_ID _ID, EEQUIP_SLOT _Slot)
{
	APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());
	EPlayerSound SoundEnum = EPlayerSound::EMPTY;

	FGameItemInfo* pItemInfo = UInventory_Mgr::GetInst(GetWorld())->GetItemInfo(_ID);
	if ( pItemInfo->Restore_HP >= 0 )
	{
		pState->SetPlayerCurrentHP(FMath::Clamp(pState->GetPlayerBasePower().CurHP + pItemInfo->Restore_HP, 0.f, pState->GetPlayerBasePower().MaxHP));
		SoundEnum = EPlayerSound::USERESTORE;
	}
	if ( pItemInfo->Restore_MP >= 0 )
	{
		pState->SetPlayerCurrentMP(FMath::Clamp(pState->GetPlayerBasePower().CurMP + pItemInfo->Restore_MP, 0.f, pState->GetPlayerBasePower().MaxMP));
		SoundEnum = EPlayerSound::USERESTORE;
	}
	if ( pItemInfo->Gained_Soul >= 0 )
	{
		pState->PlayerGainSoul(pItemInfo->Gained_Soul);
		m_MainUI->GetSoulUI()->RenewAmountOfSoul(pItemInfo->Gained_Soul);
		SoundEnum = EPlayerSound::USESOUL;
	}

	UNiagaraSystem* pSystem = LoadObject<UNiagaraSystem>(nullptr, *pItemInfo->NiagaraPath);
	UNiagaraComponent* EffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(pSystem, GetMesh(), FName("Root"), FVector(0.f, 0.f, 1.f), FRotator(0.f), EAttachLocation::SnapToTargetIncludingScale, true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_PlayerSound.LoadSynchronous()->GetPlayerSound(SoundEnum), GetActorLocation());
	m_AnimInst->Montage_Play(m_PlayerMontage.LoadSynchronous()->GetPlayerMontage(EPlayerMontage::USEITEM));

	// 퀵슬롯에 장착된 아이템이 아닐경우 인벤토리에서 자체적으로 수량 감소
	if ( _Slot == EEQUIP_SLOT::EMPTY )
	{
		UInventory_Mgr::GetInst(GetWorld())->DecreaseInventoryItem(_ID);
	}
	// 퀵슬롯에 장착되어있을 경우 퀵슬롯을 통해 수량 감소
	else
	{
		int32 idx = UEquip_Mgr::GetInst(GetWorld())->ConvertQuickSlotToIdx(_Slot);
		UEquip_Mgr::GetInst(GetWorld())->DecreaseLowerSlotItem(idx);
		UUI_Player_QuickSlot* pQuickSlotUI = m_MainUI->GetQuickSlotUI();
		pQuickSlotUI->SetQuickSlotOpacity(0.5f, false);
	}
}

void APlayer_Base_Knight::ItemDelaytime(float _DelayPercent)
{
	UUI_Player_QuickSlot* pQuickSlotUI = m_MainUI->GetQuickSlotUI();
	pQuickSlotUI->SetLowerSlotDelay(_DelayPercent);
	if ( _DelayPercent > 0.f )
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [_DelayPercent, this]
			{
				float fDelayPercent = FMath::Clamp(_DelayPercent - GetWorld()->GetDeltaSeconds() / 3.f, 0.f, 1.f);
				ItemDelaytime(fDelayPercent);
			}
		));
	}
	else
	{
		bItemDelay = false;
		pQuickSlotUI->SetQuickSlotOpacity(1.f, false);
	}
}

void APlayer_Base_Knight::ConsumeStaminaForMontage(EPlayerMontage _Montage)
{
	float fConsumption;
	switch (_Montage)
	{
	case EPlayerMontage::DODGE_FW:
		fConsumption = 10.f;
		break;
	case EPlayerMontage::DODGE_BW:
		fConsumption = 10.f;
		break;
	case EPlayerMontage::ATTACK:
		fConsumption = 10.f;
		break;
	case EPlayerMontage::HEAVYATTACK:
		fConsumption = 15.f;
		break;
	case EPlayerMontage::JUMPATTACK:
		fConsumption = 15.f;
		break;
	case EPlayerMontage::SLASH_CUTTER:
		fConsumption = 20.f;
		break;
	default:
		break;
	}

	APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());
	pState->SetPlayerCurrentStamina(pState->GetPlayerBasePower().CurStamina - fConsumption);
}

void APlayer_Base_Knight::ActionTriggerBeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)
{
	FName TriggerName = _OtherPrimitiveCom->GetCollisionProfileName();
	if (TriggerName.IsEqual(FName(TEXT("InteractionTrigger"))))
	{
		TScriptInterface<IPlayerInteraction> Interaction = TScriptInterface<IPlayerInteraction>(_OtherActor);
		m_MainUI->GetMainMessageUI()->SetMessageText(Interaction->GetCommand_Key(), Interaction->GetCommand_Name());
		m_MainUI->ShowMainMessageUI(true);
		OverlapInteractionArr.Emplace(Interaction);
	}
}

void APlayer_Base_Knight::ActionTriggerEndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index)
{
	FName TriggerName = _OtherPrimitiveCom->GetCollisionProfileName();
	
	if (TriggerName.IsEqual(FName(TEXT("InteractionTrigger"))))
	{
		// 오버랩 상태의 트리거에서 떨어지거나 아이템을 습득하여 트리거 오버랩이 끝났을 때
		for (int32 i = 0; i < OverlapInteractionArr.Num(); ++i)
		{
			if ( OverlapInteractionArr[i]->_getUObject()->GetName().Equals(_OtherActor->GetName()))
			{
				OverlapInteractionArr.RemoveAt(i);
				break;
			}
		}

		if ( OverlapInteractionArr.IsEmpty())
		{
			// 아이템 습득 메시지가 표시중일 때
			if (m_MainUI->GetRootMessageDisplayed())
			{
				m_MainUI->GetMainMessageUI()->SetMessageText(FText::FromString(L"F"), FText::FromString(L"확인"));
				m_MainUI->ShowMainMessageUI(true);
			}
			else
			{
				m_MainUI->ShowMainMessageUI(false);
			}
		}
	}
}

void APlayer_Base_Knight::GainMonsterSoul(int32 _GainedSoul)
{
	APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());
	pState->PlayerGainSoul(_GainedSoul);
	m_MainUI->GetSoulUI()->RenewAmountOfSoul(_GainedSoul);
}

void APlayer_Base_Knight::CloseMenuUI()
{
	APlayerController* pController = Cast<APlayerController>(GetController());
	if ( !IsValid(pController) )
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController Not Found"));
		return;
	}

	FInputModeGameOnly GameOnly;
	pController->SetInputMode(GameOnly);
	pController->bShowMouseCursor = false;
	pController->SetPause(false);

	m_MainUI->ShowMenu(false);
	bShowMenu = false;
}

// 무적시간 동안 데미지 안받도록 설정
void APlayer_Base_Knight::InvincibleTimeCheck(bool _Invincible)
{
	bToggleInvinc = _Invincible;

	if (_Invincible)
	{
		SetCanBeDamaged(false);
		bDodgeMove = true;
	}
	else
	{
		SetCanBeDamaged(true);
		bDodgeMove = false;
	}
}