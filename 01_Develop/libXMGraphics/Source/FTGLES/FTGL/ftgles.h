/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks "FTGL@opengl.geek.nz>
 * Copyright (c) 2008 Sam Hocevar <sam@zoy.org>
 * Copyright (c) 2008 Sean Morrison <learner@brlcad.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __ftgl__
#define __ftgl__

/* Floating point types used by the library */
typedef double   FTGL_DOUBLE;
typedef float    FTGL_FLOAT;

#include <kode.h>

/* We need the Freetype headers */
#define FTGL_EXPORT	
#include <XMFreeType/ft2build.h>
#include <XMFreeType/freetype.h> // FT_FREETYPE_H
#include <XMFreeType/ftglyph.h>  // FT_GLYPH_H
#include <XMFreeType/ftoutln.h>  // FT_OUTLINE_H


/* Macros used to declare C-linkage types and symbols */
#ifdef __cplusplus
#   define FTGL_BEGIN_C_DECLS extern "C" { namespace FTGL {
#   define FTGL_END_C_DECLS } }
#else
#   define FTGL_BEGIN_C_DECLS
#   define FTGL_END_C_DECLS
#endif

#ifdef __cplusplus
namespace FTGL
{
    typedef enum
    {
        RENDER_FRONT = 0x0001,
        RENDER_BACK  = 0x0002,
        RENDER_SIDE  = 0x0004,
        RENDER_ALL   = 0xffff
    } RenderMode;

    typedef enum
    {
        ALIGN_LEFT    = 0,
        ALIGN_CENTER  = 1,
        ALIGN_RIGHT   = 2,
        ALIGN_JUSTIFY = 3
    } TextAlignment;
}
#else
#   define FTGL_RENDER_FRONT 0x0001
#   define FTGL_RENDER_BACK  0x0002
#   define FTGL_RENDER_SIDE  0x0004
#   define FTGL_RENDER_ALL   0xffff

#   define FTGL_ALIGN_LEFT    0
#   define FTGL_ALIGN_CENTER  1
#   define FTGL_ALIGN_RIGHT   2
#   define FTGL_ALIGN_JUSTIFY 3
#endif

// Compiler-specific conditional compilation
#ifdef _MSC_VER // MS Visual C++

    // Disable various warning.
    // 4786: template name too long
    #pragma warning(disable : 4251)
    #pragma warning(disable : 4275)
    #pragma warning(disable : 4786)
	#pragma warning(disable : 4996)
	#pragma warning(disable : 4355)

#endif

#include "FTPoint.h"
#include "FTBBox.h"
#include "FTBuffer.h"

#include "FTGlyph.h"
#include "FTBitmapGlyph.h"
#include "FTBufferGlyph.h"
//#include "FTExtrdGlyph.h"
#include "FTOutlineGlyph.h"
//#include "FTPixmapGlyph.h"
//#include "FTPolyGlyph.h"
#include "FTTextureGlyph.h"

#include "FTFont.h"
#include "FTGLBitmapFont.h"
#include "FTBufferFont.h"
//#include "FTGLExtrdFont.h"
#include "FTGLOutlineFont.h"
//#include "FTGLPixmapFont.h"
//#include "FTGLPolygonFont.h"
#include "FTGLTextureFont.h"

#include "FTLayout.h"
#include "FTSimpleLayout.h"

#endif  //  __ftgl__
