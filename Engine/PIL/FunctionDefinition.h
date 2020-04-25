#if defined(WIN32) || defined(__WIN32)
#define CDECL __cdecl
#else
#define CDECL
#endif