/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionGrid3D.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      On-Core
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

#ifndef __CCActionGrid3D_h__
#define __CCActionGrid3D_h__

#include "CCActionGrid.h"

NS_CC_BEGIN
    
/**
 * @addtogroup actions
 * @{
 */

/** 
 *	@brief	CCWaves3D action 
 */
class CCWaves3D : public CCGrid3DAction
{
	public :

		/** create the action */
		static  CCWaves3D*		create ( KDfloat fDuration, const CCSize& tGridSize, KDuint uWaves, KDfloat fAmplitude );

	public :

		/** init the action */
		virtual KDbool			initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDuint uWaves, KDfloat fAmplitude );

		inline  KDfloat			getAmplitude ( KDvoid ) { return m_fAmplitude; }
		inline  KDvoid			setAmplitude ( KDfloat fAmplitude ) { m_fAmplitude = fAmplitude; }

		inline  KDfloat			getAmplitudeRate ( KDvoid ) { return m_fAmplitudeRate; }
		inline  KDvoid			setAmplitudeRate ( KDfloat fAmplitudeRate ) { m_fAmplitudeRate = fAmplitudeRate; }

		virtual CCObject*		copyWithZone ( CCZone* pZone );

		virtual KDvoid			update ( KDfloat fTime );

	protected :

		KDuint					m_uWaves;
		KDfloat					m_fAmplitude;
		KDfloat					m_fAmplitudeRate;
};

/** 
 *	@brief	CCFlipX3D action 
 */
class CCFlipX3D : public CCGrid3DAction
{
	public :

		/** creates the action with duration */
		static  CCFlipX3D*		create ( KDfloat fDuration );

	public :

		/** initializes the action with duration */
		virtual KDbool			initWithDuration ( KDfloat fDuration );
		virtual KDbool			initWithSize ( const CCSize& tGridSize, KDfloat fDuration );

		virtual CCObject*		copyWithZone ( CCZone* pZone );

		virtual KDvoid			update ( KDfloat fTime );
};

/** 
 *	@brief	CCFlipY3D action 
 */
class CCFlipY3D : public CCFlipX3D
{
	public :

		/** creates the action with duration */
		static  CCFlipY3D*		create ( KDfloat fDuration );

	public :

		virtual KDvoid			update ( KDfloat fTime );

		virtual CCObject*		copyWithZone ( CCZone* pZone );

};

/** 
 *	@brief	CCLens3D action 
 */
class CCLens3D : public CCGrid3DAction
{
	public :

		/** creates the action with center position, radius, a grid size and duration */
        static  CCLens3D*		create ( KDfloat fDuration, const CCSize& tGridSize, const CCPoint& tPosition, KDfloat fRadius );

	public :

		/** initializes the action with center position, radius, a grid size and duration */
		virtual KDbool			initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, const CCPoint& tPosition, KDfloat fRadius );

		/** Get lens center position */
		inline  KDfloat			getLensEffect ( KDvoid ) { return m_fLensEffect; }

		/** Set lens center position */
		inline  KDvoid			setLensEffect ( KDfloat fLensEffect ) { m_fLensEffect = fLensEffect; }
		
		/** Set whether lens is concave */
		inline  KDvoid			setConcave ( KDbool bConcave) { m_bConcave = bConcave; }

		inline  const CCPoint&	getPosition ( KDvoid )	{ return m_tPosition; }
		        KDvoid			setPosition ( const CCPoint& tPosition );

		virtual CCObject*		copyWithZone ( CCZone* pZone );

		virtual KDvoid			update ( KDfloat fTime );

	protected :

		/* lens center position */
		CCPoint					m_tPosition;
		KDfloat					m_fRadius;

		/** lens effect. Defaults to 0.7 - 0 means no effect, 1 is very strong effect */
		KDfloat					m_fLensEffect;

		/** lens is concave. (true = concave, false = convex) default is convex i.e. false */
		KDbool					m_bConcave;

        KDbool					m_bDirty;
};

/** 
 *	@brief	CCRipple3D action 
 */
class CCRipple3D : public CCGrid3DAction
{
	public :

		/** creates the action with radius, number of waves, amplitude, a grid size and duration */
		static  CCRipple3D*		create ( KDfloat fDuration, const CCSize& tGridSize, const CCPoint& tPosition, KDfloat fRadius, KDuint uWaves, KDfloat fAmplitude );

	public :

		/** initializes the action with radius, number of waves, amplitude, a grid size and duration */
		virtual KDbool			initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, const CCPoint& tPosition, KDfloat fRadius, KDuint uWaves, KDfloat fAmplitude );

		/** get center position */
		inline  const CCPoint&	getPosition ( KDvoid ) { return m_tPosition; }

		/** set center position */
		KDvoid					setPosition ( const CCPoint& tPosition );

		inline  KDfloat			getAmplitude ( KDvoid ) { return m_fAmplitude; }
		inline  KDvoid			setAmplitude ( KDfloat fAmplitude ) { m_fAmplitude = fAmplitude; }

		inline  KDfloat			getAmplitudeRate ( KDvoid ) { return m_fAmplitudeRate; }
		inline  KDvoid			setAmplitudeRate ( float fAmplitudeRate ) { m_fAmplitudeRate = fAmplitudeRate; }

		virtual CCObject*		copyWithZone ( CCZone* pZone );

		virtual KDvoid			update ( KDfloat fTime );

	protected :

		/* center position */
		CCPoint					m_tPosition;
		KDfloat					m_fRadius;
		KDuint					m_uWaves;
		KDfloat					m_fAmplitude;
		KDfloat					m_fAmplitudeRate;
};

/** 
 *	@brief	CCShaky3D action 
 */
class CCShaky3D : public CCGrid3DAction
{
	public :

		/** creates the action with a range, shake Z vertices, a grid and duration */
        static  CCShaky3D*		create ( KDfloat fDuration, const CCSize& tGridSize, KDint nRange, KDbool bShakeZ );

	public :

		/** initializes the action with a range, shake Z vertices, a grid and duration */
		virtual KDbool			initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDint nRange, KDbool bShakeZ );

		virtual CCObject*		copyWithZone ( CCZone* pZone );

		virtual KDvoid			update ( KDfloat fTime );

	protected :

		KDint					m_nRange;
		KDbool					m_bShakeZ;
};

/** 
 *	@brief	CCLiquid action 
 */
class CCLiquid : public CCGrid3DAction
{
	public :

		/** creates the action with amplitude, a grid and duration */
		static  CCLiquid*		create ( KDfloat fDuration, const CCSize& tGridSize, KDuint uWaves, KDfloat fAmplitude );

	public :

		/** initializes the action with amplitude, a grid and duration */
		virtual KDbool			initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDuint uWaves, KDfloat fAmplitude );

		inline  KDfloat			getAmplitude ( KDvoid ) { return m_fAmplitude; }
		inline  KDvoid			setAmplitude ( KDfloat fAmplitude ) { m_fAmplitude = fAmplitude; }

		inline  KDfloat			getAmplitudeRate ( KDvoid ) { return m_fAmplitudeRate;	}
		inline  KDvoid			setAmplitudeRate ( KDfloat fAmplitudeRate ) { m_fAmplitudeRate = fAmplitudeRate; }

		virtual CCObject*		copyWithZone ( CCZone* pZone );   

		virtual KDvoid			update ( KDfloat fTime );

	protected :

		KDuint					m_uWaves;
		KDfloat					m_fAmplitude;
		KDfloat					m_fAmplitudeRate;
};

/** 
 *	@brief	CCWaves action 
 */
class CCWaves : public CCGrid3DAction
{
	public :

		/** initializes the action with amplitude, horizontal sin, vertical sin, a grid and duration */
        static  CCWaves*		create ( KDfloat fDuration, const CCSize& tGridSize, KDuint uWaves, KDfloat fAmplitude, KDbool bHorizontal, KDbool bVertical );

	public :

		/** initializes the action with amplitude, horizontal sin, vertical sin, a grid and duration */
		virtual KDbool			initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDuint uWaves, KDfloat fAmplitude, KDbool bHorizontal, KDbool bVertical );

		inline  KDfloat			getAmplitude ( KDvoid ) { return m_fAmplitude; }
		inline  KDvoid			setAmplitude ( KDfloat fAmplitude ) { m_fAmplitude = fAmplitude; }

		inline  KDfloat			getAmplitudeRate ( KDvoid ) { return m_fAmplitudeRate;	}
		inline  KDvoid			setAmplitudeRate ( KDfloat fAmplitudeRate ) { m_fAmplitudeRate = fAmplitudeRate; }

		virtual CCObject*		copyWithZone ( CCZone* pZone );

		virtual KDvoid			update ( KDfloat fTime );

	protected :

		KDuint					m_uWaves;
		KDfloat					m_fAmplitude;
		KDfloat					m_fAmplitudeRate;
		KDbool					m_bVertical;
		KDbool					m_bHorizontal;
};

/** 
 *	@brief	CCTwirl action 
 */
class CCTwirl : public CCGrid3DAction
{
	public :

		/** creates the action with center position, number of twirls, amplitude, a grid size and duration */
		static  CCTwirl*		create ( KDfloat fDuration, const CCSize& tGridSize, const CCPoint& tPosition, KDuint uTwirls, KDfloat fAmplitude );

	public :

		/** initializes the action with center position, number of twirls, amplitude, a grid size and duration */
		virtual KDbool			initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, const CCPoint& tPosition, KDuint uTwirls, KDfloat fAmplitude );

		/** get twirl center */
		inline  const CCPoint&	getPosition ( KDvoid ) { return m_tPosition; }

		/** set twirl center */
		KDvoid					setPosition ( const CCPoint& tPosition );

		inline  KDfloat			getAmplitude ( KDvoid ) { return m_fAmplitude; }
		inline  KDvoid			setAmplitude ( KDfloat fAmplitude ) { m_fAmplitude = fAmplitude; }

		inline  KDfloat			getAmplitudeRate ( KDvoid ) { return m_fAmplitudeRate; }
		inline  KDvoid			setAmplitudeRate ( KDfloat fAmplitudeRate ) { m_fAmplitudeRate = fAmplitudeRate; }

		virtual CCObject*		copyWithZone ( CCZone* pZone );

		virtual KDvoid			update ( KDfloat fTime );

	protected :

		/* twirl center */
		CCPoint					m_tPosition;
		KDuint					m_uTwirls;
		KDfloat					m_fAmplitude;
		KDfloat					m_fAmplitudeRate;
};

// end of actions group
/// @}

NS_CC_END

#endif // __CCActionGrid3D_h__
