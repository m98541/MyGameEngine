#ifndef D3D11_CONTEXT_H
#define D3D11_CONTEXT_H
#include "Base.h"

#include <string>
#include <vector>

/*
	그래픽스 기능의 상태 device ,deviceContext 생성 및 관리 수행
	D3D11Context 외부에서는 device ,deviceContext 설정 및 수정 X 
	D3D11Context 에서 지원하는 기능을 통해서만 상태 변경 가능함.
*/

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct IDXGIFactory1;
struct IDXGIAdapter1;
//HWND의 경우 구조체가 아닌 typedef 된 핸들 타입임으로
//위처럼 전방 선언 안됨 아래와 같이 typedef 필요
typedef void* HWND;

// DESC 가 있지만 이걸 직접 외부 전달은X 
// DisplayAdapterInfo을 통해 필요정보..->넘버, 장치명 ,메모리 크기 정도만 외부 전달 
struct DisplayAdapterInfo
{
	uint32_t displayNum;
	std::wstring description;
	size_t dedicatedVideoMemory;
};

struct DisplayInfo
{
	HWND outPutWindow;
	uint32_t width;
	uint32_t height;
	BOOL screenMode;
};

namespace MJEngine
{
	/*
	* 다음은 디스바이스 정보를 가지고 관리하는 컨텍스트로 
	* 생성자에서는 이에대한 초기설정 해당 과정에서
	* DXGI를 통해서 디바이스 정보에 접근해서 어떤 상태를 가질지를 결정 해두기만 하고 
	*
	* init의 명시적 초기화를 통해 디바이스 및 디바이스 컨텍스트 생성
	* 이후 사용자(유저가)의 변경사항(요구)에 대해서는 
	* 기존 디바이스에 대한 완전한 release 이후 새로운 디바이스정보에대한 init이 필요함
	* 즉 디바이스 와 디바이스 컨텍스트의 생성(init 에서 명시적 진행) 과 생명 주기의 책임을 가짐
	*=> 흔히 게임 그래픽 설정에서 게임 껏다 켜야하는 것과 바로 적용되는 거 차이(?)
	*/
	class D3D11Context
	{
	public:
		D3D11Context();
		~D3D11Context() = default;

		//uint32_t width ,uint32_t height ,BOOL screenMode 여기서 다음은 초기설정 요소
		// 해상도 변경 마다 dev 와 devCon을 새로 만들 수는 없으니 
		//중간에 window의 크기 모드 변경시 init 을 새로 하는게 아닌 해당 요소만 변경 적용 가능함수 따로 필요
		void D3D11ContextInitialize(HWND outPutWindow ,uint32_t width ,uint32_t height ,BOOL screenMode);
		void D3D11ContextRelease(); // 다음에서 m_displayAdapterList 의 내부 메모리등 사용 메모리 전부 release 

		/*
			디스플레이 어뎁터 셋업 및 사용자가 리스트 받아서 설정할 수 있어야함
			초기화시에는 메모리량이 가장 큰 디스플레이 어뎁터 설정 후 
			D3D11Context는 디스플레이 리스트 사용자에게 줄 수 있어야 하고
			사용자는 리스트를 보고 원하는 디스플레이를 선택할 수 있고
			D3D11Context가 선택시 어뎁터 Set을 지원해줘야함
		*/
		void SelectDisplayAdapter(uint32_t displayNum); 

	private:
		void SelectMaxMemDisplayAdapter();

		/*
			객체 내부에서 DisplayInfo의 정보를 맴버로 담고 있는 형태가 아닌
			해당 정보 필요시 m_giSwapchain을 통하여 DisplayInfo을 받아와서 
			사용하는 방식 사용 -> DisplayInfo를 따로 상태로 저장시
			업데이트 안시키거나 값 불일치 등 관리 문제 발생 가능
		*/
		const DisplayInfo GetCurDisplayInfo() const;

		const std::vector<DisplayAdapterInfo> GetDisplayAdaptersInfo() const;
		/*
		 내부 초기화 함수(초기화 순서 중요) 
		*/
		void InitDeviceAndSwapChain(HWND outPutWindow, uint32_t width, uint32_t height, BOOL screenMode);
		void InitRenderTargetView();
		void InitDepthStencilView(uint32_t width, uint32_t height);
		void InitRasterizerState();
		void InitViewPorts(uint32_t width, uint32_t height);

	private:
		RefPtr<ID3D11Device> m_device;
		RefPtr<ID3D11DeviceContext> m_deviceContext;
		RefPtr<IDXGISwapChain> m_giSwapchain;
		RefPtr<IDXGIFactory1> m_giFactory;
		RefPtr<ID3D11RenderTargetView> m_backBufferRendetTargetView;
		RefPtr<ID3D11DepthStencilView> m_depthStencilView;
		RefPtr<std::vector<IDXGIAdapter1*>> m_displayAdapterList;
		IDXGIAdapter1* m_useDiplayAdapter;

		RefPtr<ID3D11RasterizerState> m_rasterState_BackSolid;
		RefPtr<ID3D11RasterizerState> m_rasterState_BackWire;
		RefPtr<ID3D11RasterizerState> m_rasterState_FrontSolid;
		RefPtr<ID3D11RasterizerState> m_rasterState_FrontWire;

	};

}

#endif // !D3D11_CONTEXT_H
