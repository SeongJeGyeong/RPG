// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Base_Knight.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
APlayer_Base_Knight::APlayer_Base_Knight()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	m_Arm->SetupAttachment(GetRootComponent());
	m_Camera->SetupAttachment(m_Arm);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> Skel_Mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));
	if (Skel_Mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(Skel_Mesh.Object);
	}

}

// Called when the game starts or when spawned
void APlayer_Base_Knight::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* pController = Cast<APlayerController>(GetController());

	if (pController)
	{
		ULocalPlayer* pLocalPlayer = pController->GetLocalPlayer();

		if (pLocalPlayer && !m_IMC.IsNull())
		{
			UEnhancedInputLocalPlayerSubsystem* pSubsystem = pLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			pSubsystem->AddMappingContext(m_IMC.LoadSynchronous(), 0);
		}
	}
}

// Called every frame
void APlayer_Base_Knight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayer_Base_Knight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (nullptr == InputComp)
	{
		return;
	}
}

void APlayer_Base_Knight::MoveAction(const FInputActionInstance& _Instance)
{
}

void APlayer_Base_Knight::RotateAction(const FInputActionInstance& _Instance)
{
}

void APlayer_Base_Knight::JumpAction(const FInputActionInstance& _Instance)
{
}

