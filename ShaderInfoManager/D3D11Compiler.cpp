#include "D3D11Compiler.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>

#include <vector>

PipeLineStage D3D11VersionToPipeLineStage(D3D11_SHADER_VERSION_TYPE version);

ShaderInfo D3D11Compiler::ShaderCompile(
	const wchar_t* hlslFilePath,
	const char* entryPoint,
	const char* targetProfile,
	const char* outputBinPath)
{
	
	UINT compileFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3;

#if defined(_DEBUG) // D3DCOMPILE_SKIP_OPTIMIZATION -> 어셈블리 디버깅시 순서 재배치 및 접기 방지
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif


	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(
		hlslFilePath,
		NULL,
		NULL, //include 사용시 -> D3D_COMPILE_STANDARD_FILE_INCLUDE 매그로 전달|| #define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)
		entryPoint,
		targetProfile,
		compileFlags,
		0,
		&m_shaderBlob,
		&errorBlob
	);

	if (!SUCCEEDED(hr))
	{
		//assert! D3DCompileFromFile의 성공 여부
	}

	
}


//private 필드로 ID3DBlob(컴파일된 쉐이더 바이너리 정보)와 함께 묶여서 관리 예정
//정보 추출(Reflection) 함수 각각의 함수 통과시 
// ShaderInfo의 해당하는 필드 정보 채워서 보내주는 방식으로 구성 -> 이러면 각 기능별로 보기 좋고 assert 시 콜스텍 참조하면 어디부분이 문제인지 확인이 좋아 보임
//세이더 프로그램 정보
void D3D11Compiler::ShaderReflection()
{
	HRESULT hr = D3DReflect(
		m_shaderBlob->GetBufferPointer(),
		m_shaderBlob->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		(void**)&m_reflectSource
	);
	
	if (!SUCCEEDED(hr))
	{
		//assert! D3DReflect 성공 여부 표기
	}

	D3D11_SHADER_DESC orgD3D11ShaderDESC = {}; 
	m_reflectSource->GetDesc(&orgD3D11ShaderDESC);
	

	m_shaderInfo.m_stage = D3D11VersionToPipeLineStage(
		static_cast<D3D11_SHADER_VERSION_TYPE>(orgD3D11ShaderDESC.Version)
	);

}

//상수 버퍼 정보
void D3D11Compiler::ConstantBufferReflection(D3D11_SHADER_DESC& orgD3D11ShaderDESC)
{
	m_shaderInfoDesc.constantBuffersCnt = orgD3D11ShaderDESC.ConstantBuffers;

}



//입출력 서명 정보 
void D3D11Compiler::IoSignatureReflection(D3D11_SHADER_DESC& orgD3D11ShaderDESC)
{
	m_shaderInfoDesc.inputLayoutCnt = orgD3D11ShaderDESC.InputParameters;
	m_shaderInfoDesc.outLayoutCnt = orgD3D11ShaderDESC.OutputParameters;

}

//자원 연결 정보 
void D3D11Compiler::ResourceBindingReflection(D3D11_SHADER_DESC& orgD3D11ShaderDESC)
{
	size_t resourceInfoCnt = orgD3D11ShaderDESC.BoundResources;

	std::vector<D3D11_SHADER_INPUT_BIND_DESC> textureInputDesc;
	std::vector<D3D11_SHADER_INPUT_BIND_DESC> samplerInputDesc;

	for (size_t i = 0; i < resourceInfoCnt; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC inputBindDesc;
		m_reflectSource->GetResourceBindingDesc(i , &inputBindDesc);

		switch (inputBindDesc.Type)
		{
		case D3D_SIT_TEXTURE:
			textureInputDesc.push_back(inputBindDesc);
			break;
		case D3D_SIT_SAMPLER:
			samplerInputDesc.push_back(inputBindDesc);
			break;
		default:
			break;
		}
	}
	
	m_shaderInfoDesc.texturesCnt = textureInputDesc.size();
	m_shaderInfoDesc.samplersCnt = samplerInputDesc.size();



}




PipeLineStage D3D11VersionToPipeLineStage(D3D11_SHADER_VERSION_TYPE version)
{
	switch (version)
	{
	case D3D11_SHVER_VERTEX_SHADER:
		return PipeLineStage::Vertex;

	case D3D11_SHVER_HULL_SHADER:
		return PipeLineStage::Hull;

	case D3D11_SHVER_DOMAIN_SHADER:
		return PipeLineStage::Domain;

	case D3D11_SHVER_GEOMETRY_SHADER:
		return PipeLineStage::Geometry;

	case D3D11_SHVER_PIXEL_SHADER:
		return PipeLineStage::Pixel;

	case D3D11_SHVER_COMPUTE_SHADER:
		return PipeLineStage::Compute;

	default:
		// assert! 잘못된 쉐이더 버전 참조 문제... 위 정의된 허용할수 있는 쉐이더 범위 넘어섬
		break;
	}
}