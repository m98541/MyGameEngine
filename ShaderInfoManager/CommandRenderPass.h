#ifndef COMMAND_RENDER_PASS_H
#define COMMAND_RENDER_PASS_H

#include "CommandShader.h"

#define RENDERPASS_SHADER_MAX 5

class CommandRenderPass
{
public:
	CommandRenderPass();
	CommandRenderPass(eastl::string passName);
	
	// 만일 해당 스테이지 쉐이더가 이미 존재시 기존 쉐이더 반환
	// 없으면 null 반환 
	// 쉐이더들은 테이블 에서 관리됨으로 주소를 전달하고 저장하는 방식으로
	// 실제 원본 데이터들은 아이디로 관리됨 
	// 이거는 커멘드를 위한 타입으로 해당 프로그램 내에서만 유효한 주소체계만을 이용
	// 실제 데이터에 id 부여 및 최종 저장은 이곳 책임이 아님
	// 그러면 SetRenderPassShader에 무조건 테이블 내 데이터만을 넣는게 보장되어야함
	// Command 매니저가 필요 SetRenderPassShader 유저 층에 직접 노출이 아닌 Command 매니가 사용해야함
	CommandShader* SetRenderPassShader(CommandShader shader);
	CommandShader* GetRenderPassShader(PipeLineStage stage);
	eastl::string GetRenderPassName();
	bool IsValidStage(PipeLineStage stage);

private:
	eastl::string m_passName;
	CommandShader m_renderPass[static_cast<uint8_t>(PipeLineStage::COUNT)];
	bool m_validList[static_cast<uint8_t>(PipeLineStage::COUNT)] = { false , false , false ,false , false };
};

#endif // !COMMAND_RENDER_PASS_H
