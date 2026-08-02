
#ifdef _WIN32
    #ifdef LETOCORE_EXPORT_LIBRARY
        #define LETO_CORE_EXPORT __declspec(dllexport)
    #else
        #define LETO_CORE_EXPORT __declspec(dllimport)
    #endif
#else
    #define LETO_CORE_EXPORT //__attribute__((visibility("default")))
#endif
