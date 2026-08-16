#include "ShaderInfo.h"

/*
 현재 개별 구조체 read 부분에 jsonValue 맴버 존재 여부 예외 케이스 assert 안해둔 상태임
 MJassert 개발 되는데로 바로 적용해줘야함! (적용 후 글 삭제 필수!)
*/


void ShaderInfo::WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();

	writer.Key(KEY_ID);
	writer.String(m_id.c_str());

	writer.Key(KEY_STAGE);
	writer.Int(static_cast<int>(m_stage));


	writer.Key(KEY_INPUT_LAYOUT);
	writer.StartArray();
	for (const auto& oLayout : m_inputLayout)
	{
		oLayout.WriteJsonObject(writer);
	}
	writer.EndArray();

	writer.Key(KEY_OUTPUT_LAYOUT);
	writer.StartArray();
	for (const auto& oLayout : m_outputLayout)
	{
		oLayout.WriteJsonObject(writer);
	}
	writer.EndArray();

	writer.Key(KEY_CONSTANT_BUFFERS);
	writer.StartArray();
	for (const auto& cBuffer : m_constantBuffers)
	{
		cBuffer.WriteJsonObject(writer);
	}
	writer.EndArray();

	writer.Key(KEY_TEXTURES);
	writer.StartArray();
	for (const auto& texture : m_textures)
		texture.WriteJsonObject(writer);
	writer.EndArray();


	writer.Key(KEY_SAMPLERS);
	writer.StartArray();
	for (const auto& sampler : m_samplers)
		sampler.WriteJsonObject(writer);
	writer.EndArray();



	writer.Key(KEY_TARGET_PROFILE_VERSION);
	writer.Uint(static_cast<unsigned int>(m_targetProfileVersion));


	writer.Key(KEY_ENTRY_POINT);
	writer.String(m_entryPoint.c_str());

	writer.Key(KEY_SHADER_FILE_PATH);
	writer.String(m_shaderFilePath.c_str());

	writer.Key(KEY_SHADER_IR_FILE_PATH);
	writer.String(m_shaderIRFilePath.c_str());
	
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

	if (jsonValue.HasMember(KEY_ID))
	{
		if (jsonValue[KEY_ID].IsString())
		{
			m_id = jsonValue[KEY_ID].GetString();
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(id) type is not String | ShaderInfo.cpp\ReadRadpidJson");
		}
	}

	if (jsonValue.HasMember(KEY_STAGE))
	{
		if (jsonValue[KEY_STAGE].IsInt())
		{
			m_stage = static_cast<PipeLineStage>( jsonValue[KEY_STAGE].GetInt() );
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(shader stage) type is not Int | ShaderInfo.cpp\ReadRadpidJson");
		}
	}

	if (jsonValue.HasMember(KEY_INPUT_LAYOUT))
	{
		if (jsonValue[KEY_INPUT_LAYOUT].IsArray())
		{
			m_inputLayout.clear();
			for (const auto& iLayout : jsonValue[KEY_INPUT_LAYOUT].GetArray())
			{
				if (iLayout.IsObject())
				{
					ShaderIOLayoutElement tempLayout;
					tempLayout.ReadJsonObject(iLayout);
					m_inputLayout.push_back(tempLayout);
				}
				else
				{
					assert(false && "shader jsonValue read fail - jsonValue(shader InputLayout element) type is not Object| ShaderInfo.cpp\ReadRadpidJson");
				}
			}
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(shader InputLayout) type is not Array | ShaderInfo.cpp\ReadRadpidJson");
		}
	}

	if (jsonValue.HasMember(KEY_OUTPUT_LAYOUT))
	{
		if (jsonValue[KEY_OUTPUT_LAYOUT].IsArray())
		{
			m_outputLayout.clear();
			for (const auto& iLayout : jsonValue[KEY_OUTPUT_LAYOUT].GetArray())
			{
				if (iLayout.IsObject())
				{
					ShaderIOLayoutElement tempLayout;
					tempLayout.ReadJsonObject(iLayout);
					m_outputLayout.push_back(tempLayout);
				}
				else
				{
					assert(false && "shader jsonValue read fail - jsonValue(shader outputLayout element) type is not Object | ShaderInfo.cpp\ReadRadpidJson");
				}
			}
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(shader outputLayout) type is not Array | ShaderInfo.cpp\ReadRadpidJson");
		}
	}

	if (jsonValue.HasMember(KEY_CONSTANT_BUFFERS))
	{
		if (jsonValue[KEY_CONSTANT_BUFFERS].IsArray())
		{
			m_constantBuffers.clear();
			for (const auto& iLayout : jsonValue[KEY_CONSTANT_BUFFERS].GetArray())
			{
				if (iLayout.IsObject())
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

	if (jsonValue.HasMember(KEY_TEXTURES))
	{
		if (jsonValue[KEY_TEXTURES].IsArray())
		{
			m_textures.clear();
			for (const auto& iLayout : jsonValue[KEY_TEXTURES].GetArray())
			{
				if (iLayout.IsObject())
				{
					ResourceBindingElement tempResourceBindingElement;
					tempResourceBindingElement.ReadJsonObject(iLayout);
					m_textures.push_back(tempResourceBindingElement);
				}
				else
				{
					assert(false && "shader jsonValue read fail - jsonValue(shader textureSamplers element) type is not Object | ShaderInfo.cpp\ReadRadpidJson");
				}
			}
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(shader textureSamplers) type is not Array | ShaderInfo.cpp\ReadRadpidJson");
		}
	}

	if (jsonValue.HasMember(KEY_SAMPLERS))
	{
		if (jsonValue[KEY_SAMPLERS].IsArray())
		{
			m_samplers.clear();
			for (const auto& iLayout : jsonValue[KEY_SAMPLERS].GetArray())
			{
				if (iLayout.IsObject())
				{
					ResourceBindingElement tempResourceBindingElement;
					tempResourceBindingElement.ReadJsonObject(iLayout);
					m_samplers.push_back(tempResourceBindingElement);
				}
				else
				{
					assert(false && "shader jsonValue read fail - jsonValue(shader textureSamplers element) type is not Object | ShaderInfo.cpp\ReadRadpidJson");
				}
			}
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(shader textureSamplers) type is not Array | ShaderInfo.cpp\ReadRadpidJson");
		}
	}

	


	if (jsonValue.HasMember(KEY_TARGET_PROFILE_VERSION))
	{		
		m_targetProfileVersion =static_cast<ShaderProfileVersion>(jsonValue[KEY_TARGET_PROFILE_VERSION].GetUint());
	}


	if (jsonValue.HasMember(KEY_ENTRY_POINT))
	{
		if (jsonValue[KEY_ENTRY_POINT].IsString())
		{
			m_entryPoint = jsonValue[KEY_ENTRY_POINT].GetString();
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(entryPoint) type is not String | ShaderInfo.cpp\ReadRadpidJson");
		}
	}

	if (jsonValue.HasMember(KEY_SHADER_FILE_PATH))
	{
		if (jsonValue[KEY_SHADER_FILE_PATH].IsString())
		{
			m_shaderFilePath = jsonValue[KEY_SHADER_FILE_PATH].GetString();
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(shaderFilePath) type is not String | ShaderInfo.cpp\ReadRadpidJson");
		}
	}

	if (jsonValue.HasMember(KEY_SHADER_IR_FILE_PATH))
	{
		if (jsonValue[KEY_SHADER_IR_FILE_PATH].IsString())
		{
			m_shaderIRFilePath = jsonValue[KEY_SHADER_IR_FILE_PATH].GetString();
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(shaderIRFilePath) type is not String | ShaderInfo.cpp\ReadRadpidJson");
		}
	}



}

// 개별 구조체 JSON 입력 출력 선언부
void ShaderIOLayoutElement::WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	
	writer.Key(KEY_FORMAT);
	writer.Uint(static_cast<unsigned int>(format));
	
	writer.Key(KEY_NAME);
	writer.String(name.c_str());

	writer.Key(KEY_SYSTEM_VALUE);
	writer.Bool(systemValue);

	writer.Key(KEY_LOCATION);
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

	if (jsonValue.HasMember(KEY_FORMAT))
	{
		format = static_cast<FORMAT>(jsonValue[KEY_FORMAT].GetUint());
	}
	else
	{
		//향후 assert 개발시 추가 필요... 지금 string 직접 입력 방식 너무 노가다에 중구난방임
	}

	if (jsonValue.HasMember(KEY_NAME))
	{
		name = jsonValue[KEY_NAME].GetString();
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_SYSTEM_VALUE))
	{
		systemValue = jsonValue[KEY_SYSTEM_VALUE].GetBool();
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_SEMANTIC_INDEX))
	{
		semanticIndex = jsonValue[KEY_SEMANTIC_INDEX].GetUint();
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_LOCATION))
	{
		location = jsonValue[KEY_LOCATION].GetUint();
	}
	else
	{

	}



}

void GlobalVariableElement::WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();
	
	writer.Key(KEY_VARIABLE_NAME);
	writer.String(variableName.c_str());

	writer.Key(KEY_FORMAT);
	writer.Uint(static_cast<unsigned int>(format));

	writer.Key(KEY_OFFSET);
	writer.Uint(static_cast<unsigned int>(offset));

	writer.Key(KEY_SIZE);
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

	if (jsonValue.HasMember(KEY_VARIABLE_NAME))
	{
		variableName = jsonValue[KEY_VARIABLE_NAME].GetString();
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_FORMAT))
	{
		format = static_cast<FORMAT>(jsonValue[KEY_FORMAT].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_OFFSET))
	{
		offset = static_cast<uint32_t>(jsonValue[KEY_OFFSET].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_SIZE))
	{
		size = static_cast<uint32_t>(jsonValue[KEY_SIZE].GetUint());
	}
	else
	{

	}
}

void GlobalVariableBuffer::WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();

	writer.Key(KEY_BUFFER_NAME);
	writer.String(bufferName.c_str());

	writer.Key(KEY_BUFFER_SIZE);
	writer.Uint(static_cast<unsigned int>(buffer.size()));

	writer.Key(KEY_BUFFER);
	writer.StartArray();
	for (const auto& element : buffer)
	{
		element.WriteJsonObject(writer);
	}
	writer.EndArray();

	writer.Key(KEY_REG_SPACE);
	writer.Uint(static_cast<unsigned int>(registerSpace));

	writer.Key(KEY_REG_NUM);
	writer.Uint(static_cast<unsigned int>(regNum));


	writer.Key(KEY_SET_NUM);
	writer.Uint(static_cast<unsigned int>(set));

	writer.Key(KEY_BINDING);
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

	if (jsonValue.HasMember(KEY_BUFFER_NAME))
	{
		bufferName = jsonValue[KEY_BUFFER_NAME].GetString();
	}
	else
	{

	}


	if (jsonValue.HasMember(KEY_BUFFER) && jsonValue[KEY_BUFFER].IsArray())
	{
		buffer.clear();
		const auto& arr = jsonValue[KEY_BUFFER].GetArray();
		bufferSize = arr.Size();
		for (int i = 0; i < bufferSize; i++)
		{
			GlobalVariableElement element;
			element.ReadJsonObject(arr[i]);
			buffer.push_back(element);
		}
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_REG_SPACE))
	{
		registerSpace = static_cast<uint8_t>(jsonValue[KEY_REG_SPACE].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_REG_NUM))
	{
		regNum = static_cast<uint8_t>(jsonValue[KEY_REG_NUM].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_SET_NUM))
	{
		set = static_cast<uint8_t>(jsonValue[KEY_SET_NUM].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_BINDING))
	{
		binding = static_cast<uint8_t>(jsonValue[KEY_BINDING].GetUint());
	}
	else
	{

	}



}

void ResourceBindingElement::WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();

	writer.Key(KEY_RESOURCE_NAME);
	writer.String(resourceName.c_str());

	writer.Key(KEY_REG_SPACE);
	writer.Uint(static_cast<unsigned int>(registerSpace));

	writer.Key(KEY_REG_NUM);
	writer.Uint(static_cast<unsigned int>(regNum));


	writer.Key(KEY_SET_NUM);
	writer.Uint(static_cast<unsigned int>(set));

	writer.Key(KEY_BINDING);
	writer.Uint(static_cast<unsigned int>(binding));

	writer.EndObject();
}

void ResourceBindingElement::ReadJsonObject(const rapidjson::Value& jsonValue)
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

	if (jsonValue.HasMember(KEY_RESOURCE_NAME))
	{
		resourceName = jsonValue[KEY_RESOURCE_NAME].GetString();
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_REG_SPACE))
	{
		registerSpace = static_cast<uint8_t>(jsonValue[KEY_REG_SPACE].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_REG_NUM))
	{
		regNum = static_cast<uint8_t>(jsonValue[KEY_REG_NUM].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_SET_NUM))
	{
		set = static_cast<uint8_t>(jsonValue[KEY_SET_NUM].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_BINDING))
	{
		binding = static_cast<uint8_t>(jsonValue[KEY_BINDING].GetUint());
	}
	else
	{

	}



}