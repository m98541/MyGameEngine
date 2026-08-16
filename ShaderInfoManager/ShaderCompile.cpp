#include "ShaderCompiler.h"
#include "D3D11Compiler.h"

MJEngine::ScopePtr<ShaderCompiler> ShaderCompiler::Create()
{
	switch (m_api)
	{
	case ShaderCompiler::API::None:
		break;
	case ShaderCompiler::API::D3D11:
		return MJEngine::CreateScopePtr<ShaderCompiler>();
	default:
		break;
	}
}