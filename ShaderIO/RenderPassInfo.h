#ifndef RENDER_PASS_INFO_H
#define RENDER_PASS_INFO_H


/*
해당 쉐이더 구성 툴에서 랜더 패스 순서까지 지정하여
함께 엔진에 제공해주어야함(엔진에서 전부 개별 쉐이더 받아서 패스조합하는 건 어려움)

패스 정보에는 기본적으로 패스 식별을 위한 이름과(+ 식별자가 따로 필요 할듯)
랜더 순서에 맞춘 ShaderInfo 벡터 필요( 개별 스테이지의 정보는 ShaderInfo 에서 제공해줌 ,세부적인 정보도)
eastl::string targetProfileVersion; 는 여기서도 관리(사실상 디버깅 용?)
패스 검증 기능은 일단 개발자에게 맡기는 방향으로 개발 이후에 차후 개발 필요..
*/
#include "ShaderInfo.h"
#include <EASTL/array.h>


/*
	semanticIndex 의 경우
	동일 semanticName 을 여러번 재사용을 위함(동일 semanticName 의 인덱스 부여 통한 식별)
	즉 텍스처의 경우 하나의 정점 배치내에서도 여러 집합으로 사용함 => TEXCOORD0 , TEXCOORD1 ..

*/
struct InputLayoutElement
{
	eastl::string name;
	FORMAT format;
	uint32_t inputSlot = 0;
	uint32_t alignedByteOffset;

	//HLSL 전용 필드
	uint32_t semanticIndex = 0;
	//GLSL 전용 필드
	uint32_t location = 0;

	void WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadJsonObject(const rapidjson::Value& jsonValue);

	static constexpr const char* KEY_NAME = "name";
	static constexpr const char* KEY_FORMAT = "format";
	static constexpr const char* KEY_INPUT_SLOT = "inputSlot";
	static constexpr const char* KEY_ALIGNED_BYTE_OFFSET = "alignedByteOffset";
	static constexpr const char* KEY_SEMANTIC_INDEX = "semanticIndex";
	static constexpr const char* KEY_LOCATION = "location";
};

struct PassInputLayoutContext
{
	PRIMITIVE_TOPOLOGY primitiveTopology;
	eastl::vector<InputLayoutElement> inputLayout;

	void WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadJsonObject(const rapidjson::Value& jsonValue);

	static constexpr const char* KEY_PRIMITIVE = "primitive";
	static constexpr const char* KEY_INPUT_LAYOUT = "inputLayout";
};

struct PassElement
{
	eastl::string shaderId;
	PipeLineStage stage;
	bool isValid;
	
	void WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadJsonObject(const rapidjson::Value& jsonValue);

	static constexpr const char* KEY_SHADER_ID = "shaderId";
	static constexpr const char* KEY_STAGE = "stage";
	static constexpr const char* KEY_IS_VALID = "isValid";
};

struct PassCheckResult
{
	bool errorFlag = 0; // 에러면 true 아니면 false
	eastl::vector<PipeLineStage> errorPipe;
	eastl::string errorLog;
};


class RenderPassInfo
{

public:
	//set에서 IsVaild 상태 true로 설정 Set과정 ShaderInfo의 pipeline stage 정보 일치 확인 필요
	void SetShader(PipeLineStage stage, const ShaderInfo& shader);

	//delete 과정에서는 IsVaild만 false 처리
	void DeleteShader(PipeLineStage stage);

	//get 통해서 id 정보 받아 조회 , IsVail 통과시 값 넣고 true 못하면 값 안넣어주고 false 반환
	bool GetVertexShaderId(PipeLineStage stage, eastl::string& id)const;

	//정말 간단한 패스 검증 기능(일단은 vertex pixel 이 존재 하는지 , Hull 과 Domain 이 함께 있는지 , Layout 규격정보 정도만 검사)
	PassCheckResult RenderPassCheck() const;

	//해당 렌더 페스의 IA 포맷 및 프리미티브 정보 입력 
	void SetPassInputLayoutInfo(PassInputLayoutContext passInputLayoutInfo);

	// IA 에 필요한 정보 제공 IA 포맷 , 프리미티브 , 정점 입력 레이아웃 정보 매개변수 방식 전달
	void GetPassInputLayoutInfo(PassInputLayoutContext& outIAPrimitiveAndFormat ) const;

	void WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadJsonObject(const rapidjson::Value& jsonValue);

	static constexpr const char* KEY_RENDER_PASS_ID = "renderPassId";
	static constexpr const char* KEY_MATERIAL_TAG = "materialTag";
	static constexpr const char* KEY_PASS_INPUT_CONTEXT = "passInputContext";
	static constexpr const char* KEY_PIPELINES = "pipeLines";

	eastl::string renderPassId;
	eastl::string materialTag;
private:

	PassInputLayoutContext m_passInputContext;
	eastl::array<PassElement, PipeLineStageCnt()> m_pipeLines;
};

#endif // !RENDERPASSINFO_H
