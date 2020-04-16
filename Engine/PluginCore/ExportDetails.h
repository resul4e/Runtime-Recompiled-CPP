#pragma once
#ifdef PLUGIN_DLL
#if defined(WIN32) || defined(_WIN32)
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API
#endif
#else
#if defined(WIN32) || defined(_WIN32)
#define PLUGIN_API __declspec(dllimport)
#else
#define PLUGIN_API
#endif
#endif