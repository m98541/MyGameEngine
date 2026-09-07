#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#define COMMAND_MAX_CHAR 30
#define COMMAND_LIST_CNT 7

enum CommandEnum {
    EXIT,
    HELP,
    RENDER_PASS_LOAD,
    REGISTER_RENDER_PASS,
    VIEW_TABLE_RENDER_PASS,
    VIEW_TABLE_SHADER,
    SAVE_FILE
};

constexpr char CommandListStr[COMMAND_LIST_CNT][COMMAND_MAX_CHAR] = {
       "EXIT",
       "HELP",
       "RENDER_PASS_LOAD",
       "REGISTER_RENDER_PASS", // 쉐이더 테이블에 등록 하면서 패스 지정
       "VIEW_TABLE_RENDER_PASS",
       "VIEW_TABLE_SHADER",
       "SAVE_FILE"
       //"shaderLoad",
       //"registerShader", // 쉐이더만 테이블에 등록
       //"registerPass", // 등록되어진 쉐이더 테이블 기반으로 패스 지정하여 이를 등록
       // 해당 명령 이전까지는 커멘드 쉐이더 , 커멘드 페스 형태로만 가지고 있다가 해당 명령시 
       // 컴파일 -> 정보 추출 -> shaderInfo 테이블 형성 -> RenderPass 테이블 형성 -> shader , renderPass IO 통한 저장 일어남
};

#endif // !COMMANDLIST_H
