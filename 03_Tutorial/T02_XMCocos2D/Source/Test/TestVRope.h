/* -----------------------------------------------------------------------------------
 *
 *      File            TestVRope.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2012      Creeng Ltd. ( Ported to cocos2d-x 12/7/2012 )
 *      Copyright (c) 2012      Flightless Ltd. ( Modified 20/4/2012 )
 *      Copyright (c) 2010      Clever Hamster Games. ( Created by patrick 14/10/2010 )
 *
 *         http://www.www.flightless.co.nz 
 *         http://www.creeng.com      
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy 
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights 
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is furnished
 *      to do so, subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be included in all 
 *      copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *      INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *      A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *      HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
 *      OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 *      SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#ifndef __TestVRope_h__
#define __TestVRope_h__

#include "../TestBasic.h"

class TestVRope : public TestBasic
{
	public :

		CREATE_FUNC ( TestVRope );

	protected :		

		virtual KDbool			init ( KDvoid );

		virtual	KDuint			count ( KDvoid );

		virtual KDvoid			update ( KDfloat fDelta );

		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid					initPhysics ( KDvoid );

		KDvoid					addNewSpriteWithCoords ( const CCPoint& tPoint );

	private :

		b2World*				m_pWorld;
		b2Body*					m_pTopAnchorBody;
		CCTexture2D*			m_pSpriteTexture;
};

#endif	// __TestVRope_h__
