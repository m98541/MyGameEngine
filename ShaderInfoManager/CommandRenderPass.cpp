#include "CommandRenderPass.h"

CommandRenderPass::CommandRenderPass()
{
}
CommandRenderPass::CommandRenderPass(eastl::string passName)
{
	m_passName = passName;
}

eastl::string CommandRenderPass::GetRenderPassName()
{
	return m_passName;
}

CommandShader* CommandRenderPass::SetRenderPassShader(CommandShader shader)
{
	CommandShader re = {};

	uint8_t stage = static_cast<uint8_t>( shader.GetPipeLineStage() );

	if (m_validList[stage])
	{// 이미 활성 되있던 쉐이더 기존 쉐이더 주소는 반환
		re = m_renderPass[stage];
	}
	else
	{
		m_validList[stage] = true;
	}

	m_renderPass[stage] = shader;
	
	return &re;
}

bool CommandRenderPass::IsValidStage(PipeLineStage stage)
{
	return m_validList[static_cast<uint8_t>(stage)];
}

CommandShader* CommandRenderPass::GetRenderPassShader(PipeLineStage stage)
{
	return &m_renderPass[static_cast<uint8_t>(stage)];
}