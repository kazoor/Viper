#ifdef VIPER_API
    #ifdef VIPER_EXPORT
        #define VIPER_API __declspec(dllexport)
    #else
        #define VIPER_API __declspec(dllimport)
    #endif
#else
    #error Unsupported type for build. see base.hpp / viper.
#endif