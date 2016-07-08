/* --------------------------------------------------------------------------
 *
 *      File            neon_matrix_impl.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2009 Justin Saunders
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
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

#include "Precompiled.h"
#include "kazmath/neon_matrix_impl.h"

#if defined(__ARM_NEON__)

void NEON_Matrix4Mul(const float* a, const float* b, float* output )
{
    __asm__ volatile
    (
     // Store A & B leaving room for q4-q7, which should be preserved
     "vldmia %1, { q0-q3 } \n\t"
     "vldmia %2, { q8-q11 }\n\t"

     // result = first column of B x first row of A
     "vmul.f32 q12, q8, d0[0]\n\t"
     "vmul.f32 q13, q8, d2[0]\n\t"
     "vmul.f32 q14, q8, d4[0]\n\t"
     "vmul.f32 q15, q8, d6[0]\n\t"

     // result += second column of B x second row of A
     "vmla.f32 q12, q9, d0[1]\n\t"
     "vmla.f32 q13, q9, d2[1]\n\t"
     "vmla.f32 q14, q9, d4[1]\n\t"
     "vmla.f32 q15, q9, d6[1]\n\t"

     // result += third column of B x third row of A
     "vmla.f32 q12, q10, d1[0]\n\t"
     "vmla.f32 q13, q10, d3[0]\n\t"
     "vmla.f32 q14, q10, d5[0]\n\t"
     "vmla.f32 q15, q10, d7[0]\n\t"

     // result += last column of B x last row of A
     "vmla.f32 q12, q11, d1[1]\n\t"
     "vmla.f32 q13, q11, d3[1]\n\t"
     "vmla.f32 q14, q11, d5[1]\n\t"
     "vmla.f32 q15, q11, d7[1]\n\t"

     // output = result registers
     "vstmia %0, { q12-q15 }"
     : // no output
     : "r" (output), "r" (a), "r" (b)     // input - note *value* of pointer doesn't change
     : "memory", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15" //clobber
     );
}

void NEON_Matrix4Vector4Mul(const float* m, const float* v, float* output)
{
    __asm__ volatile
    (
     // Store m & v - avoiding q4-q7 which need to be preserved - q0 = result
     "vldmia %1, { q8-q11 }    \n\t"    // q8-q11 = m
     "vldmia %2, { q1 }        \n\t"    // q1     = v

     // result = first column of A x V.x
     "vmul.f32 q0, q8, d2[0]\n\t"

     // result += second column of A x V.y
     "vmla.f32 q0, q9, d2[1]\n\t"

     // result += third column of A x V.z
     "vmla.f32 q0, q10, d3[0]\n\t"

     // result += last column of A x V.w
     "vmla.f32 q0, q11, d3[1]\n\t"

     // output = result registers
     "vstmia %0, { q0 }"

     : // no output
     : "r" (output), "r" (m), "r" (v)     // input - note *value* of pointer doesn't change
     : "memory", "q0", "q1", "q8", "q9", "q10", "q11" //clobber
     );
}

#endif
