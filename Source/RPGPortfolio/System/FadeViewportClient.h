// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "FadeViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UFadeViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
private:
    bool bFading = false;
    bool bToBlack = false;
    float fFadeAlpha;
    float fFadeStartTime;
    float fFadeDuration;

public:
    // 메뉴, 콘솔 등을 렌더링하기 위해 플레이어 뷰와 HUD를 렌더링한 후 호출.
    // 이것은 렌더링 루프의 마지막 렌더링 호출입니다.
    virtual void PostRender(UCanvas* Canvas) override;

    // 페이딩 상태 초기화
    virtual void ClearFade();

    // 페이드 인/아웃 설정
    virtual void Fade(const float _Duration, const bool _ToBlack);
    
    // 스크린에 페이드 효과 적용
    void DrawScreenFade(UCanvas* Canvas);
};
