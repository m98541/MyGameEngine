#include "CommandManager.h"
#include "../GameEngine/Base.h"
#include "../ShaderIO/RenderPassInfoIO.h"
#include "../ShaderIO/ShaderInfoIO.h"


#include <string>
#include <codecvt>
#include <locale>

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

bool CommandManager::RegisterRenderPass( // invalid 쉐이더의 경우 CommandShader::INVALID_SHADER 전달 사용자가 의식적으로 전달해야함
	eastl::string passName,
	CommandShader* vertShader,
	CommandShader* hullShader,
	CommandShader* domainShader,
	CommandShader* geoShader,
	CommandShader* pixelShader
)
{

	if (vertShader == CommandShader::INVALID_SHADER || pixelShader == CommandShader::INVALID_SHADER)
	{
		//assert! vertex , pixel 쉐이더는 필수로 CommandShader::INVALID_SHADER일 수 없음

		return false;
	}

	CommandRenderPass renderPass(passName);
	CommandShader* inputCommand = CommandShader::INVALID_SHADER;

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

	
	if (hullShader != CommandShader::INVALID_SHADER)
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


	if (domainShader != CommandShader::INVALID_SHADER && !renderPass.IsValidStage(PipeLineStage::Domain))
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

	if (geoShader != CommandShader::INVALID_SHADER && !renderPass.IsValidStage(PipeLineStage::Geometry))
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

bool CommandManager::RenderPassFileCompileAndSave(eastl::string filePath, eastl::string passFileName, eastl::string shaderFileName, ShaderCompiler::API useAPI)
{
	ShaderInfoIO shaderIO;
	RenderPassInfoIO renderPassInfoIO;

	MJEngine::ScopePtr<ShaderCompiler> shaderCompiler;
	shaderCompiler->SetAPI(useAPI);

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> stdWStringConvert;
	std::wstring argOrgFilePathWstrTemp = L"";
	std::wstring argBinFilePathWstrTemp = L"";
	eastl::string argEntryPointTemp = "";
	eastl::string argTargetProfileTemp = "";
	CommandRenderPass cmdRenderPass = {};


	//compile & reflection 단계
	for (const auto& cmdRenderPassElement : m_renderPassTable)
	{
		cmdRenderPass = cmdRenderPassElement.second;
		RenderPassInfo renderPass;

		//Input Layout 등록 위한 정점 쉐이더 정보 미리 빼둠
		ShaderInfo vertexShaderInfo;
		ShaderInfoDesc vertexShaderDesc;

		for (uint8_t i = 0; i < PipeLineStageCnt(); i++)
		{
			PipeLineStage stage = static_cast<PipeLineStage>(i);
			CommandShader* cmdShader = cmdRenderPass.GetRenderPassShader(stage);


			if (cmdShader == CommandShader::INVALID_SHADER)
				continue;

			ShaderInfo shader;
			ShaderInfoDesc shaderDesc;

			argOrgFilePathWstrTemp = stdWStringConvert.from_bytes(cmdShader->GetFilePath().c_str());
			argBinFilePathWstrTemp = stdWStringConvert.from_bytes(filePath.c_str());
			argEntryPointTemp = cmdShader->GetEntryPoint();
			argTargetProfileTemp = GetShaderProfileVersionString(cmdShader->GetProfileVersion());

			shaderCompiler->ShaderCompile(
				argOrgFilePathWstrTemp.c_str(),
				argEntryPointTemp.c_str(),
				argTargetProfileTemp.c_str(),
				argBinFilePathWstrTemp.c_str(),
				shader,
				shaderDesc
			);

			if (stage == PipeLineStage::Vertex)
			{
				vertexShaderInfo = shader;
				vertexShaderDesc = shaderDesc;
			}

			shaderIO.SetShader(
				shader.m_id,
				shader
			);

			// 쉐이더 IO 테이블에 등록되어 있는 ID 기준으로 가져옴
			ShaderInfo* shaderInTable = nullptr;

			shaderIO.FindShader(shader.m_id,&shaderInTable);
			renderPass.SetShader(shader.m_stage ,*shaderInTable);
		}

	
		//Input Layout 등록 단계
		PassInputLayoutContext inputLayoutContext = {};
		uint32_t inputSlot = 0;
		for (ShaderIOLayoutElement vsElement : vertexShaderInfo.m_inputLayout)
		{
			InputLayoutElement passElement;

			passElement.name = vsElement.semanticName;
			passElement.format = vsElement.format;
			passElement.inputSlot = inputSlot++;
			passElement.alignedByteOffset = vsElement.alignedByteOffset;
			passElement.semanticIndex = vsElement.semanticIndex;
			passElement.location = vsElement.location;

			inputLayoutContext.inputLayout.push_back(passElement);
		}

		if (cmdRenderPass.IsValidStage(PipeLineStage::Hull) && cmdRenderPass.IsValidStage(PipeLineStage::Domain))
		{
			//엔진 요구사항:
			//hull -> domain 단계가 들어갈시 Topology 유형을 patch 로 해줘야함 
			//이후 엔진에서 사용자 입력을 강제해야함
			inputLayoutContext.primitiveTopology = PRIMITIVE_TOPOLOGY::UNDEFINED;
		}
		else
		{
			//엔진 요구사항:
			// 기본 유형 -> 엔진에서 set으로 변경 가능하게 해줘야함
			inputLayoutContext.primitiveTopology = PRIMITIVE_TOPOLOGY::TRIANGLE_LIST;
		}
		
		renderPass.SetPassInputLayoutInfo(inputLayoutContext);

		renderPassInfoIO.SetRenderPass(renderPass.renderPassId, renderPass);
	}
	

	// file IO 단계 쉐이더 테이블 과 렌더 패스 테이블 파일 최종 저장
	shaderIO.WriteShaderInfo(filePath.c_str(), passFileName.c_str());
	renderPassInfoIO.WriteRenderPassInfo(filePath.c_str() , passFileName.c_str());
	return true;
}