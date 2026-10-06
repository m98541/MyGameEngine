#ifndef STRING_FORMAT_ASSERT
#define STRING_FORMAT_ASSERT

#define MAX_MSG_SIZE 512

void HandleAssertFailure (const char* file, const char* func, int line, const char* expression, const char* msg, ...);

#if __cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
#define SF_ASSERTE(expression,msg,...) \
	do { \
		if(!(expression))\
		{\
			HandleAssertFailure(__FILE__, __func__, __LINE__, #expression, msg __VA_OPT__(,) __VA_ARGS__);\
		}\
	} while (0)
#elif defined(__GNUC__) || defined(__clang__)
#define SF_ASSERTE(expression,msg,...) \
	do { \
		if(!(expression))\
		{\
			HandleAssertFailure(__FILE__, __func__, __LINE__, #expression, msg , ##__VA_ARGS__);\
		}\
	} while (0)
#else 
#define SF_ASSERTE(expression,msg,...)\
	do { \
		if(!(expression))\
		{\
			HandleAssertFailure(__FILE__, __func__, __LINE__, #expression, msg , __VA_ARGS__);\
		}\
	} while (0)
#endif

#endif // !STRING_FORMAT_ASSERT
