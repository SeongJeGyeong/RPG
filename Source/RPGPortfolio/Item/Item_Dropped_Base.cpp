// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Dropped_Base.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "../UI/UI_Base.h"
#include "Kismet/GameplayStatics.h"
#include "../RPGPortfolioGameModeBase.h"


// Sets default values
AItem_Dropped_Base::AItem_Dropped_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));

	SetRootComponent(m_Trigger);
	m_Mesh->SetupAttachment(m_Trigger);
	m_Niagara->SetupAttachment(m_Trigger);

}

// Called when the game starts or when spawned
void AItem_Dropped_Base::BeginPlay()
{
	Super::BeginPlay();

	//m_Trigger->OnComponentBeginOverlap.AddDynamic(this, &AItem_Dropped_Base::BeginOverlap);
	//m_Trigger->OnComponentEndOverlap.AddDynamic(this, &AItem_Dropped_Base::EndOverlap);
	
}

// Called every frame
void AItem_Dropped_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void AItem_Dropped_Base::BeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)
//{
//	ARPGPortfolioGameModeBase* pGameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
//	if ( !IsValid(pGameMode) )
//	{
//		UE_LOG(LogTemp, Error, TEXT("GameMode Not Found"));
//		return;
//	}
//
//	UUI_Base* MainUI = pGameMode->GetMainHUD();
//	MainUI->ShowMessage(true);
//}
//
//void AItem_Dropped_Base::EndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index)
//{
//	ARPGPortfolioGameModeBase* pGameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
//	if ( !IsValid(pGameMode) )
//	{
//		UE_LOG(LogTemp, Error, TEXT("GameMode Not Found"));
//		return;
//	}
//
//	UUI_Base* MainUI = pGameMode->GetMainHUD();
//	MainUI->ShowMessage(false);
//}

void AItem_Dropped_Base::OnNiagaraFinished(UNiagaraComponent* _Niagara)
{
	Destroy();
}

