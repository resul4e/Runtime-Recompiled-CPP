#pragma once

#if defined(_MSC_VER)
    #if defined(SHAREDLIBRARYTESTDLL_EXPORTS)
        #define SHARED_LIB_EXPORT_IMPORT __declspec(dllexport)
    #else
        #define SHARED_LIB_EXPORT_IMPORT __declspec(dllimport)
    #endif
#elif defined(__GNUC__)
    #if defined(SHAREDLIBRARYTESTDLL_EXPORTS)
        #define SHARED_LIB_EXPORT_IMPORT __attribute__((visibility("default")))
    #else
        #define SHARED_LIB_EXPORT_IMPORT
    #endif
#else
    //  do nothing and hope for the best?
    #define SHARED_LIB_EXPORT_IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

extern "C" SHARED_LIB_EXPORT_IMPORT int TestFunction(int aTestParam);