// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_FootIK.h"
#include "Player_Base_Knight.h"

// Sets default values for this component's properties
UPlayer_FootIK::UPlayer_FootIK()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayer_FootIK::BeginPlay()
{
	Super::BeginPlay();

	// ...

    m_Player = Cast<APlayer_Base_Knight>(GetOwner());
    if (!IsValid(m_Player))
    {
        UE_LOG(LogTemp, Error, TEXT("Foot IK 소유한 액터 불러오기 실패"));
    }
}

void UPlayer_FootIK::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void UPlayer_FootIK::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FFootIKTraceInfo UPlayer_FootIK::IKFootTrace(float _TraceDistance, FName _Socket)
{
    FFootIKTraceInfo TraceInfo;

   /* FVector pSocketLocation = m_Player->GetMesh()->GetSocketLocation(_Socket);
    FVector pLine_Start = FVector(pSocketLocation.X, pSocketLocation.Y, m_Player->GetActorLocation().Z);
    FVector pLine_End = FVector(pSocketLocation.X, pSocketLocation.Y
        , ( m_Player->GetActorLocation().Z - m_fIKCapsuleHalkHeight ) - fTraceDistance);

    //! Process Line Trace
    FHitResult pHitResult;
    TArray<AActor*> pIgnore;
    pIgnore.Add(GetOwner());

    bool bDebug = true;
    EDrawDebugTrace::Type eDebug = EDrawDebugTrace::None;
    if ( bDebug == true ) eDebug = EDrawDebugTrace::ForOneFrame;

    bool bResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), pLine_Start, pLine_End,
        UEngineTypes::ConvertToTraceType(ECC_Visibility), true, pIgnore, eDebug, pHitResult, true);

    //! Set ImpactNormal and Offset from HitResult
    TraceInfo.pImpactLocation = pHitResult.ImpactNormal;
    if ( pHitResult.IsValidBlockingHit() == true )
    {
        float fImpactLegth = ( pHitResult.ImpactPoint - pHitResult.TraceEnd ).Size();
        TraceInfo.fOffset = 5.0f + ( fImpactLegth - fTraceDistance );
    }
    else
    {
        TraceInfo.fOffset = 0.0f;
    }
    */
    return TraceInfo; 
}

FRotator UPlayer_FootIK::NormalToRotator(FVector _Vec)
{
  /*  float fAtan2_1 = UKismetMathLibrary::DegAtan2(_Vec.Y, _Vec.Z);
    float fAtan2_2 = UKismetMathLibrary::DegAtan2(_Vec.X, _Vec.Z);
    fAtan2_2 *= -1.0f;
    FRotator pResult = FRotator(fAtan2_2, 0.0f, fAtan2_1);
    */
    return FRotator::ZeroRotator;
}

