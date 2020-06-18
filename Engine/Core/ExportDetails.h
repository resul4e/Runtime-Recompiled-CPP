#pragma once
#include "FunctionDefinition.h"

#ifdef CORE_DLL
#if defined(WIN32) || defined(_WIN32)
#define CORE_API EXPORT_SHARED_LIBRARY_FUNCTION
#else
#define CORE_API
#endif
#else
#if defined(WIN32) || defined(_WIN32)
#define CORE_API IMPORT_SHARED_LIBRARY_FUNCTION
#else
#define CORE_API
#endif
#endif