#include "RenderPassInfo.h"

void RenderPassInfo::SetShader(PipeLineStage stage, const ShaderInfo& shader)
{
	if (stage == shader.m_stage)
	{
		m_pipeLines[static_cast<int>(stage)].IsValid = true;
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
	m_pipeLines[static_cast<int>(stage)].IsValid = false;
}

bool RenderPassInfo::GetVertexShaderId(PipeLineStage stage, std::string& id) const
{
	if (m_pipeLines[static_cast<int>(stage)].IsValid)
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

//해당 렌더 페스의 IA 포맷 및 프리미티브 정보 입력 
void RenderPassInfo::SetPassInputLayoutInfo(PassInputLayoutContext passInputLayoutInfo)
{
	m_passInputContext = passInputLayoutInfo;
}

// IA 에 필요한 정보 제공 IA 포맷 , 프리미티브 , 정점 입력 레이아웃 정보 매개변수 방식 전달
void RenderPassInfo::GetPassInputLayoutInfo(PassInputLayoutContext& outIAPrimitiveAndFormat) const
{
	outIAPrimitiveAndFormat = m_passInputContext;
}

/*
	외부 쉐이더 툴에서 컴파일러를 거치게 되어짐으로 
	쉐이더 내부 문법적인 요소까지는 굳이 잡지 않아도 됨
	아래의 passCheck 는 Pass 순서의 쉐이더간 인터페이스 규격 맞췆는지
	체크해주는 용도로 기능함
*/
PassCheckResult RenderPassInfo::RenderPassCheck() const
{
	PassCheckResult result = {};



	return result;
}