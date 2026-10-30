#ifndef BUFFER_H
#define BUFFER_H
#include "Base.h"
#include <assert.h>
#include <string>
#include <vector>

/*
 	해당 코드는
	VertexElement 란 정점내의 하나의 자료형의 단위형태로 정의되며 
	여러 요소가 혼합된 정점에 대해서 
	std::vector<VertexElement> m_Elements; 다음과 같이 저장하며
	이를 통해 정점 레이아웃을 구성시킨다.
*/
namespace MJEngine
{
	enum class ShaderDataType
	{
		None = 0,
		Int, UInt, Float, Double, Bool,
		Int2, Int3, Int4,
		Float2, Float3, Float4,
		Mat4
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case MJEngine::ShaderDataType::Int: return 4;
		case MJEngine::ShaderDataType::UInt: return 4;
		case MJEngine::ShaderDataType::Float: return 4;
		case MJEngine::ShaderDataType::Double: return 8;
		case MJEngine::ShaderDataType::Bool: return 1;
		case MJEngine::ShaderDataType::Int2: return 4 * 2;
		case MJEngine::ShaderDataType::Int3: return 4 * 3;
		case MJEngine::ShaderDataType::Int4: return 4 * 4;
		case MJEngine::ShaderDataType::Float2: return 4 * 2;
		case MJEngine::ShaderDataType::Float3: return 4 * 3;
		case MJEngine::ShaderDataType::Float4: return 4 * 4;
		case MJEngine::ShaderDataType::Mat4: return 4 * 4 * 4;
		}

		assert(false && "Unknown ShaderDataType!");
		return 0;
	}

	/*
		struct : 데이터형 set
		class : 데이터 외부 접근 차단 , 규칙(매서드) 통해서만 접근
		내부적으론은 둘다 class 로 동작하지만 명시적 구분위해 struct 사용
	*/
	struct VertexElement
	{
		std::string name;
		ShaderDataType type;
		uint32_t size;
		size_t offset;
		bool normalized;

		VertexElement() = default;

		// 초기화 리스트 방식의 경우 메모리상에 생성과 동시에 값 초기화 됨
		VertexElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: name(name) , type(type) , size(ShaderDataTypeSize(type)) , offset(0) , normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const // 맴버 수정 금지
		{
			switch (type)
			{
			case MJEngine::ShaderDataType::Int: return 1;
			case MJEngine::ShaderDataType::UInt: return 1;
			case MJEngine::ShaderDataType::Float: return 1;
			case MJEngine::ShaderDataType::Double: return 1;
			case MJEngine::ShaderDataType::Bool: return 1;
			case MJEngine::ShaderDataType::Int2: return 2;
			case MJEngine::ShaderDataType::Int3: return 3;
			case MJEngine::ShaderDataType::Int4: return 4;
			case MJEngine::ShaderDataType::Float2: return 2;
			case MJEngine::ShaderDataType::Float3: return 3;
			case MJEngine::ShaderDataType::Float4: return 4;
			case MJEngine::ShaderDataType::Mat4: return 4;
			}

			assert(false && "Unknown ShaderDataType!");
			return 0;
		}
	};

	class VertexLayout
	{
	public:

		VertexLayout() {}; // = default 의 경우 위에처럼 메모리 생성과 동시에 초기화 해주는 형태가 아니면 쓰리값이 들어갈 수 있음

		VertexLayout(std::initializer_list<VertexElement> vertexLayout)
			: m_vertexLayout(vertexLayout)
		{
			CalculateOffsetsAndStride();
		}
		uint32_t GetStride() const { return m_stride; }
		const std::vector<VertexElement>& GetElements() const { return m_vertexLayout; }

		std::vector<VertexElement>::iterator begin() { return m_vertexLayout.begin(); }

		std::vector<VertexElement>::iterator end() { return m_vertexLayout.end(); }

		std::vector<VertexElement>::const_iterator begin() const { return m_vertexLayout.begin(); }

		std::vector<VertexElement>::const_iterator end() const { return m_vertexLayout.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			m_stride = 0;
			
			for (auto& element : m_vertexLayout)
			{
				element.offset = offset;
				offset += element.size;
				m_stride += element.size;
			}
		}
	private:
		std::vector<VertexElement> m_vertexLayout;
		uint32_t m_stride = 0;

	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;// = 0; 상속 받고 무조건 오버라이드 즉 해당 매서드 구현 강제 해줘야함 안해주면 컴파일 에러
		virtual void UnBind() const = 0;

		virtual void SetData(const void* data , uint32_t size) = 0;
		
		virtual const VertexLayout& GetLayout() const = 0;
		virtual void SetLayout(const VertexLayout& layout) = 0;

		/*
		정적 맴버 함수 => 맴버 함수에 대해서 함수내 객체 정보에 대한 Read / Write를 모두 막음
		즉 객체에 대한 접근 자체를 막아 객체가 없이 일반적(c 언어의 전역적 함수) 처럼 사용가능(만일 객체가 없는 매서드가 해당 매서드의 맴버 접근하면 잘못된 메모리를 참조하거나 폴트 날테니)
		해당 기능의 경우 Create 로 내부적으로 렌더러 api 에 맞는 설정이 들어가도록 타입에 따라 스위칭 해주어 생성해주는 
		팩토리 구조 필요 => VertexBuffer 의 팩토리 역할을 맡음
		*/
		static RefPtr<VertexBuffer> Create(uint32_t size);
		static RefPtr<VertexBuffer> Create(float* vertices ,uint32_t size);

	};
	

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual uint32_t GetCount() const = 0;

		static RefPtr<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
}

#endif // !BUFFER_H
