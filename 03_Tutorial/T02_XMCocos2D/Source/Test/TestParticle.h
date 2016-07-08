/* --------------------------------------------------------------------------
 *
 *      File            TestParticle.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */

#ifndef __TestParticle_h__
#define __TestParticle_h__

#include "../TestBasic.h"

class TestParticle : public TestBasic
{
	public :

		static  TestParticle*	create ( KDvoid );

	protected :		

		virtual KDbool			init  ( KDvoid );	
		virtual KDuint			count ( KDvoid );

		virtual KDvoid			ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDvoid			update ( KDfloat fDelta );

		KDvoid					setEmitterPosition ( KDvoid );

		KDvoid					toggleCallback  ( CCObject* pSender );

	protected :

		CCParticleSystem*		m_pEmitter;
		CCSprite*				m_pBackground;
};

class DemoFirework : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class DemoFire : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class DemoSun : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class DemoGalaxy : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class DemoFlower : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class DemoBigFlower : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class DemoRotFlower : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class DemoMeteor : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class DemoSpiral : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class DemoExplosion : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class DemoSmoke : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class DemoSnow : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class DemoRain : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class DemoModernArt : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class DemoRing : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class ParallaxParticle : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class DemoParticleFromFile : public TestParticle
{
	public :

		DemoParticleFromFile ( const KDchar* file )
		{	
	        m_title = file;
		}

		virtual const KDchar*	title ( KDvoid )
		{
			return m_title.c_str ( );
		}

		virtual KDvoid			onEnter ( KDvoid );

	private :

		std::string    m_title;
};

class RadiusMode1 : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class RadiusMode2 : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class ParticleBatchHybrid : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		KDvoid					switchRender ( KDfloat fDelta );

	private :

		CCNode*					m_pParent1;
		CCNode*					m_pParent2;
};

class ParticleBatchMultipleEmitters : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class ParticleReorder : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		KDvoid					reorderParticles ( KDfloat fDelta );

	private :

		KDuint					m_uOrder;
};

class MultipleParticleSystems : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			update   ( KDfloat fDelta );
};

class MultipleParticleSystemsBatched : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			update   ( KDfloat fDelta );

	private :

		CCParticleBatchNode*	m_pBatchNode;
};

class AddAndDeleteParticleSystems : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			update   ( KDfloat fDelta );

		KDvoid					removeSystem ( KDfloat fDelta );

	private :

		CCParticleBatchNode*	m_pBatchNode;
};

class ReorderParticleSystems : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			update   ( KDfloat fDelta );

		KDvoid					reorderSystem ( KDfloat fDelta );

	private :

		CCParticleBatchNode*	m_pBatchNode;
};

class PremultipliedAlphaTest : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class PremultipliedAlphaTest2 : public TestParticle
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

#endif	// __TestParticle_h__
