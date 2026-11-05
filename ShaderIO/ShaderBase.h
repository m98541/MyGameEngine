#ifndef SHADER_BASE_H
#define SHADER_BASE_H

/*	IA FORMAT 규격
	* 다음은 엔진에서 사용될 범용 포맷으로 이후
	* 엔진 내부에 각각의 그래픽 모듈에서 변환 장치 필요함
*/
#include <EASTL/string.h>
#include "../StringFormat/EASTLStringFormat.h"

enum class PRIMITIVE_TOPOLOGY : uint8_t
{

	UNDEFINED = 0xFF,

	POINT_LIST = 0,
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

enum class VARIABLE_CLASS : uint8_t
{
	SCALAR = 0,
	VECTOR,
	MATRIX_ROWS,
	MATRIX_COLUMNS,
	OBJECT,
	STRUCT,
	INTERFACE_CLASS,
	INTERFACE_POINTER
};

enum class FORMAT : uint8_t
{
	// object , struct
	VOID0 = 0,

	//scalar , vector , matrix
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

enum class PipeLineStage
{
	// HLSL 기준으로 작성 GLSL 에서는  
	// 아래의 주석 기준으로 엔진 GLSL 쉐이더 적용부에서 헬퍼함수로 만들어 주어야 함
	Unknown = 0,
	Vertex,
	Hull, // GLSL 기준 Tessellation Control 
	Domain, // GLSL 기준 Tessellation Evaluation 
	Geometry, 
	Pixel ,// GLSL 기준 Fragment
COUNT};

inline size_t PipeLineStageCnt()
{
	return static_cast<size_t>(PipeLineStage::COUNT);
}

inline PipeLineStage& operator++(PipeLineStage& stage) {
	stage = static_cast<PipeLineStage>(static_cast<uint8_t>(stage) + 1);
	return stage;
}

inline eastl::string GetPipeLineStageString(PipeLineStage stage)
{
	static const eastl::string Vertex = "Vertex";
	static const eastl::string Hull = "Hull";
	static const eastl::string Domain = "Domain";
	static const eastl::string Geometry = "Geometry";
	static const eastl::string Pixel = "Pixel";
	static const eastl::string Unknown = "Unknown";

	switch (stage)
	{
	case PipeLineStage::Vertex:  return Vertex;
	case PipeLineStage::Hull:    return Hull; 
	case PipeLineStage::Domain:  return Domain;
	case PipeLineStage::Geometry:return Geometry;
	case PipeLineStage::Pixel:   return Pixel;
	default: return Unknown;
	}
}







/*
쉐이더로 부터 리플랙션을 통해 받은 정보를 직접적으로 바로 사용하고 
정적으로 유한개의 정해진 버전이 아닌 여러 버전을 유연하게 관리하기위해 
이전의 HLSL_5_0 으로 enum class 로 고정하여 사용 하던 구조에서 
struct 가 platform 정보 major 버전 정보 minor 버전 정보를 따로 관리하도록 하고 
이에 대한 지원함수를 통해 최종 입력 string 변환 을 기능 제공 필요함

현재는 스테이별로 있는 각각의 쉐이더별 관리 체계가 아닌 렌더 페스간 관리 체계임으로
stage 정보는 기존 방식으로 따로 관리
*/
enum class ShaderPlatform: uint8_t
{
	Unknown = 0,
	HLSL,
COUNT};

inline eastl::string GetShaderPlatformTag(ShaderPlatform platform)
{
	eastl::string platformTag;
	switch (platform)
	{
	case ShaderPlatform::HLSL:
		platformTag = "HLSL";
		break;
	case ShaderPlatform::Unknown:
	default:
		platformTag = "Unknown";
		break;
	}

	return platformTag.c_str();
}

struct ShaderProfileVersion
{
	ShaderPlatform shaderPlatform;
	uint8_t majorVer;
	uint8_t minorVer;
};



inline eastl::string GetHLSLShaderProfileVersionString(ShaderProfileVersion version)
{
	return EASTLsprintf32("%s_%d_%d", GetShaderPlatformTag(version.shaderPlatform).c_str(), version.majorVer, version.minorVer);
}


inline eastl::string GetHLSLTargetString(PipeLineStage stage, ShaderProfileVersion version)
{
	
	switch (stage)
	{
	case PipeLineStage::Vertex:   return EASTLsprintf32("vs_%d_%d", version.majorVer, version.minorVer);
	case PipeLineStage::Hull:	  return EASTLsprintf32("hs_%d_%d", version.majorVer, version.minorVer);
	case PipeLineStage::Domain:   return EASTLsprintf32("ds_%d_%d", version.majorVer, version.minorVer);
	case PipeLineStage::Geometry: return EASTLsprintf32("gs_%d_%d", version.majorVer, version.minorVer);
	case PipeLineStage::Pixel:	  return EASTLsprintf32("ps_%d_%d", version.majorVer, version.minorVer);
	default: return "UnknownStage_";// error
	}

	

	
};








#endif // !SHADER_BASE_H
