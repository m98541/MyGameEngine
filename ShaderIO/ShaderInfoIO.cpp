#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "ShaderInfoIO.h"

#include <iostream>
#include <experimental/filesystem>
#include <fstream>


#include <assert.h>




void ShaderInfoIO::ReadShaderInfo(const char* jsonFilePath)
{
	std::ifstream file(jsonFilePath);
	assert(file.is_open() && "file open fail || ShaderInfoIO / ReadShaderInfo-1 ");
	
	std::string tempFileBuffer( (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	rapidjson::StringStream jsonStringStream(tempFileBuffer.c_str());
	rapidjson::Document jsonDoc;
	jsonDoc.ParseStream(jsonStringStream);
	if (jsonDoc.HasParseError()) {
		//assert! json 파싱 에러!

	}
	const rapidjson::Value& jsonValue = jsonDoc;


	m_shaderMap.clear();

	if (jsonValue.HasMember(KEY_SHADER_TABLE))
	{
		const auto& arr = jsonValue[KEY_SHADER_TABLE].GetArray();
		m_shaderCount = arr.Size();

		if (m_shaderCount == 0)
		{
			//assert! 빈테이블 가지고 있음
		}
		else
		{
			for (int i = 0; i < m_shaderCount; i++)
			{
				ShaderInfo element;
				element.ReadJsonObject(arr[i]);
				m_shaderMap[element.m_id] = element;

			}
		}
		
	}
	else
	{
		//assert! JSON 파일내 테이블 존재 X 
	}

	file.close();
}



void ShaderInfoIO::WriteShaderInfo(const char* jsonFilePath, const char* jsonFileName)
{
	std::experimental::filesystem::path  dir = jsonFilePath;
	std::experimental::filesystem::path  name = jsonFileName;
	std::experimental::filesystem::path fullPath = dir / name;

	std::ofstream file(fullPath);
	assert(file.is_open() && "file open fail || ShaderInfoIO / WriteShaderInfo-1 ");

	rapidjson::StringBuffer stringBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(stringBuffer);

	writer.StartObject();
	writer.Key(KEY_SHADER_TABLE);
	writer.StartArray();
	for (const auto& shaderInfo : m_shaderMap)
	{
		shaderInfo.second.WriteJsonObject(writer);
	}
	writer.EndArray();
	writer.EndObject();


	file << stringBuffer.GetString();
	file.close();

}



const eastl::vector<eastl::string> ShaderInfoIO::GetShaderIdList()const
{
	eastl::vector<eastl::string> re = {};

	for (const auto& shaderInfo : m_shaderMap)
	{
		re.push_back(shaderInfo.first);
	}

	return re;
}

bool ShaderInfoIO::FindShader(eastl::string id, ShaderInfo** outShader)
{
	auto shader = m_shaderMap.find(id);

	if (shader != m_shaderMap.end())
	{
		*outShader = &shader->second;
		return true;
	}
	else
	{
		return false;
	}

}

void ShaderInfoIO::SetShader(eastl::string id, ShaderInfo element)
{
	m_shaderMap[id] = element;
}

void ShaderInfoIO::DeleteShader(eastl::string id)
{
	m_shaderMap.erase(id);
}

void ShaderInfoIO::ClearShaderMap()
{
	m_shaderMap.clear();
}
