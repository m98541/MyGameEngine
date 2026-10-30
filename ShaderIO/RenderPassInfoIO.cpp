#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "RenderPassInfoIO.h"

#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <string>

eastl::string RenderPassInfoIO::GetShaderTableFilePath()
{
	return m_shdaerTableFilePath.c_str();
}
void RenderPassInfoIO::SetShaderTableFilePath(eastl::string shaderFilePath)
{
	m_shdaerTableFilePath = shaderFilePath;
}

void RenderPassInfoIO::ReadRenderPassInfo(const char* jsonFilePath)
{
	std::ifstream file(jsonFilePath);
	std::string tempFileBuffer((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

	rapidjson::StringStream jsonStringStream(tempFileBuffer.c_str());
	rapidjson::Document jsonDoc;
	jsonDoc.ParseStream(jsonStringStream);
	if (jsonDoc.HasParseError()) {
		//assert! json 파싱 에러
	}
	const rapidjson::Value& jsonValue = jsonDoc;

	if (jsonValue.HasMember(KEY_SHADER_TABLE_FILE_PATH))
	{
		m_shdaerTableFilePath = jsonValue[KEY_SHADER_TABLE_FILE_PATH].GetString();
	}
	else
	{

	}

	m_renderPassList.clear();

	if (jsonValue.HasMember(KEY_RENDER_PASS_LIST))
	{
		const auto& arr = jsonValue[KEY_RENDER_PASS_LIST].GetArray();

		if (arr.Size() == 0)
		{
			//assert! 빈 테이블 가짐
		}
		else
		{
			for (int i = 0; i < arr.Size(); i++)
			{
				RenderPassInfo element;
				element.ReadJsonObject(arr[i]);
				m_renderPassList[element.m_renderPassId] = element;
			}

		}


	}
	else
	{
		//assert!! JSON 파일내 테이블 자체가 없음
	}

	file.close();
}

void RenderPassInfoIO::WriteRenderPassInfo(const char* jsonFileName, const char* jsonFilePath)
{
	std::experimental::filesystem::path  dir = jsonFilePath;
	std::experimental::filesystem::path  name = jsonFileName;
	std::experimental::filesystem::path fullPath = dir / name;

	std::ofstream file(fullPath);
	assert(file.is_open() && "file open fail || RenderPassInfoIO / WriteRenderPassInfo ");

	rapidjson::StringBuffer stringBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(stringBuffer);

	writer.StartObject();

	writer.Key(KEY_SHADER_TABLE_FILE_PATH);
	writer.String(m_shdaerTableFilePath.c_str());

	writer.Key(KEY_RENDER_PASS_LIST);
	writer.StartArray();
	for (const auto& renderPassInfo : m_renderPassList)
	{
		renderPassInfo.second.WriteJsonObject(writer);
	}

	writer.EndArray();
	writer.EndObject();

	file << stringBuffer.GetString();
	file.close();

}

const eastl::vector<eastl::string> RenderPassInfoIO::GetRenderPassIdList()
const
{
	eastl::vector<eastl::string> re = {};

	for (const auto& renderPassInfo : m_renderPassList)
	{
		re.push_back(renderPassInfo.first);
	}

	return re;
}

bool RenderPassInfoIO::FindRenderPass(eastl::string id, RenderPassInfo& outRenderPassInfo)
const
{
	auto renderPass = m_renderPassList.find(id);

	if (renderPass != m_renderPassList.end())
	{
		outRenderPassInfo = renderPass->second;
		return true;
	}

	return false;

}

void RenderPassInfoIO::SetRenderPassMaterialTag(eastl::string id, eastl::string m_materialTag)
{
	m_renderPassList[id].m_materialTag = m_materialTag;
}

void RenderPassInfoIO::SetRenderPass(eastl::string id, RenderPassInfo element)
{
	m_renderPassList[id] = eastl::move(element);
}
void RenderPassInfoIO::ClearRenderPassList()
{
	m_renderPassList.clear();
}