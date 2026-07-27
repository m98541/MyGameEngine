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
	m_shaderMap.clear();
	while (jsonStringStream.Peek() != '\0')
	{
		while (
			jsonStringStream.Peek() == ' ' ||
			jsonStringStream.Peek() == '\n' ||
			jsonStringStream.Peek() == '\r' ||
			jsonStringStream.Peek() == '\t'
			)
		{
			jsonStringStream.Take();
		}

		if (jsonStringStream.Peek() == '\0')
			break;

		jsonDoc.ParseStream<rapidjson::kParseStopWhenDoneFlag>(jsonStringStream);

		if (jsonDoc.HasParseError())
		{
			// 이후 커스텀 개발이 assert 가 필요함... 지금 당장 Windows용 으로만 땜방해두기에는 .. 일단 ShaderInfo 이후 assert 도 바로 개발 필요..
			std::cerr << "[File Read(Parse) Error] ShaderInfoIO / ReadShaderInfo\n"
				<< "File Path : " << jsonFilePath << "\n"
				<< "Error Code: " << jsonDoc.GetParseError() << "\n"
				<< "Offset    : " << jsonDoc.GetErrorOffset() << std::endl;
			assert(false && "file read(parse) error || ShaderInfoIO / ReadShaderInfo-2 ");
		}

		ShaderInfo shaderElement = {};
		shaderElement.ReadRapidJson(jsonDoc);
		m_shaderMap[shaderElement.m_id] = shaderElement;
	}


	file.close();
}



void ShaderInfoIO::WriteShaderInfo(const char* jsonFileName, const char* jsonFilePath)
{
	std::experimental::filesystem::path  dir = jsonFilePath;
	std::experimental::filesystem::path  name = jsonFileName;
	std::experimental::filesystem::path fullPath = dir / name;

	std::ofstream file(fullPath);
	assert(file.is_open() && "file open fail || ShaderInfoIO / WriteShaderInfo-1 ");

	rapidjson::StringBuffer stringBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(stringBuffer);

	for (const auto& shaderInfo : m_shaderMap)
	{
		stringBuffer.Clear();
		writer.Reset(stringBuffer);

		shaderInfo.second.WriteRapidJson(writer);
		
		file << stringBuffer.GetString() << "\n";
	}

	file.close();

}



const std::vector<std::string>& ShaderInfoIO::GetShaderIdList()const
{
	std::vector<std::string> re = {};

	for (const auto& shaderInfo : m_shaderMap)
	{
		re.push_back(shaderInfo.first);
	}

	return re;
}

bool ShaderInfoIO::FindShader(std::string id, ShaderInfo& outShader)const
{
	auto shader = m_shaderMap.find(id);

	if (shader != m_shaderMap.end())
	{
		outShader = shader->second;
	}
	else
	{
		return false;
	}

}

void ShaderInfoIO::SetShader(std::string id, ShaderInfo element)
{
	m_shaderMap[id] = element;
}

void ShaderInfoIO::DeleteShader(std::string id)
{
	m_shaderMap.erase(id);
}

void ShaderInfoIO::ClearShaderMap()
{
	m_shaderMap.clear();
}
