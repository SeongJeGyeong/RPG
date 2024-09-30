// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Base_Knight.h"
#include "../Header/Enum.h"
#include "../RPGPortfolioGameModeBase.h"
#include "../CharacterAnim/AnimInstance_Knight.h"
#include "Player_CameraArm.h"
#include "../UI/UI_Base.h"
#include "../Item/Item_Dropped_Base.h"
#include "../Manager/Inventory_Mgr.h"
#include "../Manager/Equip_Mgr.h"
#include "../System/Component/LockOnTargetComponent.h"
#include "../System/DamageType_Base.h"
#include "../Monsters/Monster_Base.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../Manager/GISubsystem_SoundMgr.h"
#include "../Manager/GISubsystem_EffectMgr.h"
#include "../Manager/GISubsystem_StatMgr.h"
#include "Player_SkillComponent.h"
#include "../System/Subsys_ObjectPool.h"
#include "MotionWarpingComponent.h"
#include "RootMotionModifier_SkewWarp.h"
#include "Components/BoxComponent.h"

// Sets default values
APlayer_Base_Knight::APlayer_Base_Knight()
	: CurrentCombo(1)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 캐릭터에게 컨트롤러의 회전 적용안함
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // 캐릭터가 입력된 이동방향으로 자동으로 회전하도록
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1200.0f, 0.0f); // 이동방향으로의 회전 속도
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;

	m_SArm = CreateDefaultSubobject<UPlayer_CameraArm>(TEXT("SArm"));
	m_SArm->SetupAttachment(RootComponent);
	m_SArm->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	m_Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	m_Cam->SetupAttachment(m_SArm);
	m_Cam->bUsePawnControlRotation = false; // 폰과 카메라의 회전분리

	m_SkillComponent = CreateDefaultSubobject<UPlayer_SkillComponent>(TEXT("SkillComp"));
	m_MWComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarp"));

	HitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollision"));
	HitCollision->SetCollisionProfileName(TEXT("PlayerAttack"));
}

void APlayer_Base_Knight::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	APlayerController* pController = Cast<APlayerController>(GetController());

	if ( pController )
	{
		// 마우스 커서 설정
		pController->DefaultMouseCursor = EMouseCursor::Default;
		pController->CurrentMouseCursor = EMouseCursor::Default;
		// 카메라 상하 범위 제한
		pController->PlayerCameraManager->ViewPitchMin = -40.f;
		pController->PlayerCameraManager->ViewPitchMax = 40.f;
		ULocalPlayer* pLocalPlayer = pController->GetLocalPlayer();

		// 향상된 입력 매핑 컨텍스트 추가
		if ( pLocalPlayer && !m_IMC.IsNull() )
		{
			UEnhancedInputLocalPlayerSubsystem* pSubsystem = pLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			pSubsystem->AddMappingContext(m_IMC.LoadSynchronous(), 0);
		}
	}

	m_AnimInst = Cast<UAnimInstance_Knight>(GetMesh()->GetAnimInstance());
	if ( IsValid(m_AnimInst) )
	{
		m_AnimInst->OnDodgeTimeCheck.AddUObject(this, &APlayer_Base_Knight::DodgeTimeCheck);
		m_AnimInst->OnAttackMove.AddUObject(this, &APlayer_Base_Knight::AttackMove);
		m_AnimInst->OnJumpAtk.AddUObject(this, &APlayer_Base_Knight::JumpAttack);
		m_AnimInst->OnMontageEnded.AddDynamic(this, &APlayer_Base_Knight::MontageEnded);
	}
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayer_Base_Knight::ActionTriggerBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayer_Base_Knight::ActionTriggerEndOverlap);

	if ( GetMesh() && HitCollision )
	{
		FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		HitCollision->AttachToComponent(GetMesh(), Rule, TEXT("FX_Sword_Bottom"));
		HitCollision->SetRelativeLocation(FVector(0.f, 0.f, 43.f), true);
		HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called when the game starts or when spawned
void APlayer_Base_Knight::BeginPlay()
{
	Super::BeginPlay();

	ARPGPortfolioGameModeBase* pGameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if ( !IsValid(pGameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode Not Found"));
		return;
	}
	// 위젯은 BeginPlay 시점에 초기화됨
	m_MainUI = pGameMode->GetMainHUD();

	if ( !IsValid(m_MainUI) )
	{
		UE_LOG(LogTemp, Error, TEXT("메인 UI 로드 실패"));
	}
	if ( !IsValid(m_PlayerMontage) )
	{
		UE_LOG(LogTemp, Error, TEXT("플레이어 몽타주 데이터에셋 로드 실패"));
	}

	if ( !IsValid(m_PlayerSound) )
	{
		UE_LOG(LogTemp, Error, TEXT("플레이어 사운드 데이터에셋 로드 실패"));
	}

	USubsys_ObjectPool* PoolSubsystem = GetWorld()->GetSubsystem<USubsys_ObjectPool>();
	CurrentState = PoolSubsystem->GetStateFromPool(EPlayerStateType::IDLE);
	if ( CurrentState )
	{
		CurrentState->Enter(this);
	}

	HitCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayer_Base_Knight::HitBoxBeginOverlap);
	HitCollision->SetGenerateOverlapEvents(true);
	HitCollision->SetUseCCD(true);
}

// Called every frame
void APlayer_Base_Knight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState)
	{
		CurrentState->Update(this, DeltaTime);
	}

	if ( bInputGuard && (CurrentState->GetStateType() == EPlayerStateType::IDLE || CurrentState->GetStateType() == EPlayerStateType::GUARD) )
	{
		fGuardWeight = FMath::Clamp(fGuardWeight + DeltaTime * 10.f, 0.f, 1.f);
	}
	else
	{
		fGuardWeight = FMath::Clamp(fGuardWeight - DeltaTime * 15.f, 0.f, 1.f);
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
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::SprintAction);
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
				InputComp->BindAction(pIADA->IADataArr[i].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &APlayer_Base_Knight::HeavyAttack);
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

void APlayer_Base_Knight::SetState(EPlayerStateType _StateType)
{
	if ( CurrentState->GetStateType() == _StateType )
	{
		if ( _StateType != EPlayerStateType::HIT && _StateType != EPlayerStateType::ATTACK && _StateType != EPlayerStateType::HEAVYATTACK )
		{
			UE_LOG(LogTemp, Warning, TEXT("State is Same"));
			return;
		}
	}

	// 기존 상태를 새로운 상태로 교체
	CurrentState->Exit(this);
	StateMachine* ReturnState = CurrentState.Release();
	USubsys_ObjectPool* PoolSubsystem = GetWorld()->GetSubsystem<USubsys_ObjectPool>();
	PoolSubsystem->ReturnStateToPool(ReturnState->GetStateType(), ReturnState);
	TUniquePtr<StateMachine> NewState = PoolSubsystem->GetStateFromPool(_StateType);
	CurrentState = MoveTemp(NewState); //TUniquePtr는 복사가 불가능하므로, 소유권을 이전하려면 반드시 MoveTemp를 사용해 소유권을 이동해야 함
	CurrentState->Enter(this);
}

////////////////////////////// 인풋액션 함수 //////////////////////////////
//////////////////////////////////////////////////////////////////////////
void APlayer_Base_Knight::MoveAction(const FInputActionInstance& _Instance)
{
	if ( Controller == NULL )
	{
		return;
	}

	if ( CurrentState->GetStateType() == EPlayerStateType::JUMP || 
		CurrentState->GetStateType() == EPlayerStateType::HIT || 
		CurrentState->GetStateType() == EPlayerStateType::ACTION )
	{
		return;
	}

	FVector2D vInput = _Instance.GetValue().Get<FVector2D>();
	// 일부 모션의 경우 후딜레이 모션을 캔슬하고 바로 이동모션으로 전환한다.

	if ( vInput.X != 0.0f )
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		// 컨트롤러의 회전 행렬에서 x축이 가리키고 있는 방향을 구한다.
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, vInput.X);
	}
	if ( vInput.Y != 0.0f )
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		// 컨트롤러의 회전 행렬에서 y축이 가리키고 있는 방향을 구한다.
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, vInput.Y);
	}

	if ( !m_SArm->IsLockedOn() )
	{
		// 락온 중이 아닐때는 캐릭터의 정면으로만 이동하므로
		vInput.X = 1.f;
		vInput.Y = 0.f;
	}
	if (bSprint)
	{
		vInput *= 2;
	}

	fForwardSpeed = vInput.X;
	fRightSpeed = vInput.Y;
	//m_AnimInst->SetLocalVelocityXY(vInput);
}

void APlayer_Base_Knight::RotateAction(const FInputActionInstance& _Instance)
{
	FVector2D vInput = _Instance.GetValue().Get<FVector2D>();

	if (!m_SArm->IsLockedOn())
	{
		if (LockOnFailedTimer.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(LockOnFailedTimer);
		}

		AddControllerYawInput(vInput.X);
		AddControllerPitchInput(-vInput.Y);
	}
}

void APlayer_Base_Knight::JumpAction(const FInputActionInstance& _Instance)
{
	if (CurrentState->GetStateType() != EPlayerStateType::IDLE && CurrentState->GetStateType() != EPlayerStateType::SPRINT)
	{
		return;
	}

	SetState(EPlayerStateType::JUMP);
	ACharacter::Jump();
}

void APlayer_Base_Knight::SprintAction(const FInputActionInstance& _Instance)
{
	if (!(CurrentState->GetStateType() == EPlayerStateType::IDLE || CurrentState->GetStateType() == EPlayerStateType::SPRINT))
	{
		return;
	}

	UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
	// 스테미너가 0일 경우 달리기 불가
	if ( StatMgr->GetPlayerBasePower().CurStamina <= 0.f )
	{
		return;
	}
	// 이동 중에만 토글되도록
	if (GetCharacterMovement()->Velocity.Size2D() <= 0.f || GetCharacterMovement()->GetCurrentAcceleration().IsZero() )
	{
		return;
	}

	bSprint = _Instance.GetValue().Get<bool>();

	if ( bSprint )
	{
		SetState(EPlayerStateType::SPRINT);
	}
	else
	{
		SetState(EPlayerStateType::IDLE);
	}
}

void APlayer_Base_Knight::GuardAction(const FInputActionInstance& _Instance)
{
	bInputGuard = _Instance.GetValue().Get<bool>();


	SetbHoldGuard(bInputGuard);

	//if (CurrentState->GetStateType() == EPlayerStateType::IDLE)
	//{
	//	//m_AnimInst->SetbIsGuard(bInputGuard);
	//}
}

void APlayer_Base_Knight::AttackAction(const FInputActionInstance& _Instance)
{
	if (!IsValid(m_AnimInst))
	{
		UE_LOG(LogTemp, Warning, TEXT("애님인스턴스를 찾을 수 없음"));
		return;
	}

	if ( CurrentState->GetStateType() == EPlayerStateType::DODGE ||
	CurrentState->GetStateType() == EPlayerStateType::USEITEM ||
	CurrentState->GetStateType() == EPlayerStateType::USESKILL_1 ||
	CurrentState->GetStateType() == EPlayerStateType::HIT )
	{
		return;
	}

	// 공격중인 상태에서 다음 공격 입력 기간이 아닐경우
	if ( !bNextAtkCheckOn && CurrentCombo != 1 )
	{
		return;
	}

	if ( bHeavyHold && CurrentState->GetStateType() == EPlayerStateType::ATTACK)
	{
		return;
	}
	bIsAttacking = _Instance.GetValue().Get<bool>();

	// 공격 중일 경우 어택 스테이트에서 다음 콤보 발동 처리
	if ( CurrentCombo != 1 )
	{
		return;
	}

	AttackStart();
}

void APlayer_Base_Knight::HeavyAttack(const FInputActionInstance& _Instance)
{
	bHeavyHold = _Instance.GetValue().Get<bool>();
}

void APlayer_Base_Knight::DodgeAction(const FInputActionInstance& _Instance)
{
	// 서 있거나 이동, 달리기 중에만 회피 가능
	if ( !(	CurrentState->GetStateType() == EPlayerStateType::IDLE || 
			//CurrentState->GetStateType() == EPlayerStateType::DODGE ||
			CurrentState->GetStateType() == EPlayerStateType::SPRINT)
		)
	{
		return;
	}
	
	//if (bInvalidInput)
	//{
	//	return;
	//}

	// 현재 스태미나가 소비량보다 적을 경우 회피 불가
	UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
	if ( !ConsumeStaminaForMontage(EPlayerMontage::DODGE_FW) )
	{
		return;
	}

	SetState(EPlayerStateType::DODGE);
}

void APlayer_Base_Knight::ParryAction(const FInputActionInstance& _Instance)
{
	// 미구현
	//m_AnimInst->Montage_Play(m_PlayerMontage->GetPlayerMontage(EPlayerMontage::PARRY));
}

void APlayer_Base_Knight::LockOnToggleAction(const FInputActionInstance& _Instance)
{
	bool bTargetLocked = m_SArm->ToggleCameraLockOn(_Instance.GetValue().Get<bool>());
	// 락온 중이 아닐 때 락온 대상을 찾지 못할 경우 카메라 리셋
	if (!bTargetLocked)
	{
		GetWorld()->GetTimerManager().ClearTimer(LockOnFailedTimer);
		GetWorld()->GetTimerManager().SetTimer(LockOnFailedTimer, FTimerDelegate::CreateUObject(this, &APlayer_Base_Knight::ResetCamera, GetActorRotation()), 0.01f, true);
	}
}

void APlayer_Base_Knight::SwitchLockOnTarget(const FInputActionInstance& _Instance)
{
	float SwitchDirection = _Instance.GetValue().Get<float>();

	if (SwitchDirection > 0.f)
	{
		m_SArm->SwitchTarget(ELockOnDirection::Left);
	}
	else if (SwitchDirection < 0.f)
	{
		m_SArm->SwitchTarget(ELockOnDirection::Right);
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
		pGameMode->CloseSubMenu();
		return;
	}

	if (m_MainUI->IsOpendMenu())
	{
		CloseMenuUI();
	}
	else
	{
		FInputModeGameAndUI GAU;
		GAU.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
		GAU.SetHideCursorDuringCapture(false);
		pController->SetInputMode(GAU);

		UGameplayStatics::PlaySound2D(GetWorld(), GETMENUSOUND(EMenuSound::MENU_OPEN));
		m_MainUI->MenuVisibility(ESlateVisibility::SelfHitTestInvisible);
		pController->bShowMouseCursor = true;
		pController->SetPause(true);
	}
}

void APlayer_Base_Knight::ActionCommand(const FInputActionInstance& _Instance)
{
	//if ( CurrentState->GetStateType() == EPlayerStateType::JUMP || bInvalidInput || m_AnimInst->GetbIsGuard())	
	if ( CurrentState->GetStateType() != EPlayerStateType::IDLE && CurrentState->GetStateType() != EPlayerStateType::SPRINT )
	{
		return;
	}
	if ( bInputGuard )
	{
		return;
	}

	if (!OverlapInteractionArr.IsEmpty())
	{
		if ( OverlapInteractionArr[OverlapInteractionArr.Num() - 1]->_getUObject()->IsA(AItem_Dropped_Base::StaticClass()) )
		{
			m_AnimInst->Montage_Play(m_PlayerMontage->GetPlayerMontage(EPlayerMontage::ACTION_ITEM));
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_PlayerSound->GetPlayerSound(EPlayerSound::GETITEM), GetActorLocation());
		}
		else
		{
			m_AnimInst->Montage_Play(m_PlayerMontage->GetPlayerMontage(EPlayerMontage::ACTION_PROP));
		}

		SetState(EPlayerStateType::ACTION);
		OverlapInteractionArr[OverlapInteractionArr.Num() - 1]->Interaction(this);
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
		m_MainUI->RenewQuickSlotUI(Idx);
	}
}

void APlayer_Base_Knight::UseLowerQuickSlot(const FInputActionInstance& _Instance)
{
	//if ( CurrentState->GetStateType() != EPlayerStateType::IDLE || m_AnimInst->GetbIsGuard() )
	if ( CurrentState->GetStateType() != EPlayerStateType::IDLE )
	{
		return;
	}

	if ( !bItemDelay )
	{
		int32 iCurIdx = UEquip_Mgr::GetInst(GetWorld())->GetCurrentIndex();
		if ( UEquip_Mgr::GetInst(GetWorld())->QuickSlotValidForIdx(iCurIdx) )
		{
			TSharedPtr<FInvenItemRow> pItem = UEquip_Mgr::GetInst(GetWorld())->GetQSItemForIndex(iCurIdx);
			UseItem(pItem->ID, pItem->EquipedSlot);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("퀵슬롯에 지정된 아이템 없음"));
			return;
		}
	}
}
void APlayer_Base_Knight::UseSkill_1(const FInputActionInstance& _Instance)
{
	if ( !IsValid(m_AnimInst) )
	{
		UE_LOG(LogTemp, Warning, TEXT("애님인스턴스를 찾을 수 없음"));
		return;
	}
	if (bInputGuard || CurrentState->GetStateType() == EPlayerStateType::JUMP)
	{
		return;
	}
	if ( m_SkillComponent->GetSkillName() == ESkillName::NONE )
	{
		return;
	}
	FSkillAsset* Skill = m_SkillComponent->GetEquippedSkill();

	UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
	if ( StatMgr->GetPlayerBasePower().CurMP < Skill->MP_Consumption || StatMgr->GetPlayerBasePower().CurStamina < Skill->Stamina_Consumption )
	{
		return;
	}

	ConsumeMP(Skill->MP_Consumption);
	ConsumeStamina(Skill->Stamina_Consumption);
	m_AnimInst->Montage_Play(Skill->Animation);
	//SetState(EPlayerStateType::USESKILL_1);
	// ShotProjectile로
}
//////////////////////////////////////////////////////////////////////////
////////////////////////////// 인풋액션 함수 //////////////////////////////

// 공격 모션 중 이동
void APlayer_Base_Knight::AttackMove()
{
	if ( m_SArm->IsLockedOn() )
	{
		m_SArm->m_Target->GetComponentLocation();
		float fDist = ( GetActorLocation() - m_SArm->m_Target->GetComponentLocation() ).Size();
		float fImpulsePower = 600.f;
		if ( fDist <= 200.f )
		{
			fImpulsePower = FMath::Clamp(fDist, 1.f, 200.f);
		}
		GetCharacterMovement()->AddImpulse(GetActorForwardVector() * fImpulsePower, true);
	}
	else
	{
		GetCharacterMovement()->AddImpulse(GetActorForwardVector() * 500.f, true);
	}
}

void APlayer_Base_Knight::JumpAttack()
{
	GetWorldTimerManager().SetTimer(JumpAtkTimer, [this]
		{
			// 바닥에 착지하면 애니메이션을 다시 재생
			if ( !GetCharacterMovement()->IsFalling() )
			{
				m_AnimInst->Montage_Resume(m_PlayerMontage->GetPlayerMontage(EPlayerMontage::JUMPATTACK));
				GetWorldTimerManager().ClearTimer(JumpAtkTimer);
			}
		}
	, 0.001f, true);
}

void APlayer_Base_Knight::ApplyPointDamage(FHitResult const& HitInfo, EATTACK_TYPE _AtkType, EPlayerMontage _AtkMontage)
{
	m_AnimInst->Montage_Pause();
	GetWorld()->GetTimerManager().SetTimer(HitStiffTimer, [this, _AtkMontage]()
	{
		m_AnimInst->Montage_Resume(NULL);
		GetWorld()->GetTimerManager().ClearTimer(HitStiffTimer);
	},
	0.1f, false);

	float iDamage = 0.f;
	UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();

	switch (_AtkType)
	{
	case EATTACK_TYPE::PHYSIC_MELEE:
	case EATTACK_TYPE::PHYSIC_RANGE:
		iDamage = StatMgr->GetPlayerBasePower().PhysicAtk;
		if (_AtkMontage == EPlayerMontage::HEAVYATTACK)
		{
			iDamage = iDamage * 1.5f;
		}
		break;
	case EATTACK_TYPE::MAGIC_MELEE:
	case EATTACK_TYPE::MAGIC_RANGE:
		iDamage = StatMgr->GetPlayerBasePower().MagicAtk;
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
	UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();

	// 받은 공격타입에 따라 몬스터의 방어력 설정
	float fPlayerDef = 0.f;
	switch (pDamageType->GetAtkType())
	{
	case EATTACK_TYPE::PHYSIC_MELEE:
	case EATTACK_TYPE::PHYSIC_RANGE:
		fPlayerDef = StatMgr->GetPlayerBasePower().PhysicDef;
		break;
	case EATTACK_TYPE::MAGIC_MELEE:
	case EATTACK_TYPE::MAGIC_RANGE:
		fPlayerDef = StatMgr->GetPlayerBasePower().MagicDef;
		break;
	default:
		break;
	}

	FinalDamage = FMath::Clamp(FinalDamage - fPlayerDef, 0.f, FinalDamage);
	int32 iCurHP = StatMgr->GetPlayerBasePower().CurHP;
	iCurHP = FMath::Clamp(iCurHP - FinalDamage, 0.f, StatMgr->GetPlayerBasePower().MaxHP);

	StatMgr->SetPlayerCurrentHP(iCurHP);

	if ( iCurHP <= 0.f && GetController() )
	{
		// 사망처리
		return 0.f;
	}

	// 피격 이펙트 스폰
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		UGISubsystem_EffectMgr* EffectMgr = GetGameInstance()->GetSubsystem<UGISubsystem_EffectMgr>();
		
		UParticleSystem* Particle = EffectMgr->GetHitEffect();
		UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAttached(Particle, GetMesh(), PointDamageEvent->HitInfo.BoneName);
	}

	// 피격 시 공격대상 반대방향으로 밀려나도록
	FVector LaunchVec = GetActorLocation() - DamageCauser->GetActorLocation();
	FVector LaunchForce = LaunchVec.GetSafeNormal() * 300.f;
	LaunchCharacter(LaunchForce, false, false);
	
	// 피격 사운드 재생
	if (!IsValid(m_PlayerSound->GetPlayerSound(EPlayerSound::HIT)))
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 피격사운드 로드 실패"));
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_PlayerSound->GetPlayerSound(EPlayerSound::HIT), GetActorLocation());
	}

	// 피격 애니메이션 재생
	m_AnimInst->Montage_Stop(1.f);
	if ( !IsValid(m_PlayerMontage->GetPlayerMontage(EPlayerMontage::HIT)) )
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 피격몽타주 로드 실패"));
	}
	else
	{
		m_AnimInst->Montage_Play(m_PlayerMontage->GetPlayerMontage(EPlayerMontage::HIT));
	}

	SetState(EPlayerStateType::HIT);
	ResetVarsOnHitState();

	return 0.0f;
}

// 공격 트레이스 함수
void APlayer_Base_Knight::AttackHitCheck()
{
	TArray<FHitResult> OutHits;
	FCollisionQueryParams Params(NAME_None, false, this);
	FVector CurrentLoc = HitCollision->GetComponentLocation();  // 현재 위치
	FQuat Rotation = HitCollision->GetComponentQuat();             // 현재 회전
	float CapsuleRadius = HitCollision->GetUnscaledCapsuleRadius();
	float CapsuleHalfHeight = HitCollision->GetUnscaledCapsuleHalfHeight();
	FCollisionShape Shape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
	if ( PrevTraceLoc.IsZero() )
	{
		PrevTraceLoc = CurrentLoc;
	}

	bool bResult = GetWorld()->SweepMultiByChannel
	(
		OutHits,
		PrevTraceLoc,
		CurrentLoc,
		Rotation,
		ECollisionChannel::ECC_GameTraceChannel5,	// 트레이스 채널 설정은 Block으로 해놔야 HitResult에 걸림(Block으로 해도 밀려나진 않음)
		Shape,
		Params
	);

	DrawDebugAttackTrace(GetWorld(), PrevTraceLoc, CurrentLoc, Rotation, Shape, bResult, OutHits, FLinearColor::Green, FLinearColor::Red, 1.f);
	PrevTraceLoc = CurrentLoc;
	if ( bResult )
	{
		for ( FHitResult HitInfo : OutHits )
		{
			if ( HitInfo.GetActor()->IsValidLowLevel() )
			{
				for ( AActor* HitActor : HitActorArr )
				{
					if ( HitInfo.GetActor() == HitActor )
					{
						return;
					}
				}

				if ( m_AnimInst->Montage_IsPlaying(m_PlayerMontage->GetPlayerMontage(EPlayerMontage::HEAVYATTACK)) )
				{
					ApplyPointDamage(HitInfo, EATTACK_TYPE::PHYSIC_MELEE, EPlayerMontage::HEAVYATTACK);
				}
				else
				{
					ApplyPointDamage(HitInfo, EATTACK_TYPE::PHYSIC_MELEE, EPlayerMontage::ATTACK);
				}

				HitActorArr.Add(HitInfo.GetActor());
			}
		}
	}
}

void APlayer_Base_Knight::AttackStart()
{
	// 점프공격
	if ( CurrentState->GetStateType() == EPlayerStateType::JUMP )
	{
		if ( GetRootComponent()->GetRelativeRotation().UnrotateVector(GetCharacterMovement()->Velocity).Z >= 30.f )
		{
			bIsAttacking = ConsumeStaminaForMontage(EPlayerMontage::JUMPATTACK);
			if ( bIsAttacking )
			{
				SetState(EPlayerStateType::JUMPATTACK);
			}
		}
	}
	else
	{
		if ( bHeavyHold )
		{
			bIsAttacking = ConsumeStaminaForMontage(EPlayerMontage::HEAVYATTACK);
			if ( bIsAttacking )
			{
				SetState(EPlayerStateType::HEAVYATTACK);
			}
		}
		else if ( CurrentState->GetStateType() != EPlayerStateType::HEAVYATTACK )
		{
			bIsAttacking = ConsumeStaminaForMontage(EPlayerMontage::ATTACK);
			if ( bIsAttacking )
			{
				// 약공격
				SetState(EPlayerStateType::ATTACK);
			}
		}
	}
}

void APlayer_Base_Knight::HitBoxActivate(bool _Activate)
{
	if ( _Activate )
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitbox On"));
		HitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitbox Off"));
		HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	HitActorArr.Empty();
}

bool APlayer_Base_Knight::ConsumeStaminaForMontage(EPlayerMontage _Montage)
{
	float fConsumption = 0.f;
	switch ( _Montage )
	{
	case EPlayerMontage::DODGE_FW:
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

	return ConsumeStamina(fConsumption);
}

bool APlayer_Base_Knight::ConsumeStamina(float _Consumption)
{
	UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
	if ( !IsValid(StatMgr) )
	{
		UE_LOG(LogTemp, Warning, TEXT("스탯 매니저 가져오기 실패"));
		return false;
	}
	if ( StatMgr->GetPlayerBasePower().CurStamina < _Consumption )
	{
		return false;
	}

	StatMgr->SetPlayerCurrentStamina(StatMgr->GetPlayerBasePower().CurStamina - _Consumption);
	return true;
}

bool APlayer_Base_Knight::ConsumeMP(float _Consumption)
{
	UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
	if ( !IsValid(StatMgr) )
	{
		UE_LOG(LogTemp, Warning, TEXT("스탯 매니저 가져오기 실패"));
		return false;
	}
	if ( StatMgr->GetPlayerBasePower().CurMP < _Consumption )
	{
		return false;
	}

	StatMgr->SetPlayerCurrentMP(StatMgr->GetPlayerBasePower().CurMP - _Consumption);
	return true;
}

bool APlayer_Base_Knight::BlockEnemyAttack(float _Damage, FVector _MonDir)
{
	// 플레이어와 몬스터가 바라보는 방향 사이의 각도 구하기
	// fDegree가 180도에 가까울수록 서로 마주보고 있음(몬스터가 플레이어의 정면에 있음)
	// fDegree가 0도에 가까울수록 서로 같은 방향을 보고 있음(몬스터가 플레이어의 뒤를 노림)
	// 플레이어 정면 기준으로 160도 각도 안에서 공격했을 경우 막히도록
	// 보스의 경우 히트한 트레이스 정보의 노멀 벡터를 반전시켜 히트 콜리전에서 플레이어를 바라보는 벡터를 구해 적용함
	FVector vPlayerDir = GetActorForwardVector().GetSafeNormal();
	float fDot = FVector::DotProduct(vPlayerDir, _MonDir);
	float fAcosAngle = FMath::Acos(fDot);
	float fDegree = FMath::RadiansToDegrees(fAcosAngle);

	// 가드 범위 밖에서 공격받을 경우 가드 실패
	if (fDegree < 100.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("가드 실패"));
		return false;
	}

	UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
	// 몬스터의 공격력의 10분의 1 만큼 스태미나 감소
	StatMgr->SetPlayerCurrentStamina(FMath::Clamp(StatMgr->GetPlayerBasePower().CurStamina - _Damage * 0.1f, 0.f, StatMgr->GetPlayerBasePower().CurStamina));
	if (StatMgr->GetPlayerBasePower().CurStamina <= 0.f)
	{
		// 방어 풀리고 경직상태 되도록
		SetState(EPlayerStateType::HIT);
	}
	else
	{
		SetState(EPlayerStateType::GUARD);
	}

	return true;
}

void APlayer_Base_Knight::BreakLockOn()
{
	m_SArm->BreakLockOnTarget();
}

void APlayer_Base_Knight::ResetCamera(FRotator _Rotate)
{
	FRotator NewRot = FMath::RInterpTo(GetControlRotation(), _Rotate, 0.01f, 10.f);
	GetController()->SetControlRotation(NewRot);
	if ( GetControlRotation().Equals(_Rotate, 1.f) )
	{
		GetWorld()->GetTimerManager().ClearTimer(LockOnFailedTimer);
	}
}

void APlayer_Base_Knight::ShotProjectile()
{
	// 투사체 생성위치	
	FVector ProjectileLocation = GetActorLocation() + FVector(0.f, 0.f, 10.f) + GetActorForwardVector() * 200.f;

	// 투사체 발사 방향
	FVector vDir = GetActorForwardVector() * 1000.f;
	m_SkillComponent->ShotSkillProj(ProjectileLocation, GetActorRotation(), vDir);
}

void APlayer_Base_Knight::UseItem(EITEM_ID _ID, EEQUIP_SLOT _Slot)
{
	UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
	EPlayerSound SoundEnum = EPlayerSound::EMPTY;

	FGameItemInfo* pItemInfo = UInventory_Mgr::GetInst(GetWorld())->GetItemInfo(_ID);
	if ( pItemInfo->Restore_HP >= 0 )
	{
		StatMgr->SetPlayerCurrentHP(FMath::Clamp(StatMgr->GetPlayerBasePower().CurHP + pItemInfo->Restore_HP, 0.f, StatMgr->GetPlayerBasePower().MaxHP));
		SoundEnum = EPlayerSound::USERESTORE;
	}
	if ( pItemInfo->Restore_MP >= 0 )
	{
		StatMgr->SetPlayerCurrentMP(FMath::Clamp(StatMgr->GetPlayerBasePower().CurMP + pItemInfo->Restore_MP, 0.f, StatMgr->GetPlayerBasePower().MaxMP));
		SoundEnum = EPlayerSound::USERESTORE;
	}
	if ( pItemInfo->Gained_Soul >= 0 )
	{
		GainMonsterSoul(pItemInfo->Gained_Soul);
		SoundEnum = EPlayerSound::USESOUL;
	}

	UGISubsystem_EffectMgr* EffectMgr = GetGameInstance()->GetSubsystem<UGISubsystem_EffectMgr>();
	EffectMgr->SpawnEffectAttached(pItemInfo->EffectType, GetMesh(), FName("Root"), FVector(0.f, 0.f, 1.f), FRotator(0.f), EAttachLocation::SnapToTargetIncludingScale, true);
	if(!IsValid(m_PlayerSound->GetPlayerSound(SoundEnum)))
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템 사운드 로드 실패"));
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_PlayerSound->GetPlayerSound(SoundEnum), GetActorLocation());
	}

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
	}

	m_AnimInst->Montage_Play(m_PlayerMontage->GetPlayerMontage(EPlayerMontage::USEITEM));

	SetState(EPlayerStateType::ACTION);
	// 아이템 사용후 대기시간 on
	ItemDelaytime(1.f);
}

void APlayer_Base_Knight::ItemDelaytime(float _DelayPercent)
{
	bItemDelay = true;

	m_MainUI->SetQuickSlotUIOpacity(0.5f, false);
	m_MainUI->SetQuickSlotUIDelay(1.f);
	fDelayRate = _DelayPercent;

	GetWorldTimerManager().SetTimer(ItemDelayTimer, FTimerDelegate::CreateWeakLambda(this, [_DelayPercent, this]
		{
			fDelayRate = FMath::Clamp((fDelayRate - 0.01f) / _DelayPercent, 0.f, _DelayPercent);
			m_MainUI->SetQuickSlotUIDelay(fDelayRate);
			if ( fDelayRate <= 0.f )
			{
				bItemDelay = false;
				m_MainUI->SetQuickSlotUIOpacity(1.f, false);
				GetWorldTimerManager().ClearTimer(ItemDelayTimer);
			}
		})
	,0.01f, true);
}

void APlayer_Base_Knight::GainMonsterSoul(int32 _GainedSoul)
{
	UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
	StatMgr->PlayerGainSoul(_GainedSoul);
	m_MainUI->RenewAmountSoul(_GainedSoul);
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

	m_MainUI->MenuVisibility(ESlateVisibility::Collapsed);
	UGameplayStatics::PlaySound2D(GetWorld(), GETMENUSOUND(EMenuSound::MENU_CLOSE));
}

void APlayer_Base_Knight::ResetVarsOnHitState()
{
	// 현재 행동상태 해제
	ClearMontageRelatedTimer();
	//bAtkTrace = false;
	bNextAtkCheckOn = false;
}

void APlayer_Base_Knight::ClearMontageRelatedTimer()
{
	GetWorldTimerManager().ClearTimer(BlockReactTimer);
	GetWorldTimerManager().ClearTimer(JumpAtkTimer);
	GetWorldTimerManager().ClearTimer(HitStiffTimer);
}

void APlayer_Base_Knight::MotionWarping_Attack(UAnimSequenceBase* _Anim, float _EndTime)
{
	FVector PlayerLoc = GetActorLocation();
	m_AnimInst->RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
	if ( GetbToggleLockOn() )
	{
		const FVector TargetLoc = GetLockOnTarget()->GetComponentLocation();
		float Distance = ( TargetLoc - PlayerLoc ).Size();

		FVector MoveLoc = PlayerLoc + (TargetLoc - PlayerLoc).GetSafeNormal() * 60.f;
		if ( Distance <= 300.f )
		{
			if ( Distance >= 200.f )
			{
				// 락온 대상과의 거리가 2m 이상 3m 이하면 translation on
				// 캐릭터가 대상 앞 2m 거리로 워프하도록 하여 공격 모션의 이동 거리 때문에 적의 뒤까지 이동하지 않도록
				MoveLoc = ( ( PlayerLoc - TargetLoc ).GetSafeNormal() * 200.f ) + TargetLoc;
			}
			else
			{
				// 락온 대상과의 거리가 2m 미만이면 앞으로 이동하지 않고 현재 위치에 멈추도록 설정
				MoveLoc = PlayerLoc;
				m_AnimInst->RootMotionMode = ERootMotionMode::IgnoreRootMotion;
			}
			URootMotionModifier_SkewWarp::AddRootMotionModifierSkewWarp(m_MWComponent, _Anim, 0.f, _EndTime, TEXT("AttackWarp"), EWarpPointAnimProvider::None, FTransform(), TEXT("None"), true, true, true);
		} 
		else
		{
			// 락온 대상과의 거리가 3m 보다 크면 translation off
			// 루트모션의 기본 이동거리를 유지한다.
			URootMotionModifier_SkewWarp::AddRootMotionModifierSkewWarp(m_MWComponent, _Anim, 0.f, _EndTime, TEXT("AttackWarp"), EWarpPointAnimProvider::None, FTransform(), TEXT("None"), false, true, true);
		}

		FRotator TargetRot = FRotationMatrix::MakeFromX( (TargetLoc - PlayerLoc).GetSafeNormal() ).Rotator();
		TargetRot.Pitch = 0.f;
		TargetRot.Roll = 0.f;
		m_MWComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("AttackWarp"), MoveLoc, TargetRot);

		return;
	}

	const FVector InputVector = GetLastMovementInputVector();
	if ( InputVector.IsZero() )
	{
		m_MWComponent->RemoveWarpTarget(TEXT("AttackWarp"));
	}
	else
	{
		const FRotator TargetRot = FRotationMatrix::MakeFromX(InputVector).Rotator();
		// 락온 중이 아닐 때는 translation off
		URootMotionModifier_SkewWarp::AddRootMotionModifierSkewWarp(m_MWComponent, _Anim, 0.f, _EndTime, TEXT("AttackWarp"), EWarpPointAnimProvider::None, FTransform(), TEXT("None"), false, true, true);
		m_MWComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("AttackWarp"), PlayerLoc + GetActorForwardVector() * 60.f, TargetRot);
	}
}

void APlayer_Base_Knight::MotionWarping_Dodge(UAnimSequenceBase* _Anim, float _EndTime)
{
	const FVector InputVector = GetLastMovementInputVector();
	if ( InputVector.IsZero() )
	{
		m_MWComponent->RemoveWarpTarget(TEXT("DodgeWarp"));
	}
	else
	{
		const FRotator TargetRot = FRotationMatrix::MakeFromX(InputVector).Rotator();
		// 락온 중이 아닐 때는 translation off
		URootMotionModifier_SkewWarp::AddRootMotionModifierSkewWarp(m_MWComponent, _Anim, 0.f, _EndTime, TEXT("DodgeWarp"), EWarpPointAnimProvider::None, FTransform(), TEXT("None"), false, true, true);
		m_MWComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("DodgeWarp"), FVector::ZeroVector, TargetRot);
	}
}

// 무적시간 동안 데미지 안받도록 설정
void APlayer_Base_Knight::DodgeTimeCheck(bool _Dodge)
{
	if (_Dodge)
	{
		SetCanBeDamaged(false);
	}
	else
	{
		SetCanBeDamaged(true);
	}
}

void APlayer_Base_Knight::MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if ( Montage == m_PlayerMontage->GetPlayerMontage(EPlayerMontage::HIT) )
	{
		SetState(EPlayerStateType::IDLE);
		return;
	}
}

void APlayer_Base_Knight::HitBoxBeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("HitBoxOverlap"));

	// 캡슐의 위치 및 크기 정보 가져오기
	FVector StartLocation = HitCollision->GetComponentLocation();  // 현재 위치
	FQuat Rotation = HitCollision->GetComponentQuat();             // 현재 회전
	float CapsuleRadius = HitCollision->GetUnscaledCapsuleRadius();
	float CapsuleHalfHeight = HitCollision->GetUnscaledCapsuleHalfHeight();

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);  // 자기 자신은 무시
	Params.bTraceComplex = true;   // 복잡한 충돌 검사

	// 같은 크기와 위치로 스윕 수행
	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults,                              // 충돌 결과 저장
		StartLocation,                           // 시작 위치
		StartLocation,                           // 종료 위치 (이동 없음)
		Rotation,                                // 현재 회전 값
		ECollisionChannel::ECC_GameTraceChannel5,                   // 충돌 트레이스 채널
		FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),  // 캡슐 콜리전 크기
		Params                                  // 충돌 관련 설정
	);

	FColor color;
	color = bResult ? FColor::Red : FColor::Green;
	FVector vMidpoint = HitCollision->GetComponentLocation();
	DrawDebugCapsule(GetWorld(), vMidpoint, CapsuleHalfHeight, CapsuleRadius, Rotation, color, false, 1.f);

	if ( bResult )
	{
		for ( FHitResult HitInfo : HitResults )
		{
			if ( HitInfo.GetActor()->IsValidLowLevel() )
			{
				for ( AActor* HitActor : HitActorArr )
				{
					if ( HitInfo.GetActor() == HitActor )
					{
						return;
					}
				}

				if ( m_AnimInst->Montage_IsPlaying(m_PlayerMontage->GetPlayerMontage(EPlayerMontage::HEAVYATTACK)) )
				{
					ApplyPointDamage(HitInfo, EATTACK_TYPE::PHYSIC_MELEE, EPlayerMontage::HEAVYATTACK);
				}
				else
				{
					ApplyPointDamage(HitInfo, EATTACK_TYPE::PHYSIC_MELEE, EPlayerMontage::ATTACK);
				}

				HitActorArr.Add(HitInfo.GetActor());
			}
		}
	}
}

void APlayer_Base_Knight::ActionTriggerBeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)
{
	FName TriggerName = _OtherPrimitiveCom->GetCollisionProfileName();
	if ( TriggerName.IsEqual(FName(TEXT("InteractionTrigger"))) )
	{
		if ( _OtherActor->GetClass()->ImplementsInterface(UPlayerInteraction::StaticClass()) )
		{
			TScriptInterface<IPlayerInteraction> Interaction = TScriptInterface<IPlayerInteraction>(_OtherActor);
			m_MainUI->SetMainMessageUI(Interaction->GetCommand_Key(), Interaction->GetCommand_Name());
			m_MainUI->ShowMainMessageUI(true);
			OverlapInteractionArr.Emplace(Interaction);
		}
	}
}

void APlayer_Base_Knight::ActionTriggerEndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index)
{
	FName TriggerName = _OtherPrimitiveCom->GetCollisionProfileName();

	if ( TriggerName.IsEqual(FName(TEXT("InteractionTrigger"))) )
	{
		if ( _OtherActor->GetClass()->ImplementsInterface(UPlayerInteraction::StaticClass()) )
		{
			// 오버랩 상태의 트리거에서 떨어지거나 아이템을 습득하여 트리거 오버랩이 끝났을 때
			for ( int32 i = 0; i < OverlapInteractionArr.Num(); ++i )
			{
				if ( OverlapInteractionArr[ i ]->_getUObject()->GetName().Equals(_OtherActor->GetName()) )
				{
					OverlapInteractionArr.RemoveAt(i);
					break;
				}
			}

			if ( OverlapInteractionArr.IsEmpty() )
			{
				// 아이템 습득 메시지가 표시중일 때
				if ( m_MainUI->GetRootMessageDisplayed() )
				{
					m_MainUI->SetMainMessageUI(FText::FromString(L"F"), FText::FromString(L"확인"));
					m_MainUI->ShowMainMessageUI(true);
				}
				else
				{
					m_MainUI->ShowMainMessageUI(false);
				}
			}
		}
	}
}

void APlayer_Base_Knight::DrawDebugAttackTrace(const UWorld* World, const FVector& Start, const FVector& End, FQuat& Rot, const FCollisionShape& CollisionShape, bool bHit, const TArray<FHitResult>& OutHits, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	if ( CollisionShape.ShapeType == ECollisionShape::Capsule )
	{
		FVector CapsuleTip = Rot.RotateVector(FVector(0, 0, CollisionShape.GetCapsuleHalfHeight()));
		::DrawDebugCapsule(World, Start, CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), Rot, TraceColor.ToFColor(true), false, DrawTime);
		::DrawDebugCapsule(World, End, CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), Rot, TraceColor.ToFColor(true), false, DrawTime);
		::DrawDebugLine(World, Start + CapsuleTip, End + CapsuleTip, TraceColor.ToFColor(true), false, DrawTime);
		::DrawDebugLine(World, Start - CapsuleTip, End - CapsuleTip, TraceColor.ToFColor(true), false, DrawTime);
		if ( bHit && OutHits.Last().bBlockingHit )
		{
			FVector const BlockingHitPoint = OutHits.Last().Location;
			::DrawDebugCapsule(World, BlockingHitPoint, CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), Rot, TraceHitColor.ToFColor(true), false, DrawTime);
		}
	}
	else if ( CollisionShape.ShapeType == ECollisionShape::Box )
	{
		FBox StartBox(Start - CollisionShape.GetExtent(), Start + CollisionShape.GetExtent());
		FBox EndBox(End - CollisionShape.GetExtent(), End + CollisionShape.GetExtent());

		FVector direction = Rot.Vector();
		DrawDebugBox(World, StartBox.GetCenter(), StartBox.GetExtent(), Rot, TraceColor.ToFColor(true), false, DrawTime);
		DrawDebugBox(World, EndBox.GetCenter(), EndBox.GetExtent(), Rot, TraceColor.ToFColor(true), false, DrawTime);

		// connect the vertices of start box with end box
		// since start_box and end_box are already translated applying rotation will lead to rotating translated box around origin.
		// so construct each vertices of the box as if the center of box is at origin and rotate the point and then translate it back to start and end positions.
		// 시작 상자부터 끝 상자까지 각 8개의 꼭지점의 좌표에서 라인을 쏜다.
		FTransform const box_transform(Rot);
		// (-1, -1, 1)
		FVector transform_point = box_transform.TransformPosition(FVector(-CollisionShape.GetExtent().X, -CollisionShape.GetExtent().Y, CollisionShape.GetExtent().Z));
		DrawDebugLine(World, transform_point + Start, transform_point + End, TraceColor.ToFColor(true), false, DrawTime);
		// (1, -1, 1)
		transform_point = box_transform.TransformPosition(FVector(CollisionShape.GetExtent().X, -CollisionShape.GetExtent().Y, CollisionShape.GetExtent().Z));
		DrawDebugLine(World, transform_point + Start, transform_point + End, TraceColor.ToFColor(true), false, DrawTime);
		// (1, 1, 1)
		transform_point = box_transform.TransformPosition(FVector(CollisionShape.GetExtent().X, CollisionShape.GetExtent().Y, CollisionShape.GetExtent().Z));
		DrawDebugLine(World, transform_point + Start, transform_point + End, TraceColor.ToFColor(true), false, DrawTime);
		// (-1, 1, 1)
		transform_point = box_transform.TransformPosition(FVector(-CollisionShape.GetExtent().X, CollisionShape.GetExtent().Y, CollisionShape.GetExtent().Z));
		DrawDebugLine(World, transform_point + Start, transform_point + End, TraceColor.ToFColor(true), false, DrawTime);
		//(-1, -1, -1)
		transform_point = box_transform.TransformPosition(FVector(-CollisionShape.GetExtent().X, -CollisionShape.GetExtent().Y, -CollisionShape.GetExtent().Z));
		DrawDebugLine(World, transform_point + Start, transform_point + End, TraceColor.ToFColor(true), false, DrawTime);
		// (1, -1, -1)
		transform_point = box_transform.TransformPosition(FVector(CollisionShape.GetExtent().X, -CollisionShape.GetExtent().Y, -CollisionShape.GetExtent().Z));
		DrawDebugLine(World, transform_point + Start, transform_point + End, TraceColor.ToFColor(true), false, DrawTime);
		//(1, 1, -1)
		transform_point = box_transform.TransformPosition(FVector(CollisionShape.GetExtent().X, CollisionShape.GetExtent().Y, -CollisionShape.GetExtent().Z));
		DrawDebugLine(World, transform_point + Start, transform_point + End, TraceColor.ToFColor(true), false, DrawTime);
		// (-1, 1, -1)
		transform_point = box_transform.TransformPosition(FVector(-CollisionShape.GetExtent().X, CollisionShape.GetExtent().Y, -CollisionShape.GetExtent().Z));
		DrawDebugLine(World, transform_point + Start, transform_point + End, TraceColor.ToFColor(true), false, DrawTime);

		if ( bHit && OutHits.Last().bBlockingHit )
		{
			FVector const BlockingHitPoint = OutHits.Last().Location;
			DrawDebugBox(World, BlockingHitPoint, CollisionShape.GetExtent(), Rot, TraceHitColor.ToFColor(true), false, DrawTime);
		}
	}
	else if ( CollisionShape.ShapeType == ECollisionShape::Sphere )
	{
		DrawDebugSphere(World, Start, CollisionShape.GetSphereRadius(), FMath::Max(CollisionShape.GetSphereRadius() / 4.f, 2.f), TraceColor.ToFColor(true), false, DrawTime);
		DrawDebugSphere(World, End, CollisionShape.GetSphereRadius(), FMath::Max(CollisionShape.GetSphereRadius() / 4.f, 2.f), TraceColor.ToFColor(true), false, DrawTime);
		FVector vCenter = FMath::Lerp(Start, End, 0.5f);
		DrawDebugCapsule(World, vCenter, ( Start - End ).Size() * 0.5f, CollisionShape.GetSphereRadius(), FRotationMatrix::MakeFromZ(End - Start).ToQuat(), TraceColor.ToFColor(true), false, DrawTime);

		if ( bHit && OutHits.Last().bBlockingHit )
		{
			FVector const BlockingHitPoint = OutHits.Last().Location;
			DrawDebugSphere(World, BlockingHitPoint, CollisionShape.GetSphereRadius(), FMath::Max(CollisionShape.GetSphereRadius() / 4.f, 2.f), TraceHitColor.ToFColor(true), false, DrawTime);
		}
	}
}

void APlayer_Base_Knight::SetbHoldGuard(const bool& _ToggleGuard)
{
	bHoldGuard = true;

	// 서브시스템의 경우 게임 실행 시 생성되므로 애님 블루프린트 에디터에서 해당 함수에 접근하는 경우 막아야 함
	if (GetWorld()->WorldType == EWorldType::Editor || GetWorld()->WorldType == EWorldType::EditorPreview)
	{
		return;
	}

	UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
	if ( IsValid(StatMgr) )
	{
		StatMgr->SetbSTRecovSlowly(_ToggleGuard);
	}
}

ULockOnTargetComponent* APlayer_Base_Knight::GetLockOnTarget() const
{
	return m_SArm->GetLockOnTarget();
}

bool APlayer_Base_Knight::GetbToggleLockOn() const
{
	return m_SArm->IsLockedOn();
}