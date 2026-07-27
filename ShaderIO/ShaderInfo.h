#ifndef SHADER_INFO_H
#define SHADER_INFO_H

#include <string>
#include <vector>

#include <rapidjson/document.h>

#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

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

constexpr size_t GRAPHICS_STAGE_COUNT = 5;

enum class PipeLineStage 
{
	Vertex , Hull , Domain , Geometry , Pixel , Compute 
};

struct ShaderInfo
{
	std::string m_id;
	PipeLineStage m_stage;

	std::vector<std::string> m_inputLayout;
	std::vector<std::string> m_outputLayout;
	std::vector<std::string> m_constantBuffers;
	std::vector<std::string> m_textureSamplers;

	std::string m_targetProfileVersion;
	std::string m_entryPoint;
	std::string m_shaderFilePath;
	std::string m_shaderHeader;

	void WriteRapidJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
	void ReadRapidJson(const rapidjson::Value& jsonValue);
};



#endif // !SHADER_INFO_H
