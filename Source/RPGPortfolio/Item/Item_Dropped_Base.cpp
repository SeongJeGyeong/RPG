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

	m_Trigger->SetCollisionProfileName(TEXT("ItemTrigger"));
	m_Mesh->SetCollisionProfileName(TEXT("IgnoreAll"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/RPGEffects/ParticlesNiagara/StatusEffects/MagicGlow/NS_Status_Magic_Glow.NS_Status_Magic_Glow'"));
	if (niagara.Succeeded())
	{
		m_Niagara->SetAsset(niagara.Object);
		m_Niagara->Activate(true);
	}
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