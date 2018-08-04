#ifndef __COMMON_H
#define __COMMON_H

#ifndef INCLUDED_MATH
#include <math.h>
#define INCLUDED_MATH
#endif

#if defined(__linux__)
#define DIRECTORY_SEPARATOR '/'
#else
#define DIRECTORY_SEPARATOR '\\'
#endif

#define REAL_SIZE 64

typedef unsigned int uint32;
typedef signed int int32;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned char uint8;
typedef signed char int8;
  
typedef float real32;
typedef double real64;

#if REAL_SIZE==32
typedef real32 real;

#define REAL_MIN     1.17549435E-38F
#define REAL_MAX     3.40282347E+38F
#define REAL_EPSILON 1.19209290E-07F

#elif REAL_SIZE==64
typedef real64 real;

#define REAL_MAX     1.7976931348623157E+308
#define REAL_MIN     2.2250738585072014E-308
#define REAL_EPSILON 2.2204460492503131E-016

#else
#error REAL_SIZE should be 32 or 64.
#endif

#define UINT32_MAX (4294967295UL)
#define UINT32_MIN (0)
#define UINT16_MAX (65535)
#define UINT16_MIN (0)
#define UINT8_MAX (255)
#define UINT8_MIN (0)

#define INT32_MAX (2147483647L)
#define INT32_MIN (-2147483648L)
#define INT16_MAX (32767)
#define INT16_MIN (-32768)
#define INT8_MAX (127)
#define INT8_MIN (-128)

#if !defined(M_PI)
#define M_PI 3.14159265358979323
#endif

#define foreach(it, vec) \
    for(typeof((vec).begin()) it=(vec).begin();it!=(vec).end();++it)

#endif
