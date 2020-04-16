#pragma once
#ifdef CORE_DLL
#if defined(WIN32) || defined(_WIN32)
#define CORE_API __declspec(dllexport)
#else
#define CORE_API
#endif
#else
#if defined(WIN32) || defined(_WIN32)
#define CORE_API __declspec(dllimport)
#else
#define CORE_API
#endif
#endif