#pragma once
#ifdef RENDERING_DLL
#define RENDERING_API __declspec(dllexport)
#else
#define RENDERING_API __declspec(dllimport)
#endif