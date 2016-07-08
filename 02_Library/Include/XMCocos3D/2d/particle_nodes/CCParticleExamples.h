/* -----------------------------------------------------------------------------------
 *
 *      File            CCParticleExamples.h
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

#ifndef __CCParticleExamples_h__
#define __CCParticleExamples_h__

#include "CCParticleSystemQuad.h"

NS_CC_BEGIN

/**
 *	@addtogroup particle_nodes
 *	@{
 */

//! @brief A fire particle system
class CC_DLL ParticleFire : public ParticleSystemQuad
{
	public :

		/**
		 *	@js ctor
		 */
		ParticleFire ( KDvoid ) { }
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ParticleFire ( KDvoid ) { }

		KDbool						init ( KDvoid ) { return initWithTotalParticles ( 250  ); }
		virtual KDbool				initWithTotalParticles ( KDuint uNumberOfParticles );
    
		static ParticleFire*		create ( KDvoid );
		static ParticleFire*		createWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief A fireworks particle system
class CC_DLL ParticleFireworks : public ParticleSystemQuad
{
	public :

		/**
		 *	@js ctor
		 */
		ParticleFireworks ( KDvoid ) { }

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ParticleFireworks ( KDvoid ) { }

		KDbool						init ( KDvoid ) { return initWithTotalParticles ( 1500 ); }
		virtual KDbool				initWithTotalParticles ( KDuint uNumberOfParticles );
    
		static ParticleFireworks*	create ( KDvoid );
		static ParticleFireworks*	createWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief A sun particle system
class CC_DLL ParticleSun : public ParticleSystemQuad
{
	public :

		/**
		 *	@js ctor
		 */
		ParticleSun ( KDvoid ) { }

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ParticleSun ( KDvoid ) { }

		KDbool						init ( KDvoid ) { return initWithTotalParticles ( 350 ); }
		virtual KDbool				initWithTotalParticles ( KDuint uNumberOfParticles );
    
		static ParticleSun*			create ( KDvoid );
		static ParticleSun*			createWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief A galaxy particle system
class CC_DLL ParticleGalaxy : public ParticleSystemQuad
{
	public :

		/**
		 *	@js ctor
		 */
		ParticleGalaxy ( KDvoid ) { }
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ParticleGalaxy ( KDvoid ) { }

		KDbool						init ( KDvoid ) { return initWithTotalParticles ( 200 ); }
		virtual KDbool				initWithTotalParticles ( KDuint uNumberOfParticles );
    
		static ParticleGalaxy*		create ( KDvoid );
		static ParticleGalaxy*		createWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief A flower particle system
class CC_DLL ParticleFlower : public ParticleSystemQuad
{
	public :

		/**
		 *	@js ctor
		 */
		ParticleFlower ( KDvoid ) { }
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ParticleFlower ( KDvoid ) { }

		KDbool						init ( KDvoid ) { return initWithTotalParticles ( 250 ); }
		virtual KDbool				initWithTotalParticles ( KDuint uNumberOfParticles );
    
		static ParticleFlower*		create ( KDvoid );
		static ParticleFlower*		createWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief A meteor particle system
class CC_DLL ParticleMeteor : public ParticleSystemQuad
{
	public :

		/**
		 *	@js ctor
		 */
		ParticleMeteor ( KDvoid ) { }

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ParticleMeteor ( KDvoid ) { }

		KDbool						init ( KDvoid ) { return initWithTotalParticles ( 150 ); }
		virtual KDbool				initWithTotalParticles ( KDuint uNumberOfParticles );

		static ParticleMeteor*		create ( KDvoid );
		static ParticleMeteor*		createWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief An spiral particle system
class CC_DLL ParticleSpiral : public ParticleSystemQuad
{
	public :

		/**
		 *	@js ctor
		 */
		ParticleSpiral ( KDvoid ) { }
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ParticleSpiral ( KDvoid ) { }

		KDbool						init ( KDvoid ) { return initWithTotalParticles ( 500 ); }
		virtual KDbool				initWithTotalParticles ( KDuint uNumberOfParticles );
    
		static ParticleSpiral*		create ( KDvoid );
		static ParticleSpiral*		createWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief An explosion particle system
class CC_DLL ParticleExplosion : public ParticleSystemQuad
{
	public :

		/**
		 *	@js ctor
		 */
		ParticleExplosion ( KDvoid ) { }
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ParticleExplosion ( KDvoid ) { }

		KDbool						init ( KDvoid ) { return initWithTotalParticles ( 700 ); }
		virtual KDbool				initWithTotalParticles ( KDuint uNumberOfParticles );
    
		static ParticleExplosion*	create ( KDvoid );
		static ParticleExplosion*	createWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief An smoke particle system
class CC_DLL ParticleSmoke : public ParticleSystemQuad
{
	public :

		/**
		 *	@js ctor
		 */
		ParticleSmoke ( KDvoid ) { }

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ParticleSmoke ( KDvoid ) { }

		KDbool						init ( KDvoid ) { return initWithTotalParticles ( 200 ); }
		virtual KDbool				initWithTotalParticles ( KDuint uNumberOfParticles );
    
		static ParticleSmoke*		create ( KDvoid );
		static ParticleSmoke*		createWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief An snow particle system
class CC_DLL ParticleSnow : public ParticleSystemQuad
{
	public :

		/**
		 *	@js ctor
		 */
		ParticleSnow ( KDvoid ) { }

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ParticleSnow ( KDvoid ) { }

		KDbool						init ( KDvoid ) { return initWithTotalParticles ( 700 ); }
		virtual KDbool				initWithTotalParticles ( KDuint uNumberOfParticles );
    
		static ParticleSnow*		create ( KDvoid );
		static ParticleSnow*		createWithTotalParticles ( KDuint uNumberOfParticles );
};

//! @brief A rain particle system
class CC_DLL ParticleRain : public ParticleSystemQuad
{
	public :

		/**
		 *	@js ctor
		 */
		ParticleRain ( KDvoid ) { }

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ParticleRain ( KDvoid ) { }

		KDbool						init ( KDvoid ) { return initWithTotalParticles ( 1000 ); }
		virtual KDbool				initWithTotalParticles ( KDuint uNumberOfParticles );
    
		static ParticleRain*		create ( KDvoid );
		static ParticleRain*		createWithTotalParticles ( KDuint uNumberOfParticles );
};

//	end of particle_nodes group
/// @}

NS_CC_END

#endif	// __CCParticleExamples_h__
