/* -----------------------------------------------------------------------------------
 *
 *      File            vstick.h
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

#ifndef __vstick_h__
#define __vstick_h__

#include "vpoint.h"

NS_CC_BEGIN


class VStick : public CCObject
{
	public :
				 VStick ( KDvoid );
		virtual ~VStick ( KDvoid );

		static	 VStick*	create ( KDvoid );
		static	 VStick*	create ( VPoint* pPointA, VPoint* pPointB );

	public :

		virtual KDbool		init ( KDvoid );

		virtual KDbool		initWithPoints ( VPoint* pPointA, VPoint* pPointB );

		virtual	KDvoid		contract ( KDvoid );

		virtual VPoint*		getPointA ( KDvoid );
		virtual	VPoint*		getPointB ( KDvoid );

		virtual KDvoid		setPointA ( VPoint* pPoint );
		virtual KDvoid		setPointB ( VPoint* pPoint );

	protected :

		VPoint*				m_pPointA;
		VPoint*				m_pPointB;
		KDfloat				m_fHypotenuse;
};


NS_CC_END

#endif	// __vstick_h__