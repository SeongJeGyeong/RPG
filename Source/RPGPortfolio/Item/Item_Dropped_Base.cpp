// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Dropped_Base.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "../UI/UI_Base.h"
#include "Kismet/GameplayStatics.h"
#include "../RPGPortfolioGameModeBase.h"
#include "../Manager/Inventory_Mgr.h"
#include "../UI/UI_Message_Item.h"
#include "../Header/Struct.h"
#include "../GameInstance_Base.h"

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
}

void AItem_Dropped_Base::OnConstruction(const FTransform& _Transform)
{
	tCommand_Key = FText::FromString(TEXT("F"));
	tCommand_Name = FText::FromString(TEXT("획득한다"));
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
	FGameItemInfo* pItemInfo = UInventory_Mgr::GetInst(GetWorld())->GetItemInfo(m_IID);
	m_Img = FSoftObjectPath(pItemInfo->IconImgPath);
	m_Img.ToSoftObjectPath().PostLoadPath(nullptr);
	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
	pGameInst->ASyncLoadDataAsset(m_Img.ToSoftObjectPath());
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
	UUI_Base* pMainUI = pGameMode->GetMainHUD();
	pMainUI->ShowItemMessageUI(true);
	UTexture2D* Img = m_Img.IsPending() ? m_Img.LoadSynchronous() : m_Img.Get();
	pMainUI->GetItemMessageUI()->SetItemMessage(pItemInfo->ItemName, Img, m_Stack);
	pMainUI->ShowMainMessageUI(true);

	Destroy();
}
