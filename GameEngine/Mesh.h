#ifndef MESH_H

#include "Buffer.h"
#include <memory>

namespace MJEngine
{
	/*
		Mesh 인터페이스 
		하나의 Mesh 정보(정점{pos , color .... } 집합과 인덱스) 관리 인터페이스

		정점 집합의 경우 동일 메시에 대해 여러 종류 가 존재 할 수 있음 
		예) 색상이 다른 동일 메시 라던가 캐릭터 포즈에 따른 스키닝 정보(즉 pos 정보 달라짐) 
		단 동일 카운트 구성은 동일

		인덱스 정보는 하나의 매시에 대해 단일 정보 관리

		Mesh 의 실제 생성을 위한 create 구현부는 해당 platform 에 종속된 D3D11Mesh 등에서 구현 되어야 하며
		Mesh.cpp 에서는 해당 렌더러에 맞게 스위칭 하여 객체 생성하는 팩토리 구조 필요
	*/

	class Mesh
	{
	public:
		virtual ~Mesh() = default;

		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;

		virtual void AddVertexBuffer(const RefPtr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const RefPtr<IndexBuffer>& indexBuffer ) = 0;
	
		virtual const std::vector<RefPtr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const RefPtr<IndexBuffer>& GetIndexBuffer() const = 0;

		static RefPtr<Mesh> Create();
	};
}

#endif // !MESH_H
