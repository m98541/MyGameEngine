#include "D3D11Compiler.h"
#include <Windows.h>
#include <EASTL/vector.h>
#include <EASTL/map.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

PipeLineStage D3D11VersionToPipeLineStage(const D3D11_SHADER_VERSION_TYPE& version);
FORMAT D3D11TypeToVariableFormat(const D3D11_SHADER_TYPE_DESC& shaderTypeDesc);
VARIABLE_CLASS D3D11ClassTOVariableClass(const D3D_SHADER_VARIABLE_CLASS& typeClass);
DXGI_FORMAT ExtractDxgiFormat(D3D_REGISTER_COMPONENT_TYPE componentType, BYTE mask);

uint32_t GetByteSizeFromMask(BYTE mask);
GlobalVariable ExtractVariableInfo(ID3D11ShaderReflectionVariable* d3dVariableReflection);
GlobalVariable TraverseVariableMember(const char* memberName, ID3D11ShaderReflectionType* d3dVariableTypeReflection, const uint32_t baseOffset);

eastl::string ToUtf8String(const wchar_t* wStr);

bool D3D11Compiler::ShaderCompile(
	const wchar_t* hlslFilePath,
	const char* entryPoint,
	const char* targetProfile,
	const wchar_t* outputBinPath,
	ShaderInfo& shaderInfo,
	ShaderInfoDesc& shaderInfoDesc
)
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
		return false;
	}

	//쉐이더 정보 초기화 ... 

	shaderInfo = {};
	shaderInfoDesc = {};

	hr = D3DWriteBlobToFile(
		m_shaderBlob,
		outputBinPath,
		TRUE
	);

	if (SUCCEEDED(hr))
	{
		// shader 정보 주출 
		ShaderReflection(shaderInfo , shaderInfoDesc);
		shaderInfo.m_shaderFilePath = ToUtf8String(hlslFilePath);
		shaderInfo.m_shaderIRFilePath = ToUtf8String(outputBinPath);
	}
	else
	{
		//assert! D3DWriteBlobToFile 실패
		return false;
	}

	return true;
	
}


//private 필드로 ID3DBlob(컴파일된 쉐이더 바이너리 정보)와 함께 묶여서 관리 예정
//정보 추출(Reflection) 함수 각각의 함수 통과시 
// ShaderInfo의 해당하는 필드 정보 채워서 보내주는 방식으로 구성 -> 이러면 각 기능별로 보기 좋고 assert 시 콜스텍 참조하면 어디부분이 문제인지 확인이 좋아 보임
//세이더 프로그램 정보
void D3D11Compiler::ShaderReflection(ShaderInfo& shaderInfo, ShaderInfoDesc& shaderInfoDesc)
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
	

	shaderInfo.m_stage = D3D11VersionToPipeLineStage(
		static_cast<D3D11_SHADER_VERSION_TYPE>(orgD3D11ShaderDESC.Version)
	);


	ConstantBufferReflection(shaderInfo , shaderInfoDesc , orgD3D11ShaderDESC);
	IoSignatureReflection(shaderInfo, shaderInfoDesc, orgD3D11ShaderDESC);
	ResourceBindingReflection(shaderInfo, shaderInfoDesc, orgD3D11ShaderDESC);

}

//상수 버퍼 정보
void D3D11Compiler::ConstantBufferReflection(ShaderInfo& shaderInfo, ShaderInfoDesc& shaderInfoDesc, D3D11_SHADER_DESC& orgD3D11ShaderDESC)
{

	shaderInfoDesc.constantBuffersCnt = orgD3D11ShaderDESC.ConstantBuffers;
	ID3D11ShaderReflectionConstantBuffer* cbReflection = nullptr;
	ID3D11ShaderReflectionVariable* cbVariableReflection = nullptr;
	
	shaderInfo.m_constantBuffers.resize(shaderInfoDesc.constantBuffersCnt);
	for (uint32_t i = 0; i < shaderInfoDesc.constantBuffersCnt; i++)
	{
		D3D11_SHADER_BUFFER_DESC cbBufferDesc = {};
		cbReflection = m_reflectSource->GetConstantBufferByIndex(i);
		cbReflection->GetDesc(&cbBufferDesc);

		shaderInfo.m_constantBuffers[i].bufferName = cbBufferDesc.Name;
		shaderInfo.m_constantBuffers[i].bufferByteSize = cbBufferDesc.Size;
		

		//해당 과정에 대하여 리프 노드의 scalar 변수까지 추적하는 재귀적 과정 필요...
		
		shaderInfo.m_constantBuffers[i].variables.resize(cbBufferDesc.Variables);
		for (uint32_t j = 0; j < cbBufferDesc.Variables; j++)
		{
			cbVariableReflection = cbReflection->GetVariableByIndex(j);
			shaderInfo.m_constantBuffers[i].variables[j] = ExtractVariableInfo(cbVariableReflection);
		}


	}

}


//입출력 서명 정보 
void D3D11Compiler::IoSignatureReflection(ShaderInfo& shaderInfo, ShaderInfoDesc& shaderInfoDesc, D3D11_SHADER_DESC& orgD3D11ShaderDESC)
{
	shaderInfoDesc.inputLayoutCnt = orgD3D11ShaderDESC.InputParameters;
	shaderInfoDesc.outLayoutCnt = orgD3D11ShaderDESC.OutputParameters;
	D3D11_SIGNATURE_PARAMETER_DESC parameterDesc = {};

	shaderInfo.m_inputLayout.reserve(shaderInfoDesc.inputLayoutCnt);
	shaderInfo.m_outputLayout.reserve(shaderInfoDesc.outLayoutCnt);

	ShaderIOLayoutElement ioLayoutElement = {};
	for (uint32_t i = 0 ,curOffset = 0; i < shaderInfoDesc.inputLayoutCnt; i++)
	{
		m_reflectSource->GetInputParameterDesc(i, &parameterDesc);

		//시스템 내부 변수 전달 값은 제외
		if (parameterDesc.SystemValueType != D3D_NAME_UNDEFINED)
			continue;

		ioLayoutElement.semanticName = parameterDesc.SemanticName;
		ioLayoutElement.semanticIndex = parameterDesc.SemanticIndex;
		ioLayoutElement.format = static_cast<uint32_t>(ExtractDxgiFormat(parameterDesc.ComponentType , parameterDesc.Mask));

		ioLayoutElement.alignedByteOffset = curOffset;
		curOffset += GetByteSizeFromMask(parameterDesc.Mask);

		shaderInfo.m_inputLayout.push_back(ioLayoutElement);
	}


	for (uint32_t i = 0 ,curOffset = 0; i < shaderInfoDesc.outLayoutCnt; i++)
	{
		m_reflectSource->GetOutputParameterDesc(i, &parameterDesc);
		
		if (parameterDesc.SystemValueType != D3D_NAME_UNDEFINED)
			continue;

		ioLayoutElement.semanticName = parameterDesc.SemanticName;
		ioLayoutElement.semanticIndex = parameterDesc.SemanticIndex;
		ioLayoutElement.format = static_cast<uint32_t>(ExtractDxgiFormat(parameterDesc.ComponentType, parameterDesc.Mask));

		ioLayoutElement.alignedByteOffset = curOffset;
		curOffset += GetByteSizeFromMask(parameterDesc.Mask);

		shaderInfo.m_outputLayout.push_back(ioLayoutElement);
	}
	//3400000 293380

}

//자원 연결 정보 
void D3D11Compiler::ResourceBindingReflection(ShaderInfo& shaderInfo, ShaderInfoDesc& shaderInfoDesc, D3D11_SHADER_DESC& orgD3D11ShaderDESC)
{
	size_t resourceInfoCnt = orgD3D11ShaderDESC.BoundResources;

	

	// 상수버퍼 바인딩 요구 데이터 추출용(이름 바인딩 시작 오프셋 등 정보 추출위해 필요)
	eastl::map<eastl::string, D3D11_SHADER_INPUT_BIND_DESC> constantBufferDescMap;

	for (size_t i = 0; i < resourceInfoCnt; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC inputBindDesc;
		m_reflectSource->GetResourceBindingDesc(i , &inputBindDesc);
		ResourceBindingElement element;

		if (inputBindDesc.Type == D3D_SIT_TEXTURE || inputBindDesc.Type == D3D_SIT_SAMPLER)
		{
			element = {};
			element.resourceType = inputBindDesc.Type;
			element.resourceName = inputBindDesc.Name;
			element.regNum = inputBindDesc.BindPoint;
			element.bindingCount = inputBindDesc.BindCount;
		}
		switch (inputBindDesc.Type)
		{
		case D3D_SIT_TEXTURE:
			shaderInfo.m_textures.push_back(element);
			break;
		case D3D_SIT_SAMPLER:
			shaderInfo.m_samplers.push_back(element);
			break;
		case D3D_SIT_CBUFFER:
			constantBufferDescMap[inputBindDesc.Name] = inputBindDesc;
			break;
		default:
			break;
		}
	}
	/*
	ConstantBufferReflection 에서의 
	orgD3D11ShaderDESC.ConstantBuffers 과
	위에서는 constantBufferDesc.size() 는 동일함을 보장할 수 있는가?
	*/

	// 음..... 일단 최종 버퍼에 담긴 갯수로 결국 사용해야 하니 다음이 맞는거 같은데...
	shaderInfoDesc.constantBuffersCnt = constantBufferDescMap.size();
	shaderInfoDesc.texturesCnt = shaderInfo.m_textures.size();
	shaderInfoDesc.samplersCnt = shaderInfo.m_samplers.size();

	for (auto& cb : shaderInfo.m_constantBuffers)
	{
		//타입정보의 경우 D3D_SIT_CBUFFER임이 확실함으로 저장 X
		auto bindInfo = constantBufferDescMap.find(cb.bufferName);
		if (bindInfo != constantBufferDescMap.end())
		{
			cb.regNum = bindInfo->second.BindPoint;
			cb.bindingCount = bindInfo->second.BindCount;
		}
		else
		{
			// bufferName이 존재하지 않는 경우 0 이나 NULL 사용 X(0 부터 시작... 끝값이 안전)
			cb.regNum = 0xFF;
		}
	}



}


PipeLineStage D3D11VersionToPipeLineStage(const D3D11_SHADER_VERSION_TYPE& version)
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

	default:
		break;
		// assert! 잘못된 쉐이더 버전 참조 문제... 위 정의된 허용할수 있는 쉐이더 범위 넘어섬
		return PipeLineStage::Unknown;
	}
}


FORMAT D3D11TypeToVariableFormat(const D3D11_SHADER_TYPE_DESC& shaderTypeDesc)
{
	// 32bit integer format
	if (shaderTypeDesc.Type == D3D_SVT_INT)
	{
		switch (shaderTypeDesc.Columns)
		{
		case 1: return FORMAT::R32_INT;
		case 3: return FORMAT::R32G32B32_INT;
		default: 
			//assert! Unknown Columns Integer case 
			break;
		}
	}

	// 32bit unsigned integer format
	if (shaderTypeDesc.Type == D3D_SVT_UINT)
	{
		switch (shaderTypeDesc.Columns)
		{
		case 1: return FORMAT::R32_UINT;
		case 3: return FORMAT::R32G32B32_UINT;
		default:
			//assert! Unknown Columns Unsigned Integer case 
			break;
		}
	}

	// 32bit floating point format
	if (shaderTypeDesc.Type == D3D_SVT_FLOAT && shaderTypeDesc.Rows == 1)
	{
		switch (shaderTypeDesc.Columns)
		{
		case 1: return FORMAT::R32_FLOAT;
		case 2: return FORMAT::R32G32_FLOAT;
		case 3: return FORMAT::R32G32B32_FLOAT;
		case 4: return FORMAT::R32G32B32A32_FLOAT;

		default:
			// assert!  Unknown Columns Float case 
			break;
		}
	}

	// 16bit half floating point & short
	if (shaderTypeDesc.Type == D3D_SVT_FLOAT16)
	{
		switch (shaderTypeDesc.Columns)
		{
	
		case 1: return FORMAT::R16_FLOAT;
		case 2: return FORMAT::R16G16_FLOAT;
		case 3: return FORMAT::R16G16B16A16_FLOAT;
		default:

			// assert!  Unknown Columns 16bit Float case 
			break;
		}
	}

	if (shaderTypeDesc.Type == D3D_SVT_UINT16)
	{
		switch (shaderTypeDesc.Columns)
		{
		case 2: return FORMAT::R16G16_UNORM;
		case 3: return FORMAT::R16G16B16A16_UNORM;
		default:
			// assert!  Unknown Columns 16bit Integer(Short) case 
			break;
		}
	}

	//8bit norm RGBA or BGRA
	if (shaderTypeDesc.Type == D3D_SVT_UINT8 && shaderTypeDesc.Columns == 3)
	{
		return FORMAT::R8G8B8A8_UNORM;
	}
	else
	{
		// assert!  Unknown Columns 8bit norm RGBA or BGRA case 
	}

	//matrix 
	if (shaderTypeDesc.Type == D3D_SVT_FLOAT && shaderTypeDesc.Rows == 4 && shaderTypeDesc.Columns == 4)
	{
		return FORMAT::MATRIX4X4;
	}
	else
	{
		// assert!  Unknown size MATRIX case 
	}

	// 미지정 타입 사용 assert!


}

VARIABLE_CLASS D3D11ClassTOVariableClass(const D3D_SHADER_VARIABLE_CLASS& typeClass)
{
	switch (typeClass)
	{
	case D3D_SVC_SCALAR: return VARIABLE_CLASS::SCALAR;
	case D3D_SVC_VECTOR: return VARIABLE_CLASS::VECTOR;
	case D3D_SVC_MATRIX_ROWS: return VARIABLE_CLASS::MATRIX_ROWS;
	case D3D_SVC_MATRIX_COLUMNS: return VARIABLE_CLASS::MATRIX_COLUMNS;
	case D3D_SVC_OBJECT: return VARIABLE_CLASS::OBJECT;
	case D3D_SVC_STRUCT: return VARIABLE_CLASS::STRUCT;
	case D3D_SVC_INTERFACE_CLASS: return VARIABLE_CLASS::INTERFACE_CLASS;
	case D3D_SVC_INTERFACE_POINTER: return VARIABLE_CLASS::INTERFACE_POINTER;
	default:
		break;
	}
}

GlobalVariable ExtractVariableInfo(ID3D11ShaderReflectionVariable* d3dVariableReflection)
{

	// 동일 포맷 값인가 아닌가로 지정 scalar , vector , matrix 등은 모두 동일 포맷 변수로 
	// D3D11TypeToVariableFormat 에 의하여 그 Type 이 결정되어 저장되어짐
	// 단일 포맷 값이 아닌 struct , class 등은 재귀적인 구조로 다시 SearchVariable 를 호출하여 
	// 동일 포맷 변수 까지 탐색함 
	// 즉 Type 으로 leaf 여부를 결정함
	// 해당 완성된 GlobalVariable 가 만들어져서 최종 반환 될 수 있게 해줘야함 
	
	// 최상의 변수 정보 추출 후(Variable 초기 정보 추출)
	// 하위 탐색을 위한 TraverseVariableMember 호출 이후 최종 변수 담아 반환 해줘야함

	D3D11_SHADER_VARIABLE_DESC varDesc = {};
	d3dVariableReflection->GetDesc(&varDesc);
	ID3D11ShaderReflectionType* varReflectionType = d3dVariableReflection->GetType();

	// 최종 반환용 변수 정보
	GlobalVariable reVariable = {};

	//초기 변수 정보 입력 (D3D11_SHADER_VARIABLE_DESC 기반)
	reVariable.variableName = varDesc.Name;
	reVariable.baseOffset = varDesc.StartOffset;
	reVariable.offset = varDesc.StartOffset;
	reVariable.byteSize = varDesc.Size;
	
	
	//맴버 정보 추출
	D3D11_SHADER_TYPE_DESC varTypeDesc;
	varReflectionType->GetDesc(&varTypeDesc);
	reVariable.variableType.variableClass = D3D11ClassTOVariableClass(varTypeDesc.Class);

	//아직 CLASS 스펙을 다 이해 못해서 확실한 두가지만 넣음 
	// 향후 추가될 수 있음....
	if (reVariable.variableType.variableClass == VARIABLE_CLASS::OBJECT ||
		reVariable.variableType.variableClass == VARIABLE_CLASS::STRUCT)
	{
		reVariable.variableType.format = FORMAT::VOID0;
		reVariable.variableType.elementsCnt = varTypeDesc.Elements;
		reVariable.members.resize(varTypeDesc.Members);

		for (int i = 0; i < varTypeDesc.Members; i++)
		{

			reVariable.members[i] = TraverseVariableMember(
				varReflectionType->GetMemberTypeName(i),
				varReflectionType->GetMemberTypeByIndex(i),
				reVariable.baseOffset);
		}
	}
	else
	{
		reVariable.variableType.format = D3D11TypeToVariableFormat(varTypeDesc);
		reVariable.variableType.elementsCnt = (varTypeDesc.Elements > 0) ? varTypeDesc.Elements : 1;
		//reVariable.members size 0 상태
	}

	return reVariable;

}

GlobalVariable TraverseVariableMember(const char* memberName , ID3D11ShaderReflectionType* d3dVariableTypeReflection,const uint32_t baseOffset)
{
	// ID3D11ShaderReflectionType 을 받아 하위 구조에 대해 재귀적으로 탐색하여 
	// 하위 GlobalVariable 을 완성하여 반환

	// 반환용 하위 맴버 정보
	GlobalVariable reVariable = {};
	uint32_t totalSize = 0;

	D3D11_SHADER_TYPE_DESC varTypeDesc;
	d3dVariableTypeReflection->GetDesc(&varTypeDesc);

	//변수 정보 입력 (ID3D11ShaderReflectionType 기반)
	reVariable.variableName = memberName;
	reVariable.offset = varTypeDesc.Offset;
	reVariable.baseOffset = baseOffset;
	reVariable.variableType.variableClass = D3D11ClassTOVariableClass(varTypeDesc.Class);
	// 하위 맴버에 대해서는 사이즈 필드에 대한 비활성화
	reVariable.byteSize = GlobalVariable::INVALID_SIZE;

	// 하위 정보 입력
	//아직 CLASS 스펙을 다 이해 못해서 확실한 두가지만 넣음 
	// 향후 추가될 수 있음....
	/*
		아마 스펙 누락시 FORMAT 정보 가 없음 
		->FORMAT 기반으로 동작하는 바이트 사이즈 계산에서 버그 발생할 수 있음
	*/
	if (reVariable.variableType.variableClass == VARIABLE_CLASS::OBJECT ||
		reVariable.variableType.variableClass == VARIABLE_CLASS::STRUCT)
	{
		reVariable.variableType.format = FORMAT::VOID0;
		
		reVariable.variableType.elementsCnt = (varTypeDesc.Elements > 0) ? varTypeDesc.Elements : 1;
		reVariable.members.resize(varTypeDesc.Members);

		for (int i = 0; i < varTypeDesc.Members; i++)
		{
			reVariable.members[i] = TraverseVariableMember(
				d3dVariableTypeReflection->GetMemberTypeName(i),
				d3dVariableTypeReflection->GetMemberTypeByIndex(i),
				reVariable.baseOffset + reVariable.offset);
			
		}

		
	}
	else
	{
		reVariable.variableType.format = D3D11TypeToVariableFormat(varTypeDesc);
		reVariable.variableType.elementsCnt = (varTypeDesc.Elements > 0) ? varTypeDesc.Elements : 1;
		
		//reVariable.members size 0 상태
	}
	return reVariable;
}

uint32_t GetByteSizeFromMask(BYTE mask)
{
	BYTE componentMask = mask & 0x0F;

	if (componentMask <= 0x01)     
		return 4;
	else if (componentMask <= 0x03)
		return 8;
	else if (componentMask <= 0x07)
		return 12;
	else if (componentMask <= 0x0F)
		return 16;

	return 0;
}

DXGI_FORMAT ExtractDxgiFormat(D3D_REGISTER_COMPONENT_TYPE componentType, BYTE mask)
{
	// Mask의 하위 4비트만 사용 (x=1, y=2, z=4, w=8)
	BYTE componentMask = mask & 0x0F;

	switch (componentType)
	{
	case D3D_REGISTER_COMPONENT_FLOAT32:
		if (componentMask <= 0x01)      // x
			return DXGI_FORMAT_R32_FLOAT;
		else if (componentMask <= 0x03) // xy
			return DXGI_FORMAT_R32G32_FLOAT;
		else if (componentMask <= 0x07) // xyz
			return DXGI_FORMAT_R32G32B32_FLOAT;
		else if (componentMask <= 0x0F) // xyzw
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		break;

	case D3D_REGISTER_COMPONENT_UINT32:
		if (componentMask <= 0x01)
			return DXGI_FORMAT_R32_UINT;
		else if (componentMask <= 0x03)
			return DXGI_FORMAT_R32G32_UINT;
		else if (componentMask <= 0x07)
			return DXGI_FORMAT_R32G32B32_UINT;
		else if (componentMask <= 0x0F)
			return DXGI_FORMAT_R32G32B32A32_UINT;
		break;

	case D3D_REGISTER_COMPONENT_SINT32:
		if (componentMask <= 0x01)
			return DXGI_FORMAT_R32_SINT;
		else if (componentMask <= 0x03)
			return DXGI_FORMAT_R32G32_SINT;
		else if (componentMask <= 0x07)
			return DXGI_FORMAT_R32G32B32_SINT;
		else if (componentMask <= 0x0F)
			return DXGI_FORMAT_R32G32B32A32_SINT;
		break;

	default:
		break;
	}

	return DXGI_FORMAT_UNKNOWN;
}

eastl::string ToUtf8String(const wchar_t* wStr) {

	if (!wStr) return "";

	int size = WideCharToMultiByte(CP_UTF8, 0, wStr, -1, nullptr, 0, nullptr, nullptr);
	if (size <= 0) return "";

	eastl::string strTo(size - 1, 0);

	WideCharToMultiByte(CP_UTF8, 0, wStr, -1, &strTo[0], size, nullptr, nullptr);

	return strTo;
}