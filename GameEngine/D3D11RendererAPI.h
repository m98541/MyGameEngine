#ifndef D3D11_RENDERER_API
#define D3D11_RENDERER_API

#include "RendererAPI.h"


namespace MJEngine
{
	class D3D11RenderAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(Vector_2i offset, uint32_t width, uint32_t height) override;

		virtual void Clear() override;

		virtual void DrawIndexed(const RefPtr<Mesh>& mesh, uint32_t indexCount = 0) override;
		virtual void DrawLines(const RefPtr<Mesh>& mesh ,uint32_t vertexCount) override;

		virtual void SetLineWidth(float width) override;
	};
}


#endif // !D3D11_RENDERER_API
