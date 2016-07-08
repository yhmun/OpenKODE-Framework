/* --------------------------------------------------------------------------
 *
 *      File            gl_platform.h
 *      Description     Platform-specific types and definitions for OpenGL ES 1.1
 *      Version         1.1, 2009-01-09 
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

#ifndef __glplatform_h_
#define __glplatform_h_

#include <KHR/khr_platform.h>

#ifndef GL_API
#define GL_API                          KHRONOS_APICALL
#endif

#ifndef GL_APIENTRY
#define GL_APIENTRY                     KHRONOS_APIENTRY
#endif

/*-------------------------------------------------------------------------
 * Data type definitions
 *-----------------------------------------------------------------------*/
#if defined ( SHP ) || defined ( TIZEN )
#include "glmacro.h"
#define GLvoid                          void
#else
typedef void                            GLvoid;
#endif

typedef khronos_uint32_t                GLenum;
typedef khronos_uint8_t                 GLboolean;
typedef khronos_uint32_t                GLbitfield;
typedef khronos_int8_t					GLchar;

#if defined ( __APPLE__ )
typedef signed char                     GLbyte;
#else
typedef khronos_int8_t                  GLbyte;
#endif

typedef khronos_int16_t                 GLshort;
typedef khronos_int32_t                 GLint;
typedef khronos_int32_t                 GLsizei;
typedef khronos_uint8_t                 GLubyte;
typedef khronos_uint16_t                GLushort;
typedef khronos_uint32_t                GLuint;
typedef khronos_float_t                 GLfloat;
typedef khronos_float_t                 GLclampf;
typedef khronos_int32_t                 GLfixed;
typedef khronos_int32_t                 GLclampx;

typedef khronos_intptr_t                GLintptr;
typedef khronos_ssize_t                 GLsizeiptr;
 
#endif
