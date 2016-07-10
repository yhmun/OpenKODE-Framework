/* --------------------------------------------------------------------------
 *
 *      File            XMKnight.cpp
 *      Description     XMKnight
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
#include "XMKnight.h"
#include "Node/Bullet/Player/XMKnightBullet.h"


XMKnight::XMKnight ( b2World* pWorld )
{
	m_pParentWorld = pWorld;
}

XMKnight::~XMKnight ( KDvoid )
{

}

XMKnight* XMKnight::create ( b2World* pWorld )
{
	XMKnight* pRet = new XMKnight ( pWorld );

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

KDbool XMKnight::init ( KDvoid )
{
	if ( !XMPlayer::initWithContentsOfFile ( "Knight/Knight.plist" ) )
	{
		return KD_FALSE;
	}
	m_pArmature->setScale ( 0.3f );
	
	return KD_TRUE;
}

KDvoid XMKnight::shootBullet ( KDvoid )
{
	XMKnightBullet*		pBullet = XMKnightBullet::create ( m_pParentWorld );
	Node*				pNode = this->getParent ( );
	Point				tPoint = m_pArmature->getPosition ( );
	Bone*				pBone = m_pArmature->getBone ( "WEAPON" );

	tPoint.x += 50;
	tPoint.y += 17;
	pBullet->setPosition ( tPoint );
	pBullet->setDamage ( this->getShootDamage ( ) );

	pNode->addChild ( pBullet, ZORDER_BULLET );
	pBullet->shootLinear ( SHOOTDIRECTION_RIGHT, 960 );
}