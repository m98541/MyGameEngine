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
#include <iostream>
#include "ShaderCompiler.h"
#include "../ShaderIO/RenderPassInfoIO.h"
#include "../ShaderIO/ShaderInfoIO.h"

int main(void)
{
    std::string input;
    bool isRunning = true;

    constexpr uint8_t rootComandListCnt = 10;
    std::string rootComandList[rootComandListCnt] = {
        "exit",
        "shaderLoad",
        "passLoad",
        "registerShader", // 쉐이더만 테이블에 등록
        "registerPass", // 등록되어진 쉐이더 테이블 기반으로 패스 지정하여 이를 등록
        "registerShaderAndPass", // 쉐이더 테이블에 등록 하면서 패스 지정
        "veiwShaderTable",
        "veiwPassTable",
        "saveShaderAndPassTable" 
        // 해당 명령 이전까지는 커멘드 쉐이더 , 커멘드 페스 형태로만 가지고 있다가 해당 명령시 
        // 컴파일 -> 정보 추출 -> shaderInfo 테이블 형성 -> RenderPass 테이블 형성 -> shader , renderPass IO 통한 저장 일어남


    };

    std::cout << "Shader Manager Program.\n";

    while (isRunning) {
        // 1.입력 시작 부분 출력
        std::cout << "> ";

        // 2. 입력(Read): 띄어쓰기를 포함한 한 줄 전체 읽기
        // 스트림 에러나 EOF(Ctrl+D/Ctrl+Z) 발생 시 루프 탈출
        if (!std::getline(std::cin, input)) {
            break;
        }

        // 아무것도 입력하지 않고 엔터를 친 경우 무시
        if (input.empty()) {
            continue;
        }

        if (input == "exit" || input == "quit") {
            std::cout << "program close\n";
            isRunning = false; // 루프 종료 플래그
        }
        else if (input == "help") {
            std::cout << "command list : help, exit, quit\n";
        }
        else {
            // 4. 출력(Print): 그 외 커스텀 명령어 처리
            std::cout << "unkown command : " << input << "'\n";
        }
    }

	return 0;
}