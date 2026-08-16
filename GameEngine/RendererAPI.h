#ifndef RENDERER_API
#define RENDERER_API
#include "Base.h"
#include "Mesh.h"
#include "Math.h"
/*
	렌더러와 그래픽스 부분간의 인터페이스
	-사용하는 그래픽스 api 타입 관리 및 팩토리 를 통한 생성관리
	-뷰포트 , 배경색 , 선 굵기 등 파이프라인 상태 설정 및 초기화 호출 위한 중간 인터페이스 필요
	-렌더러에서 호출할 기본단위(라인(디버깅용 추가 정보 제공용도 만들어 봅시다) 및 인덱스 기반 드로우 콜) 드로우 콜 하위 그래픽스 api 에 전달 위한 인터페이스 필요

*/
namespace MJEngine
{
	class RendererAPI
	{
	public:
		enum class API // 사용할 그래픽스 api 종류 지정
		{// None 의 경우 렌더링 없이 내부 로직만( 충돌 등 게임로직 ) 수행 
		//=> 해당경우에대한 널 오브젝트 패턴을 적용하여 전체 로직이 그래픽 api 사용처리 시와 예외 없이(렌더링 만 안하고 ) 통일 되게 동작하게 해야함
			None = 0, D3D11 = 1
			// 일단은 D3D11만을 지원
		};

	public:
		~RendererAPI() = default;
		
		virtual void Init() = 0;
		virtual void SetViewport(Vector_2i offset, uint32_t width,uint32_t height ) = 0;
		
		// Hazel 엔진의 virtual void SetClearColor(const glm::vec4& color) = 0; 
		// 다음의 함수가 glm 을 사용한다(해당 엔진에서는 d3d11 환경에서는 directMath 이외는 glm 사용 할 계획)
		// math 인터페이스 제작후 해당 
		// virtual void SetClearColor(const MJEngine::vec4& color) = 0; 대체 필요
		virtual void SetClearColor(const Vector_4f& color) = 0;

		virtual void Clear() = 0;

		//중요! DrawIndexed 구현부에서는 IA 전달전
		// 무조건 개발자가 입력한 Index Count 에 대한 검증 필요 
		// Mesh 의 indexBuffer 의 크기와 Index Count 비교하여 개발자가 메시에 대한 잘못된 스펙사항 입력시(indexBuffer 의 카운트를 초과하는 카운드 입력시) 
		// 곧바로 assert 통해 스펙을 잘못 이해함에 대한 정보를 함께(어떠한 방식이 좋지..?, 메시 구성 이름?인덱스 maxCnt?) 제공 해주어야함
		virtual void DrawIndexed(const RefPtr<Mesh>& mesh, uint32_t indexCount = 0) = 0; // indexCount 입력 없거나 0 전체 draw 
		virtual void DrawLines(const RefPtr<Mesh>& mesh , uint32_t vertexCount ) = 0; // 개발자가 draw를 원하는 vertexCount 명시적으로 입력해야함 
		
		virtual void SetLineWidth(float width) = 0;

		static API GetAPI() { return m_api;  }
		static ScopePtr<RendererAPI> Create();

	private:
		static API m_api;
		
	};

}

#endif // !RENDERER_API