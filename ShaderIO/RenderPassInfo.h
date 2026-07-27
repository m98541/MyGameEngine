/*
해당 쉐이더 구성 툴에서 랜더 패스 순서까지 지정하여 
함께 엔진에 제공해주어야함(엔진에서 전부 개별 쉐이더 받아서 패스조합하는 건 어려움)

패스 정보에는 기본적으로 패스 식별을 위한 이름과(+ 식별자가 따로 필요 할듯)
랜더 순서에 맞춘 ShaderInfo 벡터 필요( 개별 스테이지의 정보는 ShaderInfo 에서 제공해줌 ,세부적인 정보도)
std::string targetProfileVersion; 는 여기서도 관리(사실상 디버깅 용?)
패스 검증 기능은 일단 개발자에게 맡기는 방향으로 개발 이후에 차후 개발 필요..
*/
#include "ShaderInfo.h"
#include <array>


/*
	* 다음은 엔진에서 사용될 범용 포맷으로 이후
	* 엔진 내부에 각각의 그래픽 모듈에서 변환 장치 필요함
*/
enum class PRIMITIVE_TOPOLOGY : uint8_t
{

	
	POINT_LIST,
	LINE_LIST,
	LINE_STRIP,
	TRIANGLE_LIST,
	TRIANGLE_STRIP,

	PATCH_LIST_CONTROL_POINT_1,
	PATCH_LIST_CONTROL_POINT_2,
	PATCH_LIST_CONTROL_POINT_3,
	PATCH_LIST_CONTROL_POINT_4,
	PATCH_LIST_CONTROL_POINT_5,
	PATCH_LIST_CONTROL_POINT_6,
	PATCH_LIST_CONTROL_POINT_7,
	PATCH_LIST_CONTROL_POINT_8,
	PATCH_LIST_CONTROL_POINT_9,
	PATCH_LIST_CONTROL_POINT_10,
	PATCH_LIST_CONTROL_POINT_11,
	PATCH_LIST_CONTROL_POINT_12,
	PATCH_LIST_CONTROL_POINT_13,
	PATCH_LIST_CONTROL_POINT_14,
	PATCH_LIST_CONTROL_POINT_15,
	PATCH_LIST_CONTROL_POINT_16,
	PATCH_LIST_CONTROL_POINT_17,
	PATCH_LIST_CONTROL_POINT_18,
	PATCH_LIST_CONTROL_POINT_19,
	PATCH_LIST_CONTROL_POINT_20,
	PATCH_LIST_CONTROL_POINT_21,
	PATCH_LIST_CONTROL_POINT_22,
	PATCH_LIST_CONTROL_POINT_23,
	PATCH_LIST_CONTROL_POINT_24,
	PATCH_LIST_CONTROL_POINT_25,
	PATCH_LIST_CONTROL_POINT_26,
	PATCH_LIST_CONTROL_POINT_27,
	PATCH_LIST_CONTROL_POINT_28,
	PATCH_LIST_CONTROL_POINT_29,
	PATCH_LIST_CONTROL_POINT_30,
	PATCH_LIST_CONTROL_POINT_31,
	PATCH_LIST_CONTROL_POINT_32
};



struct PassIAContext
{
	PRIMITIVE_TOPOLOGY primitiveTopology;

};

struct PassElement
{
	std::string ShaderId;
	PipeLineStage stage;
	bool IsVaild; 
};

struct PassCheckResult
{
	bool errorFlag = 0; // 에러면 true 아니면 false
	std::vector<PipeLineStage> errorPipe;
	std::string Log;
};


class RenderPassInfo
{

public:
	//set에서 IsVaild 상태 true로 설정 Set과정 ShaderInfo의 pipeline stage 정보 일치 확인 필요
	void SetShader(PipeLineStage stage,const ShaderInfo& shader);

	//delete 과정에서는 IsVaild만 false 처리
	void DeleteShader(PipeLineStage stage);

	//get 통해서 id 정보 받아 조회 , IsVail 통과시 값 넣고 true 못하면 값 안넣어주고 false 반환
	bool GetVertexShaderId(PipeLineStage stage ,std::string& id)const;

	//정말 간단한 패스 검증 기능(일단은 vertex pixel 이 존재 하는지 , Hull 과 Domain 이 함께 있는지 , Layout 규격정보 정도만 검사)
	PassCheckResult RenderPassCheck() const;

private:
	std::array<PassElement, GRAPHICS_STAGE_COUNT> m_pipeLines;
};