
#ifdef _WIN32
    #ifdef STM32F4_LIBRARIES_EXPORT
        #define LIBRARIES_EXPORT __declspec(dllexport)
    #else
        #define LIBRARIES_EXPORT __declspec(dllimport)
    #endif
#else
    #define LIBRARIES_EXPORT //__attribute__((visibility("default")))
#endif
