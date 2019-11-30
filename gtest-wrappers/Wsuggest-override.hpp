// I prefer to have my warnings turned up pretty high, and I think the
// `-Wsuggest-override` is pretty useful. It warns you when you are creating a
// function that overrides a virtual function of the super class.
// That warning in itself is useful, but it's even more useful because it
// enforces writing `override` after all functions that are meant to override
// other functions. This leaves no room for bugs where you expect a function
// to override a parent function, but it doesn't (maybe because of a typo in
// the function name, different capitalization, or different signature).
//
// There are some places in the Google Test & Mock libraries where the
// `override` keyword is missing, so we have to temporarily disable the warning
// in these parts of the code.

#if __GNUC__ >= 5 && !defined(__clang__)
#define W_SUGGEST_OVERRIDE_OFF                                                 \
    _Pragma("GCC diagnostic push")                                             \
        _Pragma("GCC diagnostic ignored \"-Wsuggest-override\"")
#elif defined(__clang__)
#define W_SUGGEST_OVERRIDE_OFF                                                 \
    _Pragma("clang diagnostic push") _Pragma(                                  \
        "clang diagnostic ignored \"-Winconsistent-missing-override\"")
#endif

#if __GNUC__ >= 5 && !defined(__clang__)
#define W_SUGGEST_OVERRIDE_ON _Pragma("GCC diagnostic pop")
#elif defined(__clang__)
#define W_SUGGEST_OVERRIDE_ON _Pragma("clang diagnostic pop")
#endif
