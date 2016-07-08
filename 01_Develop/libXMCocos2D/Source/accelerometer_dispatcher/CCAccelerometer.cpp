/* -----------------------------------------------------------------------------------
 *
 *      File            CCAccelerometer.cpp
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

#include "Precompiled.h"
#include "accelerometer_dispatcher/CCAccelerometer.h"

NS_CC_BEGIN
    
CCAccelerometer::CCAccelerometer ( KDvoid )
{
	m_pAccelDelegate = KD_NULL;
	kdMemset ( &m_tAccelerationValue, 0, sizeof ( m_tAccelerationValue ) );
}

KDvoid CCAccelerometer::setDelegate ( CCAccelerometerDelegate* pDelegate ) 
{
	m_pAccelDelegate = pDelegate;
}

KDvoid CCAccelerometer::setAccelerometerInterval ( KDdouble dInterval )
{

}

KDvoid CCAccelerometer::update ( KDfloat x, KDfloat y, KDfloat z, KDfloat timestamp ) 
{
	if ( m_pAccelDelegate )
	{
		m_tAccelerationValue.x = x;
		m_tAccelerationValue.y = y; 
		m_tAccelerationValue.z = z; 
		m_tAccelerationValue.timestamp = timestamp;

		m_pAccelDelegate->didAccelerate ( &m_tAccelerationValue );
	}
}
    
NS_CC_END
