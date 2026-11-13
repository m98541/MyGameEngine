 #ifndef MATERIAL_TAG_GROUP_IO_H
#define MATERIAL_TAG_GROUP_IO_H

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include "MaterialTag.h"

struct MaterialTagIO : MaterialTag
{
	void ReadInfo(const rapidjson::Value& jsonValue);
	void WriteInfo(rapidjson::Writer<rapidjson::StringBuffer>& writer);

	static constexpr const char* KEY_RENDER_PASS_ID = "renderPassId";
	static constexpr const char* KEY_MATERIAL_TAG = "materialTag";

	
	MaterialTagIO& operator=(const MaterialTag& other)
	{
		MaterialTag::operator=(other);
		return *this;
	}
};

struct MaterialTagGroupIO : MaterialTagGroup
{
	void ReadInfo(const rapidjson::Value& jsonValue);
	void WriteInfo(rapidjson::Writer<rapidjson::StringBuffer>& writer);

	static constexpr const char* KEY_TAG_GROUP_ID = "tagGroupID";
	static constexpr const char* KEY_TAG_GROUP = "tagGroup";

	MaterialTagGroupIO& operator=(const MaterialTagGroup& other)
	{
		MaterialTagGroup::operator=(other);
		return *this;
	}
};

struct MaterialTagGroupTableIO : MaterialTagGroupTable
{
	void ReadInfo(const char* jsonFilePath);
	void WriteInfo(const char* jsonFilePath);

	static constexpr const char* KEY_TAG_GROUP_TABLE = "tagGroupTable";
	
	MaterialTagGroupTableIO& operator=(const MaterialTagGroupTable& other)
	{
		MaterialTagGroupTable::operator=(other);
		return *this;
	}

};

#endif // !MATERIAL_TAG_GROUP_IO_H
