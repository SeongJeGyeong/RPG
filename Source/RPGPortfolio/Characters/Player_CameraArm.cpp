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

	if (IsLockedOn())
	{
		if ( ( m_Target->GetComponentLocation() - GetComponentLocation() ).Size() > fMaxLockOnDistance + m_Target->GetScaledSphereRadius() )
		{
			BreakLockOnTarget();
			return;
		}

		FVector vTarVec = m_Target->GetComponentLocation() - ( GetComponentLocation());
		FRotator rTarRot = vTarVec.GetSafeNormal().Rotation();
		FRotator rCurRot = m_Player->GetControlRotation();
		FRotator rNewRot = FMath::RInterpTo(rCurRot, rTarRot, DeltaTime, LockonControlRotationRate);
		rNewRot.Roll = 0.f;
		// 타겟을 바라보도록 로테이션 수정
		m_Player->GetController()->SetControlRotation(rNewRot);
	}
}

// Toggle Lock On
bool UPlayer_CameraArm::ToggleCameraLockOn(const bool& _ToggleLockOn)
{
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

void UPlayer_CameraArm::LockOnTarget(ULockOnTargetComponent* _TargetComponent)
{
	m_Target = _TargetComponent;
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
	m_Player->GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.f, 0.0f); // 카메라 방향으로의 액터 회전 속도
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
	m_Player->GetCharacterMovement()->RotationRate = FRotator(0.f, 1200.f, 0.f); // 카메라 방향으로의 액터 회전 속도
}

ULockOnTargetComponent* UPlayer_CameraArm::GetSelectedTarget()
{
	TArray<ULockOnTargetComponent*> Targets = GetDetectedTargets();
	if ( Targets.Num() == 0 )
	{
		return nullptr;
	}

	float ClosestToCenter = 0.f;
	ULockOnTargetComponent* TargetComponent = nullptr;
	for ( auto Target : Targets )
	{
		float fDot = FVector::DotProduct(m_Player->GetCamera()->GetForwardVector(), ( Target->GetComponentLocation() - GetComponentLocation() ).GetSafeNormal());
		float fAngle = FMath::Acos(fDot);
		fAngle = FMath::RadiansToDegrees(fAngle);
		if ( TargetComponent == nullptr )
		{
			ClosestToCenter = fAngle;
			TargetComponent = Target;
			continue;
		}
		if ( fAngle < 25.f )
		{
			float ex_distance = ( TargetComponent->GetComponentLocation() - GetComponentLocation() ).Size();
			float new_distance = ( Target->GetComponentLocation() - GetComponentLocation() ).Size();
			if ( ex_distance > new_distance )
			{
				ClosestToCenter = fAngle;
				TargetComponent = Target;
				continue;
			}
		}
		else
		{
			if ( fAngle < ClosestToCenter )
			{
				ClosestToCenter = fDot;
				TargetComponent = Target;
			}
		}
	}

	return TargetComponent;
}

void UPlayer_CameraArm::SwitchTarget(ELockOnDirection _Direction)
{
	if (m_Target == nullptr)
	{
		return;
	}

	TArray<ULockOnTargetComponent*> DetectedTargets = GetDetectedTargets();
	// 한 개일 경우 현재 락온중인 컴포넌트이므로
	if ( DetectedTargets.Num() < 2 )
	{
		return;
	}

	FVector vCurTargetDir = ( m_Target->GetComponentLocation() - GetComponentLocation() ).GetSafeNormal();
	ULockOnTargetComponent* TargetComponent = nullptr;
	float fClosestDot = 0.f;
	for ( ULockOnTargetComponent* Target : DetectedTargets )
	{
		if ( Target == m_Target )
		{
			continue;
		}

		FVector vTargetDir = ( Target->GetComponentLocation() - GetComponentLocation() ).GetSafeNormal();
		FVector vCross = FVector::CrossProduct(vCurTargetDir, vTargetDir);
		// 언리얼은 왼손 좌표계를 사용하기 때문에 기존 타겟의 방향을 X, 교체할 타겟의 방향을 Y라고 했을 때
		// 교체할 타겟이 기존 타겟의 왼쪽에 있으면 외적의 Z는 아래를 가리키고, 오른쪽에 있으면 위를 가리킨다.
		if ( ( _Direction == ELockOnDirection::Left && vCross.Z < 0.f ) || ( _Direction == ELockOnDirection::Right && vCross.Z > 0.f ) )
		{

			float TargetDot = FVector::DotProduct(vCurTargetDir, vTargetDir);
			if ( TargetDot > fClosestDot )
			{
				fClosestDot = TargetDot;
				TargetComponent = Target;
			}
		}
	}
	if (TargetComponent == nullptr)
	{
		return;
	}

	m_Target->SetLockOn(false);
	m_Target = nullptr;
	LockOnTarget(TargetComponent);
}

TArray<class ULockOnTargetComponent*> UPlayer_CameraArm::GetDetectedTargets()
{
	TArray<UPrimitiveComponent*> OutComponents;
	// LockOnTarget 타입 오브젝트를 검출하도록 설정
	TEnumAsByte<EObjectTypeQuery> LockOnCollision = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	// 락온 범위 내의 락온 콜리전 타입을 가진 컴포넌트들을 가져옴
	UKismetSystemLibrary::SphereOverlapComponents(
		GetOwner(), 
		GetComponentLocation(), 
		fMaxLockOnDistance, 
		TArray<TEnumAsByte<EObjectTypeQuery>>{LockOnCollision}, 
		ULockOnTargetComponent::StaticClass(), 
		TArray<AActor*>{GetOwner()}, 
		OutComponents
	);

	TArray<ULockOnTargetComponent*> Targets;
	for ( UPrimitiveComponent* Comp : OutComponents )
	{
		// 카메라의 정면 반경 180도 내에 존재하는 락온 컴포넌트만 추가함
		float Dot = FVector::DotProduct(m_Player->GetCamera()->GetForwardVector(), (Comp->GetComponentLocation() - GetComponentLocation()).GetSafeNormal());
		if (Dot > 0.f)
		{
			Targets.Add(Cast<ULockOnTargetComponent>(Comp));
		}
	}

	return Targets;
}

bool UPlayer_CameraArm::IsLockedOn()
{
	return m_Target != nullptr;
}