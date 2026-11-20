#ifndef MATERIAL_TAG_H
#define MATERIAL_TAH_H
#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <EASTL/unordered_map.h>

/*
material Tag의 경우 
향후 엔진에서 모델 로드과정에서 매쉬정보에 삽입 필요함....
아직 모델 툴이 없는 관계로 모델 자체 삽입은 어렵고 (모델 툴 개발시 엔진 프로젝트가 산으로 감...)
일단 초기 과정에서는 모델 전체 메쉬의 테그를 통일해주는 방식으로
로더 호출과정에서 태그를 유저가 입력해주는 방식으로 제작 필요

+26-11-13 : 이제 renderPass 구조에서는 Material Tag 정보를 가지지 않고
이를 관리하는 주체가 Material Table 을 참조하여 renderPass 와 Material Tag 정보를 연결하여
해당 Material Tag 와 매쉬간 정보를 통해 매쉬 드로우시 어떤 renderPass 를 사용할지 결정해야함

기존구조에서는 renderPass에  Material Tag정보를 담아두고 있어 렌더패스를 유동적으로 관리하기 어렵고
테이블에서도 renderPass 와 material 를 복수적으로 관리하다 보니 무결성이 깨질 위험이 존재
*/

struct MaterialTag
{
	eastl::string materialTag; //materialTag 식별 문자열
	eastl::string renderPassId; // 참조 renderPass
};

//랜더패스간 순서가 엇갈리면 안되는 의존성을 가지는 패스간
//그룹으로 묶어서 인덱스로 드로우콜 순서 지키게 함 
//순서가 필요없는 경우 단일 랜더패스로 그룹에 1개 랜더패스 0번인덱스로만 지정
//단일 패스의 경우에는 그룹ID == 랜더패스ID 
struct MaterialTagGroup
{
	eastl::string tagGroupId;
	eastl::vector<MaterialTag> tagGroup;
	void RegisterMaterialTag(eastl::string materialTag , eastl::string renderPassId);
};

struct MaterialTagGroupTable
{
	eastl::unordered_map<eastl::string, MaterialTagGroup> tagGroupTable;

	void RegisterMaterialGroupTable(MaterialTagGroup* inMaterialTagGroup);
	MaterialTagGroup GetMaterialGroupFromTable(eastl::string groupId);
};



#endif // !MATERIAL_TAG_H
