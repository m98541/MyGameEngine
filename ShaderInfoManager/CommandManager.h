#ifndef COMMAND_MANGER_H
#define COMMAND_MANGER_H
#include "CommandRenderPass.h"
#include "CommandShader.h"
#include "ShaderCompiler.h"

#include <EASTL/vector.h>
#include <EASTL/unordered_map.h>
#include <EASTL/string.h>


struct CommandShaderElement
{
	CommandShader shader;
	uint8_t refCnt = 0;
};

class CommandManager
{
public:
	CommandManager();
	~CommandManager();

	/*
		RenderPass 파일 내부에 ShaderTable 파일 경로 담음
		파일 로드 과정에서 기존 쉐이더 테이블 과의 병합 필요 => 해당 기능은 주로 엔진 쪽에서 사용할 예정
		이때문에 에티터 에서는 현재의 상태초기화 위한 clear 명령 필요
	*/
	bool RenderPassFileLoad(eastl::string renderPassFilePath);
	bool RenderPassFileCompileAndSave(eastl::string filePath, eastl::string passFileName, eastl::string 
		,ShaderCompiler::API useAPI);

	//bool registerShader(CommandShader& shader);
	bool RegisterRenderPass( // invalid 쉐이더의 경우 nullptr 전달 사용자가 의식적으로 전달해야함
		CommandRenderPass renderPass
	);

	void DeleteRenderPass(eastl::string passName);
	// 스테이지 별 등록된 쉐이더 이름 벡터 반환
	eastl::vector<eastl::string> GetStageShaderNamesTable(PipeLineStage stage);
	// 저장된 랜더 패스이름 벡터 반환
	eastl::vector<eastl::string> GetRenderPassNames();

private:
	// 다음의 경우 render Pass 등록을 위한 내부 호출용
	// 테이블에 저장된 쉐이더의 주소를 등록과 즉시 반환
	void RegisterShaderInTable(CommandShader& shader ,CommandShader** outTableShaderAddr);
	// 테이블 내 쉐이더의 삭제 명령: 즉시 삭제가 아닌 refCount 를 1씩 줄이다 1이면 삭제함
	void DeleteShaderInTable(CommandShader& shader);
	// 쉐이더 명령의 경우
	// 각 계층별 map의 형태로 가지고 있어야함
	// 쉐이더 경로값을 키로 중복성 체크
	// 중복된 쉐이더 포함된 패스 정보 입력시 참조 카운터 추가
	// delete 시 카운트 1감소 1이면 바로 삭제 필요
	eastl::unordered_map<eastl::string ,CommandShaderElement> m_shaderTable[static_cast<size_t>(PipeLineStage::COUNT)];
	// 해당 renderPassTable의 CommandRenderPass은 
	// shaderTable에 존재하는 CommandShader로만 구현되어야함
	eastl::unordered_map<eastl::string ,CommandRenderPass> m_renderPassTable;

};

#endif // !COMMAND_MANGER_H 
