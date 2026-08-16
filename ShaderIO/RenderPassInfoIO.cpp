#include "RenderPassInfoIO.h"

#include <iostream>
#include <experimental/filesystem>
#include <fstream>
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
				m_renderPassList[element.renderPassId] = element;
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

const std::vector<std::string> RenderPassInfoIO::GetRenderPassIdList()
const
{
	std::vector<std::string> re = {};

	for (const auto& renderPassInfo : m_renderPassList)
	{
		re.push_back(renderPassInfo.first);
	}

	return re;
}

bool RenderPassInfoIO::FindRenderPass(std::string id, RenderPassInfo& outRenderPassInfo)
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

void RenderPassInfoIO::SetRenderPassMaterialTag(std::string id, std::string materialTag)
{
	m_renderPassList[id].materialTag = materialTag;
}

void RenderPassInfoIO::SetRenderPass(std::string id, RenderPassInfo element)
{
	m_renderPassList[id] = element;
}
void RenderPassInfoIO::ClearRenderPassList()
{
	m_renderPassList.clear();
}