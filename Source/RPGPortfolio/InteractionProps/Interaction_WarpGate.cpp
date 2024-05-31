// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction_WarpGate.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteraction_WarpGate::AInteraction_WarpGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(m_Mesh);
	m_Trigger->SetupAttachment(m_Mesh);

	m_Trigger->SetCollisionProfileName(TEXT("InteractionTrigger"));
	m_Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));

}

void AInteraction_WarpGate::OnConstruction(const FTransform& _Transform)
{
	Super::OnConstruction(_Transform);

	tCommand_Key = FText::FromString(TEXT("F"));
	tCommand_Name = FText::FromString(TEXT("들어간다"));
}

// Called when the game starts or when spawned
void AInteraction_WarpGate::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteraction_WarpGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteraction_WarpGate::Interaction()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("BossRoom"));
}

