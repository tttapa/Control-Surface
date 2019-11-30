#define COUNT(...) COUNT_HELPER(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define COUNT_HELPER(N1, N2, N3, N4, N5, N6, N7, N8, N9, N10, N, ...) N

#define DEBUGVALN(N) DEBUGVALN_HELPER(N)
#define DEBUGVALN_HELPER(N) DEBUGVAL##N

#define DEBUGVAL10(x, ...)                                                     \
    do {                                                                       \
        DEBUG_OUT << NAMEDVALUE(x) << ", ";                                    \
        DEBUGVAL9(__VA_ARGS__);                                                \
    } while (0)
#define DEBUGVAL9(x, ...)                                                      \
    do {                                                                       \
        DEBUG_OUT << NAMEDVALUE(x) << ", ";                                    \
        DEBUGVAL8(__VA_ARGS__);                                                \
    } while (0)
#define DEBUGVAL8(x, ...)                                                      \
    do {                                                                       \
        DEBUG_OUT << NAMEDVALUE(x) << ", ";                                    \
        DEBUGVAL7(__VA_ARGS__);                                                \
    } while (0)
#define DEBUGVAL7(x, ...)                                                      \
    do {                                                                       \
        DEBUG_OUT << NAMEDVALUE(x) << ", ";                                    \
        DEBUGVAL6(__VA_ARGS__);                                                \
    } while (0)
#define DEBUGVAL6(x, ...)                                                      \
    do {                                                                       \
        DEBUG_OUT << NAMEDVALUE(x) << ", ";                                    \
        DEBUGVAL5(__VA_ARGS__);                                                \
    } while (0)
#define DEBUGVAL5(x, ...)                                                      \
    do {                                                                       \
        DEBUG_OUT << NAMEDVALUE(x) << ", ";                                    \
        DEBUGVAL4(__VA_ARGS__);                                                \
    } while (0)
#define DEBUGVAL4(x, ...)                                                      \
    do {                                                                       \
        DEBUG_OUT << NAMEDVALUE(x) << ", ";                                    \
        DEBUGVAL3(__VA_ARGS__);                                                \
    } while (0)
#define DEBUGVAL3(x, ...)                                                      \
    do {                                                                       \
        DEBUG_OUT << NAMEDVALUE(x) << ", ";                                    \
        DEBUGVAL2(__VA_ARGS__);                                                \
    } while (0)
#define DEBUGVAL2(x, ...)                                                      \
    do {                                                                       \
        DEBUG_OUT << NAMEDVALUE(x) << ", ";                                    \
        DEBUGVAL1(__VA_ARGS__);                                                \
    } while (0)
#define DEBUGVAL1(x)                                                           \
    do {                                                                       \
        DEBUG_OUT << NAMEDVALUE(x) << DEBUG_ENDL;                              \
    } while (0)
