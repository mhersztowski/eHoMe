#ifndef BASE_H
#define BASE_H

#include <stddef.h>
#include <stdio.h>

#include <time.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <assert.h>

#if defined(_WIN32) && !defined(__MINGW32__)

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;  // NOLINT
typedef unsigned short uint16_t;  // NOLINT
typedef int int32_t;
typedef unsigned int uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
// intptr_t and friends are defined in crtdefs.h through stdio.h.

#else

#include <stdint.h>

#endif

#ifdef _WIN32
    #define INLINE		__forceinline
    #define FASTCALL	__fastcall
    #define RESTRICT	__restrict
#elif _ANDROID
    #define INLINE		__inline__
    #define FASTCALL
    #define RESTRICT	__restrict
#else
    #define INLINE		__inline__ __attribute__ ((always_inline))
    #define FASTCALL
    #define RESTRICT	__restrict
#endif

#endif // BASE_H
