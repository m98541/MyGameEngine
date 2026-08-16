#include "RenderPassInfo.h"

void RenderPassInfo::SetShader(PipeLineStage stage, const ShaderInfo& shader)
{
	if (stage == shader.m_stage)
	{
		m_pipeLines[static_cast<int>(stage)].isValid = true;
		m_pipeLines[static_cast<int>(stage)].shaderId = shader.m_id;
		m_pipeLines[static_cast<int>(stage)].stage = stage;
	}
	else
	{
		//MJAssert 로 정의 후 개발
	}
}

void RenderPassInfo::DeleteShader(PipeLineStage stage)
{
	m_pipeLines[static_cast<int>(stage)].isValid = false;
}

bool RenderPassInfo::GetVertexShaderId(PipeLineStage stage, std::string& id) const
{
	if (m_pipeLines[static_cast<int>(stage)].isValid)
	{
		id = m_pipeLines[static_cast<int>(stage)].shaderId;
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



//입출력 버퍼 인터페이스

void InputLayoutElement::WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();

	writer.Key(KEY_NAME);
	writer.String(name.c_str());

	writer.Key(KEY_FORMAT);
	writer.Uint(static_cast<unsigned int>(format));

	writer.Key(KEY_INPUT_SLOT);
	writer.Uint(static_cast<unsigned int>(inputSlot));

	writer.Key(KEY_ALIGNED_BYTE_OFFSET);
	writer.Uint(static_cast<unsigned int>(alignedByteOffset));

	writer.Key(KEY_SEMANTIC_INDEX);
	writer.Uint(static_cast<unsigned int>(semanticIndex));

	writer.Key(KEY_LOCATION);
	writer.Uint(static_cast<unsigned int>(location));

	writer.EndObject();
}

void InputLayoutElement::ReadJsonObject(const rapidjson::Value& jsonValue)
{
	if (!jsonValue.IsObject())
	{
		if (jsonValue.IsArray())
			assert(false && "InputLayoutElement jsonValue read fail - jsonValue type: Array! (Object type required) | RenderPassInfo.cpp\InputLayoutElement");
		else if (jsonValue.IsNull())
			assert(false && "InputLayoutElement jsonValue read fail - jsonValue is Null! | RenderPassInfo.cpp\InputLayoutElement");
		else
			assert(false && "InputLayoutElement jsonValue read fail - jsonValue type is not Object (Object type required) | RenderPassInfo.cpp\InputLayoutElement");
	}


	if (jsonValue.HasMember(KEY_NAME))
	{
		name = jsonValue[KEY_NAME].GetString();
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

	if (jsonValue.HasMember(KEY_INPUT_SLOT))
	{
		inputSlot = static_cast<uint32_t>(jsonValue[KEY_INPUT_SLOT].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_ALIGNED_BYTE_OFFSET))
	{
		alignedByteOffset = static_cast<uint32_t>(jsonValue[KEY_ALIGNED_BYTE_OFFSET].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_SEMANTIC_INDEX))
	{
		semanticIndex = static_cast<uint32_t>(jsonValue[KEY_SEMANTIC_INDEX].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_LOCATION))
	{
		location = static_cast<uint32_t>(jsonValue[KEY_LOCATION].GetUint());
	}
	else
	{

	}
	

}


void PassInputLayoutContext::WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();

	writer.Key(KEY_PRIMITIVE);
	writer.Uint(static_cast<unsigned int>(primitiveTopology));

	writer.Key(KEY_INPUT_LAYOUT);
	writer.StartArray();
	for (InputLayoutElement element : inputLayout)
	{
		element.WriteJsonObject(writer);
	}
	writer.EndArray();

	writer.EndObject();
}

void PassInputLayoutContext::ReadJsonObject(const rapidjson::Value& jsonValue)
{
	if (!jsonValue.IsObject())
	{
		if (jsonValue.IsArray())
			assert(false && "PassInputLayoutContext jsonValue read fail - jsonValue type: Array! (Object type required) | RenderPassInfo.cpp\PassInputLayoutContext");
		else if (jsonValue.IsNull())
			assert(false && "PassInputLayoutContext jsonValue read fail - jsonValue is Null! | RenderPassInfo.cpp\PassInputLayoutContext");
		else
			assert(false && "PassInputLayoutContext jsonValue read fail - jsonValue type is not Object (Object type required) | RenderPassInfo.cpp\PassInputLayoutContext");
	}

	if (jsonValue.HasMember(KEY_PRIMITIVE))
	{
		primitiveTopology = static_cast<PRIMITIVE_TOPOLOGY>(jsonValue[KEY_PRIMITIVE].GetUint());
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_INPUT_LAYOUT))
	{
		inputLayout.clear();
		for (const auto& iLayoutElement : jsonValue[KEY_INPUT_LAYOUT].GetArray())
		{
			InputLayoutElement element;
			element.ReadJsonObject(iLayoutElement);
			inputLayout.push_back(element);
		}
	}
	else
	{
	}

}


void PassElement::WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();

	writer.Key(KEY_SHADER_ID);
	writer.String(shaderId.c_str());
	
	writer.Key(KEY_STAGE);
	writer.Uint(static_cast<unsigned int>(stage));

	writer.Key(KEY_IS_VALID);
	writer.Bool(isValid);

	writer.EndObject();
}

void PassElement::ReadJsonObject(const rapidjson::Value& jsonValue)
{
	if (!jsonValue.IsObject())
	{
		if (jsonValue.IsArray())
			assert(false && "PassElement jsonValue read fail - jsonValue type: Array! (Object type required) | RenderPassInfo.cpp\PassElement");
		else if (jsonValue.IsNull())
			assert(false && "PassElement jsonValue read fail - jsonValue is Null! | RenderPassInfo.cpp\PassElement");
		else
			assert(false && "PassElement jsonValue read fail - jsonValue type is not Object (Object type required) | RenderPassInfo.cpp\PassElement");
	}

	if (jsonValue.HasMember(KEY_SHADER_ID))
	{
		shaderId = jsonValue[KEY_SHADER_ID].GetString();
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_STAGE))
	{
		stage = static_cast<PipeLineStage>(jsonValue[KEY_STAGE].GetUint());
	}
	else
	{
	}


	if (jsonValue.HasMember(KEY_IS_VALID))
	{
		isValid = jsonValue[KEY_IS_VALID].GetBool();
	}
	else
	{

	}

}


void RenderPassInfo::WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
{
	writer.StartObject();

	writer.Key(KEY_RENDER_PASS_ID);
	writer.String(renderPassId.c_str());

	writer.Key(KEY_MATERIAL_TAG);
	writer.String(materialTag.c_str());

	writer.Key(KEY_PASS_INPUT_CONTEXT);
	m_passInputContext.WriteJsonObject(writer);

	writer.Key(KEY_PIPELINES);
	writer.StartArray();

	for (PassElement element : m_pipeLines)
	{
		element.WriteJsonObject(writer);
	}
	
	writer.EndArray();

	writer.EndObject();
}

void RenderPassInfo::ReadJsonObject(const rapidjson::Value& jsonValue)
{
	if (!jsonValue.IsObject())
	{
		if (jsonValue.IsArray())
			assert(false && "PassElement jsonValue read fail - jsonValue type: Array! (Object type required) | RenderPassInfo.cpp\RenderPassInfo");
		else if (jsonValue.IsNull())
			assert(false && "PassElement jsonValue read fail - jsonValue is Null! | RenderPassInfo.cpp\RenderPassInfo");
		else
			assert(false && "PassElement jsonValue read fail - jsonValue type is not Object (Object type required) | RenderPassInfo.cpp\RenderPassInfo");
	}

	if (jsonValue.HasMember(KEY_RENDER_PASS_ID))
	{
		renderPassId = jsonValue[KEY_RENDER_PASS_ID].GetString();
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_MATERIAL_TAG))
	{
		materialTag = jsonValue[KEY_MATERIAL_TAG].GetString();
	}
	else
	{

	}

	if (jsonValue.HasMember(KEY_PASS_INPUT_CONTEXT))
	{
		PassInputLayoutContext temp;
		temp.ReadJsonObject(jsonValue[KEY_PASS_INPUT_CONTEXT].GetObject());
		m_passInputContext = temp;
	}
	else
	{

	}
	
	if (jsonValue.HasMember(KEY_PIPELINES))
	{
		const auto& arr = jsonValue["pipeLines"].GetArray();
		for (int i = 0; i < arr.Size(); i++)
		{
			m_pipeLines[i].ReadJsonObject(arr[i]);
		}
	}
	else
	{

	}
}