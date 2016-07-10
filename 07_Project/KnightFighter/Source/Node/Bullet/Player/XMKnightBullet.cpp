/* --------------------------------------------------------------------------
 *
 *      File            XMKnightBullet.cpp
 *      Description     XMKnightBullet
 *      Author          J.S Park
 *      Contact         pparkppark84@gmail.com
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
#include "XMKnightBullet.h"
#include "Node/Effect/Shoot/XMShootEffect.h"
#include "Manager/XMGlobalManager.h"

XMKnightBullet::XMKnightBullet ( b2World* pWorld )
{
	m_pParentWorld = pWorld;
}

XMKnightBullet::~XMKnightBullet ( KDvoid )
{

}

XMKnightBullet* XMKnightBullet::create ( b2World* pWorld )
{
	XMKnightBullet* pRet = new XMKnightBullet ( pWorld );
	
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

KDbool XMKnightBullet::init ( KDvoid )
{
	if ( !XMPlayerBullet::initWithFile ( "Knight/shoot_1.png" ) )
	{
		return KD_FALSE;
	}

	this->initVariable ( );

	return KD_TRUE;
}

KDvoid XMKnightBullet::initVariable ( )
{

}

KDvoid XMKnightBullet::setVisible ( KDbool bVisible )
{
	XMPlayerBullet::setVisible ( bVisible );

	XMShootEffect*	pEffect = XMShootEffect::create ( "Knight" );
	pEffect->setPosition ( this->getPosition ( ) + Point ( this->getContentSize ( ).width / 2, 0 ) );
	pEffect->setScale ( 0.3f );
	
	this->getParent ( )->addChild ( pEffect, ZORDER_EFFECT );

	
}

KDvoid XMKnightBullet::destroy ( KDvoid )
{
	Node*			pNode = this->getParent ( );

	pNode->removeChild ( this, KD_TRUE );

}