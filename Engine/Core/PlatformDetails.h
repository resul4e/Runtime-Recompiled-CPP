#pragma once
#ifdef _DEBUG
#define PROJECT_CONFIGURATION "Debug"
#else
#define PROJECT_CONFIGURATION "Release"
#endif

#ifndef _WIN64
#define PROJECT_PLATFORM "32" 
#else 
#define PROJECT_PLATFORM "64"
#endif