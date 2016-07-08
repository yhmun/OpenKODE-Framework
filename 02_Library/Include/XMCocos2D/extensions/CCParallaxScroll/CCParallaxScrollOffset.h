/* -----------------------------------------------------------------------------------
 *
 *      File            CCParallaxScrollOffset.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2012      Created by Thomas Broquist on 3/26/12. 
 *      Copyright (c) 2012      Little Wins LLC. All rights reserved.
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *      
 *      The above copyright notice and this permission notice shall be included in
 *       all copies or substantial portions of the Software.
 *      
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCParallaxScrollOffset_h__
#define __CCParallaxScrollOffset_h__

#include "../../layers_scenes_transitions_nodes/CCLayer.h"

NS_CC_BEGIN

class CCParallaxScrollOffset : public CCLayer
{
	public :

		static CCParallaxScrollOffset*	create ( CCNode* pNode, const CCPoint& tRatio, const CCPoint& tPosition, const CCPoint& tOffset );
		static CCParallaxScrollOffset*	create ( CCNode* pNode, const CCPoint& tRatio, const CCPoint& tPosition, const CCPoint& tOffset, const CCPoint& tVelocity );

		static CCParallaxScrollOffset*	create ( KDvoid );

	public :

		virtual KDbool	initWithNode ( CCNode* pNode, const CCPoint& tRatio, const CCPoint& tPosition, const CCPoint& tOffset );
		virtual KDbool	initWithNode ( CCNode* pNode, const CCPoint& tRatio, const CCPoint& tPosition, const CCPoint& tOffset, const CCPoint& tVelocity );

		virtual KDbool	init ( KDvoid );  

	public :

		CC_SYNTHESIZE ( CCPoint, m_tScrollOffset, ScrollOffset	);
		CC_SYNTHESIZE ( CCPoint, m_tOrigPosition, OrigPosition	);
		CC_SYNTHESIZE ( CCPoint, m_tRelVelocity	, RelVelocity	);
		CC_SYNTHESIZE ( CCPoint, m_tRatio		, Ratio			);
		CC_SYNTHESIZE ( CCNode*, m_pTheChild	, TheChild		);	   
};

NS_CC_END

#endif	// __CCParallaxScrollOffset_h__
	