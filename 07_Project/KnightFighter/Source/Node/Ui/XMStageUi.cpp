/* --------------------------------------------------------------------------
 *
 *      File            XMStageUi.cpp
 *      Description     XMStageUi
 *      Author          Pill-Gwan Jeon
 *      Contact         jpg3087@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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
#include "XMStageUi.h"

XMStageUi::XMStageUi ( )
{
	m_pPlayerIcon = nullptr;
	m_nMaxHp = 0;
	m_nCurrentHp = 0;
}

XMStageUi::~XMStageUi ( )
{
	
}


KDbool XMStageUi::init ( )
{
	if ( !Layer::init ( ) )
	{
		return KD_FALSE;
	}

	Sprite*		pMaximumHp	= Sprite::create ( "HP_BAR_1.png" );
	pMaximumHp->setPosition ( Point ( 116, m_tContentSize.cy -10 ) );
	pMaximumHp->setAnchorPoint ( Point ( 0, 1 ) );
	this->addChild ( pMaximumHp );

	m_pPrograssHp = ProgressTimer::create ( Sprite::create ( "HP_BAR_2.png" ) );
	m_pPrograssHp->setType ( ProgressTimer::Type::BAR );
	m_pPrograssHp->setMidpoint ( Point ( 0, 0 ) );
	m_pPrograssHp->setBarChangeRate ( Point ( 1, 0 ) );
	m_pPrograssHp->setPosition ( Point ( pMaximumHp->getContentSize ( ) / 2 ) );

	pMaximumHp->addChild ( m_pPrograssHp );

	return KD_TRUE;
}

KDvoid XMStageUi::setMaxHp ( KDint nMaxHp )
{

	m_nMaxHp = nMaxHp;
}
KDvoid XMStageUi::setCurrentHp ( KDint nCurrentHp )
{
	m_nCurrentHp = nCurrentHp;

	m_pPrograssHp->setPercentage ( ( (KDfloat) m_nCurrentHp / (KDfloat) m_nMaxHp ) * 100 );

}

KDvoid XMStageUi::setPlayerIconName ( const KDchar* sPlayerName )
{
	if ( m_pPlayerIcon == nullptr )
	{
		m_pPlayerIcon	=	Sprite::create ( ccszf ( "%s.png", sPlayerName ) );
		m_pPlayerIcon->setPosition ( Point ( 10, m_tContentSize.cy -10 ) );
		m_pPlayerIcon->setAnchorPoint ( Point ( 0, 1 ) );
		this->addChild ( m_pPlayerIcon );
	}
	else
	{
		m_pPlayerIcon->initWithFile ( ccszf ( "%s.png", sPlayerName ) );
	}
	
}
