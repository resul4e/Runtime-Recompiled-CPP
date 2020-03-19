#pragma once
#ifdef PHYSICS_DLL
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif