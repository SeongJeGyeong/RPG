// Fill out your copyright notice in the Description page of Project Settings.

#include "FadeViewportClient.h"
#include "Engine/Canvas.h"

void UFadeViewportClient::PostRender(UCanvas* Canvas)
{
	Super::PostRender(Canvas);

	if ( bFade )
	{
		DrawScreenFade(Canvas);
	}
}

void UFadeViewportClient::ClearFade()
{
	bFade = false;
}

void UFadeViewportClient::Fade(const float _Duration, const bool _OutOrIn)
{
	const UWorld* pWorld = GetWorld();
	if (pWorld)
	{
		bFade = true;
		bOutOrIn = _OutOrIn;
		fFadeDuration = _Duration;
		fFadeStartTime = pWorld->GetTimeSeconds();
	}
}

void UFadeViewportClient::DrawScreenFade(UCanvas* Canvas)
{
	if ( bFade )
	{
		const UWorld* pWorld = GetWorld();
		if ( pWorld )
		{
			const float fCurrentTime = pWorld->GetTimeSeconds();
			const float fAlpha = FMath::Clamp((fCurrentTime - fFadeStartTime) / fFadeDuration, 0.f, 1.f);

			if (fAlpha >= 1.f && !bOutOrIn )
			{
				bFade = false;
			}
			else
			{
				FLinearColor FadeColor = FLinearColor::Black;
				// true면 페이드 아웃, false 면 페이드 인
				FadeColor.A = bOutOrIn ? fAlpha : 1 - fAlpha;
				Canvas->DrawColor = FadeColor.ToFColor(true);
				//Tex : 새로 그릴 텍스처
				//X, Y : 타일 시작지점(좌상단)의 좌표
				//XL, YL : 타일의 폭과 높이
				//U, V : 텍스처 시작지점(좌상단)의 좌표
				//UL, VL : 그릴 텍스처의 폭과 높이
				Canvas->DrawTile(Canvas->DefaultTexture, 0, 0, Canvas->ClipX, Canvas->ClipY, 0, 0, Canvas->DefaultTexture->GetSizeX(), Canvas->DefaultTexture->GetSizeY());
			}
		}
	}
}