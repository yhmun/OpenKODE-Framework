/*
 *
 *      File            glu.h
 *      Description     This port is based on original GLU 1.3 and has original
 *                      libutil and libtess (part of GLU) only.
 *      Version         1.4, 2009-05-17 
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
	
 * 
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
 */

/*
 *
 * SGI FREE SOFTWARE LICENSE B (Version 2.0, Sept. 18, 2008)
 * Copyright (C) 1991-2000 Silicon Graphics, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice including the dates of first publication and
 * either this permission notice or a reference to
 * http://oss.sgi.com/projects/FreeB/
 * shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * SILICON GRAPHICS, INC. BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of Silicon Graphics, Inc.
 * shall not be used in advertising or otherwise to promote the sale, use or
 * other dealings in this Software without prior written authorization from
 * Silicon Graphics, Inc.
 *
 * OpenGL ES 1.0 CM port of part of GLU by Mike Gorchak <mike@malva.ua>
 *
 */

#ifndef __glues_h__
#define __glues_h__

#include <GLUES/glu_platform.h>
#include <GLES/gl.h>

#ifdef __cplusplus
   extern "C" {
#endif

// Boolean
#define GLU_FALSE                          0
#define GLU_TRUE                           1

// Version
#define GLU_VERSION_1_1                    1
#define GLU_VERSION_1_2                    1
#define GLU_VERSION_1_3                    1

// StringName
#define GLU_VERSION                        100800
#define GLU_EXTENSIONS                     100801

// ErrorCode
#define GLU_INVALID_ENUM                   100900
#define GLU_INVALID_VALUE                  100901
#define GLU_OUT_OF_MEMORY                  100902
#define GLU_INCOMPATIBLE_GL_VERSION        100903
#define GLU_INVALID_OPERATION              100904

// QuadricDrawStyle
#define GLU_POINT                          100010
#define GLU_LINE                           100011
#define GLU_FILL                           100012
#define GLU_SILHOUETTE                     100013

// QuadricCallback
#define GLU_ERROR                          100103

// QuadricNormal
#define GLU_SMOOTH                         100000
#define GLU_FLAT                           100001
#define GLU_NONE                           100002

// QuadricOrientation
#define GLU_OUTSIDE                        100020
#define GLU_INSIDE                         100021

#ifdef __cplusplus
class GLUquadric;
class GLUtesselator;
#else
typedef struct GLUquadric GLUquadric;
typedef struct GLUtesselator GLUtesselator;
#endif

typedef GLUquadric    GLUquadricObj;
typedef GLUtesselator GLUtesselatorObj;
typedef GLUtesselator GLUtriangulatorObj;

// Internal convenience typedefs
typedef void ( GLU_APIENTRY* _GLUfuncptr ) ( );

GLU_API GLboolean GLU_APIENTRY gluCheckExtension ( const GLubyte* extName, const GLubyte* extString );
GLU_API void GLU_APIENTRY gluCylinder ( GLUquadric* quad, GLfloat base, GLfloat top, GLfloat height, GLint slices, GLint stacks );
GLU_API void GLU_APIENTRY gluDeleteQuadric ( GLUquadric* quad );
GLU_API void GLU_APIENTRY gluDisk ( GLUquadric* quad, GLfloat inner, GLfloat outer, GLint slices, GLint loops );
GLU_API const GLubyte* GLU_APIENTRY gluErrorString ( GLenum error );
GLU_API const GLubyte * GLU_APIENTRY gluGetString ( GLenum name );
GLU_API void GLU_APIENTRY gluLookAt ( GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat upX, GLfloat upY, GLfloat upZ );
GLU_API GLUquadric* GLU_APIENTRY gluNewQuadric ( void );
GLU_API void GLU_APIENTRY gluOrtho2D ( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top );
GLU_API void GLU_APIENTRY gluPartialDisk ( GLUquadric* quad, GLfloat inner, GLfloat outer, GLint slices, GLint loops, GLfloat start, GLfloat sweep );
GLU_API void GLU_APIENTRY gluPerspective ( GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar );
GLU_API void GLU_APIENTRY gluPickMatrix ( GLfloat x, GLfloat y, GLfloat delX, GLfloat delY, GLint *viewport );
GLU_API GLint GLU_APIENTRY gluProject ( GLfloat objX, GLfloat objY, GLfloat objZ, const GLfloat* model, const GLfloat *proj, const GLint* view, GLfloat* winX, GLfloat* winY, GLfloat* winZ );
GLU_API void GLU_APIENTRY gluQuadricCallback ( GLUquadric* quad, GLenum which, _GLUfuncptr CallBackFunc );
GLU_API void GLU_APIENTRY gluQuadricDrawStyle ( GLUquadric* quad, GLenum draw );
GLU_API void GLU_APIENTRY gluQuadricNormals ( GLUquadric* quad, GLenum normal );
GLU_API void GLU_APIENTRY gluQuadricOrientation ( GLUquadric* quad, GLenum orientation );
GLU_API void GLU_APIENTRY gluQuadricTexture ( GLUquadric* quad, GLboolean texture );
GLU_API void GLU_APIENTRY gluSphere ( GLUquadric* quad, GLfloat radius, GLint slices, GLint stacks );
GLU_API GLint GLU_APIENTRY gluUnProject ( GLfloat winX, GLfloat winY, GLfloat winZ, const GLfloat* model, const GLfloat* proj, const GLint* view, GLfloat* objX, GLfloat* objY, GLfloat* objZ );
GLU_API GLint GLU_APIENTRY gluUnProject4 ( GLfloat winX, GLfloat winY, GLfloat winZ, GLfloat clipW, const GLfloat* model, const GLfloat* proj, const GLint* view, GLfloat nearVal, GLfloat farVal, GLfloat* objX, GLfloat* objY, GLfloat* objZ, GLfloat* objW );
GLU_API GLint GLU_APIENTRY gluScaleImage ( GLenum format, GLsizei widthin, GLsizei heightin, GLenum typein, const void* datain, GLsizei widthout, GLsizei heightout, GLenum typeout, void* dataout );
GLU_API GLint GLU_APIENTRY gluBuild2DMipmapLevels ( GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, GLint userLevel, GLint baseLevel, GLint maxLevel, const void* data );
GLU_API GLint GLU_APIENTRY gluBuild2DMipmaps ( GLenum target, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data );

#define GLU_TESS_MAX_COORD                 1.0e37f

// TessCallback
#define GLU_TESS_BEGIN                     100100
#define GLU_BEGIN                          100100
#define GLU_TESS_VERTEX                    100101
#define GLU_VERTEX                         100101
#define GLU_TESS_END                       100102
#define GLU_END                            100102
#define GLU_TESS_ERROR                     100103
#define GLU_TESS_EDGE_FLAG                 100104
#define GLU_EDGE_FLAG                      100104
#define GLU_TESS_COMBINE                   100105
#define GLU_TESS_BEGIN_DATA                100106
#define GLU_TESS_VERTEX_DATA               100107
#define GLU_TESS_END_DATA                  100108
#define GLU_TESS_ERROR_DATA                100109
#define GLU_TESS_EDGE_FLAG_DATA            100110
#define GLU_TESS_COMBINE_DATA              100111

// TessContour
#define GLU_CW                             100120
#define GLU_CCW                            100121
#define GLU_INTERIOR                       100122
#define GLU_EXTERIOR                       100123
#define GLU_UNKNOWN                        100124

// TessProperty
#define GLU_TESS_WINDING_RULE              100140
#define GLU_TESS_BOUNDARY_ONLY             100141
#define GLU_TESS_TOLERANCE                 100142

// TessError
#define GLU_TESS_ERROR1                    100151
#define GLU_TESS_ERROR2                    100152
#define GLU_TESS_ERROR3                    100153
#define GLU_TESS_ERROR4                    100154
#define GLU_TESS_ERROR5                    100155
#define GLU_TESS_ERROR6                    100156
#define GLU_TESS_ERROR7                    100157
#define GLU_TESS_ERROR8                    100158
#define GLU_TESS_MISSING_BEGIN_POLYGON     100151
#define GLU_TESS_MISSING_BEGIN_CONTOUR     100152
#define GLU_TESS_MISSING_END_POLYGON       100153
#define GLU_TESS_MISSING_END_CONTOUR       100154
#define GLU_TESS_COORD_TOO_LARGE           100155
#define GLU_TESS_NEED_COMBINE_CALLBACK     100156

// TessWinding
#define GLU_TESS_WINDING_ODD               100130
#define GLU_TESS_WINDING_NONZERO           100131
#define GLU_TESS_WINDING_POSITIVE          100132
#define GLU_TESS_WINDING_NEGATIVE          100133
#define GLU_TESS_WINDING_ABS_GEQ_TWO       100134

GLU_API void GLU_APIENTRY gluBeginPolygon ( GLUtesselator* tess );
GLU_API void GLU_APIENTRY gluDeleteTess ( GLUtesselator* tess );
GLU_API void GLU_APIENTRY gluEndPolygon ( GLUtesselator* tess );
GLU_API void GLU_APIENTRY gluGetTessProperty ( GLUtesselator* tess, GLenum which, GLfloat* data );
GLU_API GLUtesselator* GLU_APIENTRY gluNewTess ( void );
GLU_API void GLU_APIENTRY gluNextContour ( GLUtesselator* tess, GLenum type );
GLU_API void GLU_APIENTRY gluTessBeginContour ( GLUtesselator* tess );
GLU_API void GLU_APIENTRY gluTessBeginPolygon ( GLUtesselator* tess, GLvoid* data );
GLU_API void GLU_APIENTRY gluTessCallback ( GLUtesselator* tess, GLenum which, _GLUfuncptr CallBackFunc );
GLU_API void GLU_APIENTRY gluTessEndContour ( GLUtesselator* tess );
GLU_API void GLU_APIENTRY gluTessEndPolygon ( GLUtesselator* tess );
GLU_API void GLU_APIENTRY gluTessNormal ( GLUtesselator* tess, GLfloat valueX, GLfloat valueY, GLfloat valueZ );
GLU_API void GLU_APIENTRY gluTessProperty ( GLUtesselator* tess, GLenum which, GLfloat data );
GLU_API void GLU_APIENTRY gluTessVertex ( GLUtesselator* tess, GLfloat* location, GLvoid* data );

// NurbsError
#define GLU_NURBS_ERROR1                   100251
#define GLU_NURBS_ERROR2                   100252
#define GLU_NURBS_ERROR3                   100253
#define GLU_NURBS_ERROR4                   100254
#define GLU_NURBS_ERROR5                   100255
#define GLU_NURBS_ERROR6                   100256
#define GLU_NURBS_ERROR7                   100257
#define GLU_NURBS_ERROR8                   100258
#define GLU_NURBS_ERROR9                   100259
#define GLU_NURBS_ERROR10                  100260
#define GLU_NURBS_ERROR11                  100261
#define GLU_NURBS_ERROR12                  100262
#define GLU_NURBS_ERROR13                  100263
#define GLU_NURBS_ERROR14                  100264
#define GLU_NURBS_ERROR15                  100265
#define GLU_NURBS_ERROR16                  100266
#define GLU_NURBS_ERROR17                  100267
#define GLU_NURBS_ERROR18                  100268
#define GLU_NURBS_ERROR19                  100269
#define GLU_NURBS_ERROR20                  100270
#define GLU_NURBS_ERROR21                  100271
#define GLU_NURBS_ERROR22                  100272
#define GLU_NURBS_ERROR23                  100273
#define GLU_NURBS_ERROR24                  100274
#define GLU_NURBS_ERROR25                  100275
#define GLU_NURBS_ERROR26                  100276
#define GLU_NURBS_ERROR27                  100277
#define GLU_NURBS_ERROR28                  100278
#define GLU_NURBS_ERROR29                  100279
#define GLU_NURBS_ERROR30                  100280
#define GLU_NURBS_ERROR31                  100281
#define GLU_NURBS_ERROR32                  100282
#define GLU_NURBS_ERROR33                  100283
#define GLU_NURBS_ERROR34                  100284
#define GLU_NURBS_ERROR35                  100285
#define GLU_NURBS_ERROR36                  100286
#define GLU_NURBS_ERROR37                  100287

#ifdef __cplusplus
}
#endif

#endif 
