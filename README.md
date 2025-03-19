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

#### 플레이어의 입력은 향상된 입력(Enhanced Input)플러그인을 이용해 구현했습니다.
![Image](https://github.com/user-attachments/assets/ac1cf159-d8f7-4f12-a07d-b816e5e0760a)
https://github.com/SeongJeGyeong/RPG/blob/e6a45169d92dfc23bffdefdebd49f5f7d1d23db7/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L80-L85
https://github.com/SeongJeGyeong/RPG/blob/e6a45169d92dfc23bffdefdebd49f5f7d1d23db7/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L169-L240

#### 플레이어의 입력 로직은 상태 패턴을 사용해 구현했습니다.
![Image](https://github.com/user-attachments/assets/c79926c1-2b21-4cf2-8c0f-186ceaa3642d)

##### 추상클래스로 선언한 StateMachine 클래스를 상속받아 상태 클래스들을 만들어 구현했습니다.
##### 스테이트 클래스는 Tunique_Ptr로 선언해 상태를 변경할 때 마다 클래스를 새로 선언하지 않고 오브젝트 풀에서 풀링하여 재사용 하도록 만들었습니다.
https://github.com/SeongJeGyeong/RPG/blob/dc9985124fc348ad43c38dda8fb034afaae7aa5e/Source/RPGPortfolio/Characters/State/StateMachine.h#L12-L19
https://github.com/SeongJeGyeong/RPG/blob/dc9985124fc348ad43c38dda8fb034afaae7aa5e/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L242-L261
https://github.com/SeongJeGyeong/RPG/blob/dc9985124fc348ad43c38dda8fb034afaae7aa5e/Source/RPGPortfolio/System/Subsys_ObjectPool.cpp#L68-L83

</details>

## 2. 락온
![Image](https://github.com/user-attachments/assets/3be2630e-bbd2-40d7-9654-009aa67d03bf)
<details>
    <summary>접기/펼치기</summary>

#### 락온은 Player_CameraArm와 LockOnTargetComponent를 이용해 구현했습니다.

![Image](https://github.com/user-attachments/assets/174da834-e449-4860-9d69-50541b2c5265)
![Image](https://github.com/user-attachments/assets/4c0d2a80-145c-417e-8dc6-2f954246e2fe)
##### 락온을 실행하면 우선 카메라암을 중심으로 구체형 범위 내에 오버랩 이벤트를 발생시켜 오버랩된 락온 타겟 컴포넌트를 검출한 뒤, 검출된 컴포넌트 중 카메라 정면에 존재하는 컴포넌트만 타겟 배열에 추가합니다.
https://github.com/SeongJeGyeong/RPG/blob/aefae85385cf8c711bd623c7ad25993b3e4dceac/Source/RPGPortfolio/Characters/Player_CameraArm.cpp#L205-L233
##### 타겟 배열의 컴포넌트 중 가장 정면에 가까운 컴포넌트를 락온 대상으로 합니다. 단, 카메라 중앙 기준 50도 이내에 있는 컴포넌트들은 거리가 더 가까운 컴포넌트를 우선적으로 락온합니다.
https://github.com/SeongJeGyeong/RPG/blob/aefae85385cf8c711bd623c7ad25993b3e4dceac/Source/RPGPortfolio/Characters/Player_CameraArm.cpp#L119-L159

![Image](https://github.com/user-attachments/assets/95e59c6a-ba6a-4081-81ae-a396b21991d6)
![Image](https://github.com/user-attachments/assets/81324557-07b7-4759-8e1c-91d0d700d3f5)
##### 락온 대상을 스위칭 할 경우, 우선 타겟 검출 함수를 통해 유효한 타겟 목록을 가져오고 현재 타겟 기준 다른 타겟의 방향과 거리를 계산하기 위해 현재 락온된 타겟의 방향을 구합니다.
##### 검출된 타겟 목록에서 스위칭 하고자 하는 방향과 일치하는 방향에 있는 대상 중 원래 락온 대상에서 가장 가까운 대상으로 락온을 변경합니다.
https://github.com/SeongJeGyeong/RPG/blob/aefae85385cf8c711bd623c7ad25993b3e4dceac/Source/RPGPortfolio/Characters/Player_CameraArm.cpp#L161-L203

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
