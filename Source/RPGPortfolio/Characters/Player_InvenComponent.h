// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player_InvenComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAcquireItemDelegate, FString, int32, UTexture2D*);	// 아이템 획득 메시지 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQSDelayRateDelegate, float);								// 퀵슬롯 딜레이 갱신 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQSDelayDelegate, bool);									// 퀵슬롯 사용 대기상태 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnQSChangeAnimDelegate);											// 퀵슬롯 변경 애니메이션 재생 델리게이트

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UPlayer_InvenComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UGISubsystem_InvenMgr* m_InvenMgr;

	bool bItemDelay;		// 아이템 사용 딜레이 체크용
	FTimerHandle ItemDelayTimer;
	float fDelayRate;

public:
	FOnAcquireItemDelegate OnAcquireItem;
	FOnQSDelayRateDelegate OnQSDelayRate;
	FOnQSDelayDelegate OnQSDelay;
	FOnQSChangeAnimDelegate OnQSChangeAnim;

public:	
	// Sets default values for this component's properties
	UPlayer_InvenComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	bool GetbItemDelay() const { return bItemDelay; }
	void ChangeQuickSlot();
	FInvenItemRow* GetQuickSlotItem();
	FGameItemInfo* GetItemInfo(EITEM_ID _Id);
	void DecreaseInventoryItem(EITEM_ID _Id, EEQUIP_SLOT _Slot);
	void AcquireDroppedItem(EITEM_ID _Id, int32 _Stack, UTexture2D* _Img);

	void ItemDelaytime(float _DelayPercent);	// 아이템 사용 대기시간
};
