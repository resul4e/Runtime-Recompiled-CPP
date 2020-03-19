#pragma once
#ifdef INPUT_DLL
#define INPUT_API __declspec(dllexport)
#else
#define INPUT_API __declspec(dllimport)
#endif