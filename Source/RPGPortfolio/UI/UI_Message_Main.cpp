// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Message_Main.h"
#include "Components/TextBlock.h"

void UUI_Message_Main::NativeConstruct()
{
	if ( !IsValid(m_ActionText) || !IsValid(m_CommandText) )
	{
		UE_LOG(LogTemp, Warning, TEXT("메인 메시지박스 로드 실패"));
	}

	Super::NativeConstruct();
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
