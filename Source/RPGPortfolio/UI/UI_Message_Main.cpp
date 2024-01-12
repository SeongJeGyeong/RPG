// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Message_Main.h"
#include "Components/TextBlock.h"

void UUI_Message_Main::NativeConstruct()
{
	Super::NativeConstruct();
	m_ActionText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ActionText")));
	m_CommandText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CommandText")));
	
}

void UUI_Message_Main::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_Message_Main::SetMessageText(FText _Command, FText _Message)
{
	m_CommandText->SetText(_Command);
	m_ActionText->SetText(_Message);
}
