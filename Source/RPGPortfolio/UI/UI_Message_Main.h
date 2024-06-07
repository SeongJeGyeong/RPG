// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Message_Main.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_Message_Main : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UTextBlock* m_ActionText;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UTextBlock* m_CommandText;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

	void SetMessageText(FText _Command, FText _Action);

};
