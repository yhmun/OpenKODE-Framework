/* -----------------------------------------------------------------------------------
 *
 *      File            Particle.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
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
#include "Particle.h"

cParticle::cParticle ( Node* pNode, const KDchar* pParticleFileName, KDint z, Point tPoint, KDbool bIsCheckTime, KDdouble dCheckTime )
: m_pNode			( pNode			)
, m_pParticle		( nullptr		)
, m_bIsCheckTime	( bIsCheckTime	)
, m_dCheckTime		( dCheckTime	)
, m_bIsMove			( false			)
, m_tMoveBefore		( Point::ZERO	)
, m_tMove			( Point::ZERO	)
, m_dTimeMoveFull	( 0				)
, m_dTimeMoveLeft	( 0				)
{
	// particle
	m_pParticle = ParticleSystemQuad::create ( pParticleFileName );
	m_pParticle->setPosition ( tPoint );
	m_pNode->addChild ( m_pParticle, z );
}

cParticle::~cParticle ( KDvoid )
{
	if ( m_pParticle )
	{
		m_pNode->removeChild ( m_pParticle, true );
		m_pParticle = nullptr;
	}
}

KDvoid cParticle::setPointTime ( Point tPoint, KDdouble dLeftTime )
{
	if ( m_pParticle == nullptr )
	{
		return;
	}

	m_bIsMove		= true;
	m_tMove			= tPoint - m_pParticle->getPosition ( );
	m_tMoveBefore	= m_pParticle->getPosition ( );
	m_dTimeMoveFull	= dLeftTime;
	m_dTimeMoveLeft	= dLeftTime;
}

KDvoid cParticle::start ( KDvoid )
{
	m_pParticle->resetSystem ( );
}

KDvoid cParticle::stop ( KDvoid )
{
	m_pParticle->stopSystem ( );
}

KDvoid cParticle::update ( KDdouble dLeftTime )
{
	if ( m_bIsCheckTime == true )
	{
		m_dCheckTime -= dLeftTime;
	}

	if ( m_bIsMove == true )
	{
		m_dTimeMoveLeft -= dLeftTime;

		if ( m_dTimeMoveLeft < 0 )
		{
			Point		tPoint = Point ( m_tMoveBefore.x + m_tMove.x, m_tMoveBefore.y + m_tMove.y );
			m_pParticle->setPosition ( tPoint );
			m_bIsMove = false;
		}
		else
		{
			Point		tPoint = Point ( m_tMoveBefore.x + ( m_dTimeMoveFull - m_dTimeMoveLeft ) * m_tMove.x / m_dTimeMoveFull,
										 m_tMoveBefore.y + ( m_dTimeMoveFull - m_dTimeMoveLeft ) * m_tMove.y / m_dTimeMoveFull );

			m_pParticle->setPosition ( tPoint );
		}
	}
}
