#ifndef RENDER_PASS_INFO_IO
#define RENDER_PASS_INFO_IO

#include <unordered_map>
#include "RenderPassInfo.h"


class RenderPassInfoIO
{
public:
	void ReadRenderPassInfo(const char* jsonFilePath);
	void WriteRenderPassInfo(const char* jsonFileName , const char* jsonFilePath);

	const std::vector<std::string> GetRenderPassIdList()const;
	bool FindRenderPass(std::string id , RenderPassInfo& outRenderPassInfo)const;
	void SetRenderPassMaterialTag(std::string id , std::string materialTag);
	void SetRenderPass(std::string id, RenderPassInfo element);
	void ClearRenderPassList();

	static constexpr const char* KEY_RENDER_PASS_LIST = "renderPassList";

private:

	std::unordered_map< std::string, RenderPassInfo > m_renderPassList;

};

#endif // !RENDER_PASS_INFO_IO

