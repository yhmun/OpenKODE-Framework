/* -----------------------------------------------------------------------------------
 *
 *      File            CCDrawNode.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Scott Lembcke and Howling Moon Software
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

/**
 *	Code copied & pasted from SpacePatrol game https://github.com/slembcke/SpacePatrol
 *
 *	Renamed and added some changes for cocos2d
 *
 */

#ifndef __CCDrawNode_h__
#define __CCDrawNode_h__

#include "../../ccTypes.h"
#include "../base_nodes/CCNode.h"

NS_CC_BEGIN

/**
 *	DrawNode
 *	Node that draws dots, segments and polygons.
 *	Faster than the "drawing primitives" since they it draws everything in one single batch.
 *
 *	@since v2.1
 */
class CC_DLL DrawNode : public Node
{
	public :

		/** creates and initialize a DrawNode node */
		static DrawNode*	create ( KDvoid );

		/**
		 *	@js ctor
		 */
		DrawNode ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~DrawNode ( KDvoid );
    
		virtual KDbool		init ( KDvoid );

		/** draw a dot at a position, with a given radius and color */
		KDvoid				drawDot ( const Point& tPos, KDfloat fRadius, const Color4F& tColor );
    
		/** draw a segment with a radius and color */
		KDvoid				drawSegment ( const Point& tFrom, const Point& tTo, KDfloat fRadius, const Color4F& tColor );
    
		/** 
		 *	draw a polygon with a fill color and line color
		 *	@code
		 *	When this function bound into js or lua,the parameter will be changed
		 *	In js: var drawPolygon(var Arrayofpoints, var fillColor, var width, var borderColor)
		 *	In lua:local drawPolygon(local pointTable,local tableCount,local fillColor,local width,local borderColor)
		 *	@endcode
		 */
		KDvoid				drawPolygon ( Point* pVerts, KDint32 nCount, const Color4F& tFillColor, KDfloat fBorderWidth, const Color4F& tBorderColor );
    
		/** Clear the geometry in the node's buffer. */
		KDvoid				clear ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		const BlendFunc&	getBlendFunc ( KDvoid ) const;

		/**
		 *	@code
		 *	When this function bound into js or lua,the parameter will be changed
		 *	In js: var setBlendFunc(var src, var dst)
		 *	@endcode
		 *	@lua NA
		 */
		KDvoid				setBlendFunc ( const BlendFunc& tBlendFunc );
    
		/** 
		 *	listen the event that coming to foreground on Android
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid				listenBackToForeground ( Object* pObj );

		// Overrides
		virtual KDvoid		draw ( KDvoid ) override;

	protected :

		KDvoid				ensureCapacity ( KDint32 nCount );
		KDvoid				render ( KDvoid );

		GLuint				m_uVao;
		GLuint				m_uVbo;

		KDint32				m_nBufferCapacity;
		GLsizei				m_nBufferCount;
		V2F_C4B_T2F*		m_pBuffer;

		BlendFunc			m_tBlendFunc;

		KDbool				m_bDirty;
};

NS_CC_END

#endif	// __CCDrawNode_h__
