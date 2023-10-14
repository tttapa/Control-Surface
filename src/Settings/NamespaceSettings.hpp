#pragma once

#define USE_CS_NAMESPACE
#define CS_NAMESPACE_NAME cs

// ========================================================================== //

#ifdef USE_CS_NAMESPACE
#define BEGIN_CS_NAMESPACE namespace CS_NAMESPACE_NAME {
#define END_CS_NAMESPACE }
#define USING_CS_NAMESPACE using namespace CS_NAMESPACE_NAME
#else
#define BEGIN_CS_NAMESPACE
#define END_CS_NAMESPACE
#define USING_CS_NAMESPACE
#endif

#if __cplusplus >= 201402L
#define CS_DEPREC(msg) [[deprecated(msg)]]
#elif (defined(__GNUC__) && __GNUC__ > 6) || defined(__clang__)
#define CS_DEPREC(msg) __attribute__((deprecated(msg)))
#else
#define CS_DEPREC(...)
#endif