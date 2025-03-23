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
![Image](https://github.com/user-attachments/assets/fa0e758b-42ae-4a48-bff1-2c0352a9fa48)
<details>
<summary><b>보기</b></summary>

---
플레이어의 입력은 향상된 입력(Enhanced Input)플러그인을 이용해 구현했습니다.

![Image](https://github.com/user-attachments/assets/ac1cf159-d8f7-4f12-a07d-b816e5e0760a)
<details>
    <summary><b>코드</b></summary>
            
https://github.com/SeongJeGyeong/RPG/blob/e6a45169d92dfc23bffdefdebd49f5f7d1d23db7/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L80-L85
https://github.com/SeongJeGyeong/RPG/blob/e6a45169d92dfc23bffdefdebd49f5f7d1d23db7/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L169-L240
</details>

---
플레이어의 입력 로직은 상태 패턴을 사용해 구현했습니다.

![Image](https://github.com/user-attachments/assets/c79926c1-2b21-4cf2-8c0f-186ceaa3642d)

추상클래스로 선언한 StateMachine 클래스를 상속받아 상태 클래스들을 만들어 구현했습니다.<br/>
스테이트 클래스는 Tunique_Ptr로 선언해 상태를 변경할 때 마다 클래스를 새로 선언하지 않고 오브젝트 풀에서 풀링하여 재사용 하도록 만들었습니다.
<details>
    <summary><b>코드</b></summary>
    
https://github.com/SeongJeGyeong/RPG/blob/dc9985124fc348ad43c38dda8fb034afaae7aa5e/Source/RPGPortfolio/Characters/State/StateMachine.h#L12-L19
https://github.com/SeongJeGyeong/RPG/blob/dc9985124fc348ad43c38dda8fb034afaae7aa5e/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L242-L261
https://github.com/SeongJeGyeong/RPG/blob/dc9985124fc348ad43c38dda8fb034afaae7aa5e/Source/RPGPortfolio/System/Subsys_ObjectPool.cpp#L68-L83
</details>
</details>

---

## 2. 락온
![Image](https://github.com/user-attachments/assets/3be2630e-bbd2-40d7-9654-009aa67d03bf)
<details>
    <summary><b>보기</b></summary>

---
락온은 Player_CameraArm와 LockOnTargetComponent를 이용해 구현했습니다.

![Image](https://github.com/user-attachments/assets/4c0d2a80-145c-417e-8dc6-2f954246e2fe)

락온을 실행하면 우선 카메라암을 중심으로 구체형 범위 내에 오버랩 이벤트를 발생시켜 오버랩된 락온 타겟 컴포넌트를 검출한 뒤, 검출된 컴포넌트 중 카메라 정면에 존재하는 컴포넌트만 타겟 배열에 추가합니다.
<details>
    <summary><b>코드</b></summary>
    
https://github.com/SeongJeGyeong/RPG/blob/aefae85385cf8c711bd623c7ad25993b3e4dceac/Source/RPGPortfolio/Characters/Player_CameraArm.cpp#L205-L233
</details>
<br/>

타겟 배열의 컴포넌트 중 가장 정면에 가까운 컴포넌트를 락온 대상으로 합니다. 단, 카메라 중앙 기준 50도 이내에 있는 컴포넌트들은 거리가 더 가까운 컴포넌트를 우선적으로 락온합니다.
<details>
    <summary><b>코드</b></summary>
    
https://github.com/SeongJeGyeong/RPG/blob/aefae85385cf8c711bd623c7ad25993b3e4dceac/Source/RPGPortfolio/Characters/Player_CameraArm.cpp#L119-L159
</details>

---
![Image](https://github.com/user-attachments/assets/81324557-07b7-4759-8e1c-91d0d700d3f5)

락온 대상을 스위칭 할 경우, 우선 타겟 검출 함수를 통해 유효한 타겟 목록을 가져오고 현재 타겟 기준 다른 타겟의 방향과 거리를 계산하기 위해 현재 락온된 타겟의 방향을 구합니다.<br/>
검출된 타겟 목록에서 스위칭 하고자 하는 방향과 일치하는 방향에 있는 대상 중 원래 락온 대상에서 가장 가까운 대상으로 락온을 변경합니다.
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/aefae85385cf8c711bd623c7ad25993b3e4dceac/Source/RPGPortfolio/Characters/Player_CameraArm.cpp#L161-L203
</details>

---
락온 중인 대상은 락온 타겟 컴포넌트에 락온 위젯을 표시합니다.
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/7e3778cfaea6e92e17ba9f8dff82f1efc7121474/Source/RPGPortfolio/System/Component/LockOnTargetComponent.cpp#L30-L44
</details>
<br/>

몬스터가 락온 중일 때만 커스텀 뎁스 패스 렌더를 활성화하여 락온 중인 적에게 붉은 외곽선을 표시하도록 만들었습니다.

![Image](https://github.com/user-attachments/assets/8c5c6d4e-5d28-4a15-9649-4266b226764f)
![Image](https://github.com/user-attachments/assets/7dba4672-0500-44bc-947e-1f97f41a1ac3)
![Image](https://github.com/user-attachments/assets/df17e708-5872-4de3-8e3d-627c0574267f)
![Image](https://github.com/user-attachments/assets/1c68484b-3807-4791-83a4-a2917f988568)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/9cc2f76824f01a62a2472774d01e99760da45067/Source/RPGPortfolio/Monsters/Monster_Base.cpp#L368-L372
</details>
<br/>

현재 픽셀의 위치와 주변 픽셀의 위치의 CustomStencil 값을 계산하는 머티리얼 함수를 작성합니다.

![Image](https://github.com/user-attachments/assets/17d7b56b-62e9-4865-9faa-5f41f82e1ece)

포스트 프로세스용 머티리얼을 생성하고 함수를 통해 출력받은 값을 이용해
1) 현재 픽셀의 CutomStencil 값이 0일 경우(CustomStencil이 지정되지 않은 픽셀)
2) 주변 픽셀 중 하나라도 CustomStencil값이 2(몬스터 메시의 스텐실값) 이상인 경우
현재 픽셀의 색을 빨간색으로 만들어 몬스터 메시 주변의 픽셀만 빨간색으로 만들도록 만들었습니다.

![Image](https://github.com/user-attachments/assets/7a715a21-efd4-4daf-a4cc-b8615a8650a6)
![Image](https://github.com/user-attachments/assets/7068d822-9cd5-4e41-96c2-1441d704edfc)

</details>

---

## 3. 상호 작용
![Image](https://github.com/user-attachments/assets/a157deb2-67a3-4e74-bdeb-1fc04a76a9a5)
<details>
    <summary><b>보기</b></summary>

---
언리얼 인터페이스 클래스를 상속하여 상호 작용 가능한 오브젝트를 만들었습니다.

![Image](https://github.com/user-attachments/assets/16d04358-41a7-40f6-9908-47bfeb65a32d)
![Image](https://github.com/user-attachments/assets/ef037b21-fea8-4381-86cb-6426792fce9e)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/98bd45543d90aaa14750c3a5f6784e21042c6249/Source/RPGPortfolio/System/Interface/PlayerInteraction.h#L17-L25
https://github.com/SeongJeGyeong/RPG/blob/98bd45543d90aaa14750c3a5f6784e21042c6249/Source/RPGPortfolio/InteractionProps/Interaction_WarpGate.cpp#L46-L49
https://github.com/SeongJeGyeong/RPG/blob/98bd45543d90aaa14750c3a5f6784e21042c6249/Source/RPGPortfolio/Item/Item_Dropped_Base.cpp#L69-L79
https://github.com/SeongJeGyeong/RPG/blob/98bd45543d90aaa14750c3a5f6784e21042c6249/Source/RPGPortfolio/InteractionProps/Interaction_Lever.cpp#L51-L80
</details>
<br/>

캐릭터가 상호 작용 가능한 오브젝트에 오버랩될 경우 상호 작용 텍스트를 표시하고 오브젝트를 상호작용 배열에 추가합니다.
<details>
    <summary><b>코드</b></summary>
    
https://github.com/SeongJeGyeong/RPG/blob/98bd45543d90aaa14750c3a5f6784e21042c6249/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L1156-L1165
</details>
<br/>

상호작용 키를 입력 시 현재 오버랩된 상호 작용 오브젝트중 가장 최근에 오버랩된 오브젝트의 Interaction 함수를 실행합니다.
<details>
    <summary><b>코드</b></summary>
    
https://github.com/SeongJeGyeong/RPG/blob/98bd45543d90aaa14750c3a5f6784e21042c6249/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L513-L535
</details>
</details>

---

## 4. 인벤토리 & 장비
![Image](https://github.com/user-attachments/assets/254f79e0-540d-4388-b1e5-fa2c63f2ca38)
<details>
    <summary><b>보기</b></summary>

---
UGameInstanceSubsystem 클래스를 상속받아 인벤토리 매니저 클래스를 구현하여 레벨이 변경되어도 인벤토리 정보가 유지되도록 만들었습니다.<br/>
아이템들의 정보는 데이터 테이블을 통해 관리하고, 인벤토리 매니저가 초기화될 때 테이블에서 아이템 정보를 가져와 Tmap 컨테이너에 저장합니다.

![Image](https://github.com/user-attachments/assets/e54d9ebd-cd32-4e35-b5aa-cc18bcab0c8d)
<details>
    <summary><b>코드</b></summary>
    
https://github.com/SeongJeGyeong/RPG/blob/ccb1873a8b70927de28a8f7a7867db97d5a75ef3/Source/RPGPortfolio/Manager/GISubsystem_InvenMgr.cpp#L8-L21
https://github.com/SeongJeGyeong/RPG/blob/ccb1873a8b70927de28a8f7a7867db97d5a75ef3/Source/RPGPortfolio/Manager/GISubsystem_InvenMgr.cpp#L36-L48
</details>
<br/>

플레이어 캐릭터는 Player_InvenComponent를 통해 인벤토리 매니저와 통신합니다.<br/>
플레이어가 아이템을 획득할 경우 인벤토리 컴포넌트를 통해 인벤토리 매니저로 아이템 ID와 수량을 전달합니다. 퀵슬롯에 등록된 아이템의 경우 퀵슬롯 UI도 갱신합니다.
<details>
    <summary><b>코드</b></summary>
    
https://github.com/SeongJeGyeong/RPG/blob/ccb1873a8b70927de28a8f7a7867db97d5a75ef3/Source/RPGPortfolio/Characters/Player_InvenComponent.cpp#L89-L106
</details>
<br/>

인벤토리 매니저와 위젯간에는 델리게이트를 통해 연결하여 UI를 갱신하도록 만들었습니다.
<details>
    <summary><b>코드</b></summary>
    
https://github.com/SeongJeGyeong/RPG/blob/ccb1873a8b70927de28a8f7a7867db97d5a75ef3/Source/RPGPortfolio/Manager/GISubsystem_InvenMgr.h#L11-L16
https://github.com/SeongJeGyeong/RPG/blob/ccb1873a8b70927de28a8f7a7867db97d5a75ef3/Source/RPGPortfolio/Manager/GISubsystem_InvenMgr.h#L39-L47
</details>

---
인벤토리의 타일 뷰 위젯에 표시된 아이템에 커서를 올리면 아이템 정보를 표시합니다.

![Image](https://github.com/user-attachments/assets/254f79e0-540d-4388-b1e5-fa2c63f2ca38)
<details>
    <summary><b>코드</b></summary>
    
https://github.com/SeongJeGyeong/RPG/blob/ccb1873a8b70927de28a8f7a7867db97d5a75ef3/Source/RPGPortfolio/UI/UI_Inventory.cpp#L92-L113
</details>
<br/>

카테고리를 변경하는 것으로 해당 카테고리의 아이템만 표시할 수 있습니다.

![Image](https://github.com/user-attachments/assets/42555cf8-2c6a-4c94-9638-ec0c4174c003)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/dea1fea8d96c99679d439b3f07cdc70afc189cce/Source/RPGPortfolio/UI/UI_Inventory.cpp#L115-L147
</details>

---
장비창에서는 현재 장비된 아이템 목록을 표시합니다.<br/>
장비 슬롯을 클릭 시 인벤토리에서 해당 슬롯에 장비 가능한 아이템 리스트를 표시하고 아이템을 클릭하면 장착할 수 있습니다.

![Image](https://github.com/user-attachments/assets/3d3fdae2-c091-44d2-8f3a-27db6d10a1b3)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/bc8e00d5616b22817422ac8f90474e9e34b3ceb7/Source/RPGPortfolio/UI/UI_EquipItem.cpp#L76-L90
https://github.com/SeongJeGyeong/RPG/blob/bc8e00d5616b22817422ac8f90474e9e34b3ceb7/Source/RPGPortfolio/UI/UI_EquipItemList.cpp#L46-L77
https://github.com/SeongJeGyeong/RPG/blob/bc8e00d5616b22817422ac8f90474e9e34b3ceb7/Source/RPGPortfolio/UI/UI_InvenItem.cpp#L85-L123
</details>
<br/>

퀵슬롯에 장착된 아이템은 단축키를 통해 변경 및 사용이 가능합니다. 퀵슬롯으로 아이템 사용 시에도 인벤토리에서 사용할 때와 동일하게 사용 대기시간을 가집니다.

![Image](https://github.com/user-attachments/assets/4a483782-15d8-48c0-a502-3fc80517f55b)
![Image](https://github.com/user-attachments/assets/2bff713e-c9a2-4805-a4b8-f141bc446f64)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/bc8e00d5616b22817422ac8f90474e9e34b3ceb7/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L537-L561
https://github.com/SeongJeGyeong/RPG/blob/bc8e00d5616b22817422ac8f90474e9e34b3ceb7/Source/RPGPortfolio/Characters/Player_InvenComponent.cpp#L40-L51
</details>
</details>

---

## 5. 공격
![Image](https://github.com/user-attachments/assets/2eda975c-f5bf-49f0-919a-8d4b6b18f501)
<details>
    <summary><b>보기</b></summary>

---
공격 발동 시 공격 타입에 따라 스태미나 소모량을 측정하고 스탯 컴포넌트를 통해 스탯 매니저에서 현재 스태미나가 충분한지 판단합니다.<br/>
충분할 경우 스태미나를 소모하고 공격 상태에 들어가고, 현재 콤보에 따라 다른 몽타주를 실행합니다.(점프공격 제외)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/0a6422c40abf4239a9f12145d3a5b2ca86c3fd7a/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L780-L789
https://github.com/SeongJeGyeong/RPG/blob/0a6422c40abf4239a9f12145d3a5b2ca86c3fd7a/Source/RPGPortfolio/Characters/State/State_Attack.cpp#L8-L36
</details>
<br/>

애님 노티파이를 이용해 공격 몽타주의 각 타이밍 마다 로직을 실행합니다.

![Image](https://github.com/user-attachments/assets/4d1a19e8-9ae2-464f-9008-b03f6da0972e)

HitCheck 구간 동안 Tick 함수 내에서 Sweep으로 피격 대상을 검출합니다.<br/>
똑같은 대상이 여러 번 공격당하는 걸 방지하기 위해 히트한 액터를 배열에 넣고 체크하도록 만들었습니다.
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/0a6422c40abf4239a9f12145d3a5b2ca86c3fd7a/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L724-L778
</details>
<br/>

공격을 통해 소모된 스태미나는 스탯 매니저에서 타이머를 이용해 일정 시간 뒤 회복을 시작합니다.
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/0a6422c40abf4239a9f12145d3a5b2ca86c3fd7a/Source/RPGPortfolio/Manager/GISubsystem_StatMgr.cpp#L103-L113
https://github.com/SeongJeGyeong/RPG/blob/0a6422c40abf4239a9f12145d3a5b2ca86c3fd7a/Source/RPGPortfolio/Manager/GISubsystem_StatMgr.cpp#L121-L139
</details>
<br/>

모션워핑 플러그인을 적용하여 공격 몽타주 재생 중 이동 키를 입력하는 것으로 방향 전환이 가능하도록 만들었습니다.<br/>
Translation과 Rotation 설정을 유동적으로 바꾸기 위해 C++에서 Modifier를 설정합니다.<br/>
락온 중일 경우에는 방향 전환이 불가능하지만, 모션워핑을 통해 적과 일정 거리를 유지하도록 만들었습니다.
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/0a6422c40abf4239a9f12145d3a5b2ca86c3fd7a/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L963-L1049
</details>
<br/>

UDamageType 클래스를 상속받은 커스텀 클래스로 공격에 타입과 중량 정보를 추가할 수 있도록 만들었습니다.
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/9343c33f06e2285e7ccec42f9158c05a646e08c8/Source/RPGPortfolio/System/DamageType_Base.h#L14-L27
</details>
</details>

---

## 6. 몬스터 피격
![Image](https://github.com/user-attachments/assets/0a6a92a0-e8fb-4a33-9b41-d8a16f35aefe)
<details>
    <summary><b>보기</b></summary>

---
몬스터가 플레이어에게 피격 시 타격감을 주기 위해 타이머를 이용해 플레이어에게 역경직을 주었습니다.
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/9343c33f06e2285e7ccec42f9158c05a646e08c8/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L609-L625
</details>
<br/>

ApplyPointDamage 함수를 이용해 몬스터가 피격당한 위치 정보를 보내고, 해당 위치에 피격 이펙트가 발생하도록 만들었습니다.<br/>
일반 몬스터의 경우 타임라인을 이용해 메시가 진동하도록 만들었습니다.

![Image](https://github.com/user-attachments/assets/88e02a72-46c5-4a9f-a573-ec798a5c4ce7)
<details>
    <summary><b>코드</b></summary>
    
https://github.com/SeongJeGyeong/RPG/blob/9343c33f06e2285e7ccec42f9158c05a646e08c8/Source/RPGPortfolio/Monsters/Monster_Base.cpp#L224-L239
</details>

---
보스 몬스터의 경우 피직스 에셋을 만들고, 피격당한 본에 잠시동안 피직스를 활성화시켜 피격당한 부위가 흔들리도록 만들었습니다.

![Image](https://github.com/user-attachments/assets/179bf2e1-d758-4062-a3bd-cb356fe213c4)
![Image](https://github.com/user-attachments/assets/7ef41c17-90b5-4279-b473-def505cad04c)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/9343c33f06e2285e7ccec42f9158c05a646e08c8/Source/RPGPortfolio/Monsters/Boss_GreaterSpider.cpp#L430-L453
</details>

---
몬스터가 사망할 경우 아이템을 드랍합니다.

![Image](https://github.com/user-attachments/assets/273e7734-be66-4031-8864-d75c6172ab90)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/bc8e00d5616b22817422ac8f90474e9e34b3ceb7/Source/RPGPortfolio/Monsters/Monster_Base.cpp#L299-L311
</details>
<br/>

아이템은 구간 확률을 이용해 몬스터의 아이템 드랍 데이터테이블에 있는 아이템 중 랜덤으로 드랍되도록 만들었습니다.<br/>
드랍되는 아이템의 지정은 몬스터 BeginPlay 시점에 결정됩니다.

![Image](https://github.com/user-attachments/assets/2b97cb57-10ad-458f-a0e6-a9577655013d)
<details>
    <summary><b>코드</b></summary>
    
https://github.com/SeongJeGyeong/RPG/blob/bc8e00d5616b22817422ac8f90474e9e34b3ceb7/Source/RPGPortfolio/Monsters/Monster_Base.cpp#L134-L151
</details>
<br/>

사망한 몬스터의 메시는 머티리얼 함수를 이용해 일정시간 뒤 서서히 사라지도록 만들었습니다.

![Image](https://github.com/user-attachments/assets/99a7c86c-dccf-4490-a155-468d9b7c49d9)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/bc8e00d5616b22817422ac8f90474e9e34b3ceb7/Source/RPGPortfolio/Monsters/Monster_Base.cpp#L328-L340
</details>
</details>

---

## 7. 플레이어 피격 & 가드, 회피
![Image](https://github.com/user-attachments/assets/02a66082-39a4-459e-a08f-a94d8e2d9320)
<details>
    <summary><b>보기</b></summary>

---
플레이어는 피격 시 피격된 방향에 따라 다른 몽타주를 재생합니다.<br/>
공중에 있거나, 공격의 중량 타입에 따라서도 다른 몽타주를 재생합니다.

![Image](https://github.com/user-attachments/assets/0b91f62c-2762-49b0-8360-7497bc700954)
![Image](https://github.com/user-attachments/assets/5100334e-d92b-48a7-ab57-a2755eb13739)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/bc8e00d5616b22817422ac8f90474e9e34b3ceb7/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L664-L678
https://github.com/SeongJeGyeong/RPG/blob/bc8e00d5616b22817422ac8f90474e9e34b3ceb7/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L1090-L1125
</details>

---
가드 키를 누르는 동안, 플레이어는 정면에서 오는 공격을 가드할 수 있습니다.<br/>
공격을 가드 시 데미지에 비례해 스태미나를 소모하고, 공격의 중량 타입에 따라 다른 모션을 재생합니다.

![Image](https://github.com/user-attachments/assets/7798a46f-4675-4072-9f58-3754c556d738)
![Image](https://github.com/user-attachments/assets/f0ce7683-14af-41ff-97b4-db2892f31445)

스태미나가 부족한 상태에서 가드 시 가드 브레이크 상태가 되며 잠시동안 무방비 상태가 됩니다.

![Image](https://github.com/user-attachments/assets/cfc0df2d-294a-463f-b61d-6d6e7de0e11e)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/bc8e00d5616b22817422ac8f90474e9e34b3ceb7/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L680-L693
https://github.com/SeongJeGyeong/RPG/blob/bc8e00d5616b22817422ac8f90474e9e34b3ceb7/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L822-L845
</details>

---
회피 입력을 통해 일정 시간동안 적의 공격 판정을 무시할 수 있습니다.

![Image](https://github.com/user-attachments/assets/5b7fa968-fe45-4ac0-a3b4-63ac0675fc0f)
![Image](https://github.com/user-attachments/assets/5fe30ac5-1ed3-48a0-83ad-d1b486c4d72b)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/bc8e00d5616b22817422ac8f90474e9e34b3ceb7/Source/RPGPortfolio/CharacterAnim/AnimInstance_Knight.cpp#L119-L127
https://github.com/SeongJeGyeong/RPG/blob/bc8e00d5616b22817422ac8f90474e9e34b3ceb7/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L1150-L1154
</details>
</details>

---

## 8. 투사체
![Image](https://github.com/user-attachments/assets/235bd184-fd34-44d1-9141-0ac87676021a)
<details>
    <summary><b>보기</b></summary>

---
플레이어나 몬스터가 사용하는 투사체 클래스는 오브젝트 풀 패턴을 사용하여 투사체를 재사용할 수 있도록 만들었습니다.
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/5341336b919167acacbb6d0c15519b8988ae8c3d/Source/RPGPortfolio/Projectiles/Proj_Player_Cutter.h#L11-L23
https://github.com/SeongJeGyeong/RPG/blob/5341336b919167acacbb6d0c15519b8988ae8c3d/Source/RPGPortfolio/Projectiles/Proj_GS_Spiderling.h#L15-L28
https://github.com/SeongJeGyeong/RPG/blob/5341336b919167acacbb6d0c15519b8988ae8c3d/Source/RPGPortfolio/System/Subsys_ObjectPool.h#L89-L116
https://github.com/SeongJeGyeong/RPG/blob/5341336b919167acacbb6d0c15519b8988ae8c3d/Source/RPGPortfolio/System/Subsys_ObjectPool.cpp#L51-L66
</details>
<br/>

투사체를 미리 일정 수량 스폰해두고 오브젝트 풀에 담아놓는 함수를 구현하여 런타임 중에 투사체가 스폰되는 경우를 줄일 수 있도록 만들습니다.
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/5341336b919167acacbb6d0c15519b8988ae8c3d/Source/RPGPortfolio/System/Subsys_ObjectPool.h#L59-L87
</details>
</details>

---

## 9. 몬스터 AI
![Image](https://github.com/user-attachments/assets/85e2d625-45c6-45fa-9b93-d758829ddde5)
<details>
    <summary><b>보기</b></summary>

---
일반 몬스터의 비헤이비어 트리입니다.

![Image](https://github.com/user-attachments/assets/88ef3384-97ab-4c59-9997-ac0bb4655208)

일반 몬스터의 경우, 플레이어에게 FGenericTeamId를 부여하고 몬스터의 AIController에 AIPerceptionComponent를 부착하여 시야를 이용해 플레이어를 탐지하도록 만들었습니다.
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/cdd54970524f3ff3c8a8c08c3dac8dbf2b39ddf3/Source/RPGPortfolio/Characters/Player_Base_Knight.h#L222
https://github.com/SeongJeGyeong/RPG/blob/cdd54970524f3ff3c8a8c08c3dac8dbf2b39ddf3/Source/RPGPortfolio/Monsters/AI/AIC_Monster_Base.cpp#L82-L94
</details>
<br/>

플레이어가 탐지되지 않은 상태라면, 몬스터는 스폰 지점을 기준으로 주변을 랜덤하게 배회합니다.

![Image](https://github.com/user-attachments/assets/e333a83d-f2be-411c-83b9-9a18e36fc56e)
<details>
    <summary><b>코드</b></summary>
    
https://github.com/SeongJeGyeong/RPG/blob/cdd54970524f3ff3c8a8c08c3dac8dbf2b39ddf3/Source/RPGPortfolio/Monsters/AI/BTT_NextPos_Monster.cpp#L20-L53
</details>
<br/>

몬스터가 플레이어를 탐지하면 Trace_Monster 노드로 플레이어를 추적합니다.

![Image](https://github.com/user-attachments/assets/7d03093a-a6bd-4c7e-9d07-04dfdf5ac207)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/cdd54970524f3ff3c8a8c08c3dac8dbf2b39ddf3/Source/RPGPortfolio/Monsters/AI/BTT_Trace_Monster.cpp#L22-L47
</details>
<br/>

Trace_Monster 노드의 TickTask에서 타겟이 된 플레이어와의 거리와 위치를 체크합니다.<br/>
추적 중인 플레이어가 탐지 범위에서 벗어났을 경우, 몬스터는 가장 최근에 플레이어를 탐지한 위치까지 이동하며 플레이어를 찾습니다.<br/>
플레이어를 찾지 못할 경우 추적을 중단하고 스폰 지점 근처를 배회하는 루틴으로 돌아갑니다.

![Image](https://github.com/user-attachments/assets/0aa76e25-d71d-45c1-9276-fb16116c0af1)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/cdd54970524f3ff3c8a8c08c3dac8dbf2b39ddf3/Source/RPGPortfolio/Monsters/AI/BTT_Trace_Monster.cpp#L49-L97
https://github.com/SeongJeGyeong/RPG/blob/cdd54970524f3ff3c8a8c08c3dac8dbf2b39ddf3/Source/RPGPortfolio/Monsters/AI/BTT_LoseTarget_Monster.cpp#L10-L21
</details>
<br/>

플레이어가 공격 범위 안에 들어오면 공격 노드를 실행하고, 실행 후 플레이어를 바라보며 일정 시간 대기합니다.<br/>
대기 후에 플레이어가 아직 공격 범위 안에 있을 경우 다시 공격하며, 범위 밖에 있을 경우 플레이어 주변을 돌며 잠시동안 대치 상태를 이룹니다.<br/>
대치 상태 종료 후 공격 범위 안에 있으면 공격을, 밖에 있으면 추적을 실행합니다.

![Image](https://github.com/user-attachments/assets/35f47e1a-411d-48cd-bb81-a74ae827f483)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/cdd54970524f3ff3c8a8c08c3dac8dbf2b39ddf3/Source/RPGPortfolio/Monsters/AI/Undead_Assassin/BTT_UA_Atk_Melee.cpp#L13-L25
https://github.com/SeongJeGyeong/RPG/blob/cdd54970524f3ff3c8a8c08c3dac8dbf2b39ddf3/Source/RPGPortfolio/Monsters/AI/BTT_TurnToTarget_Monster.cpp#L16-L38
https://github.com/SeongJeGyeong/RPG/blob/cdd54970524f3ff3c8a8c08c3dac8dbf2b39ddf3/Source/RPGPortfolio/Monsters/AI/BTT_AroundTarget.cpp#L20-L81
</details>

---
보스 몬스터의 비헤이비어 트리입니다.

![Image](https://github.com/user-attachments/assets/05e22d10-078d-4281-a98b-9cb24de19ba7)

보스 1페이즈의 경우 플레이어의 방향이 몬스터 정면 60도 반경 기준으로 왼쪽, 오른쪽, 정면 중 어디에 있는지에 따라 사용할 공격 패턴을 결정하도록 만들었습니다.<br/>
플레이어의 방향을 판별 후 각 방향에 맞는 공격 애니메이션을 재생합니다.<br/>
공격 후 일정 시간 대기한 뒤, 플레이어가 공격 범위 안에 있는지에 따라 공격 또는 추적을 실행합니다.

![Image](https://github.com/user-attachments/assets/85e2d625-45c6-45fa-9b93-d758829ddde5)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/cdd54970524f3ff3c8a8c08c3dac8dbf2b39ddf3/Source/RPGPortfolio/Monsters/AI/Boss/Spider/BTS_GS_AttackPattern.cpp#L16-L56
https://github.com/SeongJeGyeong/RPG/blob/cdd54970524f3ff3c8a8c08c3dac8dbf2b39ddf3/Source/RPGPortfolio/Monsters/AI/Boss/Spider/BTT_GS_Attack.cpp#L79-L98
</details>
<br/>

보스의 체력이 50% 이하가 되면 2페이즈가 시작되며, 기존의 패턴에 더해 돌진과 투사체를 발사하는 2개의 패턴이 추가됩니다.<br/>
랜덤 함수와 플레이어와의 거리를 사용해 어떤 패턴을 사용할 지 판별합니다.<br/>
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/cdd54970524f3ff3c8a8c08c3dac8dbf2b39ddf3/Source/RPGPortfolio/Monsters/AI/Boss/Spider/BTT_GS_P2_PatternSelect.cpp#L10-L42
https://github.com/SeongJeGyeong/RPG/blob/cdd54970524f3ff3c8a8c08c3dac8dbf2b39ddf3/Source/RPGPortfolio/Monsters/AI/Boss/Spider/BTT_GS_Attack.cpp#L39-L77
</details>
<br/>

2페이즈의 돌진 패턴의 경우 애님 노티파이를 이용해 일정 시간 동안 일직선으로 돌진하도록 만들었습니다.<br/>
돌진하는 동안은 보스의 몸체를 중심으로 구형의 공격 판정을 발생시키며,<br/>
돌진이 끝나는 타이밍에 보스를 중심으로 더 큰 구형의 공격 판정을 한 번 발생시킵니다.

![Image](https://github.com/user-attachments/assets/13e5ac38-2057-4d34-9add-b05322ce43bd)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/cdd54970524f3ff3c8a8c08c3dac8dbf2b39ddf3/Source/RPGPortfolio/Monsters/Boss_GreaterSpider.cpp#L114-L143
</details>
<br/>

투사체 발사 패턴의 경우 SuggestProjectileVelocity_CustomArc 함수를 이용하여 플레이어 위치를 향해 포물선을 그리며 발사되도록 만들었습니다.<br/>
또한 플레이어에게 적중하지 않았을 경우 바닥의 일정 범위에 장판을 생성해 범위 내에 다시 공격 판정을 가하도록 만들었습니다.

![Image](https://github.com/user-attachments/assets/4d89c39a-442a-4539-ad33-0088129aef7f)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/cdd54970524f3ff3c8a8c08c3dac8dbf2b39ddf3/Source/RPGPortfolio/Projectiles/Proj_GS_Spiderling.cpp#L34-L98
</details>

</details>

---

## 10. Foot IK
![Image](https://github.com/user-attachments/assets/7f0de238-a277-4380-bfff-b59b8dc1b443)
![Image](https://github.com/user-attachments/assets/92ca0dbe-68fd-4139-98e0-91ddddeff22a)
<details>
    <summary><b>보기</b></summary>

언리얼 삼인칭 샘플 프로젝트의 컨트롤 릭을 사용한 Foot IK에 바닥 경사면에 맞춰 발바닥 각도를 보간하는 기능을 추가했습니다.

![Image](https://github.com/user-attachments/assets/e566ecbb-0da3-4939-991a-577d0b3ceb5d)

플레이어가 공중에 떠 있거나 몽타주를 재생중일 때는 컨트롤릭 내부의 함수를 실행하지 않도록 만들었습니다.

![Image](https://github.com/user-attachments/assets/c11b1df7-9066-4c06-905c-570b3551bb98)
![Image](https://github.com/user-attachments/assets/9da5dfb7-d67a-48aa-b4ba-1d1d00282190)

FootTrace 함수 내부에 Aim Math를 이용해 Foot 본의 벡터에서 바닥의 HitNormal 벡터로의 트랜스폼을 얻어 IK Foot 본의 회전 정보를 가져오는 로직을 추가했습니다.

![Image](https://github.com/user-attachments/assets/4c44c151-391c-4706-9677-442f22f67bc7)

FootTrace에서 얻은 회전 정보를 이용해 IK Foot 본을 보간하며 회전시키는 로직을 추가했습니다.<br/>
발각도 회전 기능은 플레이어의 속도가 100보다 낮을 경우에만 작동하도록 만들었습니다.

![Image](https://github.com/user-attachments/assets/d2f6027c-4d54-447e-adf6-943d23dde38d)

</details>

---

## 트러블 슈팅
<details>
    <summary><b>보기</b></summary>

### 1. 히트 판정

공격의 히트 판정을 공격 모션에 맞춰 갱신하기 위해 SweepMultiByChannel 함수는 Tick 함수에서 내부에서 실행됩니다.<br/>
다만 Tick 함수에서 Sweep 함수를 실행하게 되면 실행 빈도가 프레임에 의존하게 됩니다.<br/>
소울라이크 게임처럼 세밀한 히트박스를 구현하기 위해 무기의 콜리전 모양에만 맞춰 Sweep 함수를 실행할 경우, 렉으로 인해 프레임 간의 간격이 커질수록 Sweep 사이의 공백이 커지는 '터널링 현상'에 의해 공격 대상을 제대로 검출하지 못하는 문제가 발생합니다.

![Image](https://github.com/user-attachments/assets/708e0030-1c87-4495-823e-7994c04b9748)

이를 방지하기 위해 이전 Sweep의 위치를 저장해 두었다가 다음 Sweep의 시작 위치로 사용하고, 현재 콜리전의 위치를 Sweep의 끝 위치로 설정해 프레임 사이의 간격에도 히트 판정을 메꿔, 히트박스를 무기의 콜리전 모양에 맞추면서 공격 판정도 더 촘촘하게 만들었습니다.

![Image](https://github.com/user-attachments/assets/22e05132-1d44-49ae-aa87-dcb1b4b405d8)
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/d00397c86079246317d275188060e3347ac27947/Source/RPGPortfolio/Characters/Player_Base_Knight.cpp#L726-L751
</details>

---

### 2. IK 리타기팅

캐릭터 애니메이션의 경우 IK 리타기터를 이용하여 다른 스켈레톤의 애니메이션을 리타겟팅하여 사용하였습니다.

![Image](https://github.com/user-attachments/assets/b0530178-2ced-4bf1-b807-fc621c124a01)

다만 IK용 본의 경우, IK를 적용할 본과 계층구조에서 동 떨어져 있지만 IK를 적용할 본의 위치와 회전 정보가 일치하도록 애니메이션 제작자가 임의로 조정하여 만든 본이기 때문에 이를 리타겟팅할 경우 위치나 회전이 어긋나게 되는 상황이 발생했습니다.

![Image](https://github.com/user-attachments/assets/1873c95b-6395-4f68-ab24-6159d47df69b)
![Image](https://github.com/user-attachments/assets/6f88f7bb-707b-48a9-bd69-ce7a18674ab5)

컨트롤 릭에서 Set Transform 노드로 매 틱마다 IK 본의 트랜스폼을 적용할 본과 일치시킬 수도 있지만, 혹시 모를 오류가 생길 가능성을 방지하기 위해 애니메이션 시퀀스의 FK 컨트롤 릭으로 편집 기능을 사용해 IK 본의 모든 키프레임을 IK를 적용할 본의 위치, 회전과 일치시켜 IK 본의 위치 자체를 재설정할 수 있었습니다.

![Image](https://github.com/user-attachments/assets/0b9f223e-9930-4613-bd4e-cdf209a8ac3f)
![Image](https://github.com/user-attachments/assets/ba1f8021-a574-4b8a-8925-f4ebf763bc69)
![Image](https://github.com/user-attachments/assets/cc20ecc8-6a66-46eb-8098-3460e2f07abb)

---

### 3. 가비지 컬렉션(GC)

언리얼 엔진의 경우 마크-스윕 방식의 가비지 컬렉션 기능을 탑재하고 있습니다. 런타임에 일정 시간마다 Reference Graph를 순회하며, 사용되지 않는 메모리를 자동으로 해제함으로써 메모리를 자동으로 관리하고 메모리 누수를 방지합니다.
개발 초기에는 가비지 컬렉션에 대해 알지 못해 인벤토리와 퀵슬롯 아이템을 관리하는 컨테이너 변수를 가비지 컬렉션 대상에 추가하지 않아 아직 사용중인 메모리가 해제되는 경우가 발생하여, 가끔씩 데이터가 이상하게 표시되거나 메모리에 접근하는 순간 크래시가 나는 등 곤혹을 겪었습니다.
멤버 변수를 가비지 컬렉션이 자동 해제하지 않도록 만들기 위해서 일반적으로는 UPROPERTY 리플렉션을 사용하지만, 중첩 컨테이너, 구조체 포인터 등 블루프린트상에 노출 할 수 없는 형식의 변수는 UPROPERTY 리플렉션을 달 수 없었습니다.

![Image](https://github.com/user-attachments/assets/af740b47-176f-4d67-bd1e-0d19ba6189a6)
![Image](https://github.com/user-attachments/assets/f9282144-692f-47ee-b8c7-367cc3a757b9)

인벤토리의 경우 아이템 조회의 편리성을 위해 중첩된 컨테이너를 사용하고 싶었기에 인터넷에서 정보를 알아본 결과, 구조체 안에 컨테이너를 선언하고 해당 구조체 유형의 컨테이너를 선언하는 방법으로 중첩 컨테이너에 UPROPERTY 리플렉션을 사용할 수 있도록 만들 수 있었습니다.<br/>
이를 이용해 인벤토리 및 오브젝트 풀의 중첩 컨테이너를 구현하였습니다.<br/>

인벤토리
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/d00397c86079246317d275188060e3347ac27947/Source/RPGPortfolio/Manager/GISubsystem_InvenMgr.h#L31-L32
https://github.com/SeongJeGyeong/RPG/blob/d00397c86079246317d275188060e3347ac27947/Source/RPGPortfolio/Header/Struct.h#L235-L241
https://github.com/SeongJeGyeong/RPG/blob/d00397c86079246317d275188060e3347ac27947/Source/RPGPortfolio/Header/Struct.h#L225-L233
</details>
<br/>

오브젝트 풀
<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/d00397c86079246317d275188060e3347ac27947/Source/RPGPortfolio/System/Subsys_ObjectPool.h#L40-L41
https://github.com/SeongJeGyeong/RPG/blob/d00397c86079246317d275188060e3347ac27947/Source/RPGPortfolio/System/Subsys_ObjectPool.h#L12-L24
</details>

---

### 4. 애니메이션 스레드 세이프

애니메이션 블루프린트에서 액터 등의 외부 객체의 변수를 가져와서 사용할 경우, 게임 스레드에서 애니메이션 스레드로 변수를 안전하게 전달하기 위한 방법이 필요합니다.
FAnimInstanceProxy 구조체를 이용하여 플레이어 액터 내부에서 실시간으로 변동되는 변수를 가져와 애니메이션 블루프린트에서 사용할 수 있도록 처리를 거친 뒤 AnimInstance 클래스의 변수를 갱신해주는 것으로 변수를 안전하게 애니메이션 블루프린트로 전달할 수 있도록 만들었습니다.

<details>
    <summary><b>코드</b></summary>

https://github.com/SeongJeGyeong/RPG/blob/53392531863dd373cdabea0a7620095e6fe350be/Source/RPGPortfolio/CharacterAnim/AnimInstance_Knight.h#L22-L56
https://github.com/SeongJeGyeong/RPG/blob/53392531863dd373cdabea0a7620095e6fe350be/Source/RPGPortfolio/CharacterAnim/AnimInstance_Knight.cpp#L11-L73
</details>
</details>
