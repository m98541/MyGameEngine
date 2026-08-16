#ifndef SHADER_BASE_H
#define SHADER_BASE_H

/*	IA FORMAT 규격
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

enum class FORMAT : uint8_t
{
	// 32bit integer format
	R32_INT,
	R32G32B32_INT,
	R32_UINT,
	R32G32B32_UINT,
	
	// 32bit floating point format
	R32_FLOAT,
	R32G32_FLOAT,
	R32G32B32_FLOAT,
	R32G32B32A32_FLOAT,

	// 16bit half floating point & short
	R16_FLOAT,
	R16G16_FLOAT,
	R16G16B16A16_FLOAT,
	R16G16_UNORM,
	R16G16B16A16_UNORM,

	//8bit norm RGBA or BGRA
	R8G8B8A8_UNORM,

	//matrix 
	MATRIX4X4

};

constexpr size_t GRAPHICS_STAGE_COUNT = 6;

enum class PipeLineStage
{
	// HLSL 기준으로 작성 GLSL 에서는  
	// 아래의 주석 기준으로 엔진 GLSL 쉐이더 적용부에서 헬퍼함수로 만들어 주어야 함
	Vertex,
	Hull, // GLSL 기준 Tessellation Control 
	Domain, // GLSL 기준 Tessellation Evaluation 
	Geometry, 
	Pixel, // GLSL 기준 Fragment
	Compute
};

enum class ShaderProfileVersion :uint8_t
{
	//HLSL Ver
	//DX11
	HLSL_5_0,
	//DX12
	HLSL_5_1,
	HLSL_6_0,
	HLSL_6_5,
	HLSL_6_6,

	//GLSL Ver
	GLSL_330,
	GLSL_430,//Compute Shader 도입
	GLSL_450,
	GLSL_460

};


/* 해당 함수는 이후 엔진 내부 쉐이더 Dx 종속 영역 Shader 적옹 부분에서 동작 시킴
// HLSL 타깃 프로필 문자열 생성 (e.g. vs_5_0, ps_6_0)
inline std::string GetHLSLTargetString(PipeLineStage stage, ShaderProfileVersion version)
{
	std::string stagePrefix;
	switch (stage)
	{
	case PipeLineStage::Vertex:   stagePrefix = "vs_"; break;
	case PipeLineStage::Hull:     stagePrefix = "hs_"; break;
	case PipeLineStage::Domain:   stagePrefix = "ds_"; break;
	case PipeLineStage::Geometry: stagePrefix = "gs_"; break;
	case PipeLineStage::Pixel:    stagePrefix = "ps_"; break;
	case PipeLineStage::Compute:  stagePrefix = "cs_"; break;
	default: return "";
	}

	std::string versionStr;
	switch (version)
	{
	case ShaderProfileVersion::HLSL_5_0: versionStr = "5_0"; break;
	case ShaderProfileVersion::HLSL_5_1: versionStr = "5_1"; break;
	case ShaderProfileVersion::HLSL_6_0: versionStr = "6_0"; break;
	case ShaderProfileVersion::HLSL_6_5: versionStr = "6_5"; break;
	case ShaderProfileVersion::HLSL_6_6: versionStr = "6_6"; break;
	default: return "";
	}

	return stagePrefix + versionStr; // e.g. "ps_5_0"
};



*/



#endif // !SHADER_BASE_H
