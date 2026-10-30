#ifndef RENDER_PASS_INFO_IO
#define RENDER_PASS_INFO_IO

#include <EASTL/unordered_map.h>
#include "RenderPassInfo.h"


class RenderPassInfoIO
{
public:
	void ReadRenderPassInfo(const char* jsonFilePath);


	const eastl::vector<eastl::string> GetRenderPassIdList()const;
	bool FindRenderPass(eastl::string id , RenderPassInfo& outRenderPassInfo)const;
	void SetRenderPassMaterialTag(eastl::string id , eastl::string m_materialTag);
	void SetRenderPass(eastl::string id, RenderPassInfo element);
	eastl::string GetShaderTableFilePath();
	void SetShaderTableFilePath(eastl::string shaderFilePath);
	void ClearRenderPassList();
	void WriteRenderPassInfo(const char* jsonFileName, const char* jsonFilePath);

	static constexpr const char* KEY_RENDER_PASS_LIST = "renderPassList";
	static constexpr const char* KEY_SHADER_TABLE_FILE_PATH = "shaderTableFilePath";

private:
	eastl::string m_shdaerTableFilePath; 
	eastl::unordered_map< eastl::string, RenderPassInfo > m_renderPassList;

};

#endif // !RENDER_PASS_INFO_IO

