#pragma once
#ifdef AUDIO_DLL
#define AUDIO_API __declspec(dllexport)
#else
#define AUDIO_API __declspec(dllimport)
#endif