#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "StringFormatAssert.h"
void HandleAssertFailure(const char* file, const char* func, int line, const char* expression, const char* msg, ...)
{
	char msgBuffer[MAX_MSG_SIZE]; 
	printf("\nAssert! %s \\ %s \n", file, func);
	printf("LINE(%d) : %s\n", line, expression);
	va_list argPtr; 
	va_start(argPtr, msg); 
	int r = vsprintf(msgBuffer , msg, argPtr); 
	if (r != -1)
	{
		printf("%s", msgBuffer); 
	}
	else
	{
		printf("outDebugMsg Fail"); 
	}
	printf("\n"); 
	va_end(argPtr); 
	abort(); 
}
