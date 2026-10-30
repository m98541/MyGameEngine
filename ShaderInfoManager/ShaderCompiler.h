#ifndef SHADER_COMPILER_H
#define SHADER_COMPILER_H

#include "../ShaderIO/ShaderInfo.h"
#include "../GameEngine/Base.h"

class ShaderCompiler
{
public:
	enum class API : uint8_t
	{
		None, D3D11
	};
	~ShaderCompiler() = default;

	virtual bool ShaderCompile(
		const wchar_t* hlslFilePath,
		const char* entryPoint,
		const char* targetProfile,
		const wchar_t* outputBinPath,
		ShaderInfo& shaderInfo,
		ShaderInfoDesc& shaderInfoDesc
	) = 0;
	
	static MJEngine::ScopePtr<ShaderCompiler> Create(API api);
	void SetAPI(API api) { m_api = api; }
	API GetAPI() { return m_api; }

private:
	static API m_api;
};
#endif // !SHADER_COMPILER_H
