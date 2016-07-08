/* --------------------------------------------------------------------------
 *
 *      File            gl2_platform.h
 *      Description     Platform-specific types and definitions for OpenGL ES 2.0
 *      Version         2.0, 2009-01-09 
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

#ifndef __gl2platform_h_
#define __gl2platform_h_

#include <KHR/khr_platform.h>

#ifndef GL_APICALL
#define GL_APICALL                      KHRONOS_APICALL
#endif

#ifndef GL_APIENTRY
#define GL_APIENTRY                     KHRONOS_APIENTRY
#endif

#define GL_GLEXT_PROTOTYPES

/*-------------------------------------------------------------------------
 * Data type definitions
 *-----------------------------------------------------------------------*/

#if defined ( SHP ) || defined ( TIZEN )
#include "gl2macro.h"
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

// GL types for handling large vertex buffer objects 
typedef khronos_intptr_t                GLintptr;
typedef khronos_ssize_t                 GLsizeiptr;

#if defined ( __APPLE__ )

#define GL_APPLE_copy_texture_levels                            1
#define GL_APPLE_framebuffer_multisample                        1
#define GL_APPLE_rgb_422                                        1
#define GL_APPLE_sync                                           1
#define GL_APPLE_texture_format_BGRA8888                        1
#define GL_APPLE_texture_max_level                              1
//#define GL_EXT_occlusion_query_boolean                        1
//#define GL_EXT_texture_rg                                     1

#else

#endif // __APPLE__

#define GL_EXT_occlusion_query_boolean                          0
#define GL_EXT_texture_rg                                       0
#define GL_EXT_separate_shader_objects                          0

#define GL_EXT_blend_minmax                                     1
#define GL_EXT_color_buffer_half_float                          1
#define GL_EXT_debug_label                                      1
#define GL_EXT_debug_marker                                     1
#define GL_EXT_discard_framebuffer                              1
#define GL_EXT_map_buffer_range                                 1
#define GL_EXT_read_format_bgra                                 1
#define GL_EXT_shader_framebuffer_fetch                         1
#define GL_EXT_shader_texture_lod                               1
#define GL_EXT_shadow_samplers                                  1
#define GL_EXT_texture_filter_anisotropic                       1
#define GL_EXT_texture_storage                                  1
#define GL_IMG_read_format                                      1
#define GL_IMG_texture_compression_pvrtc                        1
#define GL_OES_depth_texture                                    1
#define GL_OES_depth24                                          1
#define GL_OES_element_index_uint                               1
#define GL_OES_fbo_render_mipmap                                1
#define GL_OES_mapbuffer                                        1
#define GL_OES_packed_depth_stencil                             1
#define GL_OES_rgb8_rgba8                                       1
#define GL_OES_standard_derivatives                             1
#define GL_OES_texture_float                                    1
#define GL_OES_texture_half_float                               1
#define GL_OES_vertex_array_object                              1
#define GL_OES_compressed_ETC1_RGB8_texture						1
#define GL_AMD_compressed_ATC_texture							1

#endif

 
