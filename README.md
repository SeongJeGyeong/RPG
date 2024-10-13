# 소개
언리얼 엔진 학습&포트폴리오 목적으로 제작한 프로젝트입니다.
소울라이크 RPG를 기반으로 한 시스템 및 기능 구현을 중심으로 개발했습니다.

+ 사용엔진 : UE 5.1
+ 플랫폼 : PC
+ 개발 인원 : 1

# 시연 영상
[![Video Label](http://img.youtube.com/vi/7MU7Xy-_0Ls/0.jpg)](https://youtu.be/7MU7Xy-_0Ls)

```c++
UCLASS()
class RPGPORTFOLIO_API UDamageType_Base : public UDamageType
{
	GENERATED_BODY()

private:
	EATTACK_TYPE	Atk_Type;
	EATTACK_WEIGHT	Atk_Weight;
public:
	EATTACK_TYPE GetAtkType() const { return Atk_Type; }
	void SetAtkType(const EATTACK_TYPE& _AtkType) { Atk_Type = _AtkType; }

	EATTACK_WEIGHT GetAtkWeight() const { return Atk_Weight; }
	void SetAtkWeight(const EATTACK_WEIGHT& _AtkWeight) { Atk_Weight = _AtkWeight; }
};
```
```c++
TSubclassOf<UDamageType_Base> DamageTypeBase = UDamageType_Base::StaticClass();
DamageTypeBase.GetDefaultObject()->SetAtkType(_AtkType);
DamageTypeBase.GetDefaultObject()->SetAtkWeight(Weight);	
UGameplayStatics::ApplyPointDamage(HitInfo.GetActor(), iDamage, HitInfo.ImpactPoint, HitInfo, GetController(), this, DamageTypeBase);
```
