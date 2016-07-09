/* -----------------------------------------------------------------------------------
 *
 *      File            CCParallaxScrollOffset.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
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

#include "../ExtensionMacros.h"
#include "../../ccMacros.h"
#include "../../2d/layers_scenes_transitions_nodes/CCLayer.h"

NS_CC_EXT_BEGIN

class ParallaxScrollOffset : public Layer
{
	public :

		static ParallaxScrollOffset*	create ( Node* pNode, const Point& tRatio, const Point& tPosition, const Point& tOffset );
		static ParallaxScrollOffset*	create ( Node* pNode, const Point& tRatio, const Point& tPosition, const Point& tOffset, const Point& tVelocity );

		static ParallaxScrollOffset*	create ( KDvoid );

	public :

		virtual KDbool					initWithNode ( Node* pNode, const Point& tRatio, const Point& tPosition, const Point& tOffset );
		virtual KDbool					initWithNode ( Node* pNode, const Point& tRatio, const Point& tPosition, const Point& tOffset, const Point& tVelocity );

		virtual KDbool					init ( KDvoid );  

	public :

		CC_SYNTHESIZE ( Point, m_tScrollOffset	, ScrollOffset	);
		CC_SYNTHESIZE ( Point, m_tOrigPosition	, OrigPosition	);
		CC_SYNTHESIZE ( Point, m_tRelVelocity	, RelVelocity	);
		CC_SYNTHESIZE ( Point, m_tRatio			, Ratio			);
		CC_SYNTHESIZE ( Node*, m_pTheChild		, TheChild		);	   
};

NS_CC_EXT_END

#endif	// __CCParallaxScrollOffset_h__
	