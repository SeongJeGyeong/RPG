// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Sword_Base.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AWeapon_Sword_Base::AWeapon_Sword_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	m_HitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitCollision"));
	SetRootComponent(m_HitCollision);
	m_HitCollision->SetCollisionProfileName(TEXT("PlayerAttack"));
	m_HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	m_WeaponMesh->SetupAttachment(m_HitCollision);
	m_WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AWeapon_Sword_Base::BeginPlay()
{
	Super::BeginPlay();

	m_HitCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon_Sword_Base::CollisionBeginOverlap);
	m_HitCollision->SetGenerateOverlapEvents(true);
	m_HitCollision->SetUseCCD(true);

}

// Called every frame
void AWeapon_Sword_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon_Sword_Base::HitcollisionActivate(bool _Activate)
{
	if ( _Activate )
	{
		m_HitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		m_HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon_Sword_Base::CollisionBeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)
{
	FVector StartLocation = m_HitCollision->GetComponentLocation();
	FQuat Rotation = m_HitCollision->GetComponentQuat();
	float CapsuleRadius = m_HitCollision->GetUnscaledCapsuleRadius();
	float CapsuleHalfHeight = m_HitCollision->GetUnscaledCapsuleHalfHeight();

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
	FVector vMidpoint = m_HitCollision->GetComponentLocation();
	DrawDebugCapsule(GetWorld(), vMidpoint, CapsuleHalfHeight, CapsuleRadius, Rotation, color, false, 1.f);
}

