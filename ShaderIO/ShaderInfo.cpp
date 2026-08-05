#include "ShaderInfo.h"

/*
 현재 개별 구조체 read 부분에 jsonValue 맴버 존재 여부 예외 케이스 assert 안해둔 상태임
 MJassert 개발 되는데로 바로 적용해줘야함! (적용 후 글 삭제 필수!)
*/


void ShaderInfo::WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer)
const
{
	writer.StartObject();

	writer.Key("id");
	writer.String(m_id.c_str());

	writer.Key("stage");
	writer.Int(static_cast<int>(m_stage));


	writer.Key("inputLayout");
	writer.StartArray();
	for (const auto& oLayout : m_inputLayout)
	{
		oLayout.WriteJsonObject(writer);
	}
	writer.EndArray();

	writer.Key("outputLayout");
	writer.StartArray();
	for (const auto& oLayout : m_outputLayout)
	{
		oLayout.WriteJsonObject(writer);
	}
	writer.EndArray();

	writer.Key("constantBuffers");
	writer.StartArray();
	for (const auto& cBuffer : m_constantBuffers)
	{
		cBuffer.WriteJsonObject(writer);
	}
	writer.EndArray();

	writer.Key("textureSamplers");
	writer.StartArray();
	for (const auto& texSampler : m_textureSamplers)
		writer.String(texSampler.c_str());
	writer.EndArray();



	writer.Key("targetProfileVersion");
	writer.StartObject();

	writer.EndObject();


	writer.Key("entryPoint");
	writer.String(m_entryPoint.c_str());

	writer.Key("shaderFilePath");
	writer.String(m_shaderFilePath.c_str());

	writer.Key("shaderHeader");
	writer.String(m_shaderHeader.c_str());
	
	writer.EndObject();
}

void ShaderInfo::ReadJsonObject(const rapidjson::Value& jsonValue)
{
	if (!jsonValue.IsObject())
	{
		if (jsonValue.IsArray())
			assert(false && "shader jsonValue read fail - jsonValue type: Array! (Object type required) | ShaderInfo.cpp\ReadRadpidJson");
		else if(jsonValue.IsNull())
			assert(false && "shader jsonValue read fail - jsonValue is Null! | ShaderInfo.cpp\ReadRadpidJson");
		else
			assert(false && "shader jsonValue read fail - jsonValue type is not Object (Object type required) | ShaderInfo.cpp\ReadRadpidJson");
	}

	if (jsonValue.HasMember("id"))
	{
		if (jsonValue["id"].IsString())
		{
			m_id = jsonValue["id"].GetString();
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(id) type is not String | ShaderInfo.cpp\ReadRadpidJson");
		}
	}

	if (jsonValue.HasMember("stage"))
	{
		if (jsonValue["stage"].IsInt())
		{
			m_stage = static_cast<PipeLineStage>( jsonValue["stage"].GetInt() );
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(shader stage) type is not Int | ShaderInfo.cpp\ReadRadpidJson");
		}
	}

	if (jsonValue.HasMember("inputLayout"))
	{
		if (jsonValue["inputLayout"].IsArray())
		{
			m_inputLayout.clear();
			for (const auto& iLayout : jsonValue["inputLayout"].GetArray())
			{
				if (iLayout.IsString())
				{
					ShaderIOLayoutElement tempLayout;
					tempLayout.ReadJsonObject(iLayout);
					m_inputLayout.push_back(tempLayout);
				}
				else
				{
					assert(false && "shader jsonValue read fail - jsonValue(shader InputLayout element) type is not String | ShaderInfo.cpp\ReadRadpidJson");
				}
			}
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(shader InputLayout) type is not Array | ShaderInfo.cpp\ReadRadpidJson");
		}
	}

	if (jsonValue.HasMember("outputLayout"))
	{
		if (jsonValue["outputLayout"].IsArray())
		{
			m_outputLayout.clear();
			for (const auto& iLayout : jsonValue["outputLayout"].GetArray())
			{
				if (iLayout.IsString())
				{
					ShaderIOLayoutElement tempLayout;
					tempLayout.ReadJsonObject(iLayout);
					m_outputLayout.push_back(tempLayout);
				}
				else
				{
					assert(false && "shader jsonValue read fail - jsonValue(shader outputLayout element) type is not String | ShaderInfo.cpp\ReadRadpidJson");
				}
			}
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(shader outputLayout) type is not Array | ShaderInfo.cpp\ReadRadpidJson");
		}
	}

	if (jsonValue.HasMember("constantBuffers"))
	{
		if (jsonValue["constantBuffers"].IsArray())
		{
			m_constantBuffers.clear();
			for (const auto& iLayout : jsonValue["constantBuffers"].GetArray())
			{
				if (iLayout.IsString())
				{
					GlobalVariableBuffer tempGlobalVariableBuffer;
					tempGlobalVariableBuffer.ReadJsonObject(iLayout);
					m_constantBuffers.push_back(tempGlobalVariableBuffer);
				}
				else
				{
					assert(false && "shader jsonValue read fail - jsonValue(shader constantBuffers element) type is not String | ShaderInfo.cpp\ReadRadpidJson");
				}
			}
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(shader constantBuffers) type is not Array | ShaderInfo.cpp\ReadRadpidJson");
		}
	}

	if (jsonValue.HasMember("textureSamplers"))
	{
		if (jsonValue["textureSamplers"].IsArray())
		{
			m_textureSamplers.clear();
			for (const auto& iLayout : jsonValue["textureSamplers"].GetArray())
			{
				if (iLayout.IsString())
				{
					m_textureSamplers.push_back(iLayout.GetString());
				}
				else
				{
					assert(false && "shader jsonValue read fail - jsonValue(shader textureSamplers element) type is not String | ShaderInfo.cpp\ReadRadpidJson");
				}
			}
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(shader textureSamplers) type is not Array | ShaderInfo.cpp\ReadRadpidJson");
		}
	}


	if (jsonValue.HasMember("targetProfileVersion"))
	{
		if (jsonValue["targetProfileVersion"].IsString())
		{
			
			m_targetProfileVersion =static_cast<ShaderProfileVersion>(jsonValue["targetProfileVersion"].GetUint());
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(targetProfileVersion) type is not String | ShaderInfo.cpp\ReadRadpidJson");
		}
	}

	if (jsonValue.HasMember("entryPoint"))
	{
		if (jsonValue["entryPoint"].IsString())
		{
			m_entryPoint = jsonValue["entryPoint"].GetString();
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(entryPoint) type is not String | ShaderInfo.cpp\ReadRadpidJson");
		}
	}

	if (jsonValue.HasMember("shaderFilePath"))
	{
		if (jsonValue["shaderFilePath"].IsString())
		{
			m_shaderFilePath = jsonValue["shaderFilePath"].GetString();
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(shaderFilePath) type is not String | ShaderInfo.cpp\ReadRadpidJson");
		}
	}

	if (jsonValue.HasMember("shaderHeader"))
	{
		if (jsonValue["shaderHeader"].IsString())
		{
			m_shaderHeader = jsonValue["shaderHeader"].GetString();
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(shaderHeader) type is not String | ShaderInfo.cpp\ReadRadpidJson");
		}
	}



}

// 개별 구조체 JSON 입력 출력 선언부
void ShaderIOLayoutElement::WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	
	writer.Key("format");
	writer.Uint(static_cast<unsigned int>(format));
	
	writer.Key("name");
	writer.String(name.c_str());

	writer.Key("systemValue");
	writer.Bool(systemValue);

	writer.Key("location");
	writer.Uint(static_cast<unsigned int>(location));

	writer.EndObject();

}

void ShaderIOLayoutElement::ReadJsonObject(const rapidjson::Value& jsonValue)
{


	if (!jsonValue.IsObject())
	{
		if (jsonValue.IsArray())
			assert(false && "shader jsonValue read fail - jsonValue type: Array! (Object type required) | ShaderInfo.cpp-ShaderIOLayoutElement Read");
		else if (jsonValue.IsNull())
			assert(false && "shader jsonValue read fail - jsonValue is Null! | ShaderInfo.cpp-ShaderIOLayoutElement Read");
		else
			assert(false && "shader jsonValue read fail - jsonValue type is not Object (Object type required) | ShaderInfo.cpp-ShaderIOLayoutElement Read");
	}

	if (jsonValue.HasMember("format"))
	{
		format = static_cast<FORMAT>(jsonValue["format"].GetUint());
	}
	else
	{
		//향후 assert 개발시 추가 필요... 지금 string 직접 입력 방식 너무 노가다에 중구난방임
	}

	if (jsonValue.HasMember("name"))
	{
		name = jsonValue["name"].GetString();
	}
	else
	{

	}

	if (jsonValue.HasMember("systemValue"))
	{
		systemValue = jsonValue["systemValue"].GetBool();
	}
	else
	{

	}

	if (jsonValue.HasMember("semanticIndex"))
	{
		semanticIndex = jsonValue["semanticIndex"].GetUint();
	}
	else
	{

	}

	if (jsonValue.HasMember("location"))
	{
		location = jsonValue["location"].GetUint();
	}
	else
	{

	}



}

void GlobalVariableElement::WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	
	writer.Key("bufferName");
	writer.String(variableName.c_str());

	writer.Key("format");
	writer.Uint(static_cast<unsigned int>(format));

	writer.Key("offset");
	writer.Uint(static_cast<unsigned int>(offset));

	writer.Key("size");
	writer.Uint(static_cast<unsigned int>(size));

	writer.EndObject();
}

void GlobalVariableElement::ReadJsonObject(const rapidjson::Value& jsonValue)
{

	if (!jsonValue.IsObject())
	{
		if (jsonValue.IsArray())
			assert(false && "shader jsonValue read fail - jsonValue type: Array! (Object type required) | ShaderInfo.cpp-GlobalVariableElement Read");
		else if (jsonValue.IsNull())
			assert(false && "shader jsonValue read fail - jsonValue is Null! | ShaderInfo.cpp-GlobalVariableElement Read");
		else
			assert(false && "shader jsonValue read fail - jsonValue type is not Object (Object type required) | ShaderInfo.cpp-GlobalVariableElement Read");
	}

	if (jsonValue.HasMember("variableName"))
	{
		variableName = jsonValue["variableName"].GetString();
	}
	else
	{

	}

	if (jsonValue.HasMember("format"))
	{
		format = static_cast<FORMAT>(jsonValue["format"].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember("offset"))
	{
		offset = static_cast<uint32_t>(jsonValue["offset"].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember("size"))
	{
		size = static_cast<uint32_t>(jsonValue["size"].GetUint());
	}
	else
	{
	}
}

void GlobalVariableBuffer::WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();

	writer.Key("bufferName");
	writer.String(bufferName.c_str());

	writer.Key("bufferSize");
	writer.Uint(static_cast<unsigned int>(bufferSize));

	writer.Key("buffer");
	writer.StartArray();
	for (const auto& element : buffer)
	{
		element.WriteJsonObject(writer);
	}
	writer.EndArray();

	writer.Key("registerSpace");
	writer.Uint(static_cast<unsigned int>(registerSpace));

	writer.Key("regNum");
	writer.Uint(static_cast<unsigned int>(regNum));


	writer.Key("set");
	writer.Uint(static_cast<unsigned int>(set));

	writer.Key("binding");
	writer.Uint(static_cast<unsigned int>(binding));
	
	writer.EndObject();
}

void GlobalVariableBuffer::ReadJsonObject(const rapidjson::Value& jsonValue)
{
	if (!jsonValue.IsObject())
	{
		if (jsonValue.IsArray())
			assert(false && "shader jsonValue read fail - jsonValue type: Array! (Object type required) | ShaderInfo.cpp-GlobalVariableBuffer Read");
		else if (jsonValue.IsNull())
			assert(false && "shader jsonValue read fail - jsonValue is Null! | ShaderInfo.cpp-GlobalVariableBuffer Read");
		else
			assert(false && "shader jsonValue read fail - jsonValue type is not Object (Object type required) | ShaderInfo.cpp-GlobalVariableBuffer Read");
	}

	if (jsonValue.HasMember("bufferName"))
	{
		bufferName = jsonValue["bufferName"].GetString();
	}
	else
	{

	}

	if (jsonValue.HasMember("bufferSize"))
	{
		bufferSize = static_cast<uint32_t>(jsonValue["bufferSize"].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember("buffer"))
	{
		buffer.clear();
		for (int i = 0; i < bufferSize; i++)
		{
			GlobalVariableElement element;
			element.ReadJsonObject(jsonValue);
			buffer.push_back(element);
		}
	}
	else
	{

	}

	if (jsonValue.HasMember("registerSpace"))
	{
		registerSpace = static_cast<uint8_t>(jsonValue["registerSpace"].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember("regNum"))
	{
		regNum = static_cast<uint8_t>(jsonValue["regNum"].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember("set"))
	{
		set = static_cast<uint8_t>(jsonValue["set"].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember("binding"))
	{
		binding = static_cast<uint8_t>(jsonValue["binding"].GetUint());
	}
	else
	{

	}



}