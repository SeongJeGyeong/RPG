# 소개
언리얼 엔진 학습&포트폴리오 목적으로 제작한 프로젝트입니다.
소울라이크 RPG를 기반으로 한 시스템 및 기능 구현을 중심으로 개발했습니다.

+ 사용엔진 : UE 5.1
+ 플랫폼 : PC
+ 개발 인원 : 1

# 시연 영상
[![Video Label](http://img.youtube.com/vi/HorUYzrwmZI/0.jpg)](https://youtu.be/HorUYzrwmZI)

# 기능 설명

## 1. 플레이어 입력 로직
<details>
    <summary>보기</summary>

---
플레이어의 입력은 향상된 입력(Enhanced Input)플러그인을 이용해 구현했습니다.

![Image](https://github.com/user-attachments/assets/ac1cf159-d8f7-4f12-a07d-b816e5e0760a)
<details>
    <summary>코드</summary>
            
https://github.com/SeongJeGyeong/RPG/blob/e6a45169d92dfc23bffdefdebd49f5f7d1d23db7/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L80-L85
https://github.com/SeongJeGyeong/RPG/blob/e6a45169d92dfc23bffdefdebd49f5f7d1d23db7/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L169-L240
</details>

---
플레이어의 입력 로직은 상태 패턴을 사용해 구현했습니다.

![Image](https://github.com/user-attachments/assets/c79926c1-2b21-4cf2-8c0f-186ceaa3642d)

추상클래스로 선언한 StateMachine 클래스를 상속받아 상태 클래스들을 만들어 구현했습니다.
스테이트 클래스는 Tunique_Ptr로 선언해 상태를 변경할 때 마다 클래스를 새로 선언하지 않고 오브젝트 풀에서 풀링하여 재사용 하도록 만들었습니다.
<details>
    <summary>코드</summary>
    
https://github.com/SeongJeGyeong/RPG/blob/dc9985124fc348ad43c38dda8fb034afaae7aa5e/Source/RPGPortfolio/Characters/State/StateMachine.h#L12-L19
https://github.com/SeongJeGyeong/RPG/blob/dc9985124fc348ad43c38dda8fb034afaae7aa5e/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L242-L261
https://github.com/SeongJeGyeong/RPG/blob/dc9985124fc348ad43c38dda8fb034afaae7aa5e/Source/RPGPortfolio/System/Subsys_ObjectPool.cpp#L68-L83
</details>
</details>

## 2. 락온
![Image](https://github.com/user-attachments/assets/3be2630e-bbd2-40d7-9654-009aa67d03bf)
<details>
    <summary>보기</summary>

---
락온은 Player_CameraArm와 LockOnTargetComponent를 이용해 구현했습니다.

![Image](https://github.com/user-attachments/assets/4c0d2a80-145c-417e-8dc6-2f954246e2fe)

락온을 실행하면 우선 카메라암을 중심으로 구체형 범위 내에 오버랩 이벤트를 발생시켜 오버랩된 락온 타겟 컴포넌트를 검출한 뒤, 검출된 컴포넌트 중 카메라 정면에 존재하는 컴포넌트만 타겟 배열에 추가합니다.
<details>
    <summary>코드</summary>
    
https://github.com/SeongJeGyeong/RPG/blob/aefae85385cf8c711bd623c7ad25993b3e4dceac/Source/RPGPortfolio/Characters/Player_CameraArm.cpp#L205-L233
</details>

타겟 배열의 컴포넌트 중 가장 정면에 가까운 컴포넌트를 락온 대상으로 합니다. 단, 카메라 중앙 기준 50도 이내에 있는 컴포넌트들은 거리가 더 가까운 컴포넌트를 우선적으로 락온합니다.
<details>
    <summary>코드</summary>
    
https://github.com/SeongJeGyeong/RPG/blob/aefae85385cf8c711bd623c7ad25993b3e4dceac/Source/RPGPortfolio/Characters/Player_CameraArm.cpp#L119-L159
</details>

---
![Image](https://github.com/user-attachments/assets/81324557-07b7-4759-8e1c-91d0d700d3f5)

락온 대상을 스위칭 할 경우, 우선 타겟 검출 함수를 통해 유효한 타겟 목록을 가져오고 현재 타겟 기준 다른 타겟의 방향과 거리를 계산하기 위해 현재 락온된 타겟의 방향을 구합니다.
검출된 타겟 목록에서 스위칭 하고자 하는 방향과 일치하는 방향에 있는 대상 중 원래 락온 대상에서 가장 가까운 대상으로 락온을 변경합니다.
<details>
    <summary>코드</summary>

https://github.com/SeongJeGyeong/RPG/blob/aefae85385cf8c711bd623c7ad25993b3e4dceac/Source/RPGPortfolio/Characters/Player_CameraArm.cpp#L161-L203
</details>

---
락온 중인 대상은 락온 타겟 컴포넌트에 락온 위젯을 표시합니다.
<details>
    <summary>코드</summary>

https://github.com/SeongJeGyeong/RPG/blob/7e3778cfaea6e92e17ba9f8dff82f1efc7121474/Source/RPGPortfolio/System/Component/LockOnTargetComponent.cpp#L30-L44
</details>

몬스터가 락온 중일 때만 커스텀 뎁스 패스 렌더를 활성화하여 락온 중인 적에게 붉은 외곽선을 표시하도록 만들었습니다.

![Image](https://github.com/user-attachments/assets/8c5c6d4e-5d28-4a15-9649-4266b226764f)
![Image](https://github.com/user-attachments/assets/7dba4672-0500-44bc-947e-1f97f41a1ac3)
![Image](https://github.com/user-attachments/assets/df17e708-5872-4de3-8e3d-627c0574267f)
![Image](https://github.com/user-attachments/assets/1c68484b-3807-4791-83a4-a2917f988568)

현재 픽셀의 위치와 주변 픽셀의 위치의 CustomStencil 값을 계산하는 머티리얼 함수를 작성합니다.
![Image](https://github.com/user-attachments/assets/17d7b56b-62e9-4865-9faa-5f41f82e1ece)

포스트 프로세스용 머티리얼을 생성하고 함수를 통해 출력받은 값을 이용해
1) 현재 픽셀의 CutomStencil 값이 0일 경우(CustomStencil이 지정되지 않은 픽셀)
2) 주변 픽셀 중 하나라도 CustomStencil값이 2(몬스터 메시의 스텐실값) 이상인 경우
현재 픽셀의 색을 빨간색으로 만들어 몬스터 메시 주변의 픽셀만 빨간색으로 만들도록 만들었습니다.

![Image](https://github.com/user-attachments/assets/7a715a21-efd4-4daf-a4cc-b8615a8650a6)
![Image](https://github.com/user-attachments/assets/7068d822-9cd5-4e41-96c2-1441d704edfc)

</details>

## 3. 상호 작용
![Image](https://github.com/user-attachments/assets/a157deb2-67a3-4e74-bdeb-1fc04a76a9a5)
<details>
    <summary>보기</summary>

---
언리얼 인터페이스 클래스를 상속하여 상호 작용 가능한 오브젝트를 만들었습니다.

![Image](https://github.com/user-attachments/assets/16d04358-41a7-40f6-9908-47bfeb65a32d)
![Image](https://github.com/user-attachments/assets/ef037b21-fea8-4381-86cb-6426792fce9e)
<details>
    <summary>코드</summary>

https://github.com/SeongJeGyeong/RPG/blob/98bd45543d90aaa14750c3a5f6784e21042c6249/Source/RPGPortfolio/System/Interface/PlayerInteraction.h#L17-L25
https://github.com/SeongJeGyeong/RPG/blob/98bd45543d90aaa14750c3a5f6784e21042c6249/Source/RPGPortfolio/InteractionProps/Interaction_WarpGate.cpp#L46-L49
https://github.com/SeongJeGyeong/RPG/blob/98bd45543d90aaa14750c3a5f6784e21042c6249/Source/RPGPortfolio/Item/Item_Dropped_Base.cpp#L69-L79
https://github.com/SeongJeGyeong/RPG/blob/98bd45543d90aaa14750c3a5f6784e21042c6249/Source/RPGPortfolio/InteractionProps/Interaction_Lever.cpp#L51-L80
</details>

캐릭터가 상호 작용 가능한 오브젝트에 오버랩될 경우 상호 작용 텍스트를 표시하고 오브젝트를 상호작용 배열에 추가합니다.
<details>
    <summary>코드</summary>
    
https://github.com/SeongJeGyeong/RPG/blob/98bd45543d90aaa14750c3a5f6784e21042c6249/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L1156-L1165
</details>

상호작용 키를 입력 시 현재 오버랩된 상호 작용 오브젝트중 가장 최근에 오버랩된 오브젝트의 Interaction 함수를 실행합니다.
<details>
    <summary>코드</summary>
    
https://github.com/SeongJeGyeong/RPG/blob/98bd45543d90aaa14750c3a5f6784e21042c6249/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L513-L535
</details>
</details>

## 4. 인벤토리 & 장비 & 아이템
![Image](https://github.com/user-attachments/assets/254f79e0-540d-4388-b1e5-fa2c63f2ca38)
<details>
    <summary>보기</summary>

---
UGameInstanceSubsystem 클래스를 상속받아 인벤토리 매니저 클래스를 구현하여 레벨이 변경되어도 인벤토리 정보가 유지되도록 만들었습니다.
아이템들의 정보는 데이터 테이블을 통해 관리하고, 인벤토리 매니저가 초기화될 때 테이블에서 아이템 정보를 가져와 Tmap 컨테이너에 저장합니다.
![Image](https://github.com/user-attachments/assets/e54d9ebd-cd32-4e35-b5aa-cc18bcab0c8d)
<details>
    <summary>코드</summary>
    
https://github.com/SeongJeGyeong/RPG/blob/ccb1873a8b70927de28a8f7a7867db97d5a75ef3/Source/RPGPortfolio/Manager/GISubsystem_InvenMgr.cpp#L8-L21
https://github.com/SeongJeGyeong/RPG/blob/ccb1873a8b70927de28a8f7a7867db97d5a75ef3/Source/RPGPortfolio/Manager/GISubsystem_InvenMgr.cpp#L36-L48
</details>

플레이어 캐릭터는 Player_InvenComponent를 통해 인벤토리 매니저와 통신합니다.
플레이어가 아이템을 획득할 경우 인벤토리 컴포넌트를 통해 인벤토리 매니저로 아이템 ID와 수량을 전달합니다. 퀵슬롯에 등록된 아이템의 경우 퀵슬롯 UI도 갱신합니다.
<details>
    <summary>코드</summary>
    
https://github.com/SeongJeGyeong/RPG/blob/ccb1873a8b70927de28a8f7a7867db97d5a75ef3/Source/RPGPortfolio/Characters/Player_InvenComponent.cpp#L89-L106
</details>

인벤토리 매니저와 위젯간에는 델리게이트를 통해 연결하여 UI를 갱신하도록 만들었습니다.
<details>
    <summary>코드</summary>
    
https://github.com/SeongJeGyeong/RPG/blob/ccb1873a8b70927de28a8f7a7867db97d5a75ef3/Source/RPGPortfolio/Manager/GISubsystem_InvenMgr.h#L11-L16
https://github.com/SeongJeGyeong/RPG/blob/ccb1873a8b70927de28a8f7a7867db97d5a75ef3/Source/RPGPortfolio/Manager/GISubsystem_InvenMgr.h#L39-L47
</details>

---
인벤토리의 타일 뷰 위젯에 표시된 아이템에 커서를 올리면 아이템 정보를 표시합니다.

![Image](https://github.com/user-attachments/assets/254f79e0-540d-4388-b1e5-fa2c63f2ca38)
<details>
    <summary>코드</summary>
    
https://github.com/SeongJeGyeong/RPG/blob/ccb1873a8b70927de28a8f7a7867db97d5a75ef3/Source/RPGPortfolio/UI/UI_Inventory.cpp#L92-L113
</details>

카테고리를 변경하는 것으로 해당 카테고리의 아이템만 표시할 수 있습니다.

![Image](https://github.com/user-attachments/assets/42555cf8-2c6a-4c94-9638-ec0c4174c003)
<details>
    <summary>코드</summary>
    
https://github.com/SeongJeGyeong/RPG/blob/19be35acea13fefd64342ecf4c7d382733a4afdd/Source/RPGPortfolio/UI/UI_Inventory.cpp#L115-L147
</details>

아이템을 클릭하면 메뉴 앵커를 생성하고 메뉴 앵커의 사용 버튼을 클릭하여 인벤토리에서 아이템을 사용할 수 있습니다.

![Image](https://github.com/user-attachments/assets/26603ad2-fc18-48d7-b3e3-9711ee7df0d6)
<details>
    <summary>코드</summary>
    
https://github.com/SeongJeGyeong/RPG/blob/ccb1873a8b70927de28a8f7a7867db97d5a75ef3/Source/RPGPortfolio/UI/UI_InvenItem.cpp#L75-L84
https://github.com/SeongJeGyeong/RPG/blob/ccb1873a8b70927de28a8f7a7867db97d5a75ef3/Source/RPGPortfolio/UI/UI_InvenItem.cpp#L126-L136
https://github.com/SeongJeGyeong/RPG/blob/ccb1873a8b70927de28a8f7a7867db97d5a75ef3/Source/RPGPortfolio/UI/UI_ItemSelectMenu.cpp#L46-L59
</details>

---
장비창에서는 현재 장비된 아이템 목록을 표시합니다.
장비 슬롯을 클릭 시 인벤토리에서 해당 슬롯에 장비 가능한 아이템 리스트를 표시하고 아이템을 클릭하면 장착할 수 있습니다.

![Image](https://github.com/user-attachments/assets/3d3fdae2-c091-44d2-8f3a-27db6d10a1b3)

퀵슬롯에 장착된 아이템은 단축키를 통해 변경 및 사용이 가능합니다. 퀵슬롯으로 아이템 사용 시에도 인벤토리에서 사용할 때와 동일하게 사용 대기시간을 가집니다.

![Image](https://github.com/user-attachments/assets/4a483782-15d8-48c0-a502-3fc80517f55b)
![Image](https://github.com/user-attachments/assets/2bff713e-c9a2-4805-a4b8-f141bc446f64)

</details>

## 5. 공격 & 피격 시스템
<details>
    <summary>보기</summary>

### 설명

</details>

## 6. 투사체
<details>
    <summary>보기</summary>

### 설명

</details>

## 7. 몬스터 AI
<details>
    <summary>보기</summary>

### 설명

</details>

## 8. Foot IK
<details>
    <summary>보기</summary>

### 설명

</details>
