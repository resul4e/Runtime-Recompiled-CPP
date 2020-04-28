/**
 * \brief Define cdecl for windows
 */
#if defined(WIN32) || defined(__WIN32)
#define FUNCTION_CDECL __cdecl
#else
#define FUNCTION_CDECL
#endif

/**
 * \brief Define export and import macros
 */
#if defined(WIN32) || defined(__WIN32)
#define EXPORT_SHARED_LIBRARY_FUNCTION __declspec(dllexport)
#define IMPORT_SHARED_LIBRARY_FUNCTION __declspec(dllimport)
#else
#define EXPORT_SHARED_LIBRARY_FUNCTION __attribute__((visibility("default")))
#define IMPORT_SHARED_LIBRARY_FUNCTION 
#endif