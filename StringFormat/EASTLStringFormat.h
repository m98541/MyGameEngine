#ifndef EASTL_STRING_FORMAT
#define EASTL_STRING_FORMAT

#include <EASTL/string.h>
/*
현재 따로 힙 allocator 및 메모리 메니저 구성이 없는 관계로
정적인 스텍 메모리 문자열 부여 일단 용도에 따라 사용하도록 32 , 128 , 256 바이트 구성
향후 힙 관리 모듈 개발 후 동적으로 할당(이미 할당 받은 메모리에서) 받을 수 있는 형태로 가변 문자열 크기의 힙 메모리 사용하도록 구조 추가 필요
*/
eastl::string EASTLsprintf32(const char* msg, ...);
eastl::string EASTLsprintf128(const char* msg, ...);
eastl::string EASTLsprintf256(const char* msg, ...);
#endif // !EASTL_STRING_FORMAT
