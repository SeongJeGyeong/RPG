// Fill out your copyright notice in the Description page of Project Settings.

#include "Player_Base_Knight.h"
#include "../Header/Enum.h"
#include "../RPGPortfolioGameModeBase.h"
#include "../CharacterAnim/AnimInstance_Knight.h"
#include "Player_CameraArm.h"
#include "../UI/UI_Base.h"
#include "../Item/Item_Dropped_Base.h"
#include "../System/DamageType_Base.h"
#include "../Monsters/Monster_Base.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../Manager/GISubsystem_SoundMgr.h"
#include "../Manager/GISubsystem_EffectMgr.h"
#include "Comp/Player_SkillComponent.h"
#include "Comp/Player_StatComponent.h"
#include "Comp/Player_InvenComponent.h"
#include "Comp/Player_InputComponent.h"
#include "MotionWarpingComponent.h"
#include "RootMotionModifier_SkewWarp.h"
#include "Kismet/KismetMathLibrary.h"
#include "../GameInstance_Base.h"

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
	GetCharacterMovement()->RotationRate = FRotator(0.f, 1200.f, 0.f); // 이동방향으로의 회전 속도
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;

	m_SArm = CreateDefaultSubobject<UPlayer_CameraArm>(TEXT("SArm"));
	m_SArm->SetupAttachment(RootComponent);
	m_SArm->SetRelativeLocation(FVector(0.f, 0.f, 80.f));

	m_Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	m_Cam->SetupAttachment(m_SArm);
	m_Cam->bUsePawnControlRotation = false; // 폰과 카메라의 회전분리

	m_SkillComponent = CreateDefaultSubobject<UPlayer_SkillComponent>(TEXT("SkillComp"));
	m_StatComponent = CreateDefaultSubobject<UPlayer_StatComponent>(TEXT("StatComp"));
	m_InvenComponent = CreateDefaultSubobject<UPlayer_InvenComponent>(TEXT("InventoryComp"));
	m_InputComponent = CreateDefaultSubobject<UPlayer_InputComponent>(TEXT("InputComp"));
	m_MWComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarp"));

	HitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollision"));

	bDebugOn = false;
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

		if ( pLocalPlayer )
		{
			// 향상된 입력 매핑 컨텍스트 추가
			m_InputComponent->SetMappingContext(pLocalPlayer);
		}
	}

	// AddUObject : 멀티캐스트 델리게이트(여러 함수 바인딩 가능)
	// BindUObject : 싱글캐스트 델리게이트
	m_InputComponent->OnQSChange.AddUObject(m_InvenComponent, &UPlayer_InvenComponent::ChangeQuickSlot);
	m_InputComponent->OnConsumeStamina.BindUObject(m_StatComponent, &UPlayer_StatComponent::GetConsumeStaminaForState);

	m_AnimInst = Cast<UAnimInstance_Knight>(GetMesh()->GetAnimInstance());
	if ( IsValid(m_AnimInst) )
	{
		m_AnimInst->OnInvincibleState.AddUObject(this, &APlayer_Base_Knight::InvincibleCheck);
		m_AnimInst->OnJumpAtk.AddUObject(this, &APlayer_Base_Knight::JumpAttack);
		m_AnimInst->OnDead.AddUObject(this, &APlayer_Base_Knight::PlayerDead);
		m_AnimInst->OnShotProj.AddUObject(m_SkillComponent, &UPlayer_SkillComponent::ShotProjectile);
		m_AnimInst->OnSetState.AddUObject(m_InputComponent, &UPlayer_InputComponent::SetState);
		m_AnimInst->OnEnableComboInput.AddUObject(this, &APlayer_Base_Knight::SetbEnableComboInput);
		m_AnimInst->OnSetAtkTrace.AddUObject(this, &APlayer_Base_Knight::SetAttackTrace);
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
	pGameMode->GetMainHUD()->BindPlayerWidget(this);

	if ( !IsValid(m_PlayerMontage) )
	{
		UE_LOG(LogTemp, Error, TEXT("플레이어 몽타주 데이터에셋 로드 실패"));
	}

	if ( !IsValid(m_PlayerSound) )
	{
		UE_LOG(LogTemp, Error, TEXT("플레이어 사운드 데이터에셋 로드 실패"));
	}
}

// Called every frame
void APlayer_Base_Knight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayer_Base_Knight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent"));

	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (nullptr == InputComp)
	{
		return;
	}

	m_InputComponent->Init(InputComp);
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

	float iDamage = m_StatComponent->CalculApplyDamage(_AtkType, _AtkMontage);

	TSubclassOf<UDamageType_Base> DamageTypeBase = UDamageType_Base::StaticClass();
	DamageTypeBase.GetDefaultObject()->SetAtkType(_AtkType);

	UGameplayStatics::ApplyPointDamage(HitInfo.GetActor(), iDamage, HitInfo.Normal, HitInfo, GetController(), this, DamageTypeBase);
}

void APlayer_Base_Knight::PlayerDead()
{
	m_AnimInst->Montage_Pause();
	OnSetHUDVisibility.Broadcast(ESlateVisibility::Collapsed);

	TSubclassOf<UUserWidget> DeadUIClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/UI_DeadScreen.UI_DeadScreen_C'"));
	if ( !IsValid(DeadUIClass) )
	{
		return;
	}

	UUserWidget* DeadUI = CreateWidget(GetWorld(), DeadUIClass);
	DeadUI->SetRenderOpacity(0.f);
	DeadUI->AddToViewport(6);
	GetWorldTimerManager().SetTimer(DeadTimer, FTimerDelegate::CreateWeakLambda(this, [DeadUI, this]
		{
			DeadUI->SetRenderOpacity(DeadUI->GetRenderOpacity() + 0.003f);
			if ( DeadUI->GetRenderOpacity() >= 1.f )
			{
				m_StatComponent->ResetPlayer();
				UGameInstance_Base* GameInst = Cast<UGameInstance_Base>(GetGameInstance());
				if ( IsValid(GameInst) )
				{
					GameInst->RestartPlayer();
					GetWorldTimerManager().ClearTimer(DeadTimer);
				}
			}
		}
	)
	, 0.01f, true);
}

float APlayer_Base_Knight::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bDead) return 0.f;
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>( &DamageEvent );
	// 공격받은 방향 구하기
	// 일반 몬스터의 경우 몬스터의 위치, 보스의 경우 공격의 ImpactPoint
	bool IsCommonMon = DamageCauser->IsA(AMonster_Base::StaticClass());
	FVector vOffset;
	if (IsCommonMon)
	{
		vOffset = ( DamageCauser->GetActorLocation() - GetActorLocation() ).GetSafeNormal();
	}
	else
	{
		vOffset = ( PointDamageEvent->HitInfo.ImpactPoint - GetActorLocation() ).GetSafeNormal();
	}

	// 공격 방향 구하기
	uint8 HitDir = GetHitDirection(vOffset);
	UDamageType_Base* pDamageType = Cast<UDamageType_Base>(DamageEvent.DamageTypeClass->GetDefaultObject());
	// 플레이어가 가드중이고 공격이 정면에서 오고 있을때
	if ( bHoldGuard && HitDir == 1 )
	{
		// 스태미너가 부족해 가드브레이크 상태가 되면 false
		if ( GuardEnemyAttack(FinalDamage, pDamageType->GetAtkWeight()) )
		{
			if (IsCommonMon)
			{
				AMonster_Base* Monster = Cast<AMonster_Base>(DamageCauser);
				Monster->PlayAtkBlockedAnim();
			}
		}
		return 0.f;
	}

	// HP 감소
	m_StatComponent->DecreasePlayerHP(pDamageType->GetAtkType(), FinalDamage);
	if ( m_StatComponent->IsHPZero() && !bDead )
	{
		bDead = true;
		// 사망처리
		DisableInput(NULL);
		BreakLockOn();
		Play_PlayerMontage(EPlayerMontage::DIE);
		Play_PlayerSound(EPlayerSound::DIE);
		return 0.f;
	}

	// 피격 이펙트 스폰
	UGISubsystem_EffectMgr* pEffectMgr = GetGameInstance()->GetSubsystem<UGISubsystem_EffectMgr>();
	if ( IsValid(pEffectMgr) )
	{
		pEffectMgr->SpawnHitEffect(GetMesh(), PointDamageEvent->HitInfo.BoneName, FVector::ZeroVector, FRotator::ZeroRotator, FVector(1.f));
	}
	// 피격 몽타주 재생
	PlayHitAnimation(HitDir, pDamageType->GetAtkWeight());
	// 피격 사운드 재생
	Play_PlayerSound(EPlayerSound::HIT);

	m_InputComponent->SetState(EPlayerStateType::HIT);
	return 0.0f;
}

// 공격 트레이스 함수
void APlayer_Base_Knight::AttackHitCheck()
{
	TArray<FHitResult> OutHits;
	FCollisionQueryParams Params(NAME_None, false, this);
	FCollisionShape Shape = HitCollision->GetCollisionShape();
	FVector CurrentLoc = HitCollision->GetComponentLocation();
	FQuat Rotation = HitCollision->GetComponentQuat();
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

	if ( bDebugOn )
	{
		DrawDebugAttackTrace(GetWorld(), PrevTraceLoc, CurrentLoc, Rotation, Shape, bResult, OutHits, FLinearColor::Green, FLinearColor::Red, 1.f);
	}
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
				if ( CurrentState->GetStateType() == EPlayerStateType::HEAVYATTACK )
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

void APlayer_Base_Knight::AttackStart(EPlayerStateType _State)
{
	float fConsumption = m_StatComponent->GetConsumeStaminaForState(_State);
	bIsAttacking = m_StatComponent->IsEnoughStamina(fConsumption);
	if ( bIsAttacking )
	{
		m_StatComponent->DecreasePlayerStamina(fConsumption);
		m_InputComponent->SetState(_State);
	}
}

uint8 APlayer_Base_Knight::GetHitDirection(FVector _MonVec)
{
	// 내적을 이용해 각도를 판별
	float fDot = FVector::DotProduct(GetActorForwardVector(), _MonVec);	// 벡터의 내적으로 코사인세타를 구함
	float fAngle = FMath::Acos(fDot);									// 아크코사인 함수로 세타만 구함
	fAngle = FMath::RadiansToDegrees(fAngle);							// 라디안 각도를 디그리로 변환

	// 외적을 이용해 방향을 판별
	FVector vCross = FVector::CrossProduct(GetActorForwardVector(), _MonVec);

	// 적이 왼쪽에 있을 경우 각도가 음수가 되도록 만듬
	if ( vCross.Z < 0.f ) fAngle *= -1;

	// 4방향을 int로 반환
	uint8 iDir = 4;	// 후
	if ( -45.f < fAngle && fAngle < 45.f )
	{
		iDir = 1;	// 전
	}
	else if ( -135 < fAngle && fAngle <= -45.f )
	{
		iDir = 2;	// 좌
	}
	else if ( 45.f <= fAngle && fAngle < 135.f )
	{
		iDir = 3;	// 우
	}

	return iDir;
}

bool APlayer_Base_Knight::GuardEnemyAttack(float _Damage, EATTACK_WEIGHT _WeightType)
{
	// 몬스터의 공격력의 10분의 1 만큼 스태미나 감소
	m_StatComponent->DecreasePlayerStamina(_Damage * 0.1f);
	if (m_StatComponent->IsStaminaZero())
	{
		// 방어 풀리고 경직상태 되도록
		m_InputComponent->SetState(EPlayerStateType::GUARDBREAK);
		return false;
	}

	if ( _WeightType == EATTACK_WEIGHT::LIGHT )
	{
		Play_PlayerMontage(EPlayerMontage::GUARD_LIGHT);
	}
	else
	{
		Play_PlayerMontage(EPlayerMontage::GUARD_HEAVY);
	}

	m_InputComponent->SetState(EPlayerStateType::GUARD);
	return true;
}

void APlayer_Base_Knight::BreakLockOn()
{
	m_SArm->BreakLockOnTarget();
}

void APlayer_Base_Knight::UseInventoryItem(EITEM_ID _ID, EEQUIP_SLOT _Slot)
{
	EPlayerSound SoundEnum = EPlayerSound::EMPTY;

	FGameItemInfo* pItemInfo = m_InvenComponent->GetInventoryItemInfo(_ID);
	if ( pItemInfo == nullptr )
	{
		UE_LOG(LogTemp, Error, TEXT("UseInventoryItem : 존재하지 않는 아이템"));
		return;
	}

	if ( pItemInfo->Restore_HP >= 0 )
	{
		m_StatComponent->RestorePlayerHP(pItemInfo->Restore_HP);
		SoundEnum = EPlayerSound::USERESTORE;
	}
	if ( pItemInfo->Restore_MP >= 0 )
	{
		m_StatComponent->RestorePlayerMP(pItemInfo->Restore_MP);
		SoundEnum = EPlayerSound::USERESTORE;
	}
	if ( pItemInfo->Gained_Soul >= 0 )
	{
		m_StatComponent->GainSoul(pItemInfo->Gained_Soul);
		SoundEnum = EPlayerSound::USESOUL;
	}

	UGISubsystem_EffectMgr* EffectMgr = GetGameInstance()->GetSubsystem<UGISubsystem_EffectMgr>();
	if ( IsValid(EffectMgr) )
	{
		EffectMgr->SpawnEffectAttached(pItemInfo->EffectType, GetMesh(), FName("Root"), FVector(0.f, 0.f, 1.f), FRotator(0.f), EAttachLocation::SnapToTargetIncludingScale, true);
	}
	Play_PlayerSound(SoundEnum);

	m_InvenComponent->DecreaseInventoryItem(_ID, _Slot);
	m_InputComponent->SetState(EPlayerStateType::USEITEM);
}

void APlayer_Base_Knight::SetInputMode(bool _Visibility)
{
	APlayerController* pController = Cast<APlayerController>(GetController());
	if ( !IsValid(pController) )
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController Not Found"));
		return;
	}

	if ( _Visibility )
	{
		FInputModeGameAndUI GAU;
		GAU.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
		GAU.SetHideCursorDuringCapture(false);
		pController->SetInputMode(GAU);

		UGameplayStatics::PlaySound2D(GetWorld(), GETMENUSOUND(EMenuSound::MENU_OPEN));
		OnMenuOpen.Broadcast(ESlateVisibility::SelfHitTestInvisible);
		pController->bShowMouseCursor = true;
		pController->SetPause(true);
	}
	else
	{
		FInputModeGameOnly GameOnly;
		pController->SetInputMode(GameOnly);

		UGameplayStatics::PlaySound2D(GetWorld(), GETMENUSOUND(EMenuSound::MENU_CLOSE));
		OnMenuOpen.Broadcast(ESlateVisibility::Collapsed);
		pController->bShowMouseCursor = false;
		pController->SetPause(false);
	}
}

void APlayer_Base_Knight::ResetVarsOnHitState()
{
	// 현재 행동상태 해제
	ClearMontageRelatedTimer();
	bAtkTrace = false;
	bEnableComboInput = false;
}

void APlayer_Base_Knight::ClearMontageRelatedTimer()
{
	GetWorldTimerManager().ClearTimer(JumpAtkTimer);
	GetWorldTimerManager().ClearTimer(HitStiffTimer);
}

void APlayer_Base_Knight::PlayerMotionWarping(EPlayerMontage _MontageType, float _EndTime)
{
	if ( _MontageType == EPlayerMontage::DODGE_FW )
	{
		MotionWarping_Dodge(m_PlayerMontage->GetPlayerMontage(_MontageType), _EndTime);
	}
	else
	{
		MotionWarping_Attack(m_PlayerMontage->GetPlayerMontage(_MontageType), _EndTime);
	}
}

void APlayer_Base_Knight::MotionWarping_Attack(UAnimSequenceBase* _Anim, float _EndTime)
{
	FVector PlayerLoc = GetActorLocation();
	if ( GetbIsLockOn() )
	{
		const FVector TargetLoc = m_SArm->GetLockOnTargetLocation();
		float Distance = ( TargetLoc - PlayerLoc ).Size();
		FVector MoveLoc = FVector::ZeroVector;

		if ( Distance <= 250.f )
		{
			if ( Distance >= 200.f )
			{
				// 락온 대상과의 거리가 2m 이상 2.5m 이하면 translation on
				MoveLoc = ( ( PlayerLoc - TargetLoc ).GetSafeNormal() * 200.f ) + TargetLoc;
			}
			else
			{
				// 락온 대상과의 거리가 2m 미만이면 앞으로 이동하지 않고 현재 위치에 멈추도록 설정
				// 완전히 멈추지는 않고 조금 전진함
				MoveLoc = PlayerLoc;
			}
			URootMotionModifier_SkewWarp::AddRootMotionModifierSkewWarp(
				m_MWComponent, 
				_Anim, 
				0.f, 
				_EndTime, 
				TEXT("AttackWarp"), 
				EWarpPointAnimProvider::None, 
				FTransform(), 
				TEXT("None"), 
				true, 
				true, 
				true
			);
		} 
		else
		{
			// 락온 대상과의 거리가 2.5m 보다 크면 translation off
			// 루트모션의 기본 이동거리를 유지한다.
			URootMotionModifier_SkewWarp::AddRootMotionModifierSkewWarp(
				m_MWComponent, 
				_Anim, 0.f, 
				_EndTime, 
				TEXT("AttackWarp"), 
				EWarpPointAnimProvider::None, 
				FTransform(), 
				TEXT("None"), 
				false, 
				true, 
				true
			);
		}

		FRotator TargetRot = FRotationMatrix::MakeFromX( (TargetLoc - PlayerLoc).GetSafeNormal() ).Rotator();
		TargetRot.Pitch = 0.f;
		TargetRot.Roll = 0.f;
		m_MWComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("AttackWarp"), MoveLoc, TargetRot);
	}
	else
	{
		// 입력받은 이동 방향으로 공격 중 회전
		const FVector InputVector = GetLastMovementInputVector();
		if ( InputVector.IsZero() )
		{
			m_MWComponent->RemoveWarpTarget(TEXT("AttackWarp"));
		}
		else
		{
			const FRotator TargetRot = FRotationMatrix::MakeFromX(InputVector).Rotator();
			URootMotionModifier_SkewWarp::AddRootMotionModifierSkewWarp(
				m_MWComponent, 
				_Anim, 
				0.f, 
				_EndTime, 
				TEXT("AttackWarp"), 
				EWarpPointAnimProvider::None, 
				FTransform(), 
				TEXT("None"), 
				false, 
				true, 
				true
			);
			m_MWComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("AttackWarp"), FVector::ZeroVector, TargetRot);
		}
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
		URootMotionModifier_SkewWarp::AddRootMotionModifierSkewWarp(m_MWComponent, _Anim, 0.f, _EndTime, TEXT("DodgeWarp"), EWarpPointAnimProvider::None, FTransform(), TEXT("None"), false, true, true);
		m_MWComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("DodgeWarp"), FVector::ZeroVector, TargetRot);
	}
}

void APlayer_Base_Knight::Play_PlayerMontage(EPlayerMontage _MontageType)
{
	if ( !IsValid(m_PlayerMontage->GetPlayerMontage(_MontageType)) )
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 몽타주 로드 실패"));
	}
	else
	{
		m_AnimInst->Montage_Play(m_PlayerMontage->GetPlayerMontage(_MontageType));
	}
}

void APlayer_Base_Knight::Play_PlayerSound(EPlayerSound _SoundType)
{
	if ( !IsValid(m_PlayerSound->GetPlayerSound(_SoundType)) )
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 사운드 로드 실패"));
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_PlayerSound->GetPlayerSound(_SoundType), GetActorLocation());
	}
}

void APlayer_Base_Knight::PlayHitAnimation(uint8 _Dir, EATTACK_WEIGHT _Weight)
{
	if ( GetCharacterMovement()->IsFalling() )
	{
		Play_PlayerMontage(EPlayerMontage::HIT_AIR);
	}
	else if ( _Weight == EATTACK_WEIGHT::HEAVY )
	{
		FRotator Rot = GetActorRotation();
		switch ( _Dir )
		{
		case 2:
			Rot.Yaw -= 90.f;
			SetActorRotation(Rot);
			break;
		case 3:	
			Rot.Yaw += 90.f;
			SetActorRotation(Rot);
			break;
		case 4:
			Rot.Yaw -= 180.f;
			SetActorRotation(Rot);
			break;
		default:
			break;
		}
		Play_PlayerMontage(EPlayerMontage::HIT_HEAVY);
	}
	else
	{
		// 1 : front, 2 : left, 3 : right, 4 : back
		FName HitSection = FName(*FString::Printf(TEXT("Hit%d"), _Dir));
		Play_PlayerMontage(EPlayerMontage::HIT);
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(HitSection, m_PlayerMontage->GetPlayerMontage(EPlayerMontage::HIT));
	}
}

void APlayer_Base_Knight::GainMonsterSoul(int32 _GainedSoul)
{
	m_StatComponent->GainSoul(_GainedSoul);
}

void APlayer_Base_Knight::CloseInventory()
{
	ARPGPortfolioGameModeBase* pGameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if ( !IsValid(pGameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode Not Found"));
		return;
	}
	pGameMode->InventoryUI_SetVisibility(false);
	SetInputMode(false);
}

void APlayer_Base_Knight::AcquireItem(EITEM_ID _Id, int32 _Stack, UTexture2D* _Img)
{
	Play_PlayerMontage(EPlayerMontage::ACTION_ITEM);
	Play_PlayerSound(EPlayerSound::GETITEM);
	m_InvenComponent->AcquireDroppedItem(_Id, _Stack, _Img);
}

// 무적시간 동안 데미지 안받도록 설정
void APlayer_Base_Knight::InvincibleCheck(bool _Invinc)
{
	SetCanBeDamaged(!_Invinc);
}

void APlayer_Base_Knight::ActionTriggerBeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)
{
	if ( _OtherActor->GetClass()->ImplementsInterface(UPlayerInteraction::StaticClass()) )
	{
		TScriptInterface<IPlayerInteraction> Interaction = TScriptInterface<IPlayerInteraction>(_OtherActor);

		OnBeginOverlapInteract.Broadcast(Interaction->GetCommand_Key(), Interaction->GetCommand_Name());
		OverlapInteractionArr.Emplace(Interaction);
	}
}

void APlayer_Base_Knight::ActionTriggerEndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index)
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
			OnEndOverlapItem.Broadcast();
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
	bHoldGuard = _ToggleGuard;

	// 서브시스템의 경우 게임 실행 시 생성되므로 애님 블루프린트 에디터에서 해당 함수에 접근하는 경우 막아야 함
	if (GetWorld()->WorldType == EWorldType::Editor || GetWorld()->WorldType == EWorldType::EditorPreview)
	{
		return;
	}
	m_StatComponent->HoldGuard(_ToggleGuard);
}

float APlayer_Base_Knight::GetfGuardWeight() const
{
	return m_InputComponent->GetGuardWeight();
}

bool APlayer_Base_Knight::GetbIsLockOn() const
{
	return m_SArm->IsLockedOn();
}

void APlayer_Base_Knight::SetAttackTrace(const bool& _AtkTrace)
{
	bAtkTrace = _AtkTrace;
	if ( !_AtkTrace )
	{
		HitActorArr.Empty();
		PrevTraceLoc = FVector::ZeroVector;
	}
}

void APlayer_Base_Knight::SetbEnableComboInput(const bool& _EnableAtkInput)
{
	bEnableComboInput = _EnableAtkInput;
}

bool APlayer_Base_Knight::GetIsDelayTime() const
{
	return m_InvenComponent->GetbItemDelay();
}
