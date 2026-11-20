#include "MaterialTag.h"

void MaterialTagGroup::RegisterMaterialTag(eastl::string materialTag, eastl::string renderPassId)
{
	MaterialTag tag = { materialTag  , renderPassId };
	tagGroup.push_back(tag);
}

void MaterialTagGroupTable::RegisterMaterialGroupTable(MaterialTagGroup* inMaterialTagGroup)
{
	tagGroupTable[inMaterialTagGroup->tagGroupId] = *inMaterialTagGroup;
}

MaterialTagGroup MaterialTagGroupTable::GetMaterialGroupFromTable(eastl::string groupId)
{
	return tagGroupTable[groupId];
}