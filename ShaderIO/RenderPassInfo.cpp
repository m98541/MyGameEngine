#include "RenderPassInfo.h"

void RenderPassInfo::SetShader(PipeLineStage stage, const ShaderInfo& shader)
{
	if (stage == shader.m_stage)
	{
		m_pipeLines[static_cast<int>(stage)].IsVaild = true;
		m_pipeLines[static_cast<int>(stage)].ShaderId = shader.m_id;
		m_pipeLines[static_cast<int>(stage)].stage = stage;
	}
	else
	{
		//MJAssert 로 정의 후 개발
	}
}

void RenderPassInfo::DeleteShader(PipeLineStage stage)
{
	m_pipeLines[static_cast<int>(stage)].IsVaild = false;
}

bool RenderPassInfo::GetVertexShaderId(PipeLineStage stage, std::string& id) const
{
	if (m_pipeLines[static_cast<int>(stage)].IsVaild)
	{
		id = m_pipeLines[static_cast<int>(stage)].ShaderId;
		return true;
	}
	else
	{
		//assert 가 좋을지도.. 외부에서 assert 호출이 좋을까..?
		return false;
	}
}

PassCheckResult RenderPassInfo::RenderPassCheck() const
{

}