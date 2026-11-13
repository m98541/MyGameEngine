#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include "MaterialTagIO.h"

#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <string>

#include "../StringFormat/StringFormatAssert.h"


void MaterialTagIO::ReadInfo(const rapidjson::Value& jsonValue)
{
	if (!jsonValue.IsObject())
	{
		SF_ASSERTE(!jsonValue.IsArray(), "InputLayoutElement jsonValue read fail - jsonValue type: Array! (Object type required) ");
		SF_ASSERTE(!jsonValue.IsNull(), "InputLayoutElement jsonValue read fail - jsonValue is Null! ");
		SF_ASSERTE(false, "InputLayoutElement jsonValue read fail -  jsonValue type is not Object (Object type required) ");
	}
	
	if (jsonValue.HasMember(KEY_MATERIAL_TAG))
	{
		materialTag = jsonValue[KEY_MATERIAL_TAG].GetString();
	}
	else
	{
		SF_ASSERTE(jsonValue.HasMember(KEY_MATERIAL_TAG), "not found KEY : %s ", KEY_MATERIAL_TAG);
	}

	if (jsonValue.HasMember(KEY_RENDER_PASS_ID))
	{
		renderPassId = jsonValue[KEY_RENDER_PASS_ID].GetString();
	}
	else
	{
		SF_ASSERTE(jsonValue.HasMember(KEY_RENDER_PASS_ID ) , "not found KEY : %s ", KEY_RENDER_PASS_ID);
	}

}

void MaterialTagIO::WriteInfo(rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();

	writer.Key(KEY_MATERIAL_TAG);
	writer.String(materialTag.c_str());

	writer.Key(KEY_RENDER_PASS_ID);
	writer.String(renderPassId.c_str());

	writer.EndObject();
}

void MaterialTagGroupIO::ReadInfo(const rapidjson::Value& jsonValue)
{

	if (!jsonValue.IsObject())
	{
		SF_ASSERTE( !jsonValue.IsArray() , "InputLayoutElement jsonValue read fail - jsonValue type: Array! (Object type required) ");
		SF_ASSERTE( !jsonValue.IsNull() , "InputLayoutElement jsonValue read fail - jsonValue is Null! ");
		SF_ASSERTE(false , "InputLayoutElement jsonValue read fail -  jsonValue type is not Object (Object type required) ");
	}

	if (jsonValue.HasMember(KEY_TAG_GROUP))
	{
		tagGroupId = jsonValue[KEY_TAG_GROUP_ID].GetString();
		tagGroup.clear();

		for (const auto& tagJsonElement : jsonValue[KEY_TAG_GROUP].GetArray())
		{
			MaterialTagIO tag;
			tag.ReadInfo(tagJsonElement);
			tagGroup.push_back(tag);
		}
	}
	else
	{
		SF_ASSERTE(jsonValue.HasMember(KEY_TAG_GROUP), "not found KEY : %s ", KEY_TAG_GROUP);
	}

}

void MaterialTagGroupIO::WriteInfo(rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
	writer.StartObject();

	writer.Key(KEY_TAG_GROUP_ID);
	writer.String(tagGroupId.c_str());

	writer.Key(KEY_TAG_GROUP);
	writer.StartArray();

	MaterialTagIO materialTagIO;
	for (MaterialTag materialTag : tagGroup)
	{
		
		materialTagIO = materialTag;
		materialTagIO.WriteInfo(writer);
	}

	writer.EndArray();

	writer.EndObject();
}

void MaterialTagGroupTableIO::ReadInfo(const char* jsonFilePath)
{
	std::ifstream file(jsonFilePath);
	std::string tempFileBuffer((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	rapidjson::StringStream jsonStringStream(tempFileBuffer.c_str());
	rapidjson::Document jsonDoc;

	jsonDoc.ParseStream(jsonStringStream);
	SF_ASSERTE(jsonDoc.HasParseError(), "json parsing assert %s ", jsonStringStream);

	const rapidjson::Value& jsonValue = jsonDoc;

	if (jsonValue.HasMember(KEY_TAG_GROUP_TABLE))
	{
		const auto& arr = jsonValue[KEY_TAG_GROUP_TABLE].GetArray();

		
		SF_ASSERTE(arr.Size() != 0, "read empty table ");
		
		for (int i = 0; i < arr.Size(); i++)
		{
			MaterialTagGroupIO groupIO;
			groupIO.ReadInfo(arr[i]);
			tagGroupTable[groupIO.tagGroupId] = groupIO;
		}
	}
	else
	{
		SF_ASSERTE(jsonValue.HasMember(KEY_TAG_GROUP_TABLE) , "not found KEY : %s ", KEY_TAG_GROUP_TABLE);
	}

	file.close();

}

void MaterialTagGroupTableIO::WriteInfo(const char* jsonFilePath)
{
	std::ofstream file(jsonFilePath);
	SF_ASSERTE(file.is_open() && "file open fail || file path : %s ", jsonFilePath);
	
	rapidjson::StringBuffer stringBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(stringBuffer);


	writer.StartObject();

	writer.Key(MaterialTagGroupIO::KEY_TAG_GROUP);

	writer.StartArray();
	MaterialTagGroupIO groupIO;
	for (const auto& group : tagGroupTable)
	{	
		groupIO = group.second;
		groupIO.WriteInfo(writer);
	}
	writer.EndArray();

	writer.EndObject();
	

}