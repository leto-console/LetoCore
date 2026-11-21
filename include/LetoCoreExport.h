
#pragma once

#ifdef LETOCORE_EXPORT_LIBRARY
#include <LetoAPI_V1/LetoAPI_V1_Version.h>
#define LETO_API_V1_USE LETO_API_V1_MINOR
#endif

#if defined(_WIN32) && defined(LETOCORE_SHARED)
    #ifdef LETOCORE_EXPORT_LIBRARY
        #define LETO_CORE_EXPORT __declspec(dllexport)
    #else
        #define LETO_CORE_EXPORT __declspec(dllimport)
    #endif
#elif ! defined(_WIN32) && (defined(__GNUC__) || defined(__clang__))
    #define LETO_CORE_EXPORT __attribute__((visibility("default")))
#else
    #define LETO_CORE_EXPORT
#endif
