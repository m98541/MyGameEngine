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
};

struct GlobalVariableElement
{
	std::string variableName;
	FORMAT format;

	uint32_t offset = 0;
	uint32_t size = 0;

	void WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadJsonObject(const rapidjson::Value& jsonValue);
};

struct GlobalVariableBuffer
{
	std::string bufferName;
	uint32_t bufferSize = 0; // 16byte 정렬 위한 -> 16배수 메모리 필요
	std::vector<GlobalVariableElement> buffer;

	//HLSL 
	uint8_t registerSpace = 0;
	uint8_t regNum = 0;

	//GLSL 
	uint8_t set = 0;
	uint8_t binding = 0;

	void WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadJsonObject(const rapidjson::Value& jsonValue);
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
	std::string m_shaderHeader;

	void WriteJsonObject(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadJsonObject(const rapidjson::Value& jsonValue);
	
};



#endif // !SHADER_INFO_H
