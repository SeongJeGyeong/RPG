// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_CameraArm.h"
#include "../System/Component/LockOnTargetComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"
#include "Player_Base_Knight.h"
#include "GameFramework/Controller.h"


UPlayer_CameraArm::UPlayer_CameraArm()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 스프링 암 내장 설정
	TargetArmLength = 500.0f;			// 스프링암 길이
	bUsePawnControlRotation = true;		// 스프링암이 플레이어의 회전을 따라가도록 설정
	bInheritPitch = true;
	bInheritYaw = true;
	bInheritRoll = true;
	bDoCollisionTest = true;
	bEnableCameraLag = true;			// 카메라 위치가 조금 지연되서 따라오도록 설정
	bEnableCameraRotationLag = false;	// 카메라 회전 지연(록온 상태일때만 활성화)
	CameraLagSpeed = 3.f;				// 위치 지연 속도
	CameraRotationLagSpeed = 5.f;		// 회전 지연 속도
	CameraLagMaxDistance = 100.f;		// 카메라가 현재 위치보다 지연될 수 있는 최대거리

	fMaxLockOnDistance = 1500.f;		// 록온 범위
	LockonControlRotationRate = 10.f;	// 록온 시 타겟으로의 카메라 회전 보간 속도
	bDrawDebug = false;
}

void UPlayer_CameraArm::BeginPlay()
{
	Super::BeginPlay();

	m_Player = Cast<APlayer_Base_Knight>(GetOwner());
	if ( !IsValid(m_Player) )
	{
		UE_LOG(LogTemp, Error, TEXT("스프링 암의 루트 액터 찾지 못함"));
	}
}

void UPlayer_CameraArm::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bDrawDebug)
	{
		for ( ULockOnTargetComponent* Target : GetTargetComponents() )
		{
			DrawDebugLine(GetWorld(), GetComponentLocation(), Target->GetComponentLocation(), FColor::Green);
		}

		DrawDebugSphere(GetWorld(), GetComponentLocation(), fMaxLockOnDistance, 32, FColor::Cyan);
	}

	if (IsLockedOn())
	{
		if ( ( m_Target->GetComponentLocation() - GetComponentLocation() ).Size() > fMaxLockOnDistance + m_Target->GetScaledSphereRadius() )
		{
			BreakLockOnTarget();
			return;
		}

		// 플레이어에서 타겟으로의 벡터
		FVector TargetVect = m_Target->GetComponentLocation() - ( GetComponentLocation());
		FRotator TargetRot = TargetVect.GetSafeNormal().Rotation();
		//TargetRot.Roll = 0.f;
		FRotator CurrentRot = m_Player->GetControlRotation();
		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, LockonControlRotationRate);
		NewRot.Roll = 0.f;
		// 타겟을 바라보도록 로테이션 수정
		m_Player->GetController()->SetControlRotation(NewRot);
	}
}

// Toggle Lock On
bool UPlayer_CameraArm::ToggleCameraLockOn(const bool& _ToggleLockOn)
{
	if (bDrawDebug)
	{
		DrawDebugSphere(GetWorld(), GetComponentLocation(), fMaxLockOnDistance, 32, FColor::Cyan, false, 1.f);
	}

	if ( m_Target != nullptr )
	{
		BreakLockOnTarget();
		return true;
	}
	else
	{
		ULockOnTargetComponent* NewLockOnTarget = GetSelectedTarget();

		if ( NewLockOnTarget != nullptr )
		{
			LockOnTarget(NewLockOnTarget);
			return true;
		}
	}

	return false;
}

void UPlayer_CameraArm::LockOnTarget(ULockOnTargetComponent* NewTargetComponent)
{
	m_Target = NewTargetComponent;
	if ( m_Target->GetOwner() )
	{
		if ( m_Target->IsOwnerDead() )
		{
			return;
		}
		// 타겟 컴포넌트에 록온 효과를 활성화
		m_Target->SetLockOn(true);
	}
	bEnableCameraRotationLag = true;
	m_Player->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	m_Player->GetCharacterMovement()->bOrientRotationToMovement = false;
	m_Player->GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.f, 0.0f); // 카메라 방향으로의 액터 회전 속도

}

void UPlayer_CameraArm::BreakLockOnTarget()
{
	if ( m_Target != nullptr )
	{
		m_Target->SetLockOn(false);
		m_Target = nullptr;
	}
	bEnableCameraRotationLag = false;
	m_Player->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	m_Player->GetCharacterMovement()->bOrientRotationToMovement = true;
	m_Player->GetCharacterMovement()->RotationRate = FRotator(0.0f, 1000.f, 0.0f); // 카메라 방향으로의 액터 회전 속도
}

ULockOnTargetComponent* UPlayer_CameraArm::GetSelectedTarget()
{
	TArray<ULockOnTargetComponent*> AvailableTargets = GetTargetComponents();
	if ( AvailableTargets.Num() == 0 )
	{
		return nullptr;
	}

	float ClosestDotToCenter = 0.f;
	ULockOnTargetComponent* TargetComponent = nullptr;

	for ( int32 i = 0; i < AvailableTargets.Num(); ++i )
	{
		// 카메라 컴포넌트의 정면 방향벡터와 카메라암 컴포넌트에서 타겟으로의 방향벡터의 내적을 구한다.
		// 내적 구하기 : A . B = Ax * Bx + Ay * By + Az * Bz
		// 1 : 정면, -1 : 정반대, 0 : 정면에서 수직
		
		float Dot = FVector::DotProduct(m_Player->GetCamera()->GetForwardVector(), (AvailableTargets[i]->GetComponentLocation() - GetComponentLocation()).GetSafeNormal());
		if (TargetComponent == nullptr)
		{
			ClosestDotToCenter = Dot;
			TargetComponent = AvailableTargets[i];
			continue;
		}

		if ( Dot > 0.9f )
		{
			float ex_distance = (TargetComponent->GetComponentLocation() - GetComponentLocation()).Size();
			float new_distance = (AvailableTargets[i]->GetComponentLocation() - GetComponentLocation()).Size();
			// 기존 타겟보다 내적값이 낮아도, 다음 타겟의 내적값이 0.9보다 크고 거리도 더 가까우면 다음타겟을 우선한다.
			if (ex_distance > new_distance)
			{
				ClosestDotToCenter = Dot;
				TargetComponent = AvailableTargets[i];
				continue;
			}
		}
		else
		{
			if (Dot > ClosestDotToCenter)
			{
				ClosestDotToCenter = Dot;
				TargetComponent = AvailableTargets[i];
			}
		}
	}

	return TargetComponent;
}

void UPlayer_CameraArm::SwitchTarget(ELockOnDirection SwitchDirection)
{
	if (m_Target == nullptr)
	{
		return;
	}

	TArray<ULockOnTargetComponent*> AvailableTargets = GetTargetComponents();	// Get targets within lock-on range	
	if ( AvailableTargets.Num() < 2 ) return;

	FVector CurrentTargetDir = ( m_Target->GetComponentLocation() - GetComponentLocation() ).GetSafeNormal();

	TArray<ULockOnTargetComponent*> ViableTargets;

	for ( ULockOnTargetComponent* Target : AvailableTargets )
	{
		if ( Target == m_Target )
		{
			continue;
		}

		FVector TargetDir = ( Target->GetComponentLocation() - GetComponentLocation() ).GetSafeNormal();
		FVector Cross = FVector::CrossProduct(CurrentTargetDir, TargetDir);
		// 언리얼은 왼손 좌표계를 사용하기 때문에 기존 타겟의 방향을 X, 교체할 타겟의 방향을 Y라고 했을 때
		// 교체할 타겟이 기존 타겟의 왼쪽에 있으면 외적의 Z는 아래를 가리키고, 오른쪽에 있으면 위를 가리킨다.
		if ( ( SwitchDirection == ELockOnDirection::Left && Cross.Z < 0.f ) || ( SwitchDirection == ELockOnDirection::Right && Cross.Z > 0.f ) )
		{
			// 타겟이 중복되어 들어가지 않도록 AddUnique로 추가함
			ViableTargets.AddUnique(Target);
		}
	}

	if ( ViableTargets.Num() == 0 )
	{
		return;
	}

	float ClosestDot = 0.f;
	ULockOnTargetComponent* TargetComponent = nullptr;
	for ( int32 i = 0; i < ViableTargets.Num(); ++i )
	{
		float TargetDot = FVector::DotProduct(CurrentTargetDir, ( ViableTargets[i]->GetComponentLocation() - GetComponentLocation() ).GetSafeNormal());
		if ( TargetDot > ClosestDot )
		{
			ClosestDot = TargetDot;
			TargetComponent = ViableTargets[i];
		}
	}
	m_Target->SetLockOn(false);
	m_Target = nullptr;
	LockOnTarget(TargetComponent);
}

TArray<class ULockOnTargetComponent*> UPlayer_CameraArm::GetTargetComponents()
{
	TArray<UPrimitiveComponent*> TargetPrimitive;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	// LockOnTarget 타입 오브젝트를 검출하도록 설정
	TEnumAsByte<EObjectTypeQuery> LockOnTarget = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	ObjectTypes.Add(LockOnTarget);
	// 락온 범위 내의 락온 콜리전 타입을 가진 컴포넌트들을 가져옴
	UKismetSystemLibrary::SphereOverlapComponents(GetOwner(), GetComponentLocation(), fMaxLockOnDistance, ObjectTypes, ULockOnTargetComponent::StaticClass(), TArray<AActor*>{GetOwner()}, TargetPrimitive);

	TArray<ULockOnTargetComponent*> TargetComps;
	for ( UPrimitiveComponent* Comp : TargetPrimitive )
	{
		// 플레이어의 정면 반경 180도 내에 존재하는 락온 컴포넌트만 추가함
		float Dot = FVector::DotProduct(m_Player->GetCamera()->GetForwardVector(), (Comp->GetComponentLocation() - GetComponentLocation()).GetSafeNormal());
		if (Dot > 0.f)
		{
			TargetComps.Add(Cast<ULockOnTargetComponent>(Comp));
		}
	}

	return TargetComps;
}

bool UPlayer_CameraArm::IsLockedOn()
{
	return m_Target != nullptr;
}