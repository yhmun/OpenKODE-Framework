/* -----------------------------------------------------------------------------------
 *
 *      File            TestParallaxScrollNode.h
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

#ifndef __TestParallaxScrollNode_h__
#define __TestParallaxScrollNode_h__

#include "../TestBasic.h"

class TestParallaxScrollNode : public TestBasic
{
	public :

		CREATE_FUNC ( TestParallaxScrollNode );

	protected :		

		virtual KDbool			init   ( KDvoid );
		virtual KDuint			count  ( KDvoid );
		virtual KDvoid			update ( KDfloat fDelta );

		KDfloat					randomValueBetween ( KDfloat fLow, KDfloat fHigh );

	private :

		CCParallaxScrollNode*	m_pParallax;
};

#endif	// __TestParallax_h__
