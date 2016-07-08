/* -----------------------------------------------------------------------------------
 *
 *      File            CCProgresstimer.h  
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCProgresstimer_h__
#define __CCProgresstimer_h__

#include "../sprite_nodes/CCSprite.h"

NS_CC_BEGIN
    
/**
 * @addtogroup misc_nodes
 * @{
 */

/**
 *	Types of progress
 *	@since	v0.99.1
 */
typedef enum 
{
	/// Radial Counter-Clockwise
    kCCProgressTimerTypeRadial	,

    /// Bar
    kCCProgressTimerTypeBar		,
} CCProgressTimerType;

/**
 *	@brief	CCProgressTimer is a subclass of CCNode.
 *	It renders the inner sprite according to the percentage.
 *	The progress can be Radial, Horizontal or vertical.
 *	@since	v0.99.1
 */
class CCProgressTimer : public CCNodeRGBA
{
	public :
		         CCProgressTimer ( KDvoid );
		virtual ~CCProgressTimer ( KDvoid );

		static   CCProgressTimer*	create ( const KDchar* szPath );

		/** Creates a progress timer with the sprite as the shape the timer goes through */
		static   CCProgressTimer*	create ( CCSprite* pSprite );

	public :

		/** Initializes a progress timer with the sprite as the shape the timer goes through */
		virtual KDbool				initWithSprite ( CCSprite* pSprite );

		/**    Change the percentage to change progress. */
		inline  CCProgressTimerType		getType ( KDvoid ) { return m_eType; }

		/** Percentages are from 0 to 100 */
		virtual KDfloat				getPercentage ( KDvoid ) { return m_fPercentage; }

		inline  CCSprite*			getSprite ( KDvoid ) { return m_pSprite; }	

		virtual KDvoid				setPercentage ( KDfloat fPercentage );

		KDvoid						setSprite ( CCSprite* pSprite );

		KDvoid						setType ( CCProgressTimerType eType );

		KDvoid						setReverseProgress ( KDbool bReverse );

		virtual KDvoid				draw ( KDvoid );

		virtual KDvoid				setAnchorPoint ( CCPoint tAnchorPoint );

		virtual KDvoid				setColor ( const ccColor3B& tColor );
		virtual const ccColor3B&	getColor ( KDvoid ) const;

		virtual GLubyte				getOpacity ( KDvoid ) const;
		virtual KDvoid				setOpacity ( GLubyte cOpacity );

		inline  KDbool				 isReverseDirection ( KDvoid ) { return m_bReverseDirection; };
		inline  KDvoid				setReverseDirection ( KDbool bValue ) { m_bReverseDirection = bValue; };

	protected :

		ccTex2F						textureCoordFromAlphaPoint ( CCPoint tAlpha );
		ccVertex2F					vertexFromAlphaPoint ( CCPoint tAlpha );
		
		KDvoid						updateProgress ( KDvoid );
		KDvoid						updateBar ( KDvoid );
		KDvoid						updateRadial ( KDvoid );
		KDvoid						updateColor ( KDvoid );

		CCPoint						boundaryTexCoord ( KDchar cIndex );

	protected :

		/**
		 *    Midpoint is used to modify the progress start position.
		 *    If you're using radials type then the midpoint changes the center point
		 *    If you're using bar type the the midpoint changes the bar growth
		 *        it expands from the center but clamps to the sprites edge so:
		 *        you want a left to right then set the midpoint all the way to ccp(0,y)
		 *        you want a right to left then set the midpoint all the way to ccp(1,y)
		 *        you want a bottom to top then set the midpoint all the way to ccp(x,0)
		 *        you want a top to bottom then set the midpoint all the way to ccp(x,1)
		 */
		CC_PROPERTY ( CCPoint, m_tMidpoint, Midpoint );

		/**
		 *    This allows the bar type to move the component at a specific rate
		 *    Set the component to 0 to make sure it stays at 100%.
		 *    For example you want a left to right bar but not have the height stay 100%
		 *    Set the rate to be ccp(0,1); and set the midpoint to = ccp(0,.5f);
		 */
		CC_SYNTHESIZE ( CCPoint, m_tBarChangeRate, BarChangeRate );

	protected :

		KDbool						m_bReverseDirection;

		CCProgressTimerType			m_eType;
		KDfloat						m_fPercentage;
		CCSprite*					m_pSprite;
		KDint						m_nVertexDataCount;
		ccV2F_C4B_T2F*				m_pVertexData;
};

// end of misc_nodes group
/// @}

NS_CC_END

#endif // __CCProgresstimer_h__
