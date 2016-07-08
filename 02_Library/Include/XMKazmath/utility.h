/* -----------------------------------------------------------------------------------
 *
 *      File            utility.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2008      Luke Benstead. All rights reserved.    
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Redistribution and use in source and binary forms, with or without modification,
 *      are permitted provided that the following conditions are met:
 *      
 *       * Redistributions of source code must retain the above copyright notice,
 *         this list of conditions and the following disclaimer.
 *       * Redistributions in binary form must reproduce the above copyright notice,
 *         this list of conditions and the following disclaimer in the documentation
 *         and/or other materials provided with the distribution.
 *      
 *       THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *       ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *       WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *       DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *       ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *       (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *       LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *       ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *       (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *       SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * --------------------------------------------------------------------------------- */ 

#ifndef __utility_h__
#define __utility_h__

#include "kode.h"

#ifndef kmScalar
#define kmScalar KDfloat
#endif

#ifndef kmBool
#define kmBool   KDbool
#endif

#ifndef kmEnum
#define kmEnum   KDuint
#endif

#ifndef KM_FALSE
#define KM_FALSE 0
#endif

#ifndef KM_TRUE
#define KM_TRUE  1
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define kmPI          3.141592f
#define kmPIOver180   0.017453f //  PI / 180
#define kmPIUnder180  57.295779f // 180 / PI
#define kmEpsilon     1.0 / 64.0

kmScalar kmSQR ( kmScalar s );
kmScalar kmDegreesToRadians ( kmScalar degrees );
kmScalar kmRadiansToDegrees ( kmScalar radians );

kmScalar kmMin ( kmScalar lhs, kmScalar rhs );
kmScalar kmMax ( kmScalar lhs, kmScalar rhs );
kmBool   kmAlmostEqual ( kmScalar lhs, kmScalar rhs );

#ifdef __cplusplus
}
#endif

#endif // __utility_h__
