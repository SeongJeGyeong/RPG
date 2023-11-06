// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_CameraArm.h"
#include "../System/Component/LockOnTargetComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Pawn.h"

UPlayer_CameraArm::UPlayer_CameraArm()
{
	MaxTargetLockDistance = 750.f;
	bDrawDebug = true;

	TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	bUsePawnControlRotation = true; // Rotate the arm based on the controller
	bEnableCameraLag = true;
	bEnableCameraRotationLag = false;
	CameraLagSpeed = 3.f;
	CameraRotationLagSpeed = 2.f;
	CameraLagMaxDistance = 100.f;
}

void UPlayer_CameraArm::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsCameraLockedToTarget())
	{
		DrawDebugSphere(GetWorld(), CameraTarget->GetComponentLocation(), 20.f, 16, FColor::Red);

		if ((CameraTarget->GetComponentLocation() - GetComponentLocation()).Size() > MaxTargetLockDistance + CameraTarget->GetScaledSphereRadius())
		{
			if (bUseSoftLock)
			{
				if (ULockOnTargetComponent* NewLockOnTarget = GetLockTarget())
				{
					LockToTarget(NewLockOnTarget);
				}
				else
				{
					BreakTargetLock();
				}
			}
			else
			{
				BreakTargetLock();
			}
		}
	}
	else
	{
		if (ULockOnTargetComponent* NewLockOnTarget = GetLockTarget())
		{
			LockToTarget(NewLockOnTarget);
		}
		else
		{
			bSoftlockRequiresReset = false;
		}
	}

	if (bDrawDebug)
	{
		for (ULockOnTargetComponent* Target : GetTargetComponents())
		{
			DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), Target->GetComponentLocation(), FColor::Green);
		}

		DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), MaxTargetLockDistance, 32, FColor::Cyan);

		UKismetSystemLibrary::DrawDebugString(this, FVector::ZeroVector, bUseSoftLock ? "Soft-lock Enabled" : "Soft-lock Disabled", GetOwner(), FLinearColor::Green);

		if (bSoftlockRequiresReset)
		{
			UKismetSystemLibrary::DrawDebugString(this, FVector(0.f, 0.f, -10.f), "Soft-lock Requires Reset", GetOwner(), FLinearColor::Green);
		}

	}

}

// Toggle Lock On
void UPlayer_CameraArm::ToggleCameraLock()
{
	if (bUseSoftLock)
	{
		bSoftlockRequiresReset = false;
		return;
	}

	if (IsCameraLockedToTarget())
	{
		BreakTargetLock();
		return;
	}

	ULockOnTargetComponent* NewLockOnTarget = GetLockTarget();

	if (NewLockOnTarget != nullptr)
	{
		LockToTarget(NewLockOnTarget);
	}

}

void UPlayer_CameraArm::ToggleSoftLock()
{
	bUseSoftLock = !bUseSoftLock;

	if (bUseSoftLock)
	{
		bSoftlockRequiresReset = false;
	}
	else
	{
		BreakTargetLock();
	}
}

void UPlayer_CameraArm::LockToTarget(ULockOnTargetComponent* NewTargetComponent)
{
	CameraTarget = NewTargetComponent;
	bEnableCameraRotationLag = true;
}

void UPlayer_CameraArm::BreakTargetLock()
{
	if (IsCameraLockedToTarget())
	{
		CameraTarget = nullptr;
		bEnableCameraRotationLag = false;
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
		float Dot = FVector::DotProduct(GetForwardVector(), (AvailableTargets[i]->GetComponentLocation() - GetComponentLocation()).GetSafeNormal());
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
