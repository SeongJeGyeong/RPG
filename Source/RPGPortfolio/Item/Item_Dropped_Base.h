// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../System/Interface/PlayerInteraction.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item_Dropped_Base.generated.h"

enum class EITEM_ID : uint8;

UCLASS()
class RPGPORTFOLIO_API AItem_Dropped_Base : public AActor, public IPlayerInteraction
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem_Dropped_Base();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	class USphereComponent* m_Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	UStaticMeshComponent* m_Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	class UNiagaraComponent* m_Niagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	EITEM_ID	m_IID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	int32		m_Stack = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	TSoftObjectPtr<UTexture2D>	m_Img;

	UPROPERTY()
	FText tCommand_Key;
	UPROPERTY()
	FText tCommand_Name;

protected:
	virtual void OnConstruction(const FTransform& _Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	const EITEM_ID GetDropItemID() { return m_IID; }
	void SetDropItemID(const EITEM_ID& _ItemID) { m_IID = _ItemID; }
	const int32 GetDropItemStack() { return m_Stack; }
	void SetDropItemStack(const int32& _Stack) { m_Stack = _Stack; }

	void SetDropItemImg(const TObjectPtr<UTexture2D> _Img) { m_Img = _Img; }
	void SetDropItemImg(const TSoftObjectPtr<UTexture2D> _Img) { m_Img = _Img.IsPending() ? _Img.LoadSynchronous() : _Img.Get(); }

	virtual FText GetCommand_Key() const override { return tCommand_Key; }
	virtual FText GetCommand_Name() const override { return tCommand_Name; }

	virtual void Interaction() override;
};
