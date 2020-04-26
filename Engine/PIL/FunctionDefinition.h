#if defined(WIN32) || defined(__WIN32)
#define FUNCTION_CDECL __cdecl
#else
#define FUNCION_CDECL
#endif

#if defined(WIN32) || defined(__WIN32)
#define EXPORT_SHARED_LIBRARY_FUNCTION __declspec(dllexport)
#define IMPORT_SHARED_LIBRARY_FUNCTION __declspec(dllimport)
#else
#define EXPORT_SHARED_LIBRARY_FUNCTION __attribute__((visibility("default")))
#define IMPORT_SHARED_LIBRARY_FUNCTION 
#endif