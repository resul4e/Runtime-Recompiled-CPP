#pragma once
#ifdef RENDERING_DLL
#if defined(WIN32) || defined(_WIN32)
#define RENDERING_API __declspec(dllexport)
#else
#define RENDERING_API
#endif
#else
#if defined(WIN32) || defined(_WIN32)
#define RENDERING_API __declspec(dllimport)
#else
#define RENDERING_API
#endif
#endif