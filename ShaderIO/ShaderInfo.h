#ifndef SHADER_INFO_H
#define SHADER_INFO_H

#include <EASTL/string.h>
#include <EASTL/vector.h>

#include <rapidjson/document.h>

#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include "ShaderBase.h"
/*
	ShaderInfo 와 ShaderIO 는 이후 툴 내부 라이브러리가 아닌
	공용라이브러리로 따로 빼서 툴과 엔진에서 공통적으로 사용될 예정
	(데이터 규격 및 IO 통일)

*/
// Shader Info 데이터의 기본적인 구조 형식 정의
/*
* 일단은 아래의 구조로 제작함
* id
* Shader INPUT Layout 구조 정보
* Shader OUTPUT Layout구조 정보
* Shader File Path
* Shader File Header , 해당 쉐이더 호출용 헤더 필요
*/

// 입력층에서의 ShaderIOLayoutElement는 HLSL GLSL 모두 지원 
// 이후 받는쪽에서 이를 선별하여 필요한 필드의 정보를 가져가는 방식으로 개발
struct ShaderIOLayoutElement
{
	//FORMAT format; 일단 범용 포맷 사용 X 각 필드별 dxgi 포맷등을 드러내는 방식으로 그대로 사용
	
	bool systemValue = false; // SV_Position, gl_Position 같은 시스템 정의 값
	uint32_t alignedByteOffset;

	//HLSL 용 필드
	eastl::string semanticName;
	uint32_t semanticIndex = 0;
	uint32_t format = 0; // 다음은 FORMAT에 정의된 format 이 아닌 dxgi 포맷을 그대로 전달함

	//GLSL 용 필드 systemValue = true 즉 시스템 내장 value 사용시 무시됨
	uint32_t location = 0;

	void WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadJsonObject(const rapidjson::Value& jsonValue);

	static constexpr const char* KEY_FORMAT = "format";
	static constexpr const char* KEY_NAME = "name";
	static constexpr const char* KEY_SYSTEM_VALUE = "systemValue";
	static constexpr const char* KEY_SEMANTIC_INDEX = "semanticIndex";
	static constexpr const char* KEY_LOCATION = "location";
};

struct GlobalVariableType
{
	FORMAT format;
	VARIABLE_CLASS variableClass;
	uint32_t elementsCnt = 1; // 배열 원소 수 (기본 1) scalar

	uint32_t GetByteSize() const; // format 크기 * elements
	uint32_t GetFormatByteSize() const; //format 크기

	void WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadJsonObject(const rapidjson::Value& jsonValue);

	static constexpr const char* KEY_FORMAT = "format";
	static constexpr const char* KEY_VARIABLE_CLASS = "variableClass";
	static constexpr const char* KEY_OFFSET = "offset";
	static constexpr const char* KEY_ELEMENTS_CNT = "elementsCnt";
};


// 동일 포맷 값인가 아닌가로 지정 scalar , vector , matrix 등은 모두 동일 포맷 변수로 
// 예시(DirectX11): D3D11TypeToVariableFormat 에 의하여 그 Type 이 결정되어 저장되어짐
// 단일 포맷 값이 아닌 struct , class 등은 재귀적인 구조로 다시 SearchVariable 를 호출하여 
// 동일 포맷 변수 까지 탐색함 
// 즉 Type 으로 leaf 여부를 결정함

struct GlobalVariable
{	
	eastl::string variableName;
	uint32_t baseOffset = 0;// 부모의 시작 offset
	uint32_t offset = 0;
	uint32_t byteSize = 0;
	GlobalVariableType variableType;
	eastl::vector<GlobalVariable> members; // scalar 인경우 size 0 호출 자체 X

	// 최상의 변수만이 사이즈를 가질 수 있음 
	// 하부 구조적인 변수에 대해서는 사이즈 필드에 대해 비활성화 
	// 
	static constexpr uint32_t INVALID_SIZE = 0xFFFFFFFF;

	void WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadJsonObject(const rapidjson::Value& jsonValue);

	static constexpr const char* KEY_VARIABLE_NAME = "variableName";
	static constexpr const char* KEY_BASE_OFFSET = "baseOffset";
	static constexpr const char* KEY_OFFSET = "offset";
	static constexpr const char* KEY_BYTE_SIZE = "byteSize";
	static constexpr const char* KEY_VARIABLE_TYPE = "variableType";
	static constexpr const char* KEY_VARIABLE_MEMBERS = "variableMembers";

};


struct GlobalVariableBuffer
{
	eastl::string bufferName;
	uint32_t bufferByteSize = 0;
	eastl::vector<GlobalVariable> variables;

	//HLSL 
	uint8_t registerSpace = 0;// Dx12 Dx11의 경우 0으로 고정
	uint8_t regNum = 0;
	uint32_t bindingCount = 0;
	//GLSL 
	uint8_t set = 0;
	uint8_t binding = 0;

	void WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadJsonObject(const rapidjson::Value& jsonValue);

	static constexpr const char* KEY_BUFFER_NAME = "bufferName";
	static constexpr const char* KEY_BUFFER_BYTE_SIZE = "bufferByteSize";
	static constexpr const char* KEY_BUFFER = "buffer";
	static constexpr const char* KEY_REG_SPACE = "registerSpace";
	static constexpr const char* KEY_REG_NUM = "registerNumber";
	static constexpr const char* KEY_SET_NUM = "set";
	static constexpr const char* KEY_BINDING = "binding";
	static constexpr const char* KEY_BINDING_COUNT = "bindingCount";
};


struct ResourceBindingElement
{
	eastl::string resourceName;

	// HLSL
	uint8_t registerSpace = 0; 
	uint8_t regNum = 0;   
	uint8_t resourceType = 0;
	uint8_t dimension = 0;
	uint32_t bindingCount = 0;

	// GLSL 
	uint8_t set = 0;          
	uint8_t binding = 0;      

	void WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadJsonObject(const rapidjson::Value& jsonValue);

	static constexpr const char* KEY_RESOURCE_NAME = "resourceName";
	static constexpr const char* KEY_RESOURCE_TYPE = "resourceType";
	static constexpr const char* KEY_REG_SPACE = "registerSpace";
	static constexpr const char* KEY_REG_NUM = "registerNumber";
	static constexpr const char* KEY_SET_NUM = "set";
	static constexpr const char* KEY_BINDING = "binding";
	static constexpr const char* KEY_BINDING_COUNT = "bindingCount";
	static constexpr const char* KEY_DIMENSION = "dimension";
};

struct ShaderInfoDesc
{
	size_t inputLayoutCnt = 0;
	size_t outLayoutCnt = 0;
	size_t constantBuffersCnt = 0;
	size_t texturesCnt = 0;
	size_t samplersCnt = 0;

};


struct ShaderInfo
{
	eastl::string m_id;
	PipeLineStage m_stage;

	eastl::vector<ShaderIOLayoutElement> m_inputLayout;
	eastl::vector<ShaderIOLayoutElement> m_outputLayout;

	//HLSL 용 필드
	eastl::vector<GlobalVariableBuffer> m_constantBuffers;

	eastl::vector<ResourceBindingElement> m_textures;

	eastl::vector<ResourceBindingElement> m_samplers;

	ShaderProfileVersion m_targetProfileVersion;

	eastl::string m_entryPoint;
	eastl::string m_shaderFilePath;
	// 중간 표현 파일 경로 쉐이더 관리 툴에서 컴파일 후 해당 경로 저장
	/*
		이후 관리과정에서
		쉐이더 저장과 컴파일 저장을 분리하면 불일치 문제가 발생 할 수 있음
		쉐이더의 경우 등록(업데이트)와 동시에 컴파일 되어 지정되어야함
	*/
	eastl::string m_shaderIRFilePath;

	void WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadJsonObject(const rapidjson::Value& jsonValue);


	static constexpr const char* KEY_ID = "id";
	static constexpr const char* KEY_STAGE = "stage";
	static constexpr const char* KEY_INPUT_LAYOUT = "inputLayout";
	static constexpr const char* KEY_OUTPUT_LAYOUT = "outputLayout";
	static constexpr const char* KEY_CONSTANT_BUFFERS = "constantBuffers";
	static constexpr const char* KEY_TEXTURES = "textures";
	static constexpr const char* KEY_SAMPLERS = "samplers";
	static constexpr const char* KEY_TARGET_PROFILE_VERSION = "targetProfileVersion";
	static constexpr const char* KEY_ENTRY_POINT = "entryPoint";
	static constexpr const char* KEY_SHADER_FILE_PATH = "shaderFilePath";
	static constexpr const char* KEY_SHADER_IR_FILE_PATH = "shaderIRFilePath";

};

#endif // !SHADER_INFO_H