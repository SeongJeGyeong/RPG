// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Dropped_Base.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "../UI/UI_Base.h"
#include "Kismet/GameplayStatics.h"
#include "../RPGPortfolioGameModeBase.h"
#include "../Manager/Inventory_Mgr.h"
#include "../UI/UI_Message_Item.h"

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

	m_Trigger->SetCollisionProfileName(TEXT("InteractionTrigger"));
	m_Mesh->SetCollisionProfileName(TEXT("IgnoreAll"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/RPGEffects/ParticlesNiagara/StatusEffects/MagicGlow/NS_Status_Magic_Glow.NS_Status_Magic_Glow'"));
	if (niagara.Succeeded())
	{
		m_Niagara->SetAsset(niagara.Object);
		m_Niagara->Activate(true);
	}
}

void AItem_Dropped_Base::OnConstruction(const FTransform& _Transform)
{
	tCommand_Key = FText::FromString(TEXT("E"));
	tCommand_Name = FText::FromString(TEXT("획득한다"));
}

// Called when the game starts or when spawned
void AItem_Dropped_Base::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItem_Dropped_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem_Dropped_Base::Interaction()
{
	ARPGPortfolioGameModeBase* pGameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if ( !IsValid(pGameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("AItem_Dropped_Base : GameMode Not Found"));
		return;
	}

	UInventory_Mgr::GetInst(GetWorld())->AddGameItem(m_IID, m_Stack);
	FGameItemInfo* pItemInfo = UInventory_Mgr::GetInst(GetWorld())->GetItemInfo(m_IID);
	USoundBase* pSound = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Blueprint/Player/Sound/SC_Player_GetItem.SC_Player_GetItem'"));

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), pSound, GetActorLocation());
	UUI_Base* pMainUI = pGameMode->GetMainHUD();
	pMainUI->ShowItemMessageUI(true);
	pMainUI->GetItemMessageUI()->SetItemMessage(pItemInfo->ItemName, pItemInfo->IconImgPath, m_Stack);
	pMainUI->ShowMainMessageUI(true);
	
	Destroy();
}
