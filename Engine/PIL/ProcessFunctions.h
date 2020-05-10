#pragma once
/**
 * \brief Define macros to open and close processes.
 */
#if defined(WIN32) || defined(__WIN32)
#define OPEN_SOME_PROCESS(aArguments, aModes) _popen(aArguments, aModes)
#define CLOSE_SOME_PROCESS(aProcess) _pclose(aProcess)
#else
#define OPEN_SOME_PROCESS(aArguments, aModes) popen(aArguments, aModes)
#define CLOSE_SOME_PROCESS(aProcess) pclose(aProcess)
#endif