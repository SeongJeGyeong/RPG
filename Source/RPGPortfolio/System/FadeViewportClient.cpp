// Fill out your copyright notice in the Description page of Project Settings.

#include "FadeViewportClient.h"
//#include "Engine/Canvas.h"

//void UFadeViewportClient::PostRender(UCanvas* Canvas)
//{
//	Super::PostRender(Canvas);
//
//	if ( bFading )
//	{
//		DrawScreenFade(Canvas);
//	}
//}
//
//void UFadeViewportClient::ClearFade()
//{
//	bFading = false;
//}
//
//void UFadeViewportClient::Fade(const float _Duration, const bool _ToBlack)
//{
//	const UWorld* pWorld = GetWorld();
//	if (pWorld)
//	{
//		bFading = true;
//		bToBlack = _ToBlack;
//		fFadeDuration = _Duration;
//		fFadeStartTime = pWorld->GetTimeSeconds();
//	}
//}
//
//void UFadeViewportClient::DrawScreenFade(UCanvas* Canvas)
//{
//	if (bFading)
//	{
//		const UWorld* pWorld = GetWorld();
//		if ( pWorld )
//		{
//			const float fCurrentTime = pWorld->GetTimeSeconds();
//			const float fAlpha = FMath::Clamp((fCurrentTime - fFadeStartTime) / fFadeDuration, 0.f, 1.f);
//
//			if (fAlpha >= 1.f && !bToBlack)
//			{
//				bFading = false;
//			}
//			else
//			{
//				FLinearColor FadeColor = FLinearColor::Black;
//				FadeColor.A = bToBlack ? fAlpha : 1 - fAlpha;
//				Canvas->DrawColor = FadeColor.ToFColor(true);
//				Canvas->DrawTile(Canvas->DefaultTexture, 0, 0, Canvas->ClipX, Canvas->ClipY, 0, 0, Canvas->DefaultTexture->GetSizeX(), Canvas->DefaultTexture->GetSizeY());
//			}
//		}
//	}
//}