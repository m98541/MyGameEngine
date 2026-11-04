#include "EASTLStringFormat.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


eastl::string EASTLsprintf32(const char* msg, ...)
{
	char buffer[32];

	va_list argPtr;
	va_start(argPtr, msg);

	vsprintf_s(buffer,32,msg,argPtr);

	va_end(argPtr);

	return eastl::string(buffer);

}
eastl::string EASTLsprintf128(const char* msg, ...)
{
	char buffer[128];

	va_list argPtr;
	va_start(argPtr, msg);

	vsprintf_s(buffer, 128, msg, argPtr);

	va_end(argPtr);

	return eastl::string(buffer);

}

eastl::string EASTLsprintf256(const char* msg, ...)
{
	char buffer[256];

	va_list argPtr;
	va_start(argPtr, msg);

	vsprintf_s(buffer, 256, msg, argPtr);

	va_end(argPtr);

	return eastl::string(buffer);

}

