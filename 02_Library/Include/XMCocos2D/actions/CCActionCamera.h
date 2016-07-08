/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionCamera.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
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

#ifndef __CCActionCamera_h__
#define __CCActionCamera_h__

#include "CCActionInterval.h"

NS_CC_BEGIN
    
/**
 * @addtogroup actions
 * @{
 */

class CCCamera;

/** 
 *	@brief	Base class for CCCamera actions
 *	@ingroup Actions
 */
class CCActionCamera : public CCActionInterval
{
	public :

		CCActionCamera ( KDvoid );

	public :

		// super methods
		virtual KDvoid				startWithTarget ( CCNode* pTarget );
		virtual CCActionInterval*	reverse ( KDvoid );

	protected :

		KDfloat			m_fCenterXOrig;
		KDfloat			m_fCenterYOrig;
		KDfloat			m_fCenterZOrig;

		KDfloat			m_fEyeXOrig;
		KDfloat			m_fEyeYOrig;
		KDfloat			m_fEyeZOrig;

		KDfloat			m_fUpXOrig;
		KDfloat			m_fUpYOrig;
		KDfloat			m_fUpZOrig;
};

/** 
 *	@brief	CCOrbitCamera action
 *	Orbits the camera around the center of the screen using spherical coordinates
 *	@ingroup Actions
 */
class CCOrbitCamera : public CCActionCamera
{
	public :

		CCOrbitCamera ( KDvoid );

		/** creates a CCOrbitCamera action with radius, delta-radius,  z, deltaZ, x, deltaX */
		static   CCOrbitCamera*		create ( KDfloat fDuration, KDfloat fRadius, KDfloat fDeltaRadius, KDfloat fAngleZ, KDfloat fDeltaAngleZ, KDfloat fAngleX, KDfloat fDeltaAngleX );

	public :

		/** initializes a CCOrbitCamera action with radius, delta-radius,  z, deltaZ, x, deltaX */
		KDbool				initWithDuration ( KDfloat fDuration, KDfloat fRadius, KDfloat fDeltaRadius, KDfloat fAngleZ, KDfloat fDeltaAngleZ, KDfloat fAngleX, KDfloat fDeltaAngleX );

		/** positions the camera according to spherical coordinates */
		KDvoid				sphericalRadius ( KDfloat* fRadius, KDfloat* fZenith, KDfloat* fAzimuth );

		// super methods
		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual KDvoid		startWithTarget ( CCNode* pTarget );

		virtual KDvoid		update ( KDfloat fDelta );

	protected :

		KDfloat				m_fRadius;
		KDfloat				m_fDeltaRadius;
		KDfloat				m_fAngleZ;
		KDfloat				m_fDeltaAngleZ;
		KDfloat				m_fAngleX;
		KDfloat				m_fDeltaAngleX;

		KDfloat				m_fRadZ;
		KDfloat				m_fRadDeltaZ;
		KDfloat				m_fRadX;
		KDfloat				m_fRadDeltaX;
};

// end of actions group
/// @}

NS_CC_END

#endif // __CCActionCamera_h__