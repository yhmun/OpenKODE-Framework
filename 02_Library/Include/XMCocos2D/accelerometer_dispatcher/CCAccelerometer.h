/* -----------------------------------------------------------------------------------
 *
 *      File            CCAccelerometer.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#ifndef __CCAccelerometer_h__
#define __CCAccelerometer_h__

#include "CCAccelerometerDelegate.h"

NS_CC_BEGIN
    
class CCAccelerometer
{
	public :
		
 		CCAccelerometer ( KDvoid );
		
	public :

		KDvoid		setDelegate ( CCAccelerometerDelegate* pDelegate );

		KDvoid		setAccelerometerInterval ( KDdouble dInterval );

		KDvoid		update ( KDfloat x, KDfloat y, KDfloat z, KDfloat timestamp );

	private :
		
		CCAcceleration              m_tAccelerationValue;
		CCAccelerometerDelegate*	m_pAccelDelegate;
};

NS_CC_END

#endif // __CCAccelerometer_h__
