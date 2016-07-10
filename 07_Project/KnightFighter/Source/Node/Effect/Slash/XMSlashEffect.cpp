/* --------------------------------------------------------------------------
 *
 *      File            XMSlashEffect.cpp
 *      Description     XMSlashEffect
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
#include "XMSlashEffect.h"

XMSlashEffect* XMSlashEffect::create ( const KDchar* szPlayerName )
{
	XMSlashEffect*	pRet = new XMSlashEffect ( );

	if ( pRet && pRet->init ( (KDchar*) szPlayerName ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool XMSlashEffect::init ( KDchar* szPlayerName )
{
	if ( !Sprite::init ( ) )
	{
		return KD_FALSE;
	}

	Animation*		pAnimation = Animation::create ( );

	for ( KDint i = 1; i <= 4; i++ )
	{
		pAnimation->addSpriteFrameWithFile ( ccszf ( "Slash/Effect_%s_Slash_%d.png", szPlayerName,i ) );
	}
	pAnimation->setDelayPerUnit ( 0.03f );

	this->runAction 
	( 
		Sequence::createWithTwoActions
		(
			Animate::create ( pAnimation ),
			CallFuncN::create ( CC_CALLBACK_0 ( Node::removeFromParent, this ) )
		)
	);

	this->setScale ( 0.5 );

	return KD_TRUE;
}
