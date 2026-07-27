#include <dxgi.h>
#include <d3d11.h>
#include <assert.h>
#include "D3D11Context.h"

using namespace MJEngine;

D3D_FEATURE_LEVEL featureLevelarr[] = {
	//D3D_FEATURE_LEVEL_12_2,
	//D3D_FEATURE_LEVEL_12_1,
	//D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_9_3,
	D3D_FEATURE_LEVEL_9_2,
	D3D_FEATURE_LEVEL_9_1,
};

D3D11Context::D3D11Context()
{
	//__uuidof: 컴파일 타임에 특정 com(component Object Model) 혹은 인터페이스의 GUID(Globally Unique Identifier)를 가져옴
	CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&m_giFactory);
	int adapter_num = 0;
	IDXGIAdapter1* tempAdapter;
	while (SUCCEEDED(m_giFactory->EnumAdapters1(adapter_num, &tempAdapter)))
	{
			m_displayAdapterList->push_back(tempAdapter);
			adapter_num++;
	}
	SelectMaxMemDisplayAdapter();
	
}

void D3D11Context::SelectMaxMemDisplayAdapter()
{
	size_t maxMem = 0;
	DXGI_ADAPTER_DESC1 tempAdapterDesc; 

	for (IDXGIAdapter1* displayAdapter : *m_displayAdapterList)
	{
		displayAdapter->GetDesc1(&tempAdapterDesc);

		if (tempAdapterDesc.DedicatedVideoMemory > maxMem)
		{
			maxMem = tempAdapterDesc.DedicatedVideoMemory;
			m_useDiplayAdapter = displayAdapter;
		}
	}
}


void D3D11Context::SelectDisplayAdapter(uint32_t DisplayNum)
{
	if (DisplayNum >= m_displayAdapterList->size()) assert(false && "SelectDisplayAdapter : DisplayNum is range over || D3D11Context.cpp file ");
	m_useDiplayAdapter = m_displayAdapterList->at(DisplayNum);
}

const std::vector<DisplayAdapterInfo> D3D11Context::GetDisplayAdaptersInfo() const
{
	std::vector<DisplayAdapterInfo> result = {};
	DXGI_ADAPTER_DESC1 tempAdapterDesc;
	uint32_t idx = 0;
	for (IDXGIAdapter1* displayAdapter : *m_displayAdapterList)
	{
		displayAdapter->GetDesc1(&tempAdapterDesc);
		DisplayAdapterInfo adapterInfo = {
			idx,
			tempAdapterDesc.Description ,
			tempAdapterDesc.DedicatedVideoMemory
		};

		result.push_back(adapterInfo);
	}

	return result;
}

const DisplayInfo D3D11Context::GetCurDisplayInfo() const
{
	DisplayInfo result = {};
	DXGI_SWAP_CHAIN_DESC desc = {};
	m_giSwapchain->GetDesc(&desc);

	result.outPutWindow = desc.OutputWindow;
	result.width = desc.BufferDesc.Width;
	result.height = desc.BufferDesc.Height;
	result.screenMode = desc.Windowed;

	return result;
}

void D3D11Context::D3D11ContextRelease()
{
	// 무조건 만들어진 순서 역순으로 Release // RefPtr 이 관리한다면 Reset
	m_rasterState_FrontWire.reset();
	m_rasterState_FrontSolid.reset();
	m_rasterState_BackWire.reset();
	m_rasterState_BackSolid.reset();

	m_depthStencilView.reset();
	m_backBufferRendetTargetView.reset();

	m_deviceContext.reset();
	m_device.reset();
	m_giSwapchain.reset();

	for (IDXGIAdapter1* adapter : *m_displayAdapterList)
	{
		adapter->Release();
	}
	m_displayAdapterList->clear();

	m_giFactory.reset();
}

void D3D11Context::D3D11ContextInitialize(HWND outPutWindow, uint32_t width, uint32_t height, BOOL screenMode)
{
	// dev , devCon initialize
	// 백버퍼 , 깊이 버퍼 생성 후 설정 
	// 기본 뷰포트 설정 및 기본 화면 설정 부여
	InitDeviceAndSwapChain(outPutWindow, width, height, screenMode);
	InitRenderTargetView();
	InitDepthStencilView(width, height);
	InitRasterizerState();
	InitViewPorts(width, height);

}


void D3D11Context::InitDeviceAndSwapChain(HWND outPutWindow, uint32_t width, uint32_t height, BOOL screenMode)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	// 일단 d3d11고정 이후 상위 레이어에서 사용할 D3D 버전에 대해서 간접적으로 변경할 수 있게 해야함!
	// 그러면 DXGI 연결부를 d3d11과 분리해야 하나(?)
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

	//flag 0 , 스왑체인의 옵션(단일 스레드 최적화 ,디버그 레이어 등등..) 모두 끈 상태
	uint32_t deviceAndSwapChainFlag = 0;

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.OutputWindow = outPutWindow;
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.Windowed = screenMode;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	//맴버의 m_device , m_deviceContext 등은 RefPtr 로 관리됨
	//해당 경우 D3D11CreateDeviceAndSwapChain 서의 매개 타입과 불일치 문제
	//형변환을 통해 형태를 맞추는 것 보다 temp 포인터를 통해서 생성후 소유권 이전 시킴
	//소유권 이전 전에 hr 검사 통해 성공 케이스에서만 적용 어차피 실패시 assert
	IDXGISwapChain* tempSwapChain = nullptr;
	ID3D11Device* tempDev = nullptr;
	ID3D11DeviceContext* tempDevCon = nullptr;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		m_useDiplayAdapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		deviceAndSwapChainFlag,
		featureLevelarr,
		sizeof(featureLevelarr) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&tempSwapChain,
		&tempDev,
		&featureLevel,
		&tempDevCon
	);

	if (SUCCEEDED(hr))
	{
		m_device.reset(tempDev);
		m_deviceContext.reset(tempDevCon);
		m_giSwapchain.reset(tempSwapChain);
	}
	else
	{
		assert(false && "D3D11CreateDeviceAndSwapChain fail !!! || D3D11Context.cpp file");
	}

}

void D3D11Context::InitRenderTargetView()
{

	//백 버퍼 생성 및 초기화
	ID3D11Texture2D* tempBackBuffer = nullptr;
	ID3D11RenderTargetView* tempRenderTargetView = nullptr;

	m_giSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)tempBackBuffer);
	HRESULT hr = m_device->CreateRenderTargetView(tempBackBuffer, NULL, &tempRenderTargetView);

	tempBackBuffer->Release();

	if (SUCCEEDED(hr))
	{
		m_backBufferRendetTargetView.reset(tempRenderTargetView);
	}
	else
	{
		assert(false && "CreateRenderTargetView fail(Create SwapChain Render Target Buffer fail) || D3D11Context.cpp file");
	}

}

void D3D11Context::InitDepthStencilView(uint32_t width, uint32_t height)
{
	//깊이 & 스텐실 버퍼 생성 및 초기화
	ID3D11RenderTargetView* tempRenderTargetView = nullptr;
	ID3D11Texture2D* tempDepthStencilhBuffer = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc = {};
	ID3D11DepthStencilView* tempDepthStencilView = nullptr;

	depthStencilBufferDesc.Width = width;
	depthStencilBufferDesc.Height = height;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.SampleDesc.Count = 4;
	depthStencilBufferDesc.SampleDesc.Quality = 0;

	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_device->CreateTexture2D(&depthStencilBufferDesc, NULL, &tempDepthStencilhBuffer);
	HRESULT hr = m_device->CreateDepthStencilView(tempDepthStencilhBuffer, nullptr, &tempDepthStencilView);

	tempDepthStencilhBuffer->Release();

	if (SUCCEEDED(hr))
	{
		m_depthStencilView.reset(tempDepthStencilView);
	}
	else
	{
		assert(false && "CreateDepthStencilView fail ||  D3D11Context.cpp file");
	}

	// 깊이 스텐실 뷰 와 스왑 체인 뷰를 통해 렌더 타겟 셋팅
	tempRenderTargetView = m_backBufferRendetTargetView.get();
	tempDepthStencilView = m_depthStencilView.get();
	m_deviceContext->OMSetRenderTargets(1, &tempRenderTargetView, tempDepthStencilView);
}

void D3D11Context::InitRasterizerState()
{
	//레스터라이저 생성 및 셋팅(ID3D11RasterizerState의 경우 1개로 set 하여 변경해가면 사용 못함
	// 즉 여러 버전의 state를 미리 만들어 두고 이후 바꿔주면서 변경 하여 사용 필요!)

	D3D11_RASTERIZER_DESC tempRasterRizerDesc = {};
	ID3D11RasterizerState* tempRasterRizerState = nullptr;
	tempRasterRizerDesc.CullMode = D3D11_CULL_BACK;
	tempRasterRizerDesc.FillMode = D3D11_FILL_SOLID;
	tempRasterRizerDesc.DepthClipEnable = TRUE;
	tempRasterRizerDesc.ScissorEnable = FALSE;// 가위테스트 X
	tempRasterRizerDesc.MultisampleEnable = FALSE;    // 멀티샘플링 X
	tempRasterRizerDesc.AntialiasedLineEnable = FALSE;// 선 안티앨리어싱X
	HRESULT hr = m_device->CreateRasterizerState(&tempRasterRizerDesc, &tempRasterRizerState);

	if (SUCCEEDED(hr))
	{
		m_rasterState_BackSolid.reset(tempRasterRizerState);
		tempRasterRizerState = nullptr;
	}
	else
	{
		assert(false && "CreateRasterizerState [CULL BACK / FILL SOILD] fail ||  D3D11Context.cpp file");
	}
}

void D3D11Context::InitViewPorts(uint32_t width, uint32_t height)
{
	// 뷰포트 기본 세팅
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport = {
		0.F , 0.F,  // top left x y  
		(float)width , (float)height , // width  ,height 
		0.F , 1.F // depth min max DirectX 0~1
	};

	m_deviceContext->RSSetViewports(1, &viewport);
}