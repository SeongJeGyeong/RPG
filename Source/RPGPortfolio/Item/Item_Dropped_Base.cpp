// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Dropped_Base.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "../Header/Struct.h"
#include "../GameInstance_Base.h"
#include "../Characters/Player_Base_Knight.h"
#include "../Manager/GISubsystem_InvenMgr.h"

// Sets default values
AItem_Dropped_Base::AItem_Dropped_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));

	SetRootComponent(m_Trigger);
	m_Mesh->SetupAttachment(m_Trigger);
	m_Niagara->SetupAttachment(m_Trigger);

	m_Trigger->SetCollisionProfileName(TEXT("InteractionTrigger"));
	m_Mesh->SetCollisionProfileName(TEXT("IgnoreAll"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/RPGEffects/ParticlesNiagara/StatusEffects/MagicGlow/NS_Status_Magic_Glow.NS_Status_Magic_Glow'"));
	if (niagara.Succeeded())
	{
		m_Niagara->SetAsset(niagara.Object);
		m_Niagara->Activate(true);
	}

	tCommand_Key = FText::FromString(TEXT("F"));
	tCommand_Name = FText::FromString(TEXT("획득한다"));
}

void AItem_Dropped_Base::OnConstruction(const FTransform& _Transform)
{
	Super::OnConstruction(_Transform);
}

// Called when the game starts or when spawned
void AItem_Dropped_Base::BeginPlay()
{
	Super::BeginPlay();

	if (bIsSpawnEditor)
	{
		LoadImg();
	}
}

void AItem_Dropped_Base::LoadImg()
{
	FGameItemInfo* pItemInfo = GetGameInstance()->GetSubsystem<UGISubsystem_InvenMgr>()->GetItemInfo(m_IID);
	m_Img = FSoftObjectPath(pItemInfo->IconImgPath);
	m_Img.ToSoftObjectPath().PostLoadPath(nullptr);
	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
	pGameInst->ASyncLoadDataAsset(m_Img.ToSoftObjectPath());
}

void AItem_Dropped_Base::Interaction(AActor* _InteractedActor)
{
	UTexture2D* Img = m_Img.IsPending() ? m_Img.LoadSynchronous() : m_Img.Get();
	APlayer_Base_Knight* Player = Cast<APlayer_Base_Knight>(_InteractedActor);
	if ( IsValid(Player) )
	{
		Player->AcquireItem(m_IID, m_Stack, Img);
	}

	Destroy();
}
