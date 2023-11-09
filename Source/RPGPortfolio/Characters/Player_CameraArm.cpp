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
	// 스프링 암 내장 설정
	TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	bUsePawnControlRotation = true; // Rotate the arm based on the controller
	bEnableCameraLag = true;
	bEnableCameraRotationLag = false;
	CameraLagSpeed = 3.f;
	CameraRotationLagSpeed = 2.f;
	CameraLagMaxDistance = 100.f;
	
	// 록온 범위
	fMaxTargetLockDistance = 750.f;

	bDrawDebug = true;
	bToggleLockOn = false;
	UCameraComponent* m_Camera = Cast<UCameraComponent>(GetChildComponent(0));
}

void UPlayer_CameraArm::BeginPlay()
{
	Super::BeginPlay();

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
			DrawDebugSphere(GetWorld(), m_Target->GetComponentLocation(), 20.f, 16, FColor::Red);

			if ((m_Target->GetComponentLocation() - GetComponentLocation()).Size() > fMaxTargetLockDistance + m_Target->GetScaledSphereRadius())
			{
				BreakLockOnTarget();
			}
		}
		else
		{
			if (ULockOnTargetComponent* NewLockOnTarget = GetLockTarget())
			{
				LockOnTarget(NewLockOnTarget);
			}
			else
			{
				// 록온 대상 찾기 실패 시 캐릭터 정면 방향으로 카메라 회전
				FRotator NewRot = FMath::RInterpTo(m_Player->GetControlRotation(), rForwardRotation, DeltaTime, 10.f);
				m_Player->GetController()->SetControlRotation(NewRot);

				if (m_Player->GetControlRotation().Equals(rForwardRotation, 1))
				{
					UE_LOG(LogTemp, Warning, TEXT("ToggleFalse"));
					bToggleLockOn = false;
				}
			}
		}
	}

	if (bDrawDebug)
	{
		for (ULockOnTargetComponent* Target : GetTargetComponents())
		{
			DrawDebugLine(GetWorld(), GetComponentLocation(), Target->GetComponentLocation(), FColor::Green);
		}
		/*FVector CameraLine = GetComponentLocation() + (m_Player->GetCamera()->GetForwardVector()) * 500.f;
		DrawDebugLine(GetWorld(), GetComponentLocation(), CameraLine, FColor::Blue);*/
		
		DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), fMaxTargetLockDistance, 32, FColor::Cyan);
	}

}

// Toggle Lock On
void UPlayer_CameraArm::ToggleCameraLockOn(const FInputActionInstance& _Instance)
{
	bToggleLockOn = (bToggleLockOn != _Instance.GetValue().Get<bool>());

	if (bToggleLockOn)
	{
		UE_LOG(LogTemp, Warning, TEXT("LockOn : True"));
		ULockOnTargetComponent* NewLockOnTarget = GetLockTarget();

		if (NewLockOnTarget != nullptr)
		{
			LockOnTarget(NewLockOnTarget);
		}
		else
		{
			rForwardRotation = m_Player->GetActorRotation();
		}
	}
	else
	{
		if (IsCameraLockedToTarget())
		{
			BreakLockOnTarget();
			return;
		}
	}
}

void UPlayer_CameraArm::LockOnTarget(ULockOnTargetComponent* NewTargetComponent)
{
	m_Target = NewTargetComponent;
	bEnableCameraRotationLag = true;
	m_Player->SetOrientRotation(false);
}

void UPlayer_CameraArm::BreakLockOnTarget()
{
	if (IsCameraLockedToTarget())
	{
		m_Target = nullptr;
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
		// 카메라 컴포넌트의 정면 방향벡터와 씬 컴포넌트에서 타겟으로의 방향벡터의 내적을 구한다.
		// 내적 구하기 : A . B = Ax * Bx + Ay * By + Az * Bz

		float Dot = FVector::DotProduct(m_Player->GetCamera()->GetForwardVector(), (AvailableTargets[i]->GetComponentLocation() - (GetComponentLocation())).GetSafeNormal());
		float DotX = m_Player->GetCamera()->GetForwardVector().X * (AvailableTargets[i]->GetComponentLocation() - (GetComponentLocation())).GetSafeNormal().X;
		float DotY = m_Player->GetCamera()->GetForwardVector().Y * (AvailableTargets[i]->GetComponentLocation() - (GetComponentLocation())).GetSafeNormal().Y;


		float acosAngleX = FMath::Acos(DotX);
		float fAngleX = FMath::RadiansToDegrees(acosAngleX);
		float acosAngleY = FMath::Acos(DotY);
		float fAngleY = FMath::RadiansToDegrees(acosAngleY);
		UE_LOG(LogTemp, Warning, TEXT("X내적 : %f"), DotX);
		UE_LOG(LogTemp, Warning, TEXT("X각도 : %f"), fAngleX);
		UE_LOG(LogTemp, Warning, TEXT("Y내적 : %f"), DotY);
		UE_LOG(LogTemp, Warning, TEXT("Y각도 : %f"), fAngleY);

		float acosAngle = FMath::Acos(Dot); //내적을 각도로 변환
		float fAngle = FMath::RadiansToDegrees(acosAngle); // 각도를 라디안 각도로 변환

		UE_LOG(LogTemp, Warning, TEXT("내적 : %f"), Dot);
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

	FVector CurrentTargetDir = (m_Target->GetComponentLocation() - GetComponentLocation()).GetSafeNormal();

	TArray<ULockOnTargetComponent*> ViableTargets;

	for (ULockOnTargetComponent* Target : AvailableTargets)
	{
		if (Target == m_Target)
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

	LockOnTarget(ViableTargets[BestDotIdx]);
}

TArray<class ULockOnTargetComponent*> UPlayer_CameraArm::GetTargetComponents()
{
	TArray<UPrimitiveComponent*> TargetPrimitive;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	// LockOnTarget 타입 오브젝트를 검출하도록 설정
	TEnumAsByte<EObjectTypeQuery> LockOnTarget = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	ObjectTypes.Add(LockOnTarget);
	UKismetSystemLibrary::SphereOverlapComponents(GetOwner(), GetComponentLocation(), fMaxTargetLockDistance, ObjectTypes, ULockOnTargetComponent::StaticClass(), TArray<AActor*>{GetOwner()}, TargetPrimitive);

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
	return m_Target != nullptr;
}
