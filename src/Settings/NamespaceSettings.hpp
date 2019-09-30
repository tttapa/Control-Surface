#pragma once

#define USE_CS_NAMESPACE
#define CS_NAMESPACE_NAME CS

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