#ifndef D3D11_SHADER_H
#define D3D11_SHADER_H
/*
	RendererAPI 의
		virtual void DrawIndexed(const RefPtr<Mesh>& mesh, uint32_t indexCount = 0) override;
		virtual void DrawLines(const RefPtr<Mesh>& mesh ,uint32_t vertexCount) override;
		virtual void SetLineWidth(float width) override;
	다음의 함수들이 구현되어질 수 있도록 기능을 지원해야함

	하나의 쉐이더 코드 내에서 분기 기능을 다르게 구현하는 것은 X
	기능별로 쉐이더를 각각 컴파일 하여 들고있다가 교체하는 방식으로 제작
		1. 여러 기능별 쉐이더 구성 과정
		2. 상위 함수에서 호출되어지는 기능에 따라 스위칭 되는 과정 
		구현 필요

	각각의 기능은 hlsli 메인 조합 쉐이더 파일은 hlsl 로 관리
	각각의 기능에서 
	Input 과 Output IA 모듈의 구성의 책임 가진다
	기능이 기존 엔트리 포인트에서 수행할 과정의 
	float4 Process(Input); 형식의 기능 동작으로 통일 시킨다.
	
	일단은 하나의 기능이 메인 코드에 대한 자체적인 완결성을 가지도록 한다(일단 이것 부터 성공시킨뒤 다른거 하자)
	(향후 목표?)이후에는 cpu 코드에서 컴파일 과정에 메인 코드를 작성하여 쉐이터 파일에 삽입해주는 방식으로 여러 기능을 동적 으로 조합한다.
	=> 일단 조합 쉐이더 hlsl 은 매우 정적인형태로 관리한다.

	위 구조를 위해서 
	
	기능 쉐이더 관리 구조 필요 해당 관리 기능은 
	해당 기능쉐이더에 대한 IA 정보 등 CPU 측의 정보와 기능 쉐이더 hlsli 의 컴파일에 필요 정보를 동시에 등록하도록 강제하고 이를 
	함께 관리시키는 것이 목적(만일 쉐이더 따로 쉐이더 정보 따로 관리할 시 정보 불일치 및 누락 문제 발생가능)
	관리의 경우 JSON 형식으로 필요 정보를 저장시키고 읽는 방식으로 관리한다
	(해더나 소스등에 직접기록하는 경우 개발자가 수기로 기록 하는 등 위험 요소 있으며 헤더나 소스 파일은 데이터 저장의 목적이 아님)
	이를 위해 hlsli 파일경로 와 IA 입력 명시 하여 ShaderManagerData JSON 파일 기록 및 삽입 삭제 기능의 외부 툴 필요
	ShaderManager 프로그램을 통해 ShaderInfoList.json 을 export함 
	ShaderManager 프로그램은 관리 프로그램으로 ShaderInfoList를 import 하여 세이더의 기록 삽입 삭제 기능 제공

	엔진에서는 ShaderInfoList.json을 import 하여 해당 정보 토대로 해당 파일내 모든 세이더들을 컴파일 시켜두고 이를 관리함
	이때 세이더 단위로 관리함
	즉 해당 구성은 쉐이더 교체간 유연성 및 외부 툴 단순화 위해서(낮은 복잡도로 구현이 먼저...) 일단 파이프전체의 쉐이더간 구성관리가 아닌 각각의 
	쉐이더를 관리시킴(예를 들어 한 파이프를 버텍 쉐이더 + 픽셀 쉐이더로 돌린다면 이를 각각 버텍 쉐이더 , 픽셀 쉐이더 로 따로 관리)
	단 import 이후 쉐이더 관리자는 파이프 구성시 쉐이더 간 input output 구성 검사정도는 필요 -> 맞지 않은경우 컴파일 에러(런타인 검은화면으로 에러 확인시 너무 크게 아쉬워짐)
	
	메인 쉐이더에 모든 헤더를
	#ifdef SYMBOL_1
	#include "shader.hlsli"
	#endif 
	를 전부 포함하는 방식으로는 어려움(할때마다 전체 포맷 유지하면서 메인 쉐이더를 수정해줘야하는 요구사항 필요해짐)
	header.hlsli를 만들어 
	JSON 집합 받으면 컴파일 전단계에서 요소를 읽어가면서(header.hlsli 는 무조건 컴퓨터가 작성해야함 사람이 수기 입력 절대 금지)
	각 요소에 대한 아래 항목을 이어 붙인 헤더 집합 생성 이후 메인 쉐이더는 header.hlsli 만을 include 하여 처리시킴 => 즉 각 JSON 요소는 SYMBOL 정보와 헤더 string 정보를 포함하고 있어야 함
	#ifdef SYMBOL_1
	#include "shader.hlsli"
	#endif
*/

#endif // !D3D11_SHADER_H