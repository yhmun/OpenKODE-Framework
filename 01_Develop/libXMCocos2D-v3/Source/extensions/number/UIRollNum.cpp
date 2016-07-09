/* --------------------------------------------------------------------------
 *
 *      File            UIRollNum.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      周海锋 on 12-04-08
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      CJLU. All rights reserved.
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
#include "UIRollNum.h"

UIRollNum::UIRollNum ( KDvoid )
{
	m_pNumArray = KD_NULL;
}

UIRollNum::~UIRollNum ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pNumArray );
}

KDbool UIRollNum::init ( KDvoid )
{
	if ( !CCSprite::init ( ) )
	{
		return KD_FALSE;
	}

	CC_SAFE_RELEASE ( m_pNumArray );

	m_nNumber	= 0;
	m_nMaxCol	= 6;
	m_pNumArray = new CCArray ( );
	m_bZeroFill = KD_TRUE;
	m_eStyle	= NumStyleNormal;

	this->clearEffect ( );

	return KD_TRUE;
}

KDvoid UIRollNum::rebuildEffect ( KDvoid )
{
	KDint	i    = 0;
	KDint	nNum = m_nNumber;

	while ( 1 )
	{
		if ( nNum <= 0 )
		{
			if ( m_nMaxCol <= i && m_bZeroFill )
			{
				break;
			}
		}

		KDint		nShowNum = nNum % 10;	    
		UINumber*	pNumber  = (UINumber*) m_pNumArray->objectAtIndex ( i );
		pNumber->setNumber ( nShowNum );

		i++;
		nNum = nNum / 10;
	}
}

KDvoid UIRollNum::clearEffect ( KDvoid )
{
	for ( KDuint i = 0; i < m_pNumArray->count ( ); i++ )
	{	    
		UINumber*	pNumber  = (UINumber*) m_pNumArray->objectAtIndex ( i );
		this->removeChild ( pNumber, KD_TRUE );
	}
	m_pNumArray->removeAllObjects ( );

	for ( KDint i = 0; i < m_nMaxCol; i++ )
	{
		UINumber*	pNumber  = UINumber::create ( m_eStyle );
		m_pNumArray->addObject ( pNumber );
		pNumber->setNumber ( 0 );
		pNumber->setPosition ( ccp ( m_tPoint.x - i * NUM_WIDTH, m_tPoint.y ) );
		pNumber->setAnchorPoint ( ccp ( 1, 0.5f ) );
		this->addChild ( pNumber, 100 );
	}
}

KDint UIRollNum::getNumber ( KDvoid )
{
	return m_nNumber;
}

KDvoid UIRollNum::setNumber ( KDint nNumber )
{
	if ( m_nNumber != nNumber )
	{
		m_nNumber = nNumber;
		this->rebuildEffect ( );
	}
}

