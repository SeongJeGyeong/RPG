// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_CameraArm.h"
#include "../System/Component/LockOnTargetComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Pawn.h"
#include "Player_Base_Knight.h"

UPlayer_CameraArm::UPlayer_CameraArm()
{
	// 스프링 암 내장 설정
	TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	bUsePawnControlRotation = true; // Rotate the arm based on the controller
	bEnableCameraLag = true;
	bEnableCameraRotationLag = false;
	CameraLagSpeed = 3.f;
	CameraRotationLagSpeed = 2.f;
	CameraLagMaxDistance = 100.f;
	
	MaxTargetLockDistance = 750.f;
	bDrawDebug = true;
	bToggleLockOn = false;

	m_Player = Cast<APlayer_Base_Knight>(GetOwner());
	if (!IsValid(m_Player))
	{
		UE_LOG(LogTemp, Error, TEXT("스프링 암의 루트 액터 찾지 못함"));
	}
}

void UPlayer_CameraArm::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bToggleLockOn)
	{
		if (IsCameraLockedToTarget())
		{
			DrawDebugSphere(GetWorld(), CameraTarget->GetComponentLocation(), 20.f, 16, FColor::Red);

			if ((CameraTarget->GetComponentLocation() - GetComponentLocation()).Size() > MaxTargetLockDistance + CameraTarget->GetScaledSphereRadius())
			{
				BreakTargetLock();
			}
		}
		else
		{
			if (ULockOnTargetComponent* NewLockOnTarget = GetLockTarget())
			{
				LockToTarget(NewLockOnTarget);
			}
		}
	}

	if (bDrawDebug)
	{
		for (ULockOnTargetComponent* Target : GetTargetComponents())
		{
			DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), Target->GetComponentLocation(), FColor::Green);
		}

		DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), MaxTargetLockDistance, 32, FColor::Cyan);

		UKismetSystemLibrary::DrawDebugString(this, FVector::ZeroVector, "LockOn-Line", GetOwner(), FLinearColor::Green);
	}

}

// Toggle Lock On
void UPlayer_CameraArm::ToggleCameraLock(const FInputActionInstance& _Instance)
{
	bToggleLockOn = (bToggleLockOn != _Instance.GetValue().Get<bool>());

	if (bToggleLockOn)
	{
		UE_LOG(LogTemp, Warning, TEXT("LockOn : True"));
		ULockOnTargetComponent* NewLockOnTarget = GetLockTarget();

		if (NewLockOnTarget != nullptr)
		{
			LockToTarget(NewLockOnTarget);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LockOn : False"));
			bToggleLockOn = false;
		}
	}
	else
	{
		if (IsCameraLockedToTarget())
		{
			BreakTargetLock();
			return;
		}
	}
}

void UPlayer_CameraArm::LockToTarget(ULockOnTargetComponent* NewTargetComponent)
{
	CameraTarget = NewTargetComponent;
	bEnableCameraRotationLag = true;
	m_Player->SetOrientRotation(false);
}

void UPlayer_CameraArm::BreakTargetLock()
{
	if (IsCameraLockedToTarget())
	{
		CameraTarget = nullptr;
		bEnableCameraRotationLag = false;
		UE_LOG(LogTemp, Warning, TEXT("LockOn : False"));
		bToggleLockOn = false;
		m_Player->SetOrientRotation(true);
	}
}

ULockOnTargetComponent* UPlayer_CameraArm::GetLockTarget()
{
	TArray<ULockOnTargetComponent*> AvailableTargets = GetTargetComponents();
	if (AvailableTargets.Num() == 0)
	{
		return nullptr;
	}

	float ClosestDotToCenter = 0.f;
	ULockOnTargetComponent* TargetComponent = nullptr;

	for (int32 i = 0; i < AvailableTargets.Num(); ++i)
	{
		// 씬 컴포넌트의 정면 방향벡터와 씬 컴포넌트에서 타겟으로의 방향벡터의 내적을 구한다.
		// 내적 구하기 : A . B = Ax * Bx + Ay * By + Az * Bz

		float Dot = FVector::DotProduct(GetForwardVector(), (AvailableTargets[i]->GetComponentLocation() - GetComponentLocation()).GetSafeNormal());
		UE_LOG(LogTemp, Warning, TEXT("내적 : %f"), Dot);
		float acosAngle = FMath::Acos(Dot); //내적을 각도로 변환
		float fAngle = FMath::RadiansToDegrees(acosAngle); // 각도를 라디안 각도로 변환
		UE_LOG(LogTemp, Warning, TEXT("각도 : %f"), fAngle);

		if (Dot > ClosestDotToCenter)
		{
			ClosestDotToCenter = Dot;
			TargetComponent = AvailableTargets[i];
		}
	}

	return TargetComponent;
}

void UPlayer_CameraArm::SwitchTarget(ELockOnDirection SwitchDirection)
{
	if (!IsCameraLockedToTarget())
	{
		return;
	}

	TArray<ULockOnTargetComponent*> AvailableTargets = GetTargetComponents();	// Get targets within lock-on range	
	if (AvailableTargets.Num() < 2) return;

	FVector CurrentTargetDir = (CameraTarget->GetComponentLocation() - GetComponentLocation()).GetSafeNormal();

	TArray<ULockOnTargetComponent*> ViableTargets;

	for (ULockOnTargetComponent* Target : AvailableTargets)
	{
		if (Target == CameraTarget)
		{
			continue;
		}

		FVector TargetDir = (Target->GetComponentLocation() - GetComponentLocation()).GetSafeNormal();
		FVector Cross = FVector::CrossProduct(CurrentTargetDir, TargetDir);

		if ((SwitchDirection == ELockOnDirection::Left && Cross.Z < 0.f) || (SwitchDirection == ELockOnDirection::Right && Cross.Z > 0.f))
		{
			ViableTargets.AddUnique(Target);
		}
	}

	if (ViableTargets.Num() == 0)
	{
		return;
	}

	int32 BestDotIdx = 0;
	for (int32 i = 1; i < ViableTargets.Num(); ++i)
	{
		float BestDot = FVector::DotProduct(CurrentTargetDir, (ViableTargets[BestDotIdx]->GetComponentLocation() - GetComponentLocation()).GetSafeNormal());
		float TestDot = FVector::DotProduct(CurrentTargetDir, (ViableTargets[i]->GetComponentLocation() - GetComponentLocation()).GetSafeNormal());

		if (TestDot > BestDot)
		{
			BestDotIdx = i;
		}
	}

	LockToTarget(ViableTargets[BestDotIdx]);
}

TArray<class ULockOnTargetComponent*> UPlayer_CameraArm::GetTargetComponents()
{
	TArray<UPrimitiveComponent*> TargetPrimitive;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { EObjectTypeQuery::ObjectTypeQuery2 }; // World dynamic object type

	UKismetSystemLibrary::SphereOverlapComponents(GetOwner(), GetComponentLocation(), MaxTargetLockDistance, ObjectTypes, ULockOnTargetComponent::StaticClass(), TArray<AActor*>{GetOwner()}, TargetPrimitive);

	TArray<ULockOnTargetComponent*> TargetComps;
	for (UPrimitiveComponent* Comp : TargetPrimitive)
	{
		TargetComps.Add(Cast<ULockOnTargetComponent>(Comp));
	}

	//return TArray<class ULockOnTargetComponent*>();
	return TargetComps;
}

bool UPlayer_CameraArm::IsCameraLockedToTarget()
{
	return CameraTarget != nullptr;
}
