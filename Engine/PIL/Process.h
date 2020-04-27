#if defined(WIN32) || defined(__WIN32)
#define OPEN_PROCESS(aArguments, aModes) _popen(aArguments, aModes)
#define CLOSE_PROCESS(aProcess) _pclose(aProcess)
#else
#define OPEN_PROCESS(aArguments, aModes) popen(aArguments, aModes)
#define CLOSE_PROCESS(aProcess) pclose(aProcess)
#endif