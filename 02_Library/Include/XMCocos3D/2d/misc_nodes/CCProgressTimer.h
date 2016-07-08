/* -----------------------------------------------------------------------------------
 *
 *      File            CCProgresstimer.h  
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Lam Pham
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

#ifndef __CCProgresstimer_h__
#define __CCProgresstimer_h__

#include "../sprite_nodes/CCSprite.h"
#ifdef EMSCRIPTEN
#include "../base_nodes/CCGLBufferedNode.h"
#endif // EMSCRIPTEN

NS_CC_BEGIN

/**
 *	@addtogroup misc_nodes
 *	@{
 */

/**
 *	@brief ProgressTimer is a subclass of Node.
 *	It renders the inner sprite according to the percentage.
 *	The progress can be Radial, Horizontal or vertical.
 *	@since v0.99.1
 */
class CC_DLL ProgressTimer : public NodeRGBA
#ifdef EMSCRIPTEN
, public GLBufferedNode
#endif // EMSCRIPTEN
{
	public :

		/** 
		 *	Types of progress
		 *	@since v0.99.1
		 */
		enum class Type
		{
			/// Radial Counter-Clockwise
			RADIAL,
			/// Bar
			BAR,
		};
    
		/** Creates a progress timer with the sprite as the shape the timer goes through */
		static ProgressTimer*	create ( Sprite* sp );

		/**
		 *	@js ctor
		 */
		ProgressTimer ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ProgressTimer ( KDvoid );

		/** Initializes a progress timer with the sprite as the shape the timer goes through */
		KDbool					initWithSprite ( Sprite* sp );

		/** Change the percentage to change progress. */
		inline Type				getType ( KDvoid ) const { return m_eType; }

		/** Percentages are from 0 to 100 */
		inline KDfloat			getPercentage ( KDvoid ) const {return m_fPercentage; }

		/** The image to show the progress percentage, retain */
		inline Sprite*			getSprite ( KDvoid ) const { return m_pSprite; }

		KDvoid					setPercentage ( KDfloat fPercentage );
		KDvoid					setSprite ( Sprite* pSprite );
		KDvoid					setType ( Type eType );
		
		/**
		 *	@js setReverseDirection
		 *	@lua setReverseDirection
		 */
		KDvoid					setReverseProgress ( KDbool bReverse );

		inline KDbool			 isReverseDirection ( KDvoid ) { return m_bReverseDirection; }
		inline KDvoid			setReverseDirection ( KDbool bValue ) { m_bReverseDirection = bValue; }

		/**
		 *    Midpoint is used to modify the progress start position.
		 *    If you're using radials type then the midpoint changes the center point
		 *    If you're using bar type the the midpoint changes the bar growth
		 *        it expands from the center but clamps to the sprites edge so:
		 *        you want a left to right then set the midpoint all the way to Point(0,y)
		 *        you want a right to left then set the midpoint all the way to Point(1,y)
		 *        you want a bottom to top then set the midpoint all the way to Point(x,0)
		 *        you want a top to bottom then set the midpoint all the way to Point(x,1)
		 */
		KDvoid					setMidpoint ( const Point& tPoint );

		/** Returns the Midpoint */
		Point					getMidpoint ( KDvoid ) const;

		/**
		 *    This allows the bar type to move the component at a specific rate
		 *    Set the component to 0 to make sure it stays at 100%.
		 *    For example you want a left to right bar but not have the height stay 100%
		 *    Set the rate to be Point(0,1); and set the midpoint to = Point(0,.5f);
		 */
		inline KDvoid			setBarChangeRate ( const Point& tBarChangeRate ) { m_tBarChangeRate = tBarChangeRate; }

		/** Returns the BarChangeRate */
		inline Point			getBarChangeRate ( KDvoid ) const { return m_tBarChangeRate; }

		// Overrides
		virtual KDvoid			draw ( KDvoid ) override;
		KDvoid					setAnchorPoint ( const Point& tAnchorPoint ) override;
		virtual KDvoid			setColor ( const Color3B& tColor ) override;
		virtual const Color3B&	getColor ( KDvoid ) const override;
		virtual GLubyte			getOpacity ( KDvoid ) const override;
		virtual KDvoid			setOpacity ( GLubyte cOpacity ) override;
    
	protected :

		Tex2F					textureCoordFromAlphaPoint ( Point tAlpha );
		Vertex2F				vertexFromAlphaPoint ( Point tAlpha );
		KDvoid					updateProgress ( KDvoid );
		KDvoid					updateBar ( KDvoid );
		KDvoid					updateRadial ( KDvoid );
		KDvoid					updateColor ( KDvoid );
		Point					boundaryTexCoord ( KDchar cindex );

		Type					m_eType;
		Point					m_tMidpoint;
		Point					m_tBarChangeRate;
		KDfloat					m_fPercentage;
		Sprite*					m_pSprite;
		KDint					m_nVertexDataCount;
		V2F_C4B_T2F*			m_pVertexData;

		KDbool					m_bReverseDirection;
};

// end of misc_nodes group
/// @}

NS_CC_END

#endif	// __CCProgresstimer_h__
