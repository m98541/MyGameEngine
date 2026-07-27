#ifndef SHADER_INFO_IO
#define SHADER_INFO_IO
#include "ShaderInfo.h"
#include <unordered_map>

class ShaderInfoIO
{

public:
	void ReadShaderInfo(const char* jsonFilePath);
	void WriteShaderInfo(const char* jsonFileName ,const char* jsonFilePath);


	const std::vector<std::string>& GetShaderIdList()const;
	bool FindShader(std::string id, ShaderInfo& outShader)const;
	void SetShader(std::string id, ShaderInfo element);
	void DeleteShader(std::string id);

	void ClearShaderMap();
	 
private:
	//읽어드린 쉐이더는 다음 리스트에 임시 저장, 이후 해당 리스트에서 삽입 삭제 이뤄지고 저장하여 출력시키는 개념
	std::unordered_map< std::string, ShaderInfo > m_shaderMap;

};


#endif // !SHADER_IN FO_IO