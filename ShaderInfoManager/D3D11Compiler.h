#pragma once
#include "ShaderCompiler.h"
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>

/*
	다음 컴파일러의 목적 
	DirectX 11 기반의 HLSL 로 작성된 
	쉐이더 컴파일 후 Reflect를 통하여 엔진 측에서 쉐이더 바인딩 과정에서 필요한
	IO서명 매새변수 정보 , 상수 버퍼 , 텍스처 - 셈플러 등 의 리소스 바인딩 정보 추출
	
	+ 컴파일 된 세이더에 대한 중간 표현 언어로 되어진 세이터 형태로 변환하여 저장
	이후 엔진에서는 컴파일 없이 읽어 바로 linking 할 수 있도록 함
*/
class D3D11Compiler : public ShaderCompiler
{
public:
	virtual bool ShaderCompile(
		const wchar_t* hlslFilePath,
		const char* entryPoint,
		const char* targetProfile,
		const wchar_t* outputBinPath,
		ShaderInfo& shaderInfo,
		ShaderInfoDesc& shaderInfoDesc
	) override;

	

private:	
	ID3DBlob* m_shaderBlob = nullptr;
	ID3D11ShaderReflection* m_reflectSource = nullptr;
	void ShaderReflection(ShaderInfo& shaderInfo, ShaderInfoDesc& shaderInfoDesc);
	void ConstantBufferReflection(ShaderInfo& shaderInfo, ShaderInfoDesc& shaderInfoDesc, D3D11_SHADER_DESC& orgD3D11ShaderDESC);
	void IoSignatureReflection(ShaderInfo& shaderInfo, ShaderInfoDesc& shaderInfoDesc , D3D11_SHADER_DESC& orgD3D11ShaderDESC);
	void ResourceBindingReflection(ShaderInfo& shaderInfo, ShaderInfoDesc& shaderInfoDesc,D3D11_SHADER_DESC& orgD3D11ShaderDESC);
};	