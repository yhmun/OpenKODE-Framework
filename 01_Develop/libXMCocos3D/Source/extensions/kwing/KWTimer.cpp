/* --------------------------------------------------------------------------
 *
 *      File            KWTimer.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/03/04
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011-2013 Kawaz. All rights reserved.                             
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

#include "Precompiled.h"
#include "KWTimer.h"

KWTimer* KWTimer::create ( KDvoid )
{
	KWTimer*		pRet = new KWTimer ( );

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KWTimer* KWTimer::create ( KDint nMax )
{
	KWTimer*		pRet = new KWTimer ( );

	if ( pRet && pRet->initWithMax ( nMax ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool KWTimer::init ( KDvoid )
{
	m_nMax = 0;
	m_nTime = 0;
	m_bFlagLoop = KD_FALSE;
	m_bFlagActive = KD_FALSE;

	return KD_TRUE;
}

KDbool KWTimer::initWithMax ( KDint nMax )
{
	this->init ( );
	this->set ( nMax );

	return KD_TRUE;
}

KDvoid KWTimer::tick ( KDvoid )
{
	this->count ( );

	if ( this->isOver ( ) )
	{
		if ( m_bFlagLoop )
		{
			this->reset ( );
		}
	}
}

KWTimer* KWTimer::play ( KDvoid )
{
	m_bFlagActive = KD_TRUE;

	return this;
}

KWTimer* KWTimer::stop ( KDvoid )
{
	this->pause ( );
	this->reset ( );

	return this;
}

KWTimer* KWTimer::pause ( KDvoid )
{
	m_bFlagActive = KD_FALSE;

	return this;
}

KWTimer* KWTimer::reset ( KDvoid )
{
	m_nTime = 0;

	return this;
}

KDvoid KWTimer::count ( KDvoid )
{
	if ( m_bFlagActive )
	{
		m_nTime += 1;
	}
}

KWTimer* KWTimer::move ( KDint n )
{
	m_nTime += n;

	if ( this->isOver ( ) && m_bFlagLoop )
	{
		m_nTime = m_nTime % m_nMax;
	}	

	return this;
}

KDbool KWTimer::isOver ( KDvoid )
{
	return m_nTime >= m_nMax;
}

KDint KWTimer::max ( KDvoid )
{
	return m_nMax;
}

KDvoid KWTimer::set ( KDint nMax )
{
	m_nMax = nMax;
}

KDbool KWTimer::isLooping ( KDvoid )
{
	return m_bFlagLoop;
}

KDvoid KWTimer::setLooping ( KDbool bLoop )
{
	m_bFlagLoop = bLoop;
}
