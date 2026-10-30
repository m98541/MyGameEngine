#include "CommandShader.h"

CommandShader::CommandShader()
{
}

CommandShader::CommandShader(
	eastl::string filePath,
	eastl::string entryPoint,
	PipeLineStage stage,
	ShaderProfileVersion profileVersion
)
{
	m_orgFilePath = filePath;
	m_orgEntryPoint = entryPoint;
	m_stage = stage;
	m_profileVersion = profileVersion;
}

CommandShader::CommandShader(ShaderInfo shader)
{
	m_orgFilePath = shader.m_shaderFilePath;
	m_orgEntryPoint = shader.m_entryPoint;
	m_stage = shader.m_stage;
	m_profileVersion = shader.m_targetProfileVersion;
}

eastl::string CommandShader::GetFilePath()
{
	return m_orgFilePath;
}
eastl::string CommandShader::GetEntryPoint()
{
	return m_orgEntryPoint;
}
PipeLineStage CommandShader::GetPipeLineStage()
{
	return m_stage;
}
ShaderProfileVersion CommandShader::GetProfileVersion()
{
	return m_profileVersion;
}	