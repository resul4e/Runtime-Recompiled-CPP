#pragma once
#ifdef AUDIO_DLL
#if defined(WIN32) || defined(_WIN32)
#define AUDIO_API __declspec(dllexport)
#else
#define AUDIO_API
#endif
#else
#if defined(WIN32) || defined(_WIN32)
#define AUDIO_API __declspec(dllimport)
#else
#define AUDIO_API
#endif
#endif