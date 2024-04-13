// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Knight.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

void UAnimInstance_Knight::NativeInitializeAnimation()
{
}

void UAnimInstance_Knight::NativeBeginPlay()
{
	m_Player = Cast<APlayer_Base_Knight>(GetOwningActor());

	if (IsValid(m_Player))
	{
		m_Movement = m_Player->GetCharacterMovement();
	}
}

void UAnimInstance_Knight::NativeUpdateAnimation(float _DT)
{
	if (!IsValid(m_Movement) || !IsValid(m_Player))
	{
		return;
	}

	FootIK(_DT);

	// bIsMove = Idle->Move Trigger
	fMoveSpeed = m_Movement->Velocity.Size2D();
	if (0.f < fMoveSpeed && !m_Movement->GetCurrentAcceleration().IsZero())
	{
		bIsMove = true;
	}
	else
	{
		bIsMove = false;
	}

	vLocalVelocity = m_Player->GetRootComponent()->GetRelativeRotation().UnrotateVector(m_Movement->Velocity);

	bIsInAir = m_Movement->IsFalling();

	if (bIsGuard && !bIsInAir)
	{
		fGuardBlendWeight = FMath::Clamp(fGuardBlendWeight + _DT * 9.f, 0.f, 1.f);
	}
	else
	{
		fGuardBlendWeight = FMath::Clamp(fGuardBlendWeight - _DT * 9.f, 0.f, 1.f);
	}

	const FAnimNode_StateMachine* state = GetStateMachineInstanceFromName(FName("Main"));
	if (FName("Idle/Move").IsEqual(state->GetCurrentStateName()))
	{
		m_Player->SetbEnableJump(false);
		m_Player->SetbEnableMove(true);
	}
	else
	{
		if (FName("Land").IsEqual(state->GetCurrentStateName()))
		{
			m_Player->SetbEnableMove(false);
		}
		m_Player->SetbEnableJump(true);
	}
}

void UAnimInstance_Knight::FootIK(float _DeltaTime)
{
	// 공중에 있지 않을 때
	if ( !m_Player->GetCharacterMovement()->IsFalling() )
	{
		IgnoreActorArr.Emplace(m_Player);

		// 캡슐컴포넌트의 중간 높이에서 발을 딛은 바닥까지의 거리를 반환
		TTuple<bool, float> Foot_R = CapsuleDistance(TEXT("Foot_R"), m_Player);
		TTuple<bool, float> Foot_L = CapsuleDistance(TEXT("Foot_L"), m_Player);
 
		// <>에 숫자를 넣으면 숫자에 해당하는 순서의 튜플 인자를 가져옴
		if ( Foot_R.Get<0>() || Foot_L.Get<0>() )
		{
			// 발을 딛은 높이가 더 낮은 쪽의 발을 고른다.
			const float fSelectFloat = UKismetMathLibrary::SelectFloat(Foot_L.Get<1>(), Foot_R.Get<1>(), Foot_L.Get<1>() >= Foot_R.Get<1>());
			// 루트 본의 높이를 보간함
			fRootDisplacement = FMath::FInterpTo(fRootDisplacement, ( fSelectFloat - 100.f ) * -1.f, _DeltaTime, fIKInterpSpeed);

			TTuple<bool, float, FVector> FootTrace_R = FootLineTrace(TEXT("Foot_R"), m_Player);
			TTuple<bool, float, FVector> FootTrace_L = FootLineTrace(TEXT("Foot_L"), m_Player);

			if ( FootTrace_R.Get<0>() )
			{
				float fDistance_R = FootTrace_R.Get<1>();
				// 들고 있는 발바닥과 땅의 간격을 맞추기 위해 보간할 값을 조정
				fDistance_R += ( Foot_L.Get<1>() > Foot_R.Get<1>() ) ? -5.f : 0.f;
				// Leg IK의 알파값을 보간 (0 ~ 1)
				fRIK = FMath::FInterpTo(fRIK, ( fDistance_R - 105.f ) / -50.f, _DeltaTime, fIKInterpSpeed);

				// 발과 닿은 바닥의 법선벡터
				const FVector vFootRVec = FootTrace_R.Get<2>();
				/*UE_LOG(LogTemp, Warning, TEXT("FootRVec X : %f"), vFootRVec.X);
				UE_LOG(LogTemp, Warning, TEXT("FootRVec Y : %f"), vFootRVec.Y);
				UE_LOG(LogTemp, Warning, TEXT("FootRVec Z : %f"), vFootRVec.Z);*/
				// DegAtan2 : A/B 의 역탄젠트(ArcTangent)를 디그리 각도로 반환함
				// Atan과 Atan2의 차이
				// Atan : 두 점 사이의 탄젠트값을 받아서 -90 ~ 90 사이의 디그리 각도를 반환한다.(방향의 개념이 없는 단순한 두 점 사이의 각도)
				// Atan2 : 두 점 사이의 상대좌표를 받아 -180 ~ 180 사이의 디그리 각도를 반환한다.
				const double rRRotPitch = UKismetMathLibrary::DegAtan2(vFootRVec.X, vFootRVec.Z) * -1.f;
				
				// 바닥이 경사지지 않은 평면일 경우 Z축을 따라 위로 향하는 벡터(0, 0, 1)가 수평선 (Y축)과 이루는 각도
				const double rRRotRoll = UKismetMathLibrary::DegAtan2(vFootRVec.Y, vFootRVec.Z);
				const FRotator MakeRRot = FRotator(rRRotPitch, 0.f, rRRotRoll);
				
				// 발의 본 각도를 바닥의 법선벡터의 역방향과 일치하도록 보간함
				rRRot = FMath::RInterpTo(rRRot, MakeRRot, _DeltaTime, fIKInterpSpeed);
			}

			if ( FootTrace_L.Get<0>() )
			{
				float fDistance_L = FootTrace_L.Get<1>();
				fDistance_L += ( Foot_L.Get<1>() < Foot_R.Get<1>() ) ? -5.f : 0.f;
				fLIK = FMath::FInterpTo(fLIK, ( fDistance_L - 105.f ) / -50.f, _DeltaTime, fIKInterpSpeed);
				// UE_LOG(LogTemp, Warning, TEXT("Left Distance : %f"), fDistance_L);
				// UE_LOG(LogTemp, Warning, TEXT("Left IK : %f"), fLIK);

				const FVector vFootLVec = FootTrace_L.Get<2>();
				const double rLRotPitch = UKismetMathLibrary::DegAtan2(vFootLVec.X, vFootLVec.Z) * -1.f;
				const double rLRotRoll = UKismetMathLibrary::DegAtan2(vFootLVec.Y, vFootLVec.Z);
				const FRotator MakeLRot = FRotator(rLRotPitch, 0.f, rLRotRoll);

				rLRot = FMath::RInterpTo(rLRot, MakeLRot, _DeltaTime, fIKInterpSpeed);
			}
		}
	}
	else
	{
		rRRot = FMath::RInterpTo(rRRot, FRotator::ZeroRotator, _DeltaTime, fIKInterpSpeed);
		fRIK = FMath::FInterpTo(fRIK, 0.f, _DeltaTime, fIKInterpSpeed);

		rLRot = FMath::RInterpTo(rLRot, FRotator::ZeroRotator, _DeltaTime, fIKInterpSpeed);
		fLIK = FMath::FInterpTo(fLIK, 0.f, _DeltaTime, fIKInterpSpeed);
	}
}

TTuple<bool, float> UAnimInstance_Knight::CapsuleDistance(FName _SocketName, ACharacter* _Char)
{
	const FVector vWorldLocation = _Char->GetMesh()->GetComponentLocation();
	const FVector vBreakVector = vWorldLocation + FVector(0.f, 0.f, _Char->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	const FVector vSocketLocation = _Char->GetMesh()->GetSocketLocation(_SocketName);

	const FVector vLineTraceStart = FVector(vSocketLocation.X, vSocketLocation.Y, vBreakVector.Z);
	const FVector vLineTraceEnd = vLineTraceStart - FVector(0.f, 0.f, _Char->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + fLineOutCapsule);

	FHitResult HitResult;

	// 양발 좌표를 기준으로 캡슐컴포넌트 중앙 높이에서 부터 캡슐 컴포넌트 밑 30cm 높이까지 트레이스
	UKismetSystemLibrary::LineTraceSingle(
		this,
		vLineTraceStart,
		vLineTraceEnd,
		ETraceTypeQuery::TraceTypeQuery1,	//TraceTypeQuery : 숫자에 해당하는 ECollisionChannel을 반환
		false,
		IgnoreActorArr,
		EDrawDebugTrace::None,
		HitResult,
		false
	);

	// 블록되는 충돌체와 Hit되었는지 반환. false인 경우 hit되지 않았거나 overlap되었음
	const bool bResult = HitResult.bBlockingHit;

	// FHitResult.Distance : 트레이스 시작지점에서 히트포인트의 월드 스페이스 사이의 거리
	return MakeTuple(bResult, HitResult.Distance);
}

TTuple<bool, float, FVector> UAnimInstance_Knight::FootLineTrace(FName _SocketName, ACharacter* _Char)
{
	const FVector vSocketLocation = _Char->GetMesh()->GetSocketLocation(_SocketName);
	const FVector vRootLocation = _Char->GetMesh()->GetSocketLocation(TEXT("Root"));

	const FVector vLineTraceStart = FVector(vSocketLocation.X, vSocketLocation.Y, vRootLocation.Z);

	FHitResult HitResult;

	// 양 발 좌표를 기준으로 캐릭터의 루트 본 높이에서 상하로 100cm의 범위를 트레이스
	UKismetSystemLibrary::LineTraceSingle(
		this,
		vLineTraceStart + FVector(0.f, 0.f, 100.f),
		vLineTraceStart + FVector(0.f, 0.f, -100.f),
		ETraceTypeQuery::TraceTypeQuery1,	//TraceTypeQuery : 숫자에 해당하는 ECollisionChannel을 반환
		false,
		IgnoreActorArr,
		EDrawDebugTrace::None,
		HitResult,
		false,
		FLinearColor::Yellow,
		FLinearColor::Blue
	);

	const bool bResult = HitResult.bBlockingHit;

	if ( bResult )
	{
		// FHitResult.Normal : 충돌한 지점의 노말벡터
		// 법선벡터(Normal Vector) : 물체의 표면이 향하고 있는 방향을 구한 벡터. 물체가 평면일 경우 물체와 수직인 방향의 벡터가 된다.
		return MakeTuple(bResult, HitResult.Distance, HitResult.Normal);
	}

	return MakeTuple(bResult, 999.f, FVector::ZeroVector);
}

void UAnimInstance_Knight::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UAnimInstance_Knight::AnimNotify_HitCheckStart()
{
	m_Player->SetbAtkTrace(true);
}

void UAnimInstance_Knight::AnimNotify_HitCheckEnd()
{
	m_Player->SetbAtkTrace(false);
}

void UAnimInstance_Knight::AnimNotify_MoveStart()
{
	OnAttackMove.Broadcast();
}

void UAnimInstance_Knight::AnimNotify_MoveEnd()
{
	m_Player->SetbAtkMove(false);
}

void UAnimInstance_Knight::AnimNotify_DodgeStart()
{
	m_Player->SetbInvincible(true);
	OnInvincibleTimeCheck.Broadcast();
}

void UAnimInstance_Knight::AnimNotify_DodgeEnd()
{
	m_Player->SetbInvincible(false);
	OnInvincibleTimeCheck.Broadcast();
}
