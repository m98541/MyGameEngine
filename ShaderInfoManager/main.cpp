/*
해당 툴에서 
랜더 페스에 대한 쉐이더 구성 및 
컴파일 통한 중간 표현식으로 변환 하여  
이후 엔진에 올리기 위한 메타데이터와 함께 
엔진으로 전달

이를 위해서 ShaderIO 프로젝트의 공용 쉐이더 규격을 맞춰줌과 동시에 
각각의 컴파일 과정은 툴의 특정 언어의 종속성을 피하기위해 
컴파일 과정 구현부는 API 격리가 필요함
*/

#include "CommandList.h"
#include "CommandManager.h"
#include "ShaderCompiler.h"
#include "../ShaderIO/RenderPassInfoIO.h"
#include "../ShaderIO/ShaderInfoIO.h"


#include <iostream>
#include <string>

void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    // MJEngine::MemoryManager::Allocate(size) 로 이후 메모리 관련 업데이트 이후 교체 필요!
    return malloc(size);
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    // 정렬(alignment)이 필요한 할당의 경우 (예: _aligned_malloc 등 사용)
#ifdef _MSC_VER
    return _aligned_malloc(size, alignment);
#else
    // POSIX 등 타 플랫폼에 맞는 aligned 할당
    void* ptr = nullptr;
    posix_memalign(&ptr, alignment, size);
    return ptr;
#endif
}



int main(void)
{
    std::string tempInput; // getline용 표준 string 유지
    eastl::string input;
    bool isRunning = true;

    /*
        먼저 렌더 패스만을 등록시킬 수 있는 형태로 개발
        이후 렌더 패스로만 등록 후 엔진에서 로드하여 
        바인딩 까지 성공하여 기능 점검이 된 후에 개별 쉐이더 핸들링 지원
    */
    CommandManager mainCmdManager;

    std::cout << "Shader Manager Program.\n";

    while (isRunning) {
        // 1.입력 시작 부분 출력
        std::cout << "> ";

        // 2. 입력(Read): 띄어쓰기를 포함한 한 줄 전체 읽기
        // 스트림 에러나 EOF(Ctrl+D/Ctrl+Z) 발생 시 루프 탈출
        if (!std::getline(std::cin , tempInput )) 
        {
            break;
        }
        input = tempInput.c_str();

        // 아무것도 입력하지 않고 엔터를 친 경우 무시
        if (input.empty()) 
        {
            continue;
        }

        if (input == CommandListStr[CommandEnum::EXIT]) 
        {
            std::cout << "program close\n";
            isRunning = false; // 루프 종료 플래그
        }
        else if (input == CommandListStr[CommandEnum::HELP]) 
        {
            std::cout << "-command list-\n";
            for (size_t i = 0; i < COMMAND_LIST_CNT; i++)
            {
                std::cout << CommandListStr[i] << "\n";
            }
            std::cout << "\n";
        } 
        else if (input == CommandListStr[CommandEnum::RENDER_PASS_LOAD])
        {
            
        }
        else if (input == CommandListStr[CommandEnum::REGISTER_RENDER_PASS])
        {
            ShaderProfileVersion renderPassProfile;

            // 아래에서 만든 쉐이더들 포인터를 임시적으로 들고 있는 RenderPass 
            CommandRenderPass tempRenderPass;
            bool successedPassProfileInput = false;
            do
            {
                std::cout << "Select Render Pass Profile Version (please input index number 1 2 3 ..)\n=>";
                for (size_t i = 0; i < ShaderProfileVersionCnt(); i++)
                {
                    std::cout << i << ":" << GetShaderProfileVersionString(static_cast<ShaderProfileVersion>(i)).c_str() << ", ";
                }
                std::cout << "\n" << ">>";
                int selectIndex;
                std::cin >> selectIndex;

                if (!std::cin.fail() && selectIndex >= 0 && selectIndex < ShaderProfileVersionCnt())
                {
                    successedPassProfileInput = true;
                    renderPassProfile = static_cast<ShaderProfileVersion>(selectIndex);
                }
                else
                {
                    std::cout << "fault input Please Retry Select Render Pass Profile Version.\n";
                }

                std::cin.clear();

            } while (!successedPassProfileInput); // 똑바로 입력 할때까지 시도하게 하기
            
            //파이프의 스테이지 별 쉐이더 입력 필수 쉐이더는 무조건 입력 나머지는 선택 입력
            for (size_t i = 0; i < PipeLineStageCnt(); i++)
            {
                std::cout << "input " << GetPipeLineStageString(static_cast<PipeLineStage>(i)).c_str() << "? (y/n) :";
               
                if (!std::getline(std::cin, tempInput))
                {
                    break;
                }
                eastl::string subInput = tempInput.c_str();

                
                if (subInput == "y" || subInput == "Y" || subInput == "yes" || subInput == "YES" || subInput == "Yes")
                {
                    eastl::string inputFilePath;
                    eastl::string entryPoint;
                    PipeLineStage stage = static_cast<PipeLineStage>(i);

                    bool inputSuccessed = true;

                    do
                    {
                        std::cout << "please input shader file info \n";
                        std::cout << "file path :";
                        if (!std::getline(std::cin, tempInput) || input.empty())
                        {
                            inputSuccessed = false;
                            std::cout << "fault input Please Retry.\n";
                        }
                        else
                        {
                            inputFilePath = tempInput.c_str();
                        }
                        
                    } while (!inputSuccessed);
                  
                    inputSuccessed = true;
                    do
                    {
                        std::cout << "please input shader entryPoint info \n";
                        std::cout << "entry Point :";
                        if (!std::getline(std::cin, tempInput) || input.empty())
                        {
                            inputSuccessed = false;
                            std::cout << "fault input Please Retry.\n";
                        }
                        else
                        {
                            entryPoint = tempInput.c_str();
                        }

                    } while (!inputSuccessed);

                    CommandShader shader(inputFilePath, entryPoint , stage , renderPassProfile);
                    tempRenderPass.SetRenderPassShader(shader);

                }
                else if ( PipeLineStage::Vertex == static_cast<PipeLineStage>(i) || PipeLineStage::Pixel == static_cast<PipeLineStage>(i))
                {
                    std::cout << "That Shader is Essential. please input 'y'\n";
                    i--;
                    continue;
                }
                else if (subInput == "n" || subInput == "N" || subInput == "no" || subInput == "NO" || subInput == "No")
                {
                    continue;
                }
                else
                {
                    // 입력안하거나 잘못된 입력시 다시 입력하게 함
                    i--;
                    continue;
                }
            }
            
            //tempRenderPass 에 넣어둔 쉐이더 정보 mainCmdManager의 테이블 에 등록

            mainCmdManager.RegisterRenderPass(
                tempRenderPass.GetRenderPassName(),
                tempRenderPass.GetRenderPassShader(PipeLineStage::Vertex),
                tempRenderPass.GetRenderPassShader(PipeLineStage::Hull),
                tempRenderPass.GetRenderPassShader(PipeLineStage::Domain),
                tempRenderPass.GetRenderPassShader(PipeLineStage::Geometry),
                tempRenderPass.GetRenderPassShader(PipeLineStage::Pixel)
            );


        }
        else if (input == CommandListStr[CommandEnum::VIEW_TABLE_RENDER_PASS])
        {

        }
        else if (input == CommandListStr[CommandEnum::VIEW_TABLE_SHADER])
        {

        }
        else if (input == CommandListStr[CommandEnum::SAVE_FILE])
        {


        }
        else 
        {
            // 4. 출력(Print): 그 외 커스텀 명령어 처리
            std::cout << "unkown command : " << input.c_str() << "'\n";
        }

     
    }

	return 0;
}