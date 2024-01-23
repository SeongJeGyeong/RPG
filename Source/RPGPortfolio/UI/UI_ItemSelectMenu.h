// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_ItemSelectMenu.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_ItemSelectMenu : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UButton* m_Btn_Use;
	class UButton* m_Btn_Drop;
	class UButton* m_Btn_Discard;
	class UButton* m_Btn_DropAll;
	class UButton* m_Btn_DiscardAll;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

};
