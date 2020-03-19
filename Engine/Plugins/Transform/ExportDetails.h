#pragma once
#ifdef TRANSFORM_DLL
#define TRANSFORM_API __declspec(dllexport)
#else
#define TRANSFORM_API __declspec(dllimport)
#endif