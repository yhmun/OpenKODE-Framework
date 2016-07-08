/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionGrid3D.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCActionGrid3D_h__
#define __CCActionGrid3D_h__

#include "CCActionGrid.h"

NS_CC_BEGIN

/**
 *	@addtogroup actions
 *	@{
 */

/** 
 *	@brief Waves3D action 
 */
class CC_DLL Waves3D : public Grid3DAction
{
	public :

		/** creates an action with duration, grid size, waves and amplitude */
		static Waves3D*			create ( KDfloat fDuration, const Size& tGridSize, KDuint uWaves, KDfloat fAmplitude );

		/** returns the amplitude of the effect */
		inline float			getAmplitude ( KDvoid ) const { return m_fAmplitude; }
		
		/** sets the amplitude to the effect */
		inline void				setAmplitude ( KDfloat fAmplitude ) { m_fAmplitude = fAmplitude; }

		/** returns the amplitude rate */
		inline float			getAmplitudeRate ( KDvoid ) const { return m_fAmplitudeRate; }

		/** sets the ampliture rate */
		inline void				setAmplitudeRate ( KDfloat fAmplitudeRate ) { m_fAmplitudeRate = fAmplitudeRate; }

		/** initializes an action with duration, grid size, waves and amplitude */
		KDbool					initWithDuration ( KDfloat fDuration, const Size& tGridSize, KDuint uWaves, KDfloat fAmplitude );

		// Overrides
		virtual Waves3D*		clone ( KDvoid ) const override;
		virtual KDvoid			update ( KDfloat fTime ) override;

	protected :

		KDuint					m_uWaves;
		KDfloat					m_fAmplitude;
		KDfloat					m_fAmplitudeRate;
};

/** @brief FlipX3D action */
class CC_DLL FlipX3D : public Grid3DAction
{
	public :

		/** creates the action with duration */
		static FlipX3D*			create ( KDfloat fDuration );

		/** initializes the action with duration */
		KDbool					initWithDuration ( KDfloat fDuration);
		virtual KDbool			initWithSize ( const Size& tGridSize, KDfloat fDuration );

		// Override
		virtual FlipX3D*		clone ( KDvoid ) const override;
		virtual KDvoid			update ( KDfloat fTime ) override;
};

/** @brief FlipY3D action */
class CC_DLL FlipY3D : public FlipX3D
{
	public :
		/** creates the action with duration */
		static FlipY3D*			create ( KDfloat fDuration);

		// Overrides
		virtual KDvoid			update ( KDfloat fTime ) override;
		virtual FlipY3D*		clone ( KDvoid ) const override;
};

/** @brief Lens3D action */
class CC_DLL Lens3D : public Grid3DAction
{
	public :

		/** creates the action with center position, radius, a grid size and duration */
		static Lens3D*			create ( KDfloat fDuration, const Size& tGridSize, const Point& tPosition, KDfloat fRadius);

		/** Get lens center position */
		inline KDfloat			getLensEffect ( KDvoid ) const { return m_fLensEffect; }

		/** Set lens center position */
		inline KDvoid			setLensEffect ( KDfloat fLensEffect ) { m_fLensEffect = fLensEffect; }

		/** Set whether lens is concave */
		inline KDvoid			setConcave ( KDbool bConcave ) { m_bConcave = bConcave; }
  
		inline const Point&		getPosition ( KDvoid ) const { return m_tPosition; }
		KDvoid					setPosition ( const Point& tPosition );

		/** initializes the action with center position, radius, a grid size and duration */
		KDbool					initWithDuration ( KDfloat fDuration, const Size& tGridSize, const Point& tPosition, KDfloat fRadius );

		// Overrides
		virtual Lens3D*			clone ( KDvoid ) const override;
		virtual KDvoid			update ( KDfloat fTime ) override;

	protected :

		/* lens center position */
		Point					m_tPosition;
		KDfloat					m_fRadius;
		/** lens effect. Defaults to 0.7 - 0 means no effect, 1 is very strong effect */
		KDfloat					m_fLensEffect;
		/** lens is concave. (true = concave, false = convex) default is convex i.e. false */
		KDbool					m_bConcave;

		KDbool					m_bDirty;
};

/** @brief Ripple3D action */
class CC_DLL Ripple3D : public Grid3DAction
{
	public :

		/** creates the action with radius, number of waves, amplitude, a grid size and duration */
		static Ripple3D*		create ( KDfloat fDuration, const Size& tGridSize, const Point& tPosition, KDfloat fRadius, KDuint uWaves, KDfloat fAmplitude );

		/** get center position */
		inline const Point&		getPosition ( KDvoid ) const { return m_tPosition; }

		/** set center position */
		KDvoid					setPosition ( const Point& tPosition );

		inline KDfloat			getAmplitude ( KDvoid ) const { return m_fAmplitude; }
		inline KDvoid			setAmplitude ( KDfloat fAmplitude) { m_fAmplitude = fAmplitude; }

		inline KDfloat			getAmplitudeRate ( KDvoid ) const { return m_fAmplitudeRate; }
		inline KDvoid			setAmplitudeRate ( KDfloat fAmplitudeRate) { m_fAmplitudeRate = fAmplitudeRate; }

		/** initializes the action with radius, number of waves, amplitude, a grid size and duration */
		KDbool					initWithDuration ( KDfloat fDuration, const Size& tGridSize, const Point& tPosition, KDfloat fRadius, KDuint uWaves, KDfloat fAmplitude );

		// Override
		virtual Ripple3D*		clone ( KDvoid ) const override;
		virtual KDvoid			update ( KDfloat fTime ) override;

	protected :

		/* center position */
		Point					m_tPosition;
		KDfloat					m_fRadius;
		KDuint					m_uWaves;
		KDfloat					m_fAmplitude;
		KDfloat					m_fAmplitudeRate;
};

/** @brief Shaky3D action */
class CC_DLL Shaky3D : public Grid3DAction
{
	public :

		/** creates the action with a range, shake Z vertices, a grid and duration */
		static Shaky3D*			create ( KDfloat fDuration, const Size& tGridSize, KDint nRange, KDbool bShakeZ );

		/** initializes the action with a range, shake Z vertices, a grid and duration */
		KDbool					initWithDuration ( KDfloat fDuration, const Size& tGridSize, KDint nRange, KDbool bShakeZ );

		// Overrides
		virtual Shaky3D*		clone ( KDvoid ) const override;
		virtual KDvoid			update ( KDfloat fTime ) override;

	protected :

		KDint					m_nRandrange;
		KDbool					m_bShakeZ;
};

/** @brief Liquid action */
class CC_DLL Liquid : public Grid3DAction
{
	public :

		/** creates the action with amplitude, a grid and duration */
		static Liquid*			create ( KDfloat fDuration, const Size& tGridSize, KDuint uWaves, KDfloat fAmplitude );

		inline KDfloat			getAmplitude ( KDvoid ) const { return m_fAmplitude; }
		inline KDvoid			setAmplitude(KDfloat fAmplitude ) { m_fAmplitude = fAmplitude; }

		inline KDfloat			getAmplitudeRate ( KDvoid ) const { return m_fAmplitudeRate; }
		inline KDvoid			setAmplitudeRate ( KDfloat fAmplitudeRate ) { m_fAmplitudeRate = fAmplitudeRate; }

		/** initializes the action with amplitude, a grid and duration */
		KDbool					initWithDuration ( KDfloat fDuration, const Size& tGridSize, KDuint uWaves, KDfloat fAmplitude );

		// Overrides
		virtual Liquid*			clone ( KDvoid ) const override;
		virtual KDvoid			update ( KDfloat fTime ) override;

	protected :

		KDuint					m_uWaves;
		KDfloat					m_fAmplitude;
		KDfloat					m_fAmplitudeRate;
};

/** @brief Waves action */
class CC_DLL Waves : public Grid3DAction
{
	public :

		/** initializes the action with amplitude, horizontal sin, vertical sin, a grid and duration */
		static Waves*			create ( KDfloat fDuration, const Size& tGridSize, KDuint uWaves, KDfloat fAmplitude, KDbool bHorizontal, KDbool bVertical );

		inline KDfloat			getAmplitude ( KDvoid ) const { return m_fAmplitude; }
		inline KDvoid			setAmplitude ( KDfloat fAmplitude ) { m_fAmplitude = fAmplitude; }

		inline KDfloat			getAmplitudeRate ( KDvoid ) const { return m_fAmplitudeRate; }
		inline KDvoid			setAmplitudeRate ( KDfloat fAmplitudeRate ) { m_fAmplitudeRate = fAmplitudeRate; }

		/** initializes the action with amplitude, horizontal sin, vertical sin, a grid and duration */
		KDbool					initWithDuration ( KDfloat fDuration, const Size& tGridSize, KDuint uWaves, KDfloat fAmplitude, KDbool bHorizontal, KDbool bVertical );

		// Overrides
		virtual Waves*			clone ( KDvoid ) const override;
		virtual KDvoid			update ( KDfloat fTime ) override;

	protected :

		KDuint					m_uWaves;
		KDfloat					m_fAmplitude;
		KDfloat					m_fAmplitudeRate;
		KDbool					m_bVertical;
		KDbool					m_bHorizontal;
};

/** @brief Twirl action */
class CC_DLL Twirl : public Grid3DAction
{
	public :

		/** creates the action with center position, number of twirls, amplitude, a grid size and duration */
		static Twirl*			create ( KDfloat fDuration, const Size& tGridSize, Point position, KDuint uTwirls, KDfloat fAmplitude );

		/** get twirl center */
		inline const Point&		getPosition ( KDvoid ) const { return m_tPosition; }
		/** set twirl center */
		KDvoid					setPosition(const Point& tPosition);

		inline KDfloat			getAmplitude ( KDvoid ) const { return m_fAmplitude; }
		inline KDvoid			setAmplitude ( KDfloat fAmplitude ) { m_fAmplitude = fAmplitude; }

		inline KDfloat			getAmplitudeRate ( KDvoid ) const { return m_fAmplitudeRate; }
		inline KDvoid			setAmplitudeRate ( KDfloat fAmplitudeRate ) { m_fAmplitudeRate = fAmplitudeRate; }

		/** initializes the action with center position, number of twirls, amplitude, a grid size and duration */
		KDbool					initWithDuration ( KDfloat fDuration, const Size& tGridSize, Point position, KDuint uTwirls, KDfloat fAmplitude );

		// Overrides
		virtual Twirl*			clone ( KDvoid ) const override;
		virtual KDvoid			update ( KDfloat fTime ) override;

	protected :

		/* twirl center */
		Point					m_tPosition;
		KDuint					m_uTwirls;
		KDfloat					m_fAmplitude;
		KDfloat					m_fAmplitudeRate;
};

//	end of actions group
/// @}

NS_CC_END

#endif	// __CCActionGrid3D_h__
