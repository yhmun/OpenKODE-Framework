/* -----------------------------------------------------------------------------------
 *
 *      File            vpoint.h
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

#ifndef __vpoint_h__
#define __vpoint_h__

#include "../../cocoa/CCGeometry.h"

NS_CC_BEGIN


class VPoint : public CCObject
{
	public :
				 VPoint ( KDvoid );
		virtual ~VPoint ( KDvoid );

		static	 VPoint*	create ( KDvoid );
		static	 VPoint*	create ( KDfloat x, KDfloat y );
		static	 VPoint*	create ( const CCPoint& tPoint );

	public :

		virtual KDbool		init ( KDvoid );
		virtual KDbool		initWithPos ( KDfloat x, KDfloat y );
		virtual KDbool		initWithPoint ( const CCPoint& tPoint );

		virtual KDvoid		setPos ( KDfloat x, KDfloat y );
		
		virtual KDvoid		update ( KDvoid );

		virtual KDvoid		applyGravity ( KDfloat fDelta );
		
		virtual KDvoid		applyGravity ( KDfloat fDelta, KDfloat gx, KDfloat gy );
		
		virtual KDvoid		applyGravityxdt ( KDfloat fDeltaX, KDfloat fDeltaY );
	   
		virtual KDvoid		setPoint ( const CCPoint& tPoint );
		virtual CCPoint		getPoint ( KDvoid );

	public :

		static KDvoid		setGravity ( KDfloat gx, KDfloat gy );
		static CCPoint		getGravity ( KDvoid );

	public :

		union 
		{
			KDfloat			x;
			KDfloat			m_fX;
		};

		union
		{
			KDfloat			y;
			KDfloat			m_fY;
		};
	    
	protected :

		KDfloat				m_fOldX;
		KDfloat				m_fOldY;
};


NS_CC_END

#endif  // __vpoint_h__