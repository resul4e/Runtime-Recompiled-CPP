#pragma once
#ifdef PHYSICS_DLL
#if defined(WIN32) || defined(_WIN32)
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API
#endif
#else
#if defined(WIN32) || defined(_WIN32)
#define PHYSICS_API __declspec(dllimport)
#else
#define PHYSICS_API
#endif
#endif