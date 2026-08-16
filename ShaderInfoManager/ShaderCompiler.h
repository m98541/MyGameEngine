#ifndef SHADER_COMPILER_H
#define SHADER_COMPILER_H

#include "../ShaderIO/ShaderInfo.h"
#include "../GameEngine/Base.h"

class ShaderCompiler
{
	enum class API : uint8_t
	{
		None , D3D11
	};
private:
	static API m_api;

public:
	~ShaderCompiler() = default;

	virtual ShaderInfo ShaderCompile(
		const wchar_t* hlslFilePath,
		const char* entryPoint,
		const char* targetProfile,
		const char* outputBinPath
	) = 0;
	
	static MJEngine::ScopePtr<ShaderCompiler> Create();
	void SetAPI(API api) { m_api = api; }
	API GetAPI() { return m_api; }

};
#endif // !SHADER_COMPILER_H
