#include "ShaderInfo.h"

void ShaderInfo::WriteRapidJson(rapidjson::Writer<rapidjson::StringBuffer>& writer)
const
{
	writer.StartObject();

	writer.Key("id");
	writer.String(m_id.c_str());

	writer.Key("stage");
	writer.Int(static_cast<int>(m_stage));


	writer.Key("inputLayout");
	writer.StartArray();
	for (const auto& iLayout : m_inputLayout)
		writer.String(iLayout.c_str());
	writer.EndArray();

	writer.Key("outputLayout");
	writer.StartArray();
	for (const auto& oLayout : m_outputLayout)
		writer.String(oLayout.c_str());
	writer.EndArray();

	writer.Key("constantBuffer");
	writer.StartArray();
	for (const auto& cBuffer : m_constantBuffers)
		writer.String(cBuffer.c_str());
	writer.EndArray();

	writer.Key("textureSamplers");
	writer.StartArray();
	for (const auto& texSampler : m_textureSamplers)
		writer.String(texSampler.c_str());
	writer.EndArray();



	writer.Key("targetProfileVersion");
	writer.String(m_targetProfileVersion.c_str());

	writer.Key("entryPoint");
	writer.String(m_entryPoint.c_str());

	writer.Key("shaderFilePath");
	writer.String(m_shaderFilePath.c_str());

	writer.Key("shaderHeader");
	writer.String(m_shaderHeader.c_str());
	
	writer.EndObject();
}

void ShaderInfo::ReadRapidJson(const rapidjson::Value& jsonValue)
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
					m_inputLayout.push_back(iLayout.GetString());
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
					m_outputLayout.push_back(iLayout.GetString());
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
					m_constantBuffers.push_back(iLayout.GetString());
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
			m_targetProfileVersion = jsonValue["targetProfileVersion"].GetString();
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
			m_id = jsonValue["shaderHeader"].GetString();
		}
		else
		{
			assert(false && "shader jsonValue read fail - jsonValue(shaderHeader) type is not String | ShaderInfo.cpp\ReadRadpidJson");
		}
	}



}