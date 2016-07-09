/* --------------------------------------------------------------------------
 *
 *      File            Global.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2012      Shengxiang Chen, Dingping Lv ( Licensed under GPL )
 *
 *         http://www.cocos2d-x.org  
 *         https://github.com/ShengxiangChen/MoonWarriors 
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
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

#include "Precompiled.h"
#include "Global.h"

KDbool Global::bSound = KD_TRUE;
KDuint Global::uBGM   = 0;

KDbool Global::bKeys[4] = { 0 };

KDint  Global::nScore = 0;
KDint  Global::nLife  = 4;
KDint  Global::nLevel = 1;

KDint  Global::nBulletNum = 0;
KDint  Global::nEnemyNum  = 0;

KDint  Global::nLifeUpScores[6] = { 50000, 100000, 150000, 200000, 250000, 300000 };

CCArray* Global::pEnemyContainer   = KD_NULL;
CCArray* Global::pEBulletContainer = KD_NULL;
CCArray* Global::pSBulletContainer = KD_NULL;

KDvoid Global::playBGM ( const KDchar* szPath )
{
	CCSound*  pSound = CCSound::sharedSound ( );

	if ( uBGM )
	{
		pSound->Stop ( uBGM );
	}

	if ( !bSound )
	{
		return;
	}

	uBGM = pSound->Open ( szPath, XM_SOUND_REPEAT );
    pSound->setVolume ( uBGM, 70 );
	pSound->Rewind ( uBGM );
}

KDvoid Global::playEffect ( const KDchar* szPath )
{
	if ( !bSound )
	{
		return;
	}

	CCSound*  pSound = CCSound::sharedSound ( );

	pSound->Rewind ( pSound->Open ( szPath, XM_SOUND_EFFECT ) );
}

KDvoid Global::setSound ( KDbool bOn )
{
	bSound = bOn;

	if ( uBGM )
	{
		CCSound*  pSound = CCSound::sharedSound ( );

		if ( bOn )
		{
			pSound->Resume ( uBGM );
		}
		else
		{
			pSound->Pause ( uBGM );
		}
	}
}