#ifndef SHADER_INFO_IO
#define SHADER_INFO_IO
#include "ShaderInfo.h"
#include <unordered_map>
#include <string_view>

class ShaderInfoIO
{

public:
	void ReadShaderInfo(const char* jsonFilePath);
	void WriteShaderInfo(const char* jsonFileName ,const char* jsonFilePath);


	const std::vector<std::string> GetShaderIdList()const;
	bool FindShader(std::string id, ShaderInfo& outShader)const;
	void SetShader(std::string id, ShaderInfo element);
	void DeleteShader(std::string id);

	void ClearShaderMap();

	//shaderInfo 부분다 이렇게 교체 필요 유지관리가 훨씬 편리함
	static constexpr const char* KEY_SHADER_COUNT = "shaderCount";
	static constexpr const char* KEY_SHADER_TABLE = "shaderTable";
	 
private:

	//읽어드린 쉐이더는 다음 리스트에 임시 저장, 이후 해당 리스트에서 삽입 삭제 이뤄지고 저장하여 출력시키는 개념
	size_t m_shaderCount = 0;
	//쉐이더 테이블은 순서 X 의 데이터 MAP 테이블로 저장 ID 기반참조
	//렌더 패스 로 { ID -> ID -> ..} 로 해당 테이블을 참조하여 쉐이더 가져옴
	
	std::unordered_map< std::string, ShaderInfo > m_shaderMap;
	
};


#endif // !SHADER_IN FO_IO