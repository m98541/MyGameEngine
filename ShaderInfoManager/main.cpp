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
#include "../StringFormat/StringFormatAssert.h"
#include <iostream>
#include <string>
#include <limits>

void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    // MJEngine::MemoryManager::Allocate(size) 로 이후 메모리 관련 업데이트 이후 교체 필요!
    return malloc(size);
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
    // 정렬(alignment)이 필요한 할당의 경우 (예: _aligned_malloc 등 사용)
#ifdef _MSC_VER
    return malloc(size);
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
            bool inputSuccessed = true;
            std::cin.clear();
            eastl::string passPath;
            do
            {
                std::cout << "please input Render Pass Path\n";
                std::cout << "RenderPass Path :";
                if (!std::getline(std::cin, tempInput) || input.empty())
                {
                    inputSuccessed = false;
                    std::cout << "fault input Please Retry.\n";
                }
                else
                {
                    passPath = tempInput.c_str();
                    mainCmdManager.RenderPassFileLoad(passPath);

                }

            } while (!inputSuccessed);

        }
        else if (input == CommandListStr[CommandEnum::REGISTER_RENDER_PASS])
        {
            ShaderProfileVersion renderPassProfile;

            bool successedShaderPlatformInput = false;
            do
            {
                std::cout << "Select Shader Platform \n=>";
                for (uint8_t i = 1; i < static_cast<uint8_t>(ShaderPlatform::COUNT); i++)
                {
                    std::cout << GetShaderPlatformTag(static_cast<ShaderPlatform>(i)).c_str() <<"[" << (int)i << "]" << ", ";
                }
                std::cout << "\n" << ">>";
                int selectIndex;
                std::cin >> selectIndex;

                if (!std::cin.fail() && selectIndex >= 0 && selectIndex < static_cast<uint8_t>(ShaderPlatform::COUNT))
                {
                    successedShaderPlatformInput = true;
                    renderPassProfile.shaderPlatform = static_cast<ShaderPlatform>(selectIndex);
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                else
                {
                    std::cout << "fault input Please Retry Select Shader Platform.\n";
                }

                std::cin.clear();

            } while (!successedShaderPlatformInput); // 똑바로 입력 할때까지 시도하게 하기
            

            bool majorSuccessed = false;
            bool minorSuccessed = false;
            int majorVer, minorVer;

            do
            {   
                std::cout << "Please Input Shader Version \n";
             
                
                if (!majorSuccessed)
                {
                    majorSuccessed = true;
                    std::cout << "major version :";
                    std::cin >> majorVer;
                    if (std::cin.fail())
                    {
                        majorSuccessed = false;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "fault input Please Retry Input Major Version.\n";
                        continue;
                    }
                }
                
                if (!minorSuccessed)
                {
                    minorSuccessed = true;
                    std::cout << std::endl << "minor version :";
                    std::cin >> minorVer;
                    
                    if (std::cin.fail())
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        minorSuccessed = false;
                        std::cout << "fault input Please Retry Input Minor Version.\n";
                    }
      
                }
      
            } while (!minorSuccessed && !majorSuccessed);

            renderPassProfile.majorVer = (uint8_t)majorVer;
            renderPassProfile.minorVer = (uint8_t)minorVer;
            

            //페스 이름 지정
            bool inputSuccessed = true;
            std::cin.ignore();
            
            eastl::string passName;

            do
            {
                std::cout << "please input Render Pass Name\n";
                std::cout << "pass name :";
                if (!std::getline(std::cin, tempInput) || tempInput.empty())
                {
                    inputSuccessed = false;
                    std::cout << "fault input Please Retry.\n";
                }
                else
                {
                    inputSuccessed = true;
                    passName = tempInput.c_str();
                }

            } while (!inputSuccessed);

            
            // 아래에서 만든 쉐이더들 포인터를 임시적으로 들고 있는 RenderPass 
            CommandRenderPass tempRenderPass(passName);
            CommandShader tempRenderPassShaderData[static_cast<uint8_t>(PipeLineStage::COUNT)];

            //파이프의 스테이지 별 쉐이더 입력 필수 쉐이더는 무조건 입력 나머지는 선택 입력
            for (size_t i = 1; i < PipeLineStageCnt(); i++)
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
                            inputSuccessed = true;
                            inputFilePath = tempInput.c_str();
                        }
                        
                    } while (!inputSuccessed);
                  
                    inputSuccessed = true;
                    do
                    {
                        std::cout << "please input shader entryPoint info \n";
                        std::cout << "entry Point :";
                        if (!std::getline(std::cin, tempInput) || tempInput.empty())
                        {
                            inputSuccessed = false;
                            std::cout << "fault input Please Retry.\n";
                        }
                        else
                        {
                            inputSuccessed = true;
                            entryPoint = tempInput.c_str();
                        }

                    } while (!inputSuccessed);

                    CommandShader shader(inputFilePath, entryPoint , stage , renderPassProfile);
                    tempRenderPassShaderData[i] = shader;
                    tempRenderPass.SetRenderPassShader(&tempRenderPassShaderData[i]);

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

            bool regPassSuccess =  mainCmdManager.RegisterRenderPass(
              tempRenderPass
            );

            if (!regPassSuccess)
            {
                std::cout << "RenderPassRegister Fail\n";
            }


        }
        else if (input == CommandListStr[CommandEnum::VIEW_TABLE_RENDER_PASS])
        {
            eastl::vector<eastl::string> passNames = mainCmdManager.GetRenderPassNames();

            std::cout << "-RenderPass Table List- \n";
            for (eastl::string name : passNames)
            {
                std::cout << name.c_str() << "\n";
            }
        }
        else if (input == CommandListStr[CommandEnum::VIEW_TABLE_SHADER])
        {

            std::cout << "-Shader Table List- \n";
            size_t pipeCnt = PipeLineStageCnt();
            for (size_t i = 0; i < pipeCnt; i++)
            {
                PipeLineStage stage = static_cast<PipeLineStage>(i);
                eastl::string pipeLineStr = GetPipeLineStageString(stage);
                eastl::vector<eastl::string> nameTable = mainCmdManager.GetStageShaderNamesTable(stage);

                std::cout << pipeLineStr.c_str() << ":";

                for (eastl::string name : nameTable)
                {
                    std::cout << name.c_str() << " ,";
                }
                std::cout << "\n";

            }
        }
        else if (input == CommandListStr[CommandEnum::SAVE_FILE])
        {
            eastl::string fileName;
            std::cout << "-SAVE FILE- \n";
            bool inputSuccessed = true;
            do
            {
                std::cout << "file name :";
                if (!std::getline(std::cin, tempInput) || tempInput.empty())
                {
                    inputSuccessed = false;
                    std::cout << "fault input Please Retry.\n";
                }
                else
                {
                    inputSuccessed = true;
                    fileName = tempInput.c_str();
                }

            } while (!inputSuccessed);

            mainCmdManager.RenderPassFileCompileAndSave("ShaderResource",fileName + ".pass", fileName + ".ShaderTable", ShaderCompiler::API::D3D11);
            
        }
        else if (input == CommandListStr[CommandEnum::REGISTER_MATERIAL_TAG])
        {
            std::cout << "- REGISTER_MATERIAL_TAG\n";

            int selectInputType;
            static constexpr const int SINGLE = 1;
            static constexpr const int GROUP = 2;
            MaterialTagGroup inputGroup;
            bool inputSuccessed = false;
            do
            {
                std::cout << "- SELECT TYPE : 1.SINGLE 2.GROUP\n";
                std::cout << "\n" << ">>";
                std::cin >> selectInputType;
                if (std::cin.fail() || (selectInputType != SINGLE && selectInputType != GROUP))
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    inputSuccessed = false;
                    std::cout << "fault input Please Retry SELECT TYPE \n";
                }
                else
                    inputSuccessed = true;


            } while (!inputSuccessed);

            if (selectInputType == SINGLE)
            {
                //single 의 경우 단일 테그 이름이 곧 그룹 이름으로 통일
                //테그가 1개짜리 그룹으로 만듬
                eastl::string tagName;
                eastl::string passName;

                bool inputSuccessed = false;
                do
                {
                    std::cout << "- Please Input Tag Name\n";
                    std::cout << "tag name :";
                    if (!std::getline(std::cin, tempInput) || tempInput.empty())
                    {
                        inputSuccessed = false;
                        std::cout << "fault input Please Retry.\n";
                    }
                    else
                    {
                        inputSuccessed = true;
                        tagName = tempInput.c_str();
                    }


                } while (!inputSuccessed);

                inputSuccessed = false;
                int selectRenderPass = -1;
                eastl::vector<eastl::string> renderPassNames = mainCmdManager.GetRenderPassNames();
                do
                {
                    std::cout << "- SELECT RenderPass :";
                    int num = 0;
                    for (const auto& name : renderPassNames)
                    {
                        std::cout << '[' << num << ']' << name.c_str() << ',';
                        num++;
                    }

                    std::cout << "\n" << ">>";
                    std::cin >> selectRenderPass;
                    if (std::cin.fail() || selectRenderPass < 0 || selectInputType > renderPassNames.size())
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        inputSuccessed = false;
                        std::cout << "fault input Please Retry SELECT RenderPass\n";
                    }
                    else
                    {
                        passName = renderPassNames[selectRenderPass];
                        inputSuccessed = true;
                    }
                    

                } while (!inputSuccessed);

                inputGroup.tagGroupId = tagName;
                inputGroup.RegisterMaterialTag(tagName , passName);

            }
            else if (selectInputType == GROUP)
            {
                eastl::string groupName;
                bool inputSuccessed = false;
                do
                {
                    std::cout << "- Please Input Tag Group Name\n";
                    std::cout << "tag group name :";
                    if (!std::getline(std::cin, tempInput) || tempInput.empty())
                    {
                        inputSuccessed = false;
                        std::cout << "fault input Please Retry.\n";
                    }
                    else
                    {
                        inputSuccessed = true;
                        groupName = tempInput.c_str();
                    }


                } while (!inputSuccessed);
                inputGroup.tagGroupId = groupName;

                bool addTag = true;

                while (addTag)
                {
                    eastl::string tagName;
                    eastl::string passName;

                    bool inputSuccessed = false;
                    do
                    {
                        std::cout << "- Please Input Tag Name\n";
                        std::cout << "tag name :";
                        if (!std::getline(std::cin, tempInput) || tempInput.empty())
                        {
                            inputSuccessed = false;
                            std::cout << "fault input Please Retry.\n";
                        }
                        else
                        {
                            inputSuccessed = true;
                            tagName = tempInput.c_str();
                        }


                    } while (!inputSuccessed);

                    inputSuccessed = false;
                    int selectRenderPass = -1;
                    eastl::vector<eastl::string> renderPassNames = mainCmdManager.GetRenderPassNames();
                    do
                    {
                        std::cout << "- SELECT RenderPass :";
                        int num = 0;
                        for (const auto& name : renderPassNames)
                        {
                            std::cout << '[' << num << ']' << name.c_str() << ',';
                            num++;
                        }

                        std::cout << "\n" << ">>";
                        std::cin >> selectRenderPass;
                        if (std::cin.fail() || selectRenderPass < 0 || selectInputType > renderPassNames.size())
                        {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            inputSuccessed = false;
                            std::cout << "fault input Please Retry SELECT RenderPass\n";
                        }
                        else
                        {
                            passName = renderPassNames[selectRenderPass];
                            inputSuccessed = true;
                        }


                    } while (!inputSuccessed);
                    inputGroup.RegisterMaterialTag(tagName, passName);

                    inputSuccessed = false;
                    eastl::string subInput;
                    do
                    {
                        std::cout << "add input Material Tag? (y/n):";
                        if (!std::getline(std::cin, tempInput) || tempInput.empty() || 
                            !(
                                tempInput == "yes"|| tempInput == "y" || tempInput == "Y" || tempInput == "YES" ||
                                tempInput == "no" || tempInput == "n" || tempInput == "N" || tempInput == "NO"
                            ))
                        {
                            inputSuccessed = false;
                            std::cout << "fault input Please Retry.\n";
                        }
                        else
                        {
                            inputSuccessed = true;
                            subInput = tempInput.c_str();
                        }


                    } while (!inputSuccessed);
                    
                    if (subInput == "yes" || subInput == "y" || subInput == "Y" || subInput == "YES")
                    {
                        addTag = true;
                    }
                    else if(subInput == "no" || subInput == "n" || subInput == "N" || subInput == "NO")
                    {
                        addTag = false;
                    }
                }


            }
            else
            {
                SF_ASSERTE(false , " [material tag input] unknown type input selectInputType: %d ", selectInputType);
            }

            mainCmdManager.RegisterMaterialTagGroup(inputGroup);
        }
        else 
        {
            // 4. 출력(Print): 그 외 커스텀 명령어 처리
            std::cout << "unkown command : " << input.c_str() << "'\n";
        }

     
    }

	return 0;
}


