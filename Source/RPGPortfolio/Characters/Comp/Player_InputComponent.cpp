// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_InputComponent.h"
#include "../Player_Base_Knight.h"
#include "../../Header/Enum.h"
#include "../../System/Subsys_ObjectPool.h"
#include "Player_InvenComponent.h"
#include "Player_StatComponent.h"
#include "Player_SkillComponent.h"
#include "../Player_CameraArm.h"
#include "../../System/Interface/PlayerInteraction.h"
#include "../../RPGPortfolioGameModeBase.h"
#include "../../UI/UI_Base.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPlayer_InputComponent::UPlayer_InputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UPlayer_InputComponent::BeginPlay()
{
	Super::BeginPlay();

	m_Player = GetOwner<APlayer_Base_Knight>();
	if ( m_Player == nullptr )
	{
		UE_LOG(LogTemp, Error, TEXT("UPlayer_InputComponent : 플레이어 연결 실패"));
		return;
	}

	USubsys_ObjectPool* PoolSubsystem = GetWorld()->GetSubsystem<USubsys_ObjectPool>();
	if ( IsValid(PoolSubsystem) )
	{
		CurrentState = PoolSubsystem->GetStateFromPool(EPlayerStateType::IDLE);
	}
	if ( CurrentState != nullptr )
	{
		CurrentState->Enter(m_Player);
	}

	UE_LOG(LogTemp, Warning, TEXT("InputComp : BeginPlay"));
}

// Called every frame
void UPlayer_InputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState)
	{
		CurrentState->Update(m_Player, DeltaTime);
	}

	if ( bInputGuard && CurrentState->GetStateType() == EPlayerStateType::IDLE )
	{
		fGuardWeight = FMath::Clamp(fGuardWeight + DeltaTime * 10.f, 0.f, 1.f);
	}
	else if( CurrentState->GetStateType() != EPlayerStateType::GUARD )
	{
		fGuardWeight = FMath::Clamp(fGuardWeight - DeltaTime * 15.f, 0.f, 1.f);
	}
}

void UPlayer_InputComponent::Init(UEnhancedInputComponent* _InputComp)
{
	UE_LOG(LogTemp, Warning, TEXT("InputComp : Init"));

	if ( !m_IA_Setting.IsNull() )
	{
		UDA_InputAction* pIADA = m_IA_Setting.LoadSynchronous();
		for ( int32 i = 0; i < pIADA->IADataArr.Num(); ++i )
		{
			switch ( pIADA->IADataArr[ i ].Type )
			{
			case EInputActionType::MOVE:
				_InputComp->BindAction(pIADA->IADataArr[ i ].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::MoveAction);
				break;
			case EInputActionType::ROTATION:
				_InputComp->BindAction(pIADA->IADataArr[ i ].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::RotateAction);
				break;
			case EInputActionType::JUMP:
				_InputComp->BindAction(pIADA->IADataArr[ i ].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::JumpAction);
				break;
			case EInputActionType::SPRINT:
				_InputComp->BindAction(pIADA->IADataArr[ i ].Action.LoadSynchronous(), ETriggerEvent::Started, this, &ThisClass::SprintStart);
				_InputComp->BindAction(pIADA->IADataArr[ i ].Action.LoadSynchronous(), ETriggerEvent::Completed, this, &ThisClass::SprintEnd);
				break;
			case EInputActionType::GUARD:
				_InputComp->BindAction(pIADA->IADataArr[ i ].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::GuardAction);
				break;
			case EInputActionType::DODGE:
				_InputComp->BindAction(pIADA->IADataArr[ i ].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::DodgeAction);
				break;
			case EInputActionType::ATTACK:
				_InputComp->BindAction(pIADA->IADataArr[ i ].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::AttackAction);
				break;
			case EInputActionType::HEAVYTOGGLE:
				_InputComp->BindAction(pIADA->IADataArr[ i ].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::HeavyAttack);
				break;
			case EInputActionType::LOCKON:
				_InputComp->BindAction(pIADA->IADataArr[ i ].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::LockOnToggleAction);
				break;
			case EInputActionType::SWITCHLOCKON:
				_InputComp->BindAction(pIADA->IADataArr[ i ].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::SwitchLockOnTarget);
				break;
			case EInputActionType::OPENMENU:
				_InputComp->BindAction(pIADA->IADataArr[ i ].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::OpenMenu);
				break;
			case EInputActionType::ACTION:
				_InputComp->BindAction(pIADA->IADataArr[ i ].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::ActionCommand);
				break;
			case EInputActionType::QUICKSLOTCHANGE:
				_InputComp->BindAction(pIADA->IADataArr[ i ].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::QuickSlotChange);
				break;
			case EInputActionType::USELOWERQUICKSLOT:
				_InputComp->BindAction(pIADA->IADataArr[ i ].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::UseLowerQuickSlot);
				break;
			case EInputActionType::USESKILL_1:
				_InputComp->BindAction(pIADA->IADataArr[ i ].Action.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::UseSkill_1);
				break;
			default:
				break;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("m_IA_Setting Is Null!"));
	}

}

void UPlayer_InputComponent::SetMappingContext(ULocalPlayer* _LocalPlayer)
{
	if ( !m_IMC.IsNull() )
	{
		UEnhancedInputLocalPlayerSubsystem* pSubsystem = _LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		pSubsystem->AddMappingContext(m_IMC.LoadSynchronous(), 0);
	}
}

void UPlayer_InputComponent::SetState(EPlayerStateType _StateType)
{
	// 기존 상태를 새로운 상태로 교체
	CurrentState->Exit(m_Player);
	StateMachine* ReturnState = CurrentState.Release();
	USubsys_ObjectPool* PoolSubsystem = GetWorld()->GetSubsystem<USubsys_ObjectPool>();
	if ( !IsValid(PoolSubsystem) )
	{
		UE_LOG(LogTemp, Error, TEXT("스테이트 풀 가져오기 실패"));
		return;
	}

	PoolSubsystem->ReturnStateToPool(ReturnState->GetStateType(), ReturnState);
	TUniquePtr<StateMachine> NewState = PoolSubsystem->GetStateFromPool(_StateType);
	if ( NewState != nullptr )
	{
		CurrentState = MoveTemp(NewState); //TUniquePtr는 복사가 불가능하므로, 소유권을 이전하려면 반드시 MoveTemp를 사용해 소유권을 이동해야 함
		CurrentState->Enter(m_Player);
	}
}

void UPlayer_InputComponent::MoveAction(const FInputActionInstance& _Instance)
{
	AController* Controller = m_Player ? m_Player->GetController() : nullptr;

	if ( Controller == NULL )
	{
		return;
	}

	if ( CurrentState->GetStateType() == EPlayerStateType::JUMP ||
		CurrentState->GetStateType() == EPlayerStateType::HIT || 
		CurrentState->GetStateType() == EPlayerStateType::ACTION ||
		CurrentState->GetStateType() == EPlayerStateType::GUARD ||
		CurrentState->GetStateType() == EPlayerStateType::JUMPATTACK ||
		CurrentState->GetStateType() == EPlayerStateType::USESKILL_1 ||
		CurrentState->GetStateType() == EPlayerStateType::USEITEM )
	{
		return;
	}

	FVector2D vInput = _Instance.GetValue().Get<FVector2D>();
	const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

	if ( vInput.X != 0.0f )
	{
		const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
		m_Player->AddMovementInput(Direction, vInput.X);
	}

	if ( vInput.Y != 0.0f )
	{
		const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
		m_Player->AddMovementInput(Direction, vInput.Y);
	}
}

void UPlayer_InputComponent::RotateAction(const FInputActionInstance& _Instance)
{
	if ( !IsValid(m_Player) )
	{
		return;
	}

	FVector2D vInput = _Instance.GetValue().Get<FVector2D>();

	if ( !m_Player->GetbIsLockOn() )
	{
		m_Player->GetCameraArm()->CancelResetCamera();

		m_Player->AddControllerYawInput(vInput.X);
		m_Player->AddControllerPitchInput(-vInput.Y);
	}
}

void UPlayer_InputComponent::JumpAction(const FInputActionInstance& _Instance)
{
	if ( !IsValid(m_Player) )
	{
		return;
	}

	if ( IsPossibleStateTransition(EPlayerStateType::JUMP) )
	{
		SetState(EPlayerStateType::JUMP);
	}
}

void UPlayer_InputComponent::SprintStart()
{
	if ( !IsValid(m_Player) )
	{
		return;
	}

	if ( !IsPossibleStateTransition(EPlayerStateType::SPRINT) )
	{
		return;
	}
	// 스테미너가 0일 경우 달리기 불가
	if ( m_Player->GetStatComp()->IsStaminaZero() )
	{
		return;
	}
	// 이동 중에만 토글되도록
	if ( m_Player->GetCharacterMovement()->Velocity.Size2D() <= 0.f || m_Player->GetCharacterMovement()->GetCurrentAcceleration().IsZero() )
	{
		return;
	}

	SetState(EPlayerStateType::SPRINT);
}

void UPlayer_InputComponent::SprintEnd()
{
	if ( !IsValid(m_Player) )
	{
		return;
	}

	if ( !IsPossibleStateTransition(EPlayerStateType::SPRINT) )
	{
		return;
	}

	SetState(EPlayerStateType::IDLE);
}

void UPlayer_InputComponent::GuardAction(const FInputActionInstance& _Instance)
{
	bInputGuard = _Instance.GetValue().Get<bool>();
}

void UPlayer_InputComponent::AttackAction(const FInputActionInstance& _Instance)
{
	if ( !IsValid(m_Player) )
	{
		return;
	}

	EPlayerStateType State = ( CurrentState->GetStateType() == EPlayerStateType::JUMP ) ? EPlayerStateType::JUMPATTACK : EPlayerStateType::ATTACK;
	if ( !IsPossibleStateTransition(State) )
	{
		return;
	}

	// 공격중인 상태에서 다음 공격 입력 기간이 아닐경우
	if ( m_Player->GetCurrentCombo() > 1 && !m_Player->GetbEnableComboInput() )
	{
		return;
	}

	m_Player->SetbIsAttacking(_Instance.GetValue().Get<bool>());

	// 첫 공격을 시작할 경우
	if ( m_Player->GetCurrentCombo() == 1 )
	{
		if ( GetbHeavyHold() && State == EPlayerStateType::ATTACK ) State = EPlayerStateType::HEAVYATTACK;
		m_Player->AttackStart(State);
	}
}

void UPlayer_InputComponent::HeavyAttack(const FInputActionInstance& _Instance)
{
	bHeavyHold = _Instance.GetValue().Get<bool>();
}

void UPlayer_InputComponent::DodgeAction(const FInputActionInstance& _Instance)
{
	if ( !IsValid(m_Player) ) return;
	if ( !IsPossibleStateTransition(EPlayerStateType::DODGE) ) return;
	if ( !OnConsumeStamina.IsBound() ) return;

	float fConsumption = OnConsumeStamina.Execute(EPlayerStateType::DODGE);
	// 현재 스태미나가 소비량보다 적을 경우 회피 불가
	if ( !m_Player->GetStatComp()->IsEnoughStamina(fConsumption) )
	{
		return;
	}

	m_Player->GetStatComp()->DecreasePlayerStamina(fConsumption);
	SetState(EPlayerStateType::DODGE);
}

void UPlayer_InputComponent::LockOnToggleAction(const FInputActionInstance& _Instance)
{
	if ( !IsValid(m_Player) )
	{
		return;
	}

	bool bTargetLocked = m_Player->GetCameraArm()->ToggleCameraLockOn(_Instance.GetValue().Get<bool>());
	// 락온 중이 아닐 때 락온 대상을 찾지 못할 경우 카메라 리셋
	if ( !bTargetLocked )
	{
		m_Player->GetCameraArm()->ResetCamera();
	}
}

void UPlayer_InputComponent::SwitchLockOnTarget(const FInputActionInstance& _Instance)
{
	if ( !IsValid(m_Player) )
	{
		return;
	}

	float SwitchDirection = _Instance.GetValue().Get<float>();

	if ( SwitchDirection > 0.f )
	{
		m_Player->GetCameraArm()->SwitchTarget(ELockOnDirection::Left);
	}
	else if ( SwitchDirection < 0.f )
	{
		m_Player->GetCameraArm()->SwitchTarget(ELockOnDirection::Right);
	}
}

void UPlayer_InputComponent::OpenMenu(const FInputActionInstance& _Instance)
{
	if ( !IsValid(m_Player) )
	{
		return;
	}

	ARPGPortfolioGameModeBase* pGameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if ( !IsValid(pGameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode Not Found"));
		return;
	}

	// 세부메뉴가 열려있을 경우
	if ( pGameMode->IsSubMenuUIOpened() )
	{
		pGameMode->CloseSubMenu();
		return;
	}

	m_Player->SetInputMode(!pGameMode->GetMainHUD()->IsOpendMenu());
}

void UPlayer_InputComponent::ActionCommand(const FInputActionInstance& _Instance)
{
	if ( !IsValid(m_Player) )
	{
		return;
	}

	if ( !IsPossibleStateTransition(EPlayerStateType::ACTION) )
	{
		return;
	}

	if ( !m_Player->GetInteractArr().IsEmpty() )
	{
		SetState(EPlayerStateType::ACTION);
		m_Player->GetInteractTop()->Interaction(m_Player);
	}
	// 주변에 아이템이 없고 아이템 획득 메시지 표시된 상태일 때
	else
	{
		m_Player->OnCloseItemMessageBox.Broadcast();
	}
}

void UPlayer_InputComponent::QuickSlotChange(const FInputActionInstance& _Instance)
{
	if ( !IsValid(m_Player) )
	{
		return;
	}

	OnQSChange.Broadcast();
}

void UPlayer_InputComponent::UseLowerQuickSlot(const FInputActionInstance& _Instance)
{
	if ( !IsValid(m_Player) )
	{
		return;
	}

	if ( CurrentState->GetStateType() != EPlayerStateType::IDLE )
	{
		return;
	}

	if ( !m_Player->GetIsDelayTime() )
	{
		FInvenItemRow* pItem = m_Player->GetInvenComp()->GetQuickSlotItem();
		if ( pItem == nullptr )
		{
			UE_LOG(LogTemp, Warning, TEXT("퀵슬롯에 지정된 아이템 없음"));
		}
		else
		{
			m_Player->UseInventoryItem(pItem->ID, pItem->EquipedSlot);
		}
	}
}

void UPlayer_InputComponent::UseSkill_1(const FInputActionInstance& _Instance)
{
	if ( !IsValid(m_Player) )
	{
		return;
	}

	if ( !IsPossibleStateTransition(EPlayerStateType::USESKILL_1) )
	{
		return;
	}

	if ( m_Player->GetSkillComp()->GetSkillName() == ESkillName::NONE )
	{
		return;
	}
	FSkillAsset* Skill = m_Player->GetSkillComp()->GetEquippedSkill();

	if ( !m_Player->GetStatComp()->IsEnoughStamina(Skill->Stamina_Consumption) || !m_Player->GetStatComp()->IsEnoughMP(Skill->MP_Consumption) )
	{
		return;
	}

	m_Player->GetStatComp()->DecreasePlayerMP(Skill->MP_Consumption);
	m_Player->GetStatComp()->DecreasePlayerStamina(Skill->Stamina_Consumption);
	m_Player->PlayAnimMontage(Skill->Animation);
	SetState(EPlayerStateType::USESKILL_1);
}

bool UPlayer_InputComponent::IsPossibleStateTransition(EPlayerStateType _State)
{
	bool IsPossible = true;
	switch ( _State )
	{
	case EPlayerStateType::IDLE:
		IsPossible = ( CurrentState->GetStateType() != EPlayerStateType::IDLE );
		break;
	case EPlayerStateType::ATTACK_WAIT:
		IsPossible = ( CurrentState->GetStateType() == EPlayerStateType::ATTACK || CurrentState->GetStateType() == EPlayerStateType::HEAVYATTACK );
		break;
	case EPlayerStateType::JUMP:
	case EPlayerStateType::SPRINT:
	case EPlayerStateType::DODGE:
	case EPlayerStateType::USESKILL_1:
	case EPlayerStateType::USEITEM:
		IsPossible = ( CurrentState->GetStateType() == EPlayerStateType::IDLE ||
					   CurrentState->GetStateType() == EPlayerStateType::SPRINT ||
					   CurrentState->GetStateType() == EPlayerStateType::ATTACK_WAIT );
		break;
	case EPlayerStateType::ATTACK:
	case EPlayerStateType::HEAVYATTACK:
		IsPossible = ( CurrentState->GetStateType() == EPlayerStateType::IDLE ||
					   CurrentState->GetStateType() == EPlayerStateType::SPRINT ||
					   CurrentState->GetStateType() == EPlayerStateType::ATTACK_WAIT ||
					   CurrentState->GetStateType() == EPlayerStateType::ATTACK ||
					   CurrentState->GetStateType() == EPlayerStateType::HEAVYATTACK );
		break;
	case EPlayerStateType::JUMPATTACK:
		IsPossible = ( CurrentState->GetStateType() == EPlayerStateType::JUMP && m_Player->GetRootComponent()->GetRelativeRotation().UnrotateVector(m_Player->GetCharacterMovement()->Velocity).Z >= 30.f );
		break;
	case EPlayerStateType::ACTION:
		IsPossible = ( ( CurrentState->GetStateType() == EPlayerStateType::IDLE || CurrentState->GetStateType() == EPlayerStateType::SPRINT ) && !bInputGuard );
		break;
	case EPlayerStateType::GUARD:
	case EPlayerStateType::GUARDBREAK:
		IsPossible = ( CurrentState->GetStateType() == EPlayerStateType::IDLE || CurrentState->GetStateType() == EPlayerStateType::GUARD );
		break;
	default:
		break;
	}

	return IsPossible;
}