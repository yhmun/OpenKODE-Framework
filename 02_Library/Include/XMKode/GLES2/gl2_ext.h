/* --------------------------------------------------------------------------
 *
 *      File            gl2_ext.h
 *      Description     OpenGL ES 2.0 Extension
 *      Version         2.0, 2009-10-26 
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

#ifndef __gl2_ext_h_
#define __gl2_ext_h_

#include "gl2_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GL_APIENTRYP
#define GL_APIENTRYP         GL_APIENTRY*
#endif

#if GL_OES_texture_half_float
typedef khronos_uint16_t	 GLhalf;
#endif
    
#if GL_APPLE_sync
typedef khronos_int64_t	     GLint64;
typedef khronos_uint64_t     GLuint64;
typedef struct __GLsync     *GLsync;
#endif
    
/*------------------------------------------------------------------------*
 * OES extension tokens
 *------------------------------------------------------------------------*/

// GL_OES_compressed_ETC1_RGB8_texture 
#if GL_OES_compressed_ETC1_RGB8_texture
#define GL_ETC1_RGB8_OES                                        0x8D64
#endif

// GL_OES_compressed_paletted_texture 
#if GL_OES_compressed_paletted_texture
#define GL_PALETTE4_RGB8_OES                                    0x8B90
#define GL_PALETTE4_RGBA8_OES                                   0x8B91
#define GL_PALETTE4_R5_G6_B5_OES                                0x8B92
#define GL_PALETTE4_RGBA4_OES                                   0x8B93
#define GL_PALETTE4_RGB5_A1_OES                                 0x8B94
#define GL_PALETTE8_RGB8_OES                                    0x8B95
#define GL_PALETTE8_RGBA8_OES                                   0x8B96
#define GL_PALETTE8_R5_G6_B5_OES                                0x8B97
#define GL_PALETTE8_RGBA4_OES                                   0x8B98
#define GL_PALETTE8_RGB5_A1_OES                                 0x8B99
#endif

// GL_OES_depth24 
#if GL_OES_depth24
#define GL_DEPTH_COMPONENT24_OES                                0x81A6
#endif

// GL_OES_depth32 
#if GL_OES_depth32
#define GL_DEPTH_COMPONENT32_OES                                0x81A7
#endif

// GL_OES_depth_texture 
// No new tokens introduced by this extension. 

// GL_OES_EGL_image 
#if GL_OES_EGL_image
typedef GLvoid* GLeglImageOES;
#endif

// GL_OES_element_index_uint //
#ifndef GL_OES_element_index_uint
#define GL_UNSIGNED_INT                                         0x1405
#endif

// GL_OES_get_program_binary 
#if GL_OES_get_program_binary
#define GL_PROGRAM_BINARY_LENGTH_OES                            0x8741
#define GL_NUM_PROGRAM_BINARY_FORMATS_OES                       0x87FE
#define GL_PROGRAM_BINARY_FORMATS_OES                           0x87FF
#endif

// GL_OES_mapbuffer 
#if GL_OES_mapbuffer
#define GL_WRITE_ONLY_OES                                       0x88B9
#define GL_BUFFER_ACCESS_OES                                    0x88BB
#define GL_BUFFER_MAPPED_OES                                    0x88BC
#define GL_BUFFER_MAP_POINTER_OES                               0x88BD
#endif

// GL_OES_packed_depth_stencil 
#if GL_OES_packed_depth_stencil
#define GL_DEPTH_STENCIL_OES                                    0x84F9
#define GL_UNSIGNED_INT_24_8_OES                                0x84FA
#define GL_DEPTH24_STENCIL8_OES                                 0x88F0
#endif

// GL_OES_rgb8_rgba8 
#if GL_OES_rgb8_rgba8
#define GL_RGB8_OES                                             0x8051
#define GL_RGBA8_OES                                            0x8058
#endif

// GL_OES_standard_derivatives 
#if GL_OES_standard_derivatives
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT_OES                  0x8B8B
#endif

// GL_OES_stencil1 
#if GL_OES_stencil1
#define GL_STENCIL_INDEX1_OES                                   0x8D46
#endif

// GL_OES_stencil4 
#if GL_OES_stencil4
#define GL_STENCIL_INDEX4_OES                                   0x8D47
#endif

// GL_OES_texture3D 
#if GL_OES_texture3D
#define GL_TEXTURE_WRAP_R_OES                                   0x8072
#define GL_TEXTURE_3D_OES                                       0x806F
#define GL_TEXTURE_BINDING_3D_OES                               0x806A
#define GL_MAX_3D_TEXTURE_SIZE_OES                              0x8073
#define GL_SAMPLER_3D_OES                                       0x8B5F
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_OES        0x8CD4
#endif

// GL_OES_texture_float 
// No new tokens introduced by this extension. 

// GL_OES_texture_float_linear 
// No new tokens introduced by this extension. 

// GL_OES_texture_half_float 
#if GL_OES_texture_half_float
#define GL_HALF_FLOAT_OES                                       0x8D61
#endif

// GL_OES_texture_half_float_linear 
// No new tokens introduced by this extension. 

// GL_OES_texture_npot 
// No new tokens introduced by this extension. 

/* GL_OES_vertex_array_object */
#if GL_OES_vertex_array_object
#define GL_VERTEX_ARRAY_BINDING_OES                             0x85B5
#endif

// GL_OES_vertex_half_float 
// GL_HALF_FLOAT_OES defined in GL_OES_texture_half_float already. 

// GL_OES_vertex_type_10_10_10_2 
#if GL_OES_vertex_type_10_10_10_2
#define GL_UNSIGNED_INT_10_10_10_2_OES                          0x8DF6
#define GL_INT_10_10_10_2_OES                                   0x8DF7
#endif

/*------------------------------------------------------------------------*
 * AMD extension tokens
 *------------------------------------------------------------------------*/

// GL_AMD_compressed_3DC_texture 
#if GL_AMD_compressed_3DC_texture
#define GL_3DC_X_AMD                                            0x87F9
#define GL_3DC_XY_AMD                                           0x87FA
#endif

// GL_AMD_compressed_ATC_texture 
#if GL_AMD_compressed_ATC_texture
#define GL_ATC_RGB_AMD                                          0x8C92
#define GL_ATC_RGBA_EXPLICIT_ALPHA_AMD                          0x8C93
#define GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD                      0x87EE
#endif

// GL_AMD_performance_monitor 
#if GL_AMD_performance_monitor
#define GL_COUNTER_TYPE_AMD                                     0x8BC0
#define GL_COUNTER_RANGE_AMD                                    0x8BC1
#define GL_UNSIGNED_INT64_AMD                                   0x8BC2
#define GL_PERCENTAGE_AMD                                       0x8BC3
#define GL_PERFMON_RESULT_AVAILABLE_AMD                         0x8BC4
#define GL_PERFMON_RESULT_SIZE_AMD                              0x8BC5
#define GL_PERFMON_RESULT_AMD                                   0x8BC6
#endif

// GL_AMD_program_binary_Z400 
#if GL_AMD_program_binary_Z400
#define GL_Z400_BINARY_AMD                                      0x8740
#endif

/*------------------------------------------------------------------------*
 * APPLE extension tokens
 *------------------------------------------------------------------------*/
#if GL_APPLE_framebuffer_multisample
#define GL_RENDERBUFFER_SAMPLES_APPLE                           0x8CAB
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_APPLE             0x8D56
#define GL_MAX_SAMPLES_APPLE                                    0x8D57
#define GL_READ_FRAMEBUFFER_APPLE                               0x8CA8
#define GL_DRAW_FRAMEBUFFER_APPLE                               0x8CA9
#define GL_DRAW_FRAMEBUFFER_BINDING_APPLE                       0x8CA6
#define GL_READ_FRAMEBUFFER_BINDING_APPLE                       0x8CAA
#endif

#if GL_APPLE_rgb_422
#define GL_RGB_422_APPLE                                        0x8A1F
#define GL_UNSIGNED_SHORT_8_8_APPLE                             0x85BA
#define GL_UNSIGNED_SHORT_8_8_REV_APPLE                         0x85BB
#endif

#if GL_APPLE_sync
#define GL_MAX_SERVER_WAIT_TIMEOUT_APPLE                        0x9111
#define GL_OBJECT_TYPE_APPLE                                    0x9112
#define GL_SYNC_CONDITION_APPLE                                 0x9113
#define GL_SYNC_STATUS_APPLE                                    0x9114
#define GL_SYNC_FLAGS_APPLE                                     0x9115
#define GL_SYNC_FENCE_APPLE                                     0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE_APPLE                     0x9117
#define GL_UNSIGNALED_APPLE                                     0x9118
#define GL_SIGNALED_APPLE                                       0x9119
#define GL_ALREADY_SIGNALED_APPLE                               0x911A
#define GL_TIMEOUT_EXPIRED_APPLE                                0x911B
#define GL_CONDITION_SATISFIED_APPLE                            0x911C
#define GL_WAIT_FAILED_APPLE                                    0x911D
#define GL_SYNC_FLUSH_COMMANDS_BIT_APPLE                        0x00000001
#define GL_TIMEOUT_IGNORED_APPLE                                0xFFFFFFFFFFFFFFFFull
#define GL_SYNC_OBJECT_APPLE                                    0x8A53
#endif

#if GL_APPLE_texture_format_BGRA8888
#define GL_BGRA_EXT                                             0x80E1
#endif

#if GL_APPLE_texture_format_BGRA8888 || GL_IMG_read_format
#define GL_BGRA                                                 0x80E1
#endif

#if GL_APPLE_texture_max_level
#define GL_TEXTURE_MAX_LEVEL_APPLE                              0x813D
#endif

    
/*------------------------------------------------------------------------*
 * EXT extension tokens
 *------------------------------------------------------------------------*/

// GL_EXT_blend_minmax 
#if GL_EXT_blend_minmax
#define GL_MIN_EXT                                              0x8007
#define GL_MAX_EXT                                              0x8008
#endif

// GL_EXT_discard_framebuffer 
#if GL_EXT_discard_framebuffer
#define GL_COLOR_EXT                                            0x1800
#define GL_DEPTH_EXT                                            0x1801
#define GL_STENCIL_EXT                                          0x1802
#endif

// GL_EXT_multi_draw_arrays 
// No new tokens introduced by this extension. 

// GL_EXT_read_format_bgra 
#if GL_EXT_read_format_bgra
#define GL_BGRA_EXT                                             0x80E1
#define GL_UNSIGNED_SHORT_4_4_4_4_REV_EXT                       0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV_EXT                       0x8366
#endif

// GL_EXT_texture_filter_anisotropic 
#if GL_EXT_texture_filter_anisotropic
#define GL_TEXTURE_MAX_ANISOTROPY_EXT                           0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT                       0x84FF
#endif

// GL_EXT_texture_format_BGRA8888 
#if GL_EXT_texture_format_BGRA8888
#define GL_BGRA_EXT                                             0x80E1
#endif

// GL_EXT_texture_type_2_10_10_10_REV 
#if GL_EXT_texture_type_2_10_10_10_REV
#define GL_UNSIGNED_INT_2_10_10_10_REV_EXT                      0x8368
#endif

/*------------------------------------------------------------------------*
 * IMG extension tokens
 *------------------------------------------------------------------------*/

// GL_IMG_program_binary 
#if GL_IMG_program_binary
#define GL_SGX_PROGRAM_BINARY_IMG                               0x9130
#endif

// GL_IMG_read_format 
#if GL_IMG_read_format
#define GL_BGRA_IMG                                             0x80E1
#define GL_UNSIGNED_SHORT_4_4_4_4_REV_IMG                       0x8365
#endif

// GL_IMG_shader_binary 
#if GL_IMG_shader_binary
#define GL_SGX_BINARY_IMG                                       0x8C0A
#endif

// GL_IMG_texture_compression_pvrtc 
#if GL_IMG_texture_compression_pvrtc
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG                      0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG                      0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG                     0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG                     0x8C03
#endif

/*------------------------------------------------------------------------*
 * NV extension tokens
 *------------------------------------------------------------------------*/

// GL_NV_fence 
#if GL_NV_fence
#define GL_ALL_COMPLETED_NV                                     0x84F2
#define GL_FENCE_STATUS_NV                                      0x84F3
#define GL_FENCE_CONDITION_NV                                   0x84F4
#endif

/*------------------------------------------------------------------------*
 * QCOM extension tokens
 *------------------------------------------------------------------------*/

// GL_QCOM_driver_control 
// No new tokens introduced by this extension. 

// GL_QCOM_extended_get 
#if GL_QCOM_extended_get
#define GL_TEXTURE_WIDTH_QCOM                                   0x8BD2
#define GL_TEXTURE_HEIGHT_QCOM                                  0x8BD3
#define GL_TEXTURE_DEPTH_QCOM                                   0x8BD4
#define GL_TEXTURE_INTERNAL_FORMAT_QCOM                         0x8BD5
#define GL_TEXTURE_FORMAT_QCOM                                  0x8BD6
#define GL_TEXTURE_TYPE_QCOM                                    0x8BD7
#define GL_TEXTURE_IMAGE_VALID_QCOM                             0x8BD8
#define GL_TEXTURE_NUM_LEVELS_QCOM                              0x8BD9
#define GL_TEXTURE_TARGET_QCOM                                  0x8BDA
#define GL_TEXTURE_OBJECT_VALID_QCOM                            0x8BDB
#define GL_STATE_RESTORE                                        0x8BDC
#endif

// GL_QCOM_extended_get2 
// No new tokens introduced by this extension. 

// GL_QCOM_perfmon_global_mode 
#if GL_QCOM_perfmon_global_mode
#define GL_PERFMON_GLOBAL_MODE_QCOM                             0x8FA0
#endif

// GL_QCOM_writeonly_rendering 
#if GL_QCOM_writeonly_rendering
#define GL_WRITEONLY_RENDERING_QCOM                             0x8823
#endif

/*------------------------------------------------------------------------*
 * End of extension tokens, start of corresponding extension functions
 *------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*
 * APPLE extension functions
 *------------------------------------------------------------------------*/
#if GL_APPLE_copy_texture_levels
GL_APICALL GLvoid glCopyTextureLevelsAPPLE(GLuint destinationTexture, GLuint sourceTexture, GLint sourceBaseLevel, GLsizei sourceLevelCount);
#endif

#if GL_APPLE_framebuffer_multisample
GL_APICALL GLvoid glRenderbufferStorageMultisampleAPPLE(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
GL_APICALL GLvoid glResolveMultisampleFramebufferAPPLE(void);
#endif

#if GL_APPLE_sync
GL_APICALL GLsync glFenceSyncAPPLE(GLenum condition, GLbitfield flags);
GL_APICALL GLboolean glIsSyncAPPLE(GLsync sync);
GL_APICALL GLvoid glDeleteSyncAPPLE(GLsync sync);
GL_APICALL GLenum glClientWaitSyncAPPLE(GLsync sync, GLbitfield flags, GLuint64 timeout);
GL_APICALL GLvoid glWaitSyncAPPLE(GLsync sync, GLbitfield flags, GLuint64 timeout);
GL_APICALL GLvoid glGetInteger64vAPPLE(GLenum pname, GLint64 *params);
GL_APICALL GLvoid glGetSyncivAPPLE(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
#endif
    
/*------------------------------------------------------------------------*
 * OES extension functions
 *------------------------------------------------------------------------*/

// GL_OES_EGL_image 
#if GL_OES_EGL_image
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL GLvoid GL_APIENTRY glEGLImageTargetTexture2DOES ( GLenum target, GLeglImageOES image );
GL_APICALL GLvoid GL_APIENTRY glEGLImageTargetRenderbufferStorageOES ( GLenum target, GLeglImageOES image );
#endif
typedef GLvoid ( GL_APIENTRYP PFNGLEGLIMAGETARGETTEXTURE2DOESPROC ) ( GLenum target, GLeglImageOES image );
typedef GLvoid ( GL_APIENTRYP PFNGLEGLIMAGETARGETRENDERBUFFERSTORAGEOESPROC ) ( GLenum target, GLeglImageOES image );
#endif

// GL_OES_get_program_binary 
#if GL_OES_get_program_binary
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL GLvoid GL_APIENTRY glGetProgramBinaryOES ( GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, GLvoid* binary );
GL_APICALL GLvoid GL_APIENTRY glProgramBinaryOES ( GLuint program, GLenum binaryFormat, const GLvoid* binary, GLint length );
#endif
typedef GLvoid ( GL_APIENTRYP PFNGLGETPROGRAMBINARYOESPROC ) ( GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, GLvoid* binary );
typedef GLvoid ( GL_APIENTRYP PFNGLPROGRAMBINARYOESPROC ) ( GLuint program, GLenum binaryFormat, const GLvoid* binary, GLint length );
#endif

// GL_OES_mapbuffer 
#if GL_OES_mapbuffer
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL GLvoid* GL_APIENTRY glMapBufferOES ( GLenum target, GLenum access );
GL_APICALL GLboolean GL_APIENTRY glUnmapBufferOES ( GLenum target );
GL_APICALL GLvoid GL_APIENTRY glGetBufferPointervOES ( GLenum target, GLenum pname, GLvoid** params );
#endif
typedef GLvoid* ( GL_APIENTRYP PFNGLMAPBUFFEROESPROC ) ( GLenum target, GLenum access );
typedef GLboolean ( GL_APIENTRYP PFNGLUNMAPBUFFEROESPROC ) ( GLenum target );
typedef GLvoid ( GL_APIENTRYP PFNGLGETBUFFERPOINTERVOESPROC ) ( GLenum target, GLenum pname, GLvoid** params );
#endif

// GL_OES_texture_3D 
#if GL_OES_texture_3D
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL GLvoid GL_APIENTRY glTexImage3DOES ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels );
GL_APICALL GLvoid GL_APIENTRY glTexSubImage3DOES ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels );
GL_APICALL GLvoid GL_APIENTRY glCopyTexSubImage3DOES ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height );
GL_APICALL GLvoid GL_APIENTRY glCompressedTexImage3DOES ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data );
GL_APICALL GLvoid GL_APIENTRY glCompressedTexSubImage3DOES ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data );
GL_APICALL GLvoid GL_APIENTRY glFramebufferTexture3DOES ( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset );
#endif
typedef GLvoid ( GL_APIENTRYP PFNGLTEXIMAGE3DOESPROC ) ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels );
typedef GLvoid ( GL_APIENTRYP PFNGLTEXSUBIMAGE3DOESPROC ) ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid* pixels );
typedef GLvoid ( GL_APIENTRYP PFNGLCOPYTEXSUBIMAGE3DOESPROC ) ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height );
typedef GLvoid ( GL_APIENTRYP PFNGLCOMPRESSEDTEXIMAGE3DOESPROC ) ( GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid* data );
typedef GLvoid ( GL_APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE3DOESPROC ) ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid* data );
typedef GLvoid ( GL_APIENTRYP PFNGLFRAMEBUFFERTEXTURE3DOES ) ( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset );
#endif

/* GL_OES_texture_float */
#ifndef GL_OES_texture_float
#define GL_OES_texture_float 1
#endif

/* GL_OES_texture_float_linear */
#ifndef GL_OES_texture_float_linear
#define GL_OES_texture_float_linear 1
#endif

/* GL_OES_texture_half_float */
#ifndef GL_OES_texture_half_float
#define GL_OES_texture_half_float 1
#endif

/* GL_OES_texture_half_float_linear */
#ifndef GL_OES_texture_half_float_linear
#define GL_OES_texture_half_float_linear 1
#endif

/* GL_OES_texture_npot */
#ifndef GL_OES_texture_npot
#define GL_OES_texture_npot 1
#endif

/* GL_OES_vertex_array_object */
#if GL_OES_vertex_array_object
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void GL_APIENTRY glBindVertexArrayOES (GLuint array);
GL_APICALL void GL_APIENTRY glDeleteVertexArraysOES (GLsizei n, const GLuint *arrays);
GL_APICALL void GL_APIENTRY glGenVertexArraysOES (GLsizei n, GLuint *arrays);
GL_APICALL GLboolean GL_APIENTRY glIsVertexArrayOES (GLuint array);
#endif
typedef void (GL_APIENTRYP PFNGLBINDVERTEXARRAYOESPROC) (GLuint array);
typedef void (GL_APIENTRYP PFNGLDELETEVERTEXARRAYSOESPROC) (GLsizei n, const GLuint *arrays);
typedef void (GL_APIENTRYP PFNGLGENVERTEXARRAYSOESPROC) (GLsizei n, GLuint *arrays);
typedef GLboolean (GL_APIENTRYP PFNGLISVERTEXARRAYOESPROC) (GLuint array);
#endif

#ifdef ANDROID

//declare here while define in EGLView_android.cpp
extern PFNGLGENVERTEXARRAYSOESPROC		glGenVertexArraysOESEXT;
extern PFNGLBINDVERTEXARRAYOESPROC		glBindVertexArrayOESEXT;
extern PFNGLDELETEVERTEXARRAYSOESPROC	glDeleteVertexArraysOESEXT;

#define glGenVertexArraysOES			glGenVertexArraysOESEXT
#define glBindVertexArrayOES			glBindVertexArrayOESEXT
#define glDeleteVertexArraysOES			glDeleteVertexArraysOESEXT

#endif

/*------------------------------------------------------------------------*
*  AMD extension functions
* ------------------------------------------------------------------------*/

// AMD_performance_monitor 
#if GL_AMD_performance_monitor
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL GLvoid GL_APIENTRY glGetPerfMonitorGroupsAMD ( GLint* numGroups, GLsizei groupsSize, GLuint* groups );
GL_APICALL GLvoid GL_APIENTRY glGetPerfMonitorCountersAMD ( GLuint group, GLint* numCounters, GLint* maxActiveCounters, GLsizei counterSize, GLuint* counters );
GL_APICALL GLvoid GL_APIENTRY glGetPerfMonitorGroupStringAMD ( GLuint group, GLsizei bufSize, GLsizei* length, char* groupString );
GL_APICALL GLvoid GL_APIENTRY glGetPerfMonitorCounterStringAMD ( GLuint group, GLuint counter, GLsizei bufSize, GLsizei* length, char* counterString );
GL_APICALL GLvoid GL_APIENTRY glGetPerfMonitorCounterInfoAMD ( GLuint group, GLuint counter, GLenum pname, GLvoid* data );
GL_APICALL GLvoid GL_APIENTRY glGenPerfMonitorsAMD ( GLsizei n, GLuint* monitors );
GL_APICALL GLvoid GL_APIENTRY glDeletePerfMonitorsAMD ( GLsizei n, GLuint* monitors );
GL_APICALL GLvoid GL_APIENTRY glSelectPerfMonitorCountersAMD ( GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint* countersList );
GL_APICALL GLvoid GL_APIENTRY glBeginPerfMonitorAMD ( GLuint monitor );
GL_APICALL GLvoid GL_APIENTRY glEndPerfMonitorAMD ( GLuint monitor );
GL_APICALL GLvoid GL_APIENTRY glGetPerfMonitorCounterDataAMD ( GLuint monitor, GLenum pname, GLsizei dataSize, GLuint* data, GLint* bytesWritten );
#endif
typedef GLvoid ( GL_APIENTRYP PFNGLGETPERFMONITORGROUPSAMDPROC ) ( GLint* numGroups, GLsizei groupsSize, GLuint* groups );
typedef GLvoid ( GL_APIENTRYP PFNGLGETPERFMONITORCOUNTERSAMDPROC ) ( GLuint group, GLint* numCounters, GLint* maxActiveCounters, GLsizei counterSize, GLuint* counters );
typedef GLvoid ( GL_APIENTRYP PFNGLGETPERFMONITORGROUPSTRINGAMDPROC ) ( GLuint group, GLsizei bufSize, GLsizei* length, char* groupString );
typedef GLvoid ( GL_APIENTRYP PFNGLGETPERFMONITORCOUNTERSTRINGAMDPROC ) ( GLuint group, GLuint counter, GLsizei bufSize, GLsizei* length, char* counterString );
typedef GLvoid ( GL_APIENTRYP PFNGLGETPERFMONITORCOUNTERINFOAMDPROC ) ( GLuint group, GLuint counter, GLenum pname, GLvoid* data );
typedef GLvoid ( GL_APIENTRYP PFNGLGENPERFMONITORSAMDPROC ) ( GLsizei n, GLuint* monitors );
typedef GLvoid ( GL_APIENTRYP PFNGLDELETEPERFMONITORSAMDPROC ) ( GLsizei n, GLuint* monitors );
typedef GLvoid ( GL_APIENTRYP PFNGLSELECTPERFMONITORCOUNTERSAMDPROC ) ( GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint* countersList );
typedef GLvoid ( GL_APIENTRYP PFNGLBEGINPERFMONITORAMDPROC ) ( GLuint monitor );
typedef GLvoid ( GL_APIENTRYP PFNGLENDPERFMONITORAMDPROC ) ( GLuint monitor );
typedef GLvoid ( GL_APIENTRYP PFNGLGETPERFMONITORCOUNTERDATAAMDPROC ) ( GLuint monitor, GLenum pname, GLsizei dataSize, GLuint* data, GLint* bytesWritten );
#endif

/*------------------------------------------------------------------------*
* APPLE extension functions
*------------------------------------------------------------------------*/

    
/*------------------------------------------------------------------------*
*  EXT extension functions
* ------------------------------------------------------------------------*/

// GL_EXT_discard_framebuffer 
#if GL_EXT_discard_framebuffer
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL GLvoid GL_APIENTRY glDiscardFramebufferEXT ( GLenum target, GLsizei numAttachments, const GLenum* attachments );
#endif
typedef GLvoid ( GL_APIENTRYP PFNGLDISCARDFRAMEBUFFEREXTPROC ) ( GLenum target, GLsizei numAttachments, const GLenum* attachments );
#endif

#if GL_EXT_multi_draw_arrays
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL GLvoid GL_APIENTRY glMultiDrawArraysEXT ( GLenum, GLint* , GLsizei* , GLsizei );
GL_APICALL GLvoid GL_APIENTRY glMultiDrawElementsEXT ( GLenum, const GLsizei* , GLenum, const GLvoid** , GLsizei );
#endif // GL_GLEXT_PROTOTYPES 
typedef GLvoid ( GL_APIENTRYP PFNGLMULTIDRAWARRAYSEXTPROC ) ( GLenum mode, GLint* first, GLsizei* count, GLsizei primcount );
typedef GLvoid ( GL_APIENTRYP PFNGLMULTIDRAWELEMENTSEXTPROC ) ( GLenum mode, const GLsizei* count, GLenum type, const GLvoid** indices, GLsizei primcount );
#endif

/*------------------------------------------------------------------------*
*  IMG extension functions
* ------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*
*  NV extension functions
* ------------------------------------------------------------------------*/

// GL_NV_fence 
#if GL_NV_fence
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL GLvoid GL_APIENTRY glDeleteFencesNV ( GLsizei, const GLuint*  );
GL_APICALL GLvoid GL_APIENTRY glGenFencesNV ( GLsizei, GLuint*  );
GL_APICALL GLboolean GL_APIENTRY glIsFenceNV ( GLuint );
GL_APICALL GLboolean GL_APIENTRY glTestFenceNV ( GLuint );
GL_APICALL GLvoid GL_APIENTRY glGetFenceivNV ( GLuint, GLenum, GLint*  );
GL_APICALL GLvoid GL_APIENTRY glFinishFenceNV ( GLuint );
GL_APICALL GLvoid GL_APIENTRY glSetFenceNV ( GLuint, GLenum );
#endif
typedef GLvoid ( GL_APIENTRYP PFNGLDELETEFENCESNVPROC ) ( GLsizei n, const GLuint* fences );
typedef GLvoid ( GL_APIENTRYP PFNGLGENFENCESNVPROC ) ( GLsizei n, GLuint* fences );
typedef GLboolean ( GL_APIENTRYP PFNGLISFENCENVPROC ) ( GLuint fence );
typedef GLboolean ( GL_APIENTRYP PFNGLTESTFENCENVPROC ) ( GLuint fence );
typedef GLvoid ( GL_APIENTRYP PFNGLGETFENCEIVNVPROC ) ( GLuint fence, GLenum pname, GLint* params );
typedef GLvoid ( GL_APIENTRYP PFNGLFINISHFENCENVPROC ) ( GLuint fence );
typedef GLvoid ( GL_APIENTRYP PFNGLSETFENCENVPROC ) ( GLuint fence, GLenum condition );
#endif

/*------------------------------------------------------------------------*
*  QCOM extension functions
* ------------------------------------------------------------------------*/

// GL_QCOM_driver_control 
#if GL_QCOM_driver_control
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL GLvoid GL_APIENTRY glGetDriverControlsQCOM ( GLint* num, GLsizei size, GLuint* driverControls );
GL_APICALL GLvoid GL_APIENTRY glGetDriverControlStringQCOM ( GLuint driverControl, GLsizei bufSize, GLsizei* length, char* driverControlString );
GL_APICALL GLvoid GL_APIENTRY glEnableDriverControlQCOM ( GLuint driverControl );
GL_APICALL GLvoid GL_APIENTRY glDisableDriverControlQCOM ( GLuint driverControl );
#endif
typedef GLvoid ( GL_APIENTRYP PFNGLGETDRIVERCONTROLSQCOMPROC ) ( GLint* num, GLsizei size, GLuint* driverControls );
typedef GLvoid ( GL_APIENTRYP PFNGLGETDRIVERCONTROLSTRINGQCOMPROC ) ( GLuint driverControl, GLsizei bufSize, GLsizei* length, char* driverControlString );
typedef GLvoid ( GL_APIENTRYP PFNGLENABLEDRIVERCONTROLQCOMPROC ) ( GLuint driverControl );
typedef GLvoid ( GL_APIENTRYP PFNGLDISABLEDRIVERCONTROLQCOMPROC ) ( GLuint driverControl );
#endif

// GL_QCOM_extended_get 
#if GL_QCOM_extended_get
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL GLvoid GL_APIENTRY glExtGetTexturesQCOM ( GLuint* textures, GLint maxTextures, GLint* numTextures );
GL_APICALL GLvoid GL_APIENTRY glExtGetBuffersQCOM ( GLuint* buffers, GLint maxBuffers, GLint* numBuffers );
GL_APICALL GLvoid GL_APIENTRY glExtGetRenderbuffersQCOM ( GLuint* renderbuffers, GLint maxRenderbuffers, GLint* numRenderbuffers );
GL_APICALL GLvoid GL_APIENTRY glExtGetFramebuffersQCOM ( GLuint* framebuffers, GLint maxFramebuffers, GLint* numFramebuffers );
GL_APICALL GLvoid GL_APIENTRY glExtGetTexLevelParameterivQCOM ( GLuint texture, GLenum face, GLint level, GLenum pname, GLint* params );
GL_APICALL GLvoid GL_APIENTRY glExtTexObjectStateOverrideiQCOM ( GLenum target, GLenum pname, GLint param );
GL_APICALL GLvoid GL_APIENTRY glExtGetTexSubImageQCOM ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLvoid* texels );
GL_APICALL GLvoid GL_APIENTRY glExtGetBufferPointervQCOM ( GLenum target, GLvoid* *params );
#endif
typedef GLvoid ( GL_APIENTRYP PFNGLEXTGETTEXTURESQCOMPROC ) ( GLuint* textures, GLint maxTextures, GLint* numTextures );
typedef GLvoid ( GL_APIENTRYP PFNGLEXTGETBUFFERSQCOMPROC ) ( GLuint* buffers, GLint maxBuffers, GLint* numBuffers );
typedef GLvoid ( GL_APIENTRYP PFNGLEXTGETRENDERBUFFERSQCOMPROC ) ( GLuint* renderbuffers, GLint maxRenderbuffers, GLint* numRenderbuffers );
typedef GLvoid ( GL_APIENTRYP PFNGLEXTGETFRAMEBUFFERSQCOMPROC ) ( GLuint* framebuffers, GLint maxFramebuffers, GLint* numFramebuffers );
typedef GLvoid ( GL_APIENTRYP PFNGLEXTGETTEXLEVELPARAMETERIVQCOMPROC ) ( GLuint texture, GLenum face, GLint level, GLenum pname, GLint* params );
typedef GLvoid ( GL_APIENTRYP PFNGLEXTTEXOBJECTSTATEOVERRIDEIQCOMPROC ) ( GLenum target, GLenum pname, GLint param );
typedef GLvoid ( GL_APIENTRYP PFNGLEXTGETTEXSUBIMAGEQCOMPROC ) ( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLvoid* texels );
typedef GLvoid ( GL_APIENTRYP PFNGLEXTGETBUFFERPOINTERVQCOMPROC ) ( GLenum target, GLvoid* *params );
#endif

// GL_QCOM_extended_get2 
#if GL_QCOM_extended_get2
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL GLvoid GL_APIENTRY glExtGetShadersQCOM ( GLuint* shaders, GLint maxShaders, GLint* numShaders );
GL_APICALL GLvoid GL_APIENTRY glExtGetProgramsQCOM ( GLuint* programs, GLint maxPrograms, GLint* numPrograms );
GL_APICALL GLvoid GL_APIENTRY glExtIsProgramBinaryQCOM ( GLuint program );
GL_APICALL GLvoid GL_APIENTRY glExtGetProgramBinarySourceQCOM ( GLuint program, GLenum shadertype, char* source, GLint* length );
#endif
typedef GLvoid ( GL_APIENTRYP PFNGLEXTGETSHADERSQCOMPROC ) ( GLuint* shaders, GLint maxShaders, GLint* numShaders );
typedef GLvoid ( GL_APIENTRYP PFNGLEXTGETPROGRAMSQCOMPROC ) ( GLuint* programs, GLint maxPrograms, GLint* numPrograms );
typedef GLvoid ( GL_APIENTRYP PFNGLEXTISPROGRAMBINARYQCOMPROC ) ( GLuint program );
typedef GLvoid ( GL_APIENTRYP PFNGLEXTGETPROGRAMBINARYSOURCEQCOMPROC ) ( GLuint program, GLenum shadertype, char* source, GLint* length );
#endif

/* -------------------- GL_EXT_texture_compression_s3tc -------------------- */

#ifndef GL_EXT_texture_compression_s3tc
#define GL_EXT_texture_compression_s3tc 1

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

//#define GLEW_EXT_texture_compression_s3tc GLEW_GET_VAR(__GLEW_EXT_texture_compression_s3tc)

#endif /* GL_EXT_texture_compression_s3tc */

#ifdef __cplusplus
}
#endif

#endif
