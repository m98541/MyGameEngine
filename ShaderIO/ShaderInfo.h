#ifndef SHADER_INFO_H
#define SHADER_INFO_H

#include <string>
#include <vector>

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
	FORMAT format;
	std::string name;
	bool systemValue = false; // SV_Position, gl_Position 같은 시스템 정의 값

	//HLSL 용 필드
	uint32_t semanticIndex = 0;

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

struct GlobalVariableElement
{
	std::string variableName;
	FORMAT format;

	uint32_t offset = 0;
	uint32_t size = 0;

	void WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadJsonObject(const rapidjson::Value& jsonValue);

	static constexpr const char* KEY_VARIABLE_NAME = "variableName";
	static constexpr const char* KEY_FORMAT = "format";
	static constexpr const char* KEY_OFFSET = "offset";
	static constexpr const char* KEY_SIZE = "size";
};

struct GlobalVariableBuffer
{
	std::string bufferName;
	size_t bufferSize = 0; 
	std::vector<GlobalVariableElement> buffer;

	//HLSL 
	uint8_t registerSpace = 0;
	uint8_t regNum = 0;

	//GLSL 
	uint8_t set = 0;
	uint8_t binding = 0;

	void WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadJsonObject(const rapidjson::Value& jsonValue);

	static constexpr const char* KEY_BUFFER_NAME = "bufferName";
	static constexpr const char* KEY_BUFFER_SIZE = "bufferSize";
	static constexpr const char* KEY_BUFFER = "buffer";
	static constexpr const char* KEY_REG_SPACE = "registerSpace";
	static constexpr const char* KEY_REG_NUM = "registerNumber";
	static constexpr const char* KEY_SET_NUM = "set";
	static constexpr const char* KEY_BINDING = "binding";
};




struct ShaderInfo
{
	std::string m_id;
	PipeLineStage m_stage;

	std::vector<ShaderIOLayoutElement> m_inputLayout;
	std::vector<ShaderIOLayoutElement> m_outputLayout;

	//HLSL 용 필드
	std::vector<GlobalVariableBuffer> m_constantBuffers;

	std::vector<std::string> m_textureSamplers;

	ShaderProfileVersion m_targetProfileVersion;

	std::string m_entryPoint;
	std::string m_shaderFilePath;
	// 중간 표현 파일 경로 쉐이더 관리 툴에서 컴파일 후 해당 경로 저장
	/*
		이후 관리과정에서 
		쉐이더 저장과 컴파일 저장을 분리하면 불일치 문제가 발생 할 수 있음
		쉐이더의 경우 등록(업데이트)와 동시에 컴파일 되어 지정되어야함 
	*/
	std::string m_shaderIRFilePath;

	void WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadJsonObject(const rapidjson::Value& jsonValue);


	static constexpr const char* KEY_ID = "id";
	static constexpr const char* KEY_STAGE = "stage";
	static constexpr const char* KEY_INPUT_LAYOUT = "inputLayout";
	static constexpr const char* KEY_OUTPUT_LAYOUT = "outputLayout";
	static constexpr const char* KEY_CONSTANT_BUFFERS = "constantBuffers";
	static constexpr const char* KEY_TEXTURE_SAMPLERS = "textureSamplers";
	static constexpr const char* KEY_TARGET_PROFILE_VERSION = "targetProfileVersion";
	static constexpr const char* KEY_ENTRY_POINT = "entryPoint";
	static constexpr const char* KEY_SHADER_FILE_PATH = "shaderFilePath";
	static constexpr const char* KEY_SHADER_IR_FILE_PATH = "shaderIRFilePath";
	
};



#endif // !SHADER_INFO_H
