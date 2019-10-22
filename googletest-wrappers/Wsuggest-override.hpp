#if __GNUC__ >= 5
#define W_SUGGEST_OVERRIDE_OFF                                                 \
    _Pragma("GCC diagnostic push")                                             \
        _Pragma("GCC diagnostic ignored \"-Wsuggest-override\"")
#elif defined(__clang__)
#define W_SUGGEST_OVERRIDE_OFF                                              \
    _Pragma("clang diagnostic push") _Pragma(                                  \
        "clang diagnostic ignored \"-Winconsistent-missing-override\"")
#endif

#if __GNUC__ >= 5
#define W_SUGGEST_OVERRIDE_ON _Pragma("GCC diagnostic pop")
#elif defined(__clang__)
#define W_SUGGEST_OVERRIDE_ON _Pragma("clang diagnostic pop")
#endif
