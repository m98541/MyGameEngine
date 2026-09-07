#include "CommandManager.h"

CommandManager::CommandManager()
{

}

CommandManager::~CommandManager()
{

}

void CommandManager::RegisterShaderInTable(CommandShader& shader, CommandShader** outTableShaderAddr)
{
	uint8_t stage = static_cast<uint8_t>(shader.GetPipeLineStage());
	eastl::string path = shader.GetFilePath();

	auto it = m_shaderTable[stage].find(path);
	if (it == m_shaderTable[stage].end())
	{
		CommandShaderElement element = {shader , 1};
		m_shaderTable[stage][path] = element;
		
	}
	else
	{
		m_shaderTable[stage][path].refCnt++;
	}

	(*outTableShaderAddr) = &m_shaderTable[stage][path].shader;
}


void CommandManager::DeleteShaderInTable(CommandShader& shader)
{
	uint8_t stage = static_cast<uint8_t>(shader.GetPipeLineStage());
	eastl::string path = shader.GetFilePath();

	auto it = m_shaderTable[stage].find(path);

	if (it != m_shaderTable[stage].end())
	{
		if (m_shaderTable[stage][path].refCnt == 1)
		{
			m_shaderTable[stage].erase(path);
		}
		else
		{
			m_shaderTable[stage][path].refCnt--;
		}
	}

}

bool CommandManager::RegisterRenderPass( // invalid 쉐이더의 경우 nullptr 전달 사용자가 의식적으로 전달해야함
	eastl::string passName,
	CommandShader* vertShader,
	CommandShader* hullShader,
	CommandShader* domainShader,
	CommandShader* geoShader,
	CommandShader* pixelShader
)
{
	// nullptr 체크 + 패스 내 해당 쉐이더 valid 여부 체크
	// 패스내 저장하려는 쉐이더가 valid 상태면 에러(중복 스테이지로 저장으로 매개변수 오입력)
	// vertex , pixel 쉐이더는 필수로 nullptr 이면 즉시 에러

	if (vertShader == nullptr || pixelShader == nullptr)
	{
		//assert! vertex , pixel 쉐이더는 필수로 nullptr일 수 없음
		return false;
	}

	CommandRenderPass renderPass(passName);
	CommandShader* inputCommand = nullptr;

	RegisterShaderInTable(*vertShader , &inputCommand);
	renderPass.SetRenderPassShader(*inputCommand);
	if (!renderPass.IsValidStage(PipeLineStage::Vertex))
	{
		//assert Vertex Shader 의 잘못 된 입력 다른 쉐이더 정보를 입력함
		DeleteShaderInTable(*inputCommand);
		return false;
	}

	RegisterShaderInTable(*pixelShader, &inputCommand);
	renderPass.SetRenderPassShader(*inputCommand);
	if (!renderPass.IsValidStage(PipeLineStage::Pixel))
	{
		//assert pixel Shader 의 잘못 된 입력 다른 쉐이더 정보를 입력함
		DeleteShaderInTable(*inputCommand);
		return false;
	}

	
	if (hullShader != nullptr)
	{
		RegisterShaderInTable(*hullShader, &inputCommand);
		renderPass.SetRenderPassShader(*inputCommand);
		// 다음처럼 에러처리를 해야 어떤 쉐이더를 잘못입력하였는지 알려줄 수 있음
		if (!renderPass.IsValidStage(PipeLineStage::Hull))
		{
			//assert hull Shader 의 잘못 된 입력 다른 쉐이더 정보를 입력함
			DeleteShaderInTable(*inputCommand);
			return false;
		}
	}


	if (domainShader != nullptr && !renderPass.IsValidStage(PipeLineStage::Domain))
	{
		RegisterShaderInTable(*domainShader, &inputCommand);
		renderPass.SetRenderPassShader(*inputCommand);
		if (!renderPass.IsValidStage(PipeLineStage::Domain))
		{
			//assert Domain Shader 의 잘못 된 입력 다른 쉐이더 정보를 입력함
			DeleteShaderInTable(*inputCommand);
			return false;
		}
	}

	if (geoShader != nullptr && !renderPass.IsValidStage(PipeLineStage::Geometry))
	{
		RegisterShaderInTable(*geoShader, &inputCommand);
		renderPass.SetRenderPassShader(*inputCommand);
		if (!renderPass.IsValidStage(PipeLineStage::Geometry))
		{
			//assert Geometry Shader 의 잘못 된 입력 다른 쉐이더 정보를 입력함
			DeleteShaderInTable(*inputCommand);
			return false;
		}
	}

	m_renderPassTable[passName] = renderPass;
	
	return true;
}

void CommandManager::DeleteRenderPass(eastl::string passName)
{

	auto it = m_renderPassTable.find(passName);
	if (it != m_renderPassTable.end())
	{
		CommandRenderPass* deleteRenderPass = &m_renderPassTable[passName];

		for (size_t stage = 0; stage < PipeLineStageCnt(); stage++)
		{
			CommandShader* deleteShader = deleteRenderPass->GetRenderPassShader(static_cast<PipeLineStage>(stage));
			if (deleteShader != nullptr)
			{
				DeleteShaderInTable(*deleteShader);
			}
		}

		m_renderPassTable.erase(passName);
	}

}

eastl::vector<eastl::string> CommandManager::GetStageShaderNamesTable(PipeLineStage stage)
{
	eastl::vector<eastl::string> re;

	for (const auto shaderMapElement : m_shaderTable[static_cast<uint8_t>(stage)])
	{
		re.push_back(shaderMapElement.first);
	}

	return re;
}

eastl::vector<eastl::string> CommandManager::GetRenderPassNames()
{
	eastl::vector<eastl::string> re;
	
	for (const auto passMapElement : m_renderPassTable)
	{
		re.push_back(passMapElement.first);
	}

	return re;
}
