#ifndef COMMAND_SHADER_H
#define COMMAND_SHADER_H
#include <eastl/string.h>
#include "../ShaderIO/ShaderBase.h"
// 쉐이더 컴파일을 위해서 사용자에게 받아야 하는 정보체
// 1. 원본 파일 이름 (경로명)
// 2. entryPoint 이름
// 3. 해당 쉐이더의 stage 정보 <- 이건 사용자 임의 입력 X 선택하게 해야함 : shaderInfo.h 의 스테이지 정보 중 1택
// 4. HLSL 버전 정보 Major_Minor 결합 형태 <- 이건 사용자 임의 입력 X 선택하게 해야함 : enum 을 통해 표준 버전 중 택 필요  



class CommandShader
{

public:
	CommandShader();

	CommandShader(
		eastl::string filePath,
		eastl::string entryPoint,
		PipeLineStage stage,
		ShaderProfileVersion profileVersion
	);
	
	eastl::string GetFilePath();
	eastl::string GetEntryPoint();
	PipeLineStage GetPipeLineStage();
	ShaderProfileVersion GetProfileVersion();

	static constexpr CommandShader* INVALID_SHADER = nullptr;
private:
	eastl::string m_orgFilePath;
	eastl::string m_orgEntryPoint;
	PipeLineStage m_stage;
	ShaderProfileVersion m_profileVersion;

};


#endif // !COMMAND_SHADER_H
