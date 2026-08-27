#include "ShaderCompiler.h"
#include "D3D11Compiler.h"

// m_api의 실제 메모리 할당 및 초기화
ShaderCompiler::API ShaderCompiler::m_api = ShaderCompiler::API::D3D11;

MJEngine::ScopePtr<ShaderCompiler> ShaderCompiler::Create()
{
	switch (m_api)
	{
	case ShaderCompiler::API::None:
		break;
	case ShaderCompiler::API::D3D11:
		return MJEngine::CreateScopePtr<D3D11Compiler>();
	default:
		break;
	}
}