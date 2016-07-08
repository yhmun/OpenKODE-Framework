/* --------------------------------------------------------------------------
 *
 *      File            khr_float64.h
 *      Description     64-bit floating point data type and associated functions. 
 *      Version         7, 2008-09-12
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */

#ifndef __kd_KHR_float64_h_
#define __kd_KHR_float64_h_

#include <KD/kd.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef double                  KDfloat64KHR;

#define KD_E_KHR                2.7182818284590452350
#define KD_PI_KHR               3.1415926535897932390
#define KD_PI_2_KHR             1.5707963267948966190
#define KD_2PI_KHR              6.2831853071795864770
#define KD_LOG2E_KHR            1.4426950408889634070
#define KD_LOG10E_KHR           0.4342944819032518276
#define KD_LN2_KHR              0.6931471805599453094
#define KD_LN10_KHR             2.3025850929940456840
#define KD_PI_4_KHR             0.7853981633974483096
#define KD_1_PI_KHR             0.3183098861837906715
#define KD_2_PI_KHR             0.6366197723675813431
#define KD_2_SQRTPI_KHR         1.1283791670955125740
#define KD_SQRT2_KHR            1.4142135623730950490
#define KD_SQRT1_2_KHR          0.7071067811865475244
#define KD_DBL_EPSILON_KHR      2.2204460492503131e-16
#define KD_DBL_MAX_KHR          1.7976931348623157e+308
#define KD_DBL_MIN_KHR          2.2250738585072014e-308
#define KD_HUGE_VAL_KHR         KD_HUGE_VALF
#define KD_DEG_TO_RAD_KHR       0.01745329251994329577
#define KD_RAD_TO_DEG_KHR       57.29577951308232088
#define KD_DTOSTR_MAXLEN_KHR    25

// kdAcosKHR : Arc cosine function. 
KD_API KDfloat64KHR KD_APIENTRY kdAcosKHR ( KDfloat64KHR x );

// kdAsinKHR : Arc sine function.
KD_API KDfloat64KHR KD_APIENTRY kdAsinKHR ( KDfloat64KHR x );

// kdAtanKHR : Arc tangent function.
KD_API KDfloat64KHR KD_APIENTRY kdAtanKHR ( KDfloat64KHR x );

// kdAtan2KHR : Arc tangent function. 
KD_API KDfloat64KHR KD_APIENTRY kdAtan2KHR ( KDfloat64KHR y, KDfloat64KHR x );

// kdCosKHR : Cosine function. 
KD_API KDfloat64KHR KD_APIENTRY kdCosKHR ( KDfloat64KHR x );

// kdSinKHR : Sine function.
KD_API KDfloat64KHR KD_APIENTRY kdSinKHR ( KDfloat64KHR x );

// kdTanKHR : Tangent function.
KD_API KDfloat64KHR KD_APIENTRY kdTanKHR ( KDfloat64KHR x );

// kdExpKHR : Exponential function. 
KD_API KDfloat64KHR KD_APIENTRY kdExpKHR ( KDfloat64KHR x );

// kdLogKHR : Natural logarithm function.
KD_API KDfloat64KHR KD_APIENTRY kdLogKHR ( KDfloat64KHR x );

// kdFabsKHR : Absolute value.
KD_API KDfloat64KHR KD_APIENTRY kdFabsKHR ( KDfloat64KHR x );

// kdPowKHR : Power function.
KD_API KDfloat64KHR KD_APIENTRY kdPowKHR ( KDfloat64KHR x, KDfloat64KHR y );

// kdSqrtKHR : Square root function.
KD_API KDfloat64KHR KD_APIENTRY kdSqrtKHR ( KDfloat64KHR x );

// kdCeilKHR : Return ceiling value.
KD_API KDfloat64KHR KD_APIENTRY kdCeilKHR ( KDfloat64KHR x );

// kdFloorKHR : Return floor value.
KD_API KDfloat64KHR KD_APIENTRY kdFloorKHR ( KDfloat64KHR x );

// kdRoundKHR : Round value to nearest integer.
KD_API KDfloat64KHR KD_APIENTRY kdRoundKHR ( KDfloat64KHR x );

// kdInvsqrtKHR : Inverse square root function.
KD_API KDfloat64KHR KD_APIENTRY kdInvsqrtKHR ( KDfloat64KHR x );

// kdFmodKHR : Calculate floating point remainder.
KD_API KDfloat64KHR KD_APIENTRY kdFmodKHR ( KDfloat64KHR x, KDfloat64KHR y );

// kdStrtodKHR : Convert a string to a 64-bit floating point number.
KD_API KDfloat64KHR KD_APIENTRY kdStrtodKHR ( const KDchar* s, KDchar** endptr );

// kdDtostrKHR : Convert a 64-bit float to a string.
KD_API KDssize KD_APIENTRY kdDtostrKHR ( KDchar* buffer, KDsize buflen, KDfloat64KHR number );

// kdCopysignKHR :a value whose absolute value matches that of x, but whose sign bit matches that of y
// XMSoft's revision : Added API.
KD_API KDfloat64KHR KD_APIENTRY kdCopysignKHR ( KDfloat64KHR x, KDfloat64KHR y );

#define kdAcos			kdAcosKHR
#define kdAsin			kdAsinKHR
#define kdAtan			kdAtanKHR
#define kdAtan2			kdAtan2KHR
#define	kdCos			kdCosKHR
#define kdSin			kdSinKHR
#define kdTan			kdTanKHR
#define kdExp			kdExpKHR
#define kdLog			kdLogKHR
#define kdFabs			kdFabsKHR
#define	kdPow			kdPowKHR
#define kdSqrt			kdSqrtKHR
#define	kdCeil			kdCeilKHR
#define kdFloor			kdFloorKHR
#define kdRound			kdRoundKHR
#define	kdInvsqrt		kdInvsqrtKHR
#define kdFmod			kdFmodKHR
#define	kdStrtod		kdStrtodKHR
#define kdDtostr		kdDtostrKHR
#define	kdCopysign		kdCopysignKHR

#ifdef __cplusplus
}
#endif

#endif 

