#pragma once

#define USE_CS_NAMESPACE

// ========================================================================== //

#ifdef USE_CS_NAMESPACE
#define BEGIN_CS_NAMESPACE namespace CS {
#define END_CS_NAMESPACE }
#else 
#define BEGIN_CS_NAMESPACE
#define END_CS_NAMESPACE
#endif