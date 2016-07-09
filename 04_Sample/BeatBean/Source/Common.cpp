/* --------------------------------------------------------------------------
 *
 *      File            Common.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      t2cn on 2009/10/09
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
#include "Common.h"

static Common*		l_pCommon = KD_NULL;

Common* Common::sharedCommon ( KDvoid )
{
	if ( !l_pCommon )
	{
		l_pCommon = new Common ( );
	}

	return l_pCommon;
}

KDvoid Common::init ( KDvoid )
{
	SimpleAudioEngine*	pSAE = SimpleAudioEngine::sharedEngine ( );

	pSAE->preloadEffect ( "1.mp3" );
	pSAE->preloadEffect ( "3.mp3" );
	pSAE->preloadEffect ( "22.mp3" );
}

KDvoid Common::playOverSound ( KDvoid )
{
	if ( m_bIsPlay )
	{
		SimpleAudioEngine::sharedEngine ( )->playEffect ( "1.mp3" );
	}
}

KDvoid Common::playRightSound ( KDvoid )
{
	if ( m_bIsPlay )
	{
		SimpleAudioEngine::sharedEngine ( )->playEffect ( "3.mp3" );
	}
}

KDvoid Common::playWrongSound ( KDvoid )
{
	if ( m_bIsPlay )
	{
		SimpleAudioEngine::sharedEngine ( )->playEffect ( "22.mp3" );
	}
}

KDvoid Common::setPlay ( KDbool bPlay )
{
	m_bIsPlay = bPlay;
}


