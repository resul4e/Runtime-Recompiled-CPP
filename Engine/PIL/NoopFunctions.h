#if !defined(WIN32) && !defined(__WIN32)
#define __noop(aUnusedVariable) (void)(aUnusedVariable)
#endif