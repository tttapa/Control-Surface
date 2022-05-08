set(COMMON_WARNINGS 
    -fdiagnostics-show-option
    -Wall
    -Wextra
    -pedantic
    -Wpedantic
    -pedantic-errors
    -Wdouble-promotion
    -Wswitch-default
    -Wswitch-enum
    -Wimplicit-fallthrough
    -Wuninitialized
    -Wno-missing-braces
)
set(GCC_WARNINGS
    -Wno-error=unused-but-set-variable
    -Wsuggest-override
    -Wno-error=attributes
)
set(CLANG_WARNINGS
    -Wno-error=unknown-warning-option
    -Wno-newline-eof
    -Wno-error=unused-but-set-variable
    -Winconsistent-missing-override
    -Wno-gnu-zero-variadic-macro-arguments
)
set(MSVC_WARNINGS
    /W4
    /wd4127 # conditional expression is constant
    /wd4458 # declaration of 'x' hides class member
    /permissive-
)
set(INTEL_WARNINGS 
    -Wall
    -Wextra
)
if (AH_WARNINGS_AS_ERRORS)
    if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        list(APPEND MSVC_WARNINGS /WX)
    else()
        list(APPEND COMMON_WARNINGS -Werror)
    endif()
endif()

add_library(warnings INTERFACE)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(warnings INTERFACE
        ${COMMON_WARNINGS} ${GCC_WARNINGS})
elseif (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(warnings INTERFACE
        ${COMMON_WARNINGS} ${CLANG_WARNINGS})
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(warnings INTERFACE
        ${MSVC_WARNINGS})
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    target_compile_options(warnings INTERFACE
        ${INTEL_WARNINGS})
else()
    message(WARNING "No known warnings for this compiler")
endif()
add_library(Arduino-Helpers::warnings ALIAS warnings)

set(CMAKE_DEBUG_POSTFIX "d")
