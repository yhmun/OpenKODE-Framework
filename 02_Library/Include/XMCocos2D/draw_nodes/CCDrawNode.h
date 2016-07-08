/* -----------------------------------------------------------------------------------
 *
 *      File            CCDrawNode.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCDrawNode_h__
#define __CCDrawNode_h__

#include "../base_nodes/CCNode.h"
#include "../ccTypes.h"

NS_CC_BEGIN
   
/** 
 *	CCDrawNode
 *	Node that draws dots, segments and polygons.
 *	Faster than the "drawing primitives" since they it draws everything in one single batch.
 *
 *	@since v2.1
 */
class CCDrawNode : public CCNode
{	    
	public :
				 CCDrawNode ( KDvoid );
		virtual ~CCDrawNode ( KDvoid );

		static   CCDrawNode*	create ( KDvoid );
		
	public :

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		draw ( KDvoid );
	    
		/** draw a dot at a position, with a given radius and color */
		KDvoid				drawDot ( const CCPoint& tPos, KDfloat fRadius, const ccColor4F& tColor );
	    
		/** draw a segment with a radius and color */
		KDvoid				drawSegment ( const CCPoint& tFrom, const CCPoint& tTo, KDfloat fRadius, const ccColor4F& tColor );
	    
		/** draw a polygon with a fill color and line color */
		KDvoid				drawPolygon ( CCPoint* pVerts, KDuint uCount, const ccColor4F& tFillColor, KDfloat fBorderWidth, const ccColor4F& tBorderColor );
	    
		/** Clear the geometry in the node's buffer. */
		KDvoid				clear ( KDvoid );
	    
		ccBlendFunc			getBlendFunc ( KDvoid ) const;
		KDvoid				setBlendFunc ( const ccBlendFunc& tBlendFunc );	    		
	    
		/** 
		 *	listen the event that coming to foreground on Android  
		 *	@js NA
		 */
		KDvoid				listenBackToForeground ( CCObject* pObject );

	private :

		KDvoid				ensureCapacity ( KDuint uCount );
		KDvoid				render ( KDvoid );
	
	protected :

		GLuint				m_uVao;
		GLuint				m_uVbo;
	    
		KDuint				m_uBufferCapacity;
		GLsizei				m_nBufferCount;
		ccV2F_C4B_T2F*		m_pBuffer;
	    
		ccBlendFunc			m_tBlendFunc;
	    
		KDbool				m_bDirty;
};

NS_CC_END

#endif // __CCDrawNode_h__
