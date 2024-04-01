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
#include "../UI/UI_StatusMain.h"
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
#include "NiagaraFunctionLibrary.h"

// Sets default values
APlayer_Base_Knight::APlayer_Base_Knight()
	: bEnableJump(true)
	, bEnableMove(true)
	, bAttackToggle(false)
	, bItemDelay(false)
	, fItemDelayTime(0.f)
	, bToggleInvinc(false)
	, fInvincTime(0.f)
	, CurrentCombo(1)
	, MaxCombo(4)
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
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.2f;

	m_Arm = CreateDefaultSubobject<UPlayer_CameraArm>(TEXT("SpringArm"));
	m_Arm->SetupAttachment(GetRootComponent());
	m_Arm->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Camera->SetupAttachment(m_Arm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	m_Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	LockonControlRotationRate = 10.f;
	BrokeLockAimingCooldown = 0.5f;

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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontage(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Player/Animation/AM_Knight_Hit.AM_Knight_Hit'"));
	if(HitMontage.Succeeded())
	{
		m_HitMontage = HitMontage.Object;
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
}

// Called every frame
void APlayer_Base_Knight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 록온 상태일 때 카메라가 록온대상에게 고정되도록
	if (m_Arm->IsCameraLockedToTarget())
	{
		// 플레이어에서 타겟으로의 벡터
		FVector TargetVect = m_Arm->m_Target->GetComponentLocation() - (m_Arm->GetComponentLocation() + FVector(0.f, 0.f, 100.f));
		FRotator TargetRot = TargetVect.GetSafeNormal().Rotation();
		FRotator CurrentRot = GetControlRotation();
		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, LockonControlRotationRate);

		// 타겟을 바라보도록 로테이션 수정
		GetController()->SetControlRotation(NewRot);
	}

	if (m_AnimInst->Montage_IsPlaying(GetAttackMontage().LoadSynchronous()))
	{

	}
	else
	{
		m_AnimInst->bIsAttack = false;
	}
	bAttackToggle = false;
	
	// 회피 애니메이션 재생중일 때
	if (m_AnimInst->Montage_IsPlaying(m_DodgeBWMontage.LoadSynchronous()))
	{
		GetCharacterMovement()->AddInputVector(vDodgeVector * -100.f * DeltaTime);
		SetActorRotation(rDodgeRotation);
	}
	else if (m_AnimInst->Montage_IsPlaying(m_DodgeMontage.LoadSynchronous()))
	{
		AddMovementInput(vDodgeVector, 200.f * DeltaTime);
		SetActorRotation(rDodgeRotation);
	}

	if (bAtkTrace)
	{
		AttackHitCheck();
	}

	if (bItemDelay)
	{
		fItemDelayTime += 1.f * DeltaTime;
		if (fItemDelayTime > 3.f)
		{
			UUI_Player_QuickSlot* pQuickSlotUI = m_MainUI->GetQuickSlotUI();
			pQuickSlotUI->SetQuickSlotColor(1.f, 1.f, 1.f, 1.f, false);
			bItemDelay = false;
			fItemDelayTime = 0.f;
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
			case EInputActionType::OPENMENU:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::OpenMenu);
				break;
			case EInputActionType::ACTION:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::ActionCommand);
				break;
			case EInputActionType::BACKTOPREV:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::BackToPrevMenu);
				break;
			case EInputActionType::QUICKSLOTCHANGE:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::QuickSlotChange);
				break;
			case EInputActionType::USELOWERQUICKSLOT:
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::UseLowerQuickSlot);
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

////////////////////////////// 인풋액션 함수 //////////////////////////////
//////////////////////////////////////////////////////////////////////////
void APlayer_Base_Knight::MoveAction(const FInputActionInstance& _Instance)
{
	FVector2D vInput = _Instance.GetValue().Get<FVector2D>();

	if (!CheckMontagePlaying() && bEnableMove)
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

	if (m_Arm->IsCameraLockedToTarget())
	{
		// do not rotate
	}
	else
	{
		if (!m_Arm->bToggleLockOn)
		{
			// If camera lock was recently broken by a large mouse delta, allow a cooldown time to prevent erratic camera movement
			bool bRecentlyBrokeLock = GetWorld()->GetRealTimeSeconds() < BrokeLockAimingCooldown;
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
	if (!CheckMontagePlaying() && !m_AnimInst->bIsGuard)
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
	if (!CheckMontagePlaying())
	{
		m_AnimInst->bIsGuard = _Instance.GetValue().Get<bool>();
		if (m_AnimInst->bIsGuard)
		{
			UE_LOG(LogTemp, Warning, TEXT("GuardTrue"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GuardFalse"));
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

	bAttackToggle = _Instance.GetValue().Get<bool>();
	
	if (!CheckMontagePlaying() && !m_AnimInst->bIsGuard)
	{
		m_AnimInst->Montage_Play(m_AttackMontage.LoadSynchronous());
		AddMovementInput(GetActorForwardVector(), 10000.f);
		SetAttackMontage(m_AttackMontage);
		m_AnimInst->bIsAttack = true;
		CurrentCombo = 1;
	}
}

void APlayer_Base_Knight::PrimaryAttackAction(const FInputActionInstance& _Instance)
{
	if (!IsValid(m_AnimInst))
	{
		UE_LOG(LogTemp, Warning, TEXT("애님인스턴스를 찾을 수 없음"));
		return;
	}

	bAttackToggle = _Instance.GetValue().Get<bool>();

	if (bAttackToggle)
	{
		UE_LOG(LogTemp, Warning, TEXT("primaryTrue"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("primaryFalse"));
	}
	if (!CheckMontagePlaying() && !m_AnimInst->bIsGuard)
	{
		m_AnimInst->Montage_Play(m_PrimaryAttackMontage.LoadSynchronous());
		SetAttackMontage(m_PrimaryAttackMontage);
		m_AnimInst->bIsAttack = true;
		CurrentCombo = 1;
	}
}

void APlayer_Base_Knight::DodgeAction(const FInputActionInstance& _Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Dodge"));

	if (!CheckMontagePlaying() && !m_AnimInst->bIsGuard)
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

void APlayer_Base_Knight::OpenMenu(const FInputActionInstance& _Instance)
{
	bShowMenu = (bShowMenu != _Instance.GetValue().Get<bool>());

	ARPGPortfolioGameModeBase* pGameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if ( !IsValid(pGameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode Not Found"));
		return;
	}
	if (pGameMode->IsSubMenuUIOpened())
	{
		return;
	}

	APlayerController* pController = Cast<APlayerController>(GetController());
	if ( !IsValid(pController) )
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController Not Found"));
		return;
	}

	if (bShowMenu)
	{
		FInputModeGameAndUI GAU;
		GAU.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
		GAU.SetHideCursorDuringCapture(false);
		pController->SetInputMode(GAU);
	}
	else
	{
		FInputModeGameOnly GameOnly;
		pController->SetInputMode(GameOnly);
	}
		
	pController->bShowMouseCursor = bShowMenu;
	pController->SetPause(bShowMenu);

	m_MainUI->ShowMenu(bShowMenu);
}

void APlayer_Base_Knight::ActionCommand(const FInputActionInstance& _Instance)
{
	if (!OverlapItemArr.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템 획득"));
		UInventory_Mgr::GetInst(GetWorld())->AddGameItem(OverlapItemArr[OverlapItemArr.Num()-1]->m_IID, (uint32)OverlapItemArr[OverlapItemArr.Num() - 1]->m_Stack);
		FGameItemInfo* pItemInfo = UInventory_Mgr::GetInst(GetWorld())->GetItemInfo(OverlapItemArr[OverlapItemArr.Num() - 1]->m_IID);
		
		m_MainUI->ShowRootingMessage(true);
		m_MainUI->GetItemMessageUI()->SetItemMessage(pItemInfo->ItemName, pItemInfo->IconImgPath, OverlapItemArr[OverlapItemArr.Num() - 1]->m_Stack);
		m_MainUI->ShowActionMessage(true);
		m_MainUI->GetMainMessageUI()->SetMessageText(FText::FromString(L"E"), FText::FromString(L"확인"));
		OverlapItemArr[OverlapItemArr.Num() - 1]->Destroy();
	}
	else if (m_MainUI->GetRootMessageDisplayed())
	{
		m_MainUI->ShowRootingMessage(false);
		m_MainUI->ShowActionMessage(false);
	}
}

void APlayer_Base_Knight::BackToPrevMenu(const FInputActionInstance& _Instance)
{
	ARPGPortfolioGameModeBase* pGameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if ( !IsValid(pGameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode Not Found"));
		return;
	}

	if (UInventory_Mgr::GetInst(GetWorld())->CheckInventoryOpened())
	{
		UInventory_Mgr::GetInst(GetWorld())->CloseInventoryUI();
		return;
	}

	UUI_StatusMain* StatusUI = pGameMode->GetStatusUI();
	pGameMode->GetStatusUI()->GetVisibility();
	if (StatusUI->GetVisibility() == ESlateVisibility::Visible)
	{
		StatusUI->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	UUI_EquipMain* EquipUI = pGameMode->GetEquipUI();
	if (EquipUI->GetVisibility() == ESlateVisibility::Visible)
	{
		if ( EquipUI->GetItemList()->GetVisibility() == ESlateVisibility::Visible)
		{
			EquipUI->GetItemList()->SetVisibility(ESlateVisibility::Hidden);
			return;
		}

		EquipUI->SetVisibility(ESlateVisibility::Hidden);
		return;
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
			UInventory_Mgr::GetInst(GetWorld())->UseInventoryItem(pItem->ItemInfo->ID);

			UEquip_Mgr::GetInst(GetWorld())->DecreaseLowerSlotItem(iCurIdx);

			// 아이템 사용후 대기시간 on
			UUI_Player_QuickSlot* pQuickSlotUI = m_MainUI->GetQuickSlotUI();
			pQuickSlotUI->SetQuickSlotColor(0.5f, 0.5f, 0.5f, 0.5f, false);
			fItemDelayTime = 0.f;
			bItemDelay = true;
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
//////////////////////////////////////////////////////////////////////////
////////////////////////////// 인풋액션 함수 //////////////////////////////

// 재생중일 때 이동입력안되는 몽타주 목록
bool APlayer_Base_Knight::CheckMontagePlaying()
{
	// true일 경우 이동 입력이 되지않도록 판단하기 위한 함수
	if (m_AnimInst->Montage_IsPlaying(m_AttackMontage.LoadSynchronous())		||
		m_AnimInst->Montage_IsPlaying(m_PrimaryAttackMontage.LoadSynchronous()) ||
		m_AnimInst->Montage_IsPlaying(m_DodgeBWMontage.LoadSynchronous())		||
		m_AnimInst->Montage_IsPlaying(m_DodgeMontage.LoadSynchronous())			||
		m_AnimInst->Montage_IsPlaying(m_HitMontage.LoadSynchronous())			||
		GetCharacterMovement()->IsFalling()
		)
	{
		return true;
	}

	return false;
}

// 연속공격 다음 모션 체크함수
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
		AddMovementInput(GetActorForwardVector(), 10000.f);
	}
}

void APlayer_Base_Knight::AttackHitCheck()
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
	// FVector vMidpoint2 = (vSwordBottom + vSwordTop) / 2;
	DrawDebugCapsule(GetWorld(), vMidpoint, (vSwordTop - vSwordBottom).Size() * 0.5f, AtkRadius, FRotationMatrix::MakeFromZ(vSwordTop - vSwordBottom).ToQuat(), color, false, 0.5f);

	if (bResult)
	{
		if (HitResult.GetActor()->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit!!!"));
			float iDamage;
			APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());

			if (GetAttackMontage() == m_AttackMontage)
			{
				iDamage = pState->GetPlayerBasePower().PhysicAtk;
				UE_LOG(LogTemp, Display, TEXT("Damage : %d"), (int)iDamage);
			}
			else if(GetAttackMontage() == m_PrimaryAttackMontage)
			{
				iDamage = pState->GetPlayerBasePower().PhysicAtk * 1.5f;
				UE_LOG(LogTemp, Display, TEXT("Damage : %d"), (int)iDamage);
			}
			AMonster_Base* pMonster = Cast<AMonster_Base>(HitResult.GetActor());
			if (IsValid(pMonster))
			{
				iDamage -= pMonster->GetMonsterInfo().PhysicDef;
				UE_LOG(LogTemp, Display, TEXT("Target Deffense : %d"), (int)pMonster->GetMonsterInfo().PhysicDef);
			}
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), iDamage, GetController(), this, UDamageType::StaticClass());

			bAtkTrace = false;
		}
	}
}

float APlayer_Base_Knight::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	APlayerState_Base* pState = Cast<APlayerState_Base>(GetPlayerState());

	FinalDamage = FMath::Clamp(FinalDamage - pState->GetPlayerBasePower().PhysicDef, 0.f, FinalDamage);

	int32 iCurHP = pState->GetPlayerBasePower().CurHP;
	iCurHP = FMath::Clamp(iCurHP - FinalDamage, 0.f, pState->GetPlayerBasePower().MaxHP);

	pState->SetPlayerCurrentHP(iCurHP);
	m_PlayerUI->SetPlayerHPRatio(iCurHP / pState->GetPlayerBasePower().MaxHP);

	if ( iCurHP <= 0.f && GetController() )
	{
		// 사망처리
	}
	else
	{
		AMonster_Base* pMonster = Cast<AMonster_Base>(DamageCauser);
		
		// 피격 시 피격지점 반대방향으로 밀려나도록
		FVector LaunchVec = GetActorLocation() - pMonster->GetActorLocation();
		FVector LaunchForce = LaunchVec.GetSafeNormal() * 300.f;
		LaunchCharacter(LaunchForce, false, false);

		// 피격 애니메이션 재생
		m_AnimInst->Montage_Play(m_HitMontage.LoadSynchronous());
	}

	return 0.0f;
}

void APlayer_Base_Knight::ActionTriggerBeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)
{
	FName TriggerName = _OtherPrimitiveCom->GetCollisionProfileName();
	if(TriggerName.IsEqual(FName(TEXT("ItemTrigger"))))
	{
		m_MainUI->ShowActionMessage(true);
		m_MainUI->GetMainMessageUI()->SetMessageText(FText::FromString(L"E"), FText::FromString(L"획득한다"));
		OverlapItemArr.Emplace(Cast<AItem_Dropped_Base>(_OtherActor));
	}
}

void APlayer_Base_Knight::ActionTriggerEndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index)
{
	FName TriggerName = _OtherPrimitiveCom->GetCollisionProfileName();

	// 아이템에 오버랩된 상태일 경우
	if (TriggerName.IsEqual(FName(TEXT("ItemTrigger"))))
	{
		for (int32 i = 0; i < OverlapItemArr.Num(); ++i)
		{
			if (OverlapItemArr[i]->GetName().Equals(_OtherActor->GetName()))
			{
				OverlapItemArr.RemoveAt(i);
				break;
			}
		}

		if (OverlapItemArr.IsEmpty())
		{
			if (m_MainUI->GetRootMessageDisplayed())
			{
				m_MainUI->ShowActionMessage(true);
				m_MainUI->GetMainMessageUI()->SetMessageText(FText::FromString(L"E"), FText::FromString(L"확인"));
			}
			else
			{
				m_MainUI->ShowActionMessage(false);
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

// 아이템 사용 후 대기시간 계산 시작
void APlayer_Base_Knight::ItemUseDelayOn()
{
	UUI_Player_QuickSlot* pQuickSlotUI = m_MainUI->GetQuickSlotUI();
	pQuickSlotUI->SetQuickSlotColor(0.5f, 0.5f, 0.5f, 0.5f, false);
	fItemDelayTime = 0.f;
	bItemDelay = true;
}

// 무적시간 동안 데미지 안받도록 설정
void APlayer_Base_Knight::InvincibleTimeCheck()
{
	if (bToggleInvinc)
	{
		SetCanBeDamaged(false);
	}
	else
	{
		SetCanBeDamaged(true);
	}
}