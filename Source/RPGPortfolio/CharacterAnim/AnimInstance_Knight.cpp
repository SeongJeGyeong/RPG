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
		m_Player = Cast<APlayer_Base_Knight>(GetOwningActor());

		if (IsValid(m_Player))
		{
			m_Movement = m_Player->GetCharacterMovement();
		}
		return;
	}
	vLocalVelocity.Z = m_Player->GetRootComponent()->GetRelativeRotation().UnrotateVector(m_Movement->Velocity).Z;

	// Foot IK
	FootIK(_DT);

	// bIsMove = Idle->Move Trigger
	fMoveSpeed = m_Movement->Velocity.Size2D();
	if (0.f < fMoveSpeed && !m_Movement->GetCurrentAcceleration().IsZero())
	{
		bIsMove = true;
		//UE_LOG(LogTemp, Warning, TEXT("vel x : %f"), vLocalVelocity.X);
		//UE_LOG(LogTemp, Warning, TEXT("vel y : %f"), vLocalVelocity.Y);
	}
	else
	{
		bIsMove = false;
		vLocalVelocity.X = 0.f;
		vLocalVelocity.Y = 0.f;
	}

	// 가드 모션
	if (bIsGuard)
	{
		fGuardBlendWeight = FMath::Clamp(fGuardBlendWeight + _DT * 15.f, 0.f, 1.f);

		if ( fGuardBlendWeight >= 1.f )
		{
			m_Player->SetbToggleGuard(true);
		}
	}
	else
	{
		fGuardBlendWeight = FMath::Clamp(fGuardBlendWeight - _DT * 15.f, 0.f, 1.f);
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
				//fDistance_L += ( Foot_L.Get<1>() < Foot_R.Get<1>() ) ? -5.f : 0.f;
				fLIK = FMath::FInterpTo(fLIK, ( fDistance_L - 105.f ) / -50.f, _DeltaTime, fIKInterpSpeed);

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

	ETraceTypeQuery MeshCollision = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel11);

	// 양발 좌표를 기준으로 캡슐컴포넌트 중앙 높이에서 부터 캡슐 컴포넌트 밑 30cm 높이까지 트레이스
	UKismetSystemLibrary::LineTraceSingle(
		this,
		vLineTraceStart,
		vLineTraceEnd,
		MeshCollision,
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

	ETraceTypeQuery MeshCollision = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel11);

	// 양 발 좌표를 기준으로 캐릭터의 루트 본 높이에서 상하로 100cm의 범위를 트레이스
	UKismetSystemLibrary::LineTraceSingle(
		this,
		vLineTraceStart + FVector(0.f, 0.f, 100.f),
		vLineTraceStart + FVector(0.f, 0.f, -100.f),
		MeshCollision,
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

void UAnimInstance_Knight::AnimNotify_AtkSectionStart()
{
	m_Player->SetbImmovableInAtk(true);
}

void UAnimInstance_Knight::AnimNotify_NextCheckStart()
{
	m_Player->SetbNextAtkCheck(true);
}

void UAnimInstance_Knight::AnimNotify_NextCheckEnd()
{
	m_Player->SetbNextAtkCheck(false);
	m_Player->SetbImmovableInAtk(false);
}

void UAnimInstance_Knight::AnimNotify_HitCheckStart()
{
	m_Player->SetbAtkTrace(true);
}

void UAnimInstance_Knight::AnimNotify_HitCheckEnd()
{
	m_Player->SetbAtkTrace(false);
	m_Player->EmptyHitActorArr();
	// 공격 판정 끝난 뒤에 공격 입력이 들어와야만 다음 콤보로 넘어가도록
	m_Player->SetbAtkToggle(false);

}

void UAnimInstance_Knight::AnimNotify_MoveStart()
{
	OnAttackMove.Broadcast(true);
}

void UAnimInstance_Knight::AnimNotify_MoveEnd()
{
	OnAttackMove.Broadcast(false);
}

void UAnimInstance_Knight::AnimNotify_DodgeStart()
{
	OnDodgeTimeCheck.Broadcast(true);
}

void UAnimInstance_Knight::AnimNotify_DodgeEnd()
{
	OnDodgeTimeCheck.Broadcast(false);
}

void UAnimInstance_Knight::AnimNotify_DodgeAnimEnd()
{
	m_Player->SetbDodging(false);
}

void UAnimInstance_Knight::AnimNotify_JumpStart()
{
	m_Player->SetbEnableMove(false);
}

void UAnimInstance_Knight::AnimNotify_JumpEnd()
{
	m_Player->SetbEnableMove(true);
}

void UAnimInstance_Knight::AnimNotify_FallEnd()
{
	m_Player->SetbEnableMove(false);
}

void UAnimInstance_Knight::AnimNotify_ShotProjectile()
{
	m_Player->ShotProjectile();
}
