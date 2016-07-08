/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionCamera.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCActionCamera_h__
#define __CCActionCamera_h__

#include "CCActionInterval.h"

NS_CC_BEGIN

class Camera;

/**
 *	@addtogroup actions
 *	@{
 */

/** 
 *	@brief Base class for Camera actions
 *	@ingroup Actions
 */
class CC_DLL ActionCamera : public ActionInterval //<NSCopying> 
{
	public :

		/**
		 *	@js ctor
		 */
		ActionCamera ( KDvoid )
		{
			m_fCenterXOrig	= 0;
			m_fCenterYOrig	= 0;
			m_fCenterZOrig	= 0;
			m_fEyeXOrig		= 0;
			m_fEyeYOrig		= 0;
			m_fEyeZOrig		= 0;
			m_fUpXOrig		= 0;
			m_fUpYOrig		= 0;
			m_fUpZOrig		= 0;
		}

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ActionCamera ( KDvoid ) { }

		// Overrides
		virtual KDvoid			startWithTarget ( Node* pTarget ) override;
		virtual ActionCamera*	reverse ( KDvoid ) const override;
		virtual ActionCamera*	clone ( KDvoid ) const override;

	protected :

		KDfloat					m_fCenterXOrig;
		KDfloat					m_fCenterYOrig;
		KDfloat					m_fCenterZOrig;

		KDfloat					m_fEyeXOrig;
		KDfloat					m_fEyeYOrig;
		KDfloat					m_fEyeZOrig;

		KDfloat					m_fUpXOrig;
		KDfloat					m_fUpYOrig;
		KDfloat					m_fUpZOrig;
};

/** 
 *	@brief OrbitCamera action
 *	Orbits the camera around the center of the screen using spherical coordinates
 *	@ingroup Actions
 */
class CC_DLL OrbitCamera : public ActionCamera //<NSCopying> 
{
	public :

		/** creates a OrbitCamera action with radius, delta-radius,  z, deltaZ, x, deltaX */
		static OrbitCamera*		create ( KDfloat t, KDfloat radius, KDfloat deltaRadius, KDfloat angleZ, KDfloat deltaAngleZ, KDfloat angleX, KDfloat deltaAngleX );

		/**
		 * @js ctor
		 */
		OrbitCamera ( KDvoid )    
		{
			m_fRadius		= 0;
			m_fDeltaRadius	= 0;
			m_fAngleZ		= 0;
			m_fDeltaAngleZ	= 0;
			m_fAngleX		= 0;      
			m_fDeltaAngleX	= 0;
			m_fRadZ			= 0;
			m_fRadDeltaZ	= 0;
			m_fRadX			= 0;                        
			m_fRadDeltaX	= 0; 
		}

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~OrbitCamera ( KDvoid ) { }
    
		/** initializes a OrbitCamera action with radius, delta-radius,  z, deltaZ, x, deltaX */
		KDbool					initWithDuration ( KDfloat t, KDfloat fRadius, KDfloat fDeltaRadius, KDfloat fAngleZ, KDfloat fDeltaAngleZ, KDfloat fAngleX, KDfloat fDeltaAngleX );

		/** positions the camera according to spherical coordinates */
		KDvoid					sphericalRadius ( KDfloat* r, KDfloat* pZenith, KDfloat* pAzimuth );

		// Overrides
		OrbitCamera*			clone ( KDvoid ) const override;
		virtual KDvoid			startWithTarget ( Node* pTarget ) override;
		virtual KDvoid			update ( KDfloat fTime ) override;

	protected :

		KDfloat					m_fRadius;
		KDfloat					m_fDeltaRadius;
		KDfloat					m_fAngleZ;
		KDfloat					m_fDeltaAngleZ;
		KDfloat					m_fAngleX;
		KDfloat					m_fDeltaAngleX;

		KDfloat					m_fRadZ;
		KDfloat					m_fRadDeltaZ;
		KDfloat					m_fRadX;
		KDfloat					m_fRadDeltaX;
};

//	end of actions group
/// @}

NS_CC_END

#endif	// __CCActionCamera_h__