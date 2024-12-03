// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Enum.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player_StatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UPlayer_StatComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UGISubsystem_StatMgr* m_StatMgr;

public:	
	// Sets default values for this component's properties
	UPlayer_StatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	float GetConsumeStaminaForState(EPlayerStateType _State); // 상태별 스태미나 소비
	
	bool IsHPZero();
	bool IsStaminaZero();
	bool IsEnoughStamina(float _Consumption);
	bool IsEnoughMP(float _Consumption);

	float CalculApplyDamage(EATTACK_TYPE _AtkType, EPlayerMontage _AtkMontage);
	float CalculTakeDamage(EATTACK_TYPE _AtkType, float _Damage);

	void DecreasePlayerHP(EATTACK_TYPE _AtkType, float _Damage);
	void DecreasePlayerMP(float _Consumption);
	void DecreasePlayerStamina(float _Consumption);

	void RestorePlayerHP(float _Recovery);
	void RestorePlayerMP(float _Recovery);

	void GainSoul(int32 _Soul);

	void HoldGuard(bool _IsGuard);
};
