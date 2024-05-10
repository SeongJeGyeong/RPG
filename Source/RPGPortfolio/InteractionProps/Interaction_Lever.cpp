// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction_Lever.h"
#include "Components/BoxComponent.h"

// Sets default values
AInteraction_Lever::AInteraction_Lever()
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

void AInteraction_Lever::OnConstruction(const FTransform& _Transform)
{
	tCommand_Key = FText::FromString(TEXT("E"));
	tCommand_Name = FText::FromString(TEXT("당긴다"));
}

// Called when the game starts or when spawned
void AInteraction_Lever::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteraction_Lever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteraction_Lever::Interaction()
{
	UE_LOG(LogTemp, Warning, TEXT("레버 돌리기"));
}

