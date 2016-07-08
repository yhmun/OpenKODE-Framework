/* -----------------------------------------------------------------------------------
 *
 *      File            CCParticleExamples.h
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

#ifndef __CCParticleExamples_h__
#define __CCParticleExamples_h__

#include "CCParticleSystemQuad.h"

NS_CC_BEGIN

/**
 * @addtogroup particle_nodes
 * @{
 */
    
//! @brief A fire particle system
class CCParticleFire : public CCParticleSystemQuad
{
	public :

		static  CCParticleFire*			create ( KDuint uNumberOfParticles = 250 );

	public :

		virtual KDbool		initWithTotalParticles ( KDuint uNumberOfParticles );		
};

//! @brief A fireworks particle system
class CCParticleFireworks : public CCParticleSystemQuad
{
	public :

		static  CCParticleFireworks*	create ( KDuint uNumberOfParticles = 1500 );

	public :

		virtual KDbool		initWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief A sun particle system
class CCParticleSun : public CCParticleSystemQuad
{
	public :

		static  CCParticleSun*			create ( KDuint uNumberOfParticles = 350 );

	public :

		virtual KDbool		initWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief A galaxy particle system
class CCParticleGalaxy : public CCParticleSystemQuad
{
	public :

		static  CCParticleGalaxy*		create ( KDuint uNumberOfParticles = 200 );

	public :

		virtual KDbool		initWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief A flower particle system
class CCParticleFlower : public CCParticleSystemQuad
{
	public :

		static  CCParticleFlower*		create ( KDuint uNumberOfParticles = 250 );

	public :

		virtual KDbool		initWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief A meteor particle system
class CCParticleMeteor : public CCParticleSystemQuad
{
	public :

		static  CCParticleMeteor*		create ( KDuint uNumberOfParticles = 150 );

	public :

		virtual KDbool		initWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief An spiral particle system
class CCParticleSpiral : public CCParticleSystemQuad
{
	public :

		static  CCParticleSpiral*		create ( KDuint uNumberOfParticles = 500 );

	public :

		virtual KDbool		initWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief An explosion particle system
class CCParticleExplosion : public CCParticleSystemQuad
{
	public :

		static  CCParticleExplosion*	create ( KDuint uNumberOfParticles = 700 );

	public :

		virtual KDbool		initWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief An smoke particle system
class  CCParticleSmoke : public CCParticleSystemQuad
{
	public :

		static  CCParticleSmoke*		create ( KDuint uNumberOfParticles = 200 );

	public :

		virtual KDbool		initWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief An snow particle system
class CCParticleSnow : public CCParticleSystemQuad
{
	public :

		static  CCParticleSnow*			create ( KDuint uNumberOfParticles = 700 );

	public :

		virtual KDbool		initWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief A rain particle system
class CCParticleRain : public CCParticleSystemQuad
{
	public :

		static  CCParticleRain*			create ( KDuint uNumberOfParticles = 1000 );

	public :

		virtual KDbool		initWithTotalParticles ( KDuint uNumberOfParticles );		
};

// end of particle_nodes group
/// @}

NS_CC_END

#endif // __CCParticleExamples_h__
