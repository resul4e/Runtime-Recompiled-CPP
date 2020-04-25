#if defined(WIN32) || defined(__WIN32)
#define FUNCTION_CDECL __cdecl
#else
#define FUNCION_CDECL
#endif