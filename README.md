# 소개
언리얼 엔진 학습&포트폴리오 목적으로 제작한 프로젝트입니다.
소울라이크 RPG를 기반으로 한 시스템 및 기능 구현을 중심으로 개발했습니다.

+ 사용엔진 : UE 5.1
+ 플랫폼 : PC
+ 개발 인원 : 1

# 시연 영상
[![Video Label](http://img.youtube.com/vi/HorUYzrwmZI/0.jpg)](https://youtu.be/HorUYzrwmZI)

# 설명

## 1. 플레이어 입력 로직
<details>
    <summary>접기/펼치기</summary>

플레이어의 입력은 향상된 입력(Enhanced Input)플러그인을 이용해 구현했습니다.
![Image](https://github.com/user-attachments/assets/ac1cf159-d8f7-4f12-a07d-b816e5e0760a)
https://github.com/SeongJeGyeong/RPG/blob/e6a45169d92dfc23bffdefdebd49f5f7d1d23db7/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L80-L85
https://github.com/SeongJeGyeong/RPG/blob/e6a45169d92dfc23bffdefdebd49f5f7d1d23db7/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L169-L240

플레이어의 입력 로직은 상태 패턴을 사용해 구현했습니다.
![Image](https://github.com/user-attachments/assets/c79926c1-2b21-4cf2-8c0f-186ceaa3642d)

추상클래스로 선언한 StateMachine 클래스를 상속받아 상태 클래스들을 만들어 구현했습니다.
스테이트 클래스는 Tunique_Ptr로 선언해 상태를 변경할 때 마다 클래스를 새로 선언하지 않고 오브젝트 풀에서 풀링하여 재사용 하도록 만들었습니다.
https://github.com/SeongJeGyeong/RPG/blob/dc9985124fc348ad43c38dda8fb034afaae7aa5e/Source/RPGPortfolio/Characters/State/StateMachine.h#L12-L19
https://github.com/SeongJeGyeong/RPG/blob/dc9985124fc348ad43c38dda8fb034afaae7aa5e/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L242-L261
https://github.com/SeongJeGyeong/RPG/blob/dc9985124fc348ad43c38dda8fb034afaae7aa5e/Source/RPGPortfolio/System/Subsys_ObjectPool.cpp#L68-L83

</details>

## 2. 락온
![Image](https://github.com/user-attachments/assets/a459849d-8a1b-4887-9781-5210f2505f7f)
<details>
    <summary>접기/펼치기</summary>

### 설명

</details>

## 3. 상호 작용
<details>
    <summary>접기/펼치기</summary>

### 설명

</details>

## 4. 인벤토리 & 장비 & 아이템
<details>
    <summary>접기/펼치기</summary>

### 설명

</details>

## 5. 공격 & 피격 시스템
<details>
    <summary>접기/펼치기</summary>

### 설명

</details>

## 6. 투사체
<details>
    <summary>접기/펼치기</summary>

### 설명

</details>

## 7. 몬스터 AI
<details>
    <summary>접기/펼치기</summary>

### 설명

</details>
