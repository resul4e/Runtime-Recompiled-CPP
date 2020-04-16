#pragma once
#ifdef INPUT_DLL
#if defined(WIN32) || defined(_WIN32)
#define INPUT_API __declspec(dllexport)
#else
#define INPUT_API
#endif
#else
#if defined(WIN32) || defined(_WIN32)
#define INPUT_API __declspec(dllimport)
#else
#define INPUT_API
#endif
#endif