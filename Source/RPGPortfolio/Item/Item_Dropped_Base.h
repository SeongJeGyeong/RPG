// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"
#include "../System/DataAsset/DA_ItemData.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item_Dropped_Base.generated.h"

UCLASS()
class RPGPORTFOLIO_API AItem_Dropped_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem_Dropped_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class USphereComponent* m_Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* m_Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UNiagaraComponent* m_Niagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	EITEM_ID	m_IID;

	// 간접 프로퍼티 참조. 에셋을 수동으로 로드해야 함
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	TSoftObjectPtr<UDA_ItemData> m_ItemData;

private:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index);

	UFUNCTION()
	void OnNiagaraFinished(UNiagaraComponent* _Niagara);

};
