# 나만의 게임 엔진 개발하기
## 참고 자료의 출처
```text
다음 프로젝트의 구조화 구성을 위해 다음의 자료를 참조하였습니다.
게임 엔진 아키텍처 3판,제이슨 그레고리 저자(글) · 박상희 번역
유영천(megayuchi) 님의 "나만의 엔진 개발하기" PDF (Renderer 및 구조화 파트 참조)
Hazel 엔진 구조를 참조하여 제작
https://github.com/TheCherno/Hazel/blob/master/Hazel
선행 R&D 프로토타입 프로젝트: m98541/CharacterCollisionMapRenderer
```
## 목적 
기존의 개발하였던 CharacterCollisionMapRenderer 의 경우 맵 공간 충돌처리, 충돌체간 충돌 처리, 캐릭터 스키닝 애니메이션 처리 등을 수행하였지만 해당 기능의 모듈화 및 설계 부족으로 인하여 각 모듈이 서로 의존성을 가지며 모든 모듈이 그래픽스 모듈(DirectX11 api)에 대한 큰 의존성을 가지게되며 또한 추상화 및 메인 로직 구성또한 분리 및 정리가 안되어 기능 추가 후 테스트 코드가 메인에 지속적으로 누적되어 관리의 어려움을 격게 되었음.

이에 해당 프로젝트에서는 각 모듈에 대한 의존성을 낮추고 그래픽스 api 에 대한 엔진 내부 코드의 노출을 없애고 엔진 외부에서(즉 클라이언트 로직)는 엔진 내부 로직의 노출을 없애고자함.

ex)DirectX 11 그래픽스 API 관련 코드는 오직 `EngineCore::Graphics` 서브시스템 내부로만 철저히 격리하며, 타 모듈(Client, Physics, Resource)은 그래픽스 디바이스의 존재를 완전히 모르는 무결한 추상화 상태를 달성하는 것을 목표로 함.

## 구조
```text
MyGameEngine.sln
├── 1. GameClient (Application, .exe) [EngineCore 및 PhysicsEngine 참조]
│   └── main.cpp (순수 게임 콘텐츠 로직 및 엔진 조립)
│
├── 2. EngineCore (Static Library, .lib) [Graphics 내부로 API 격리]
│   ├── Core/ (Window 생성, TimeManager, PoolAllocator)
│   ├── Graphics/ (GraphicDevice, VertexBuffer, ShaderFile) ◀ [D3D11 격리 구역]
|   ├── Renderer/ ➔ CharacterRenderer, MapRenderer 등 렌더링 기능 제공하는 렌더링 엔진
│   └── Resource/ (ResourceManager, OBJLoader, GLTFLoader)
│
└── 3. CollisionManager (Static Library, .lib) [그래픽스 의존성 제거 시킨다]
    ├── SpatialPartition/ (BVTree, BvNode)
    ├── Collision/ (Collision, CapsuleCollider, ConvexHull, HalfEdge)
    └── NarrowPhase/ (GJK, EPA, Simplex3D)
```
### **기존 메인 로직과 Graphics API 완전 분리 :**
   - `ID3D11Device`, `ID3D11DeviceContext` 등 모든 D3D11 인터페이스 및 데이터 타입은 오직 `Graphics/` 내부에만 존재해야 합니다.
   - `GameClient` 및 `PhysicsEngine`에서는 `d3d11.h` 헤더를 포함하는 것 자체를 금지합니다.

### **물리 엔진의 그래픽스 의존성 제거 (`ColliderManager` 자립):**
   - `PhysicsEngine`은 순수한 수학 및 기하학적 연산(Minkowski Difference, BVH 트리 빌드)만 수행합니다.
   - 렌더링용 와이어프레임 데이터 추출이 필요할 경우, 그래픽스 자원을 직접 건드리지 않고 순수한 정점 배열 데이터(CPU Memory) 형태로 `GameClient`나 `RenderSystem`에 덤프해 주는 인터페이스만 노출합니다.

### 자원(Asset)과 컨텍스트(Context)의 물리적 분리
- **Shared 자원 레이어:** `SharedMesh`, `SharedSkeleton`, `SharedAnimation` 클래스는 오직 디스크에서 파싱된 immutable(불변) 데이터와 GPU 하드웨어 버퍼 핸들만 보유합니다.
- **Instance 인스턴스 레이어:** 개별 게임 오브젝트(`GameObject`)는 원본 자원의 `std::shared_ptr`만 소유하며, 개별적인 상태값(현재 애니메이션 시간 재생 틱, 월드 변환 TRS 행렬)은 자신의 로컬 `Context` 테이블에 보관합니다.
