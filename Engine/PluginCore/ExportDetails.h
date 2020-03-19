#pragma once
#ifdef PLUGIN_DLL
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API __declspec(dllimport)
#endif