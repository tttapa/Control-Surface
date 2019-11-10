#pragma once

#define USE_AH_NAMESPACE
#define AH_NAMESPACE_NAME AH

// ========================================================================== //

#ifdef USE_AH_NAMESPACE
#define BEGIN_AH_NAMESPACE namespace AH_NAMESPACE_NAME {
#define END_AH_NAMESPACE }
#define USING_AH_NAMESPACE using namespace AH_NAMESPACE_NAME
#else
#define BEGIN_AH_NAMESPACE
#define END_AH_NAMESPACE
#define USING_AH_NAMESPACE
#endif