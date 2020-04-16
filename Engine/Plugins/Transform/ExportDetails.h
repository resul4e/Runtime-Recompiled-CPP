#pragma once
#ifdef TRANSFORM_DLL
#if defined(WIN32) || defined(_WIN32)
#define TRANSFORM_API __declspec(dllexport)
#else
#define TRANSFORM_API
#endif
#else
#if defined(WIN32) || defined(_WIN32)
#define TRANSFORM_API __declspec(dllimport)
#else
#define TRANSFORM_API
#endif
#endif