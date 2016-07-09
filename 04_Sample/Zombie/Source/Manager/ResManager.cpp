/* --------------------------------------------------------------------------
 *
 *      File            ResManager.cpp
 *      Description     Resource Manager
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
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
#include "ResManager.h"

CResManager*  g_pResMgr = KD_NULL;

static const KDchar*  l_szPath [ ] = 
{	
    "",                                                         // ePath_Null
    
	"font/NanumGothicBold.ttf",									// eFNT_Gothic
    "font/arial-unicode-26.fnt",                                // eFNT_Bitmap

    "sound/bgm.mp3",                                            // eSND_BGM1
    
    "sound/bullet.wav",                                         // eSND_Bullet
    "sound/weapon/Gun5.WAV",                                    // eSND_Ripple
    //"sound/Gun5.WAV",                                         // eSND_Machine_Gun
    "sound/machine_gun7.wav",                                   // eSND_Machine_Gun
    "sound/enemy/RUBBLE.WAV",                                   // eSND_Enemy_Spawn
    "sound/enemy/GROWL.WAV",                                    // eSND_Enemy_Harm     
    "sound/enemy/DEATH.WAV",                                    // eSND_Enemy_Dead     
    
    "sound/player/PAIN_short.WAV",                              // eSND_Player_Harm
    "sound/player/DEATH_long.WAV",                              // eSND_Player_Dead
    
    "sound/effect_heart.wav",                                   // eSND_Effect_Heart
    
    "sound/weapon/SWITCH02.WAV",                                // eSND_Effect_Change
    "sound/weapon/PKUP.WAV",                                    // eSND_Effect_Reload
    
	"map/stage01.X",                                            // eMAP_Stage1
	"map/stage02.X",											// eMAP_Stage2

    "enemy/zombie_man_basic_x.X",								// eMDL_Zombie1
    "enemy/zombie_man_basic_3ds.3DS",							// eMDL_Zombie2
    "enemy/zombie_man_basic_dae.DAE",							// eMDL_Zombie3
    "enemy/zombie_man_basic_obj.mtl",							// eMDL_Zombie4
    "enemy/zombie_man_basic_obj.obj",							// eMDL_Zombie5
    
	"enemy/dwarf.x",											// eMDL_Enemy1
	"enemy/yodan.mdl",											// eMDL_Enemy2
	"enemy/sydney.md2",											// eMDL_Enemy3
	"enemy/faerie.md2",											// eMDL_Enemy4												
	"enemy/ninja.b3d",											// eMDL_Enemy5

	"weapon/gun.md2",											// eMDL_Gun1
    "weapon/mgun.x",                                            // eMDL_Gun2

	"texture/wall.jpg",											// eTEX_Wall
    "texture/rockwall.jpg",										// eTEX_Rock
    "texture/smoke.bmp",										// eTEX_Smoke
    "texture/fireball.bmp",										// eTEX_Harm
    "texture/portal1.bmp",										// eTEX_Spawn    
    
    "texture/pointing1.bmp",									// eTEX_Pointing
    
	"enemy/sydney.bmp",											// eTEX_Enemy3
	"enemy/faerie1.bmp",										// eTEX_Enemy4
	"enemy/ninja1.jpg",											// eTEX_Enemy5

	"weapon/gun.jpg",											// eTEX_Gun1
    "texture/wall.jpg",                                         // eTEX_Gun2
    "weapon/fireball.bmp",										// eTEX_Bullet1
    
    "texture/effect_shoot_1.jpg",                               // eTEX_Effect_Fires
    
    "image/bg/splash.jpg",										// eIMG_BG_Splash
    "image/bg/loadingpage.jpg",									// eIMG_BG_Intro
    "image/bg/menupage2.jpg",									// eIMG_BG_Main
	"image/bg/stage1.jpg",										// eIMG_BG_Stage
        
    "image/btn/Button_pause.png",								// eIMG_BTN_Pause_Nor   
    "image/btn/Button_pause.png",								// eIMG_BTN_Resume_Nor 
    "image/btn/Button_gunchange.png",							// eIMG_BTN_Mode_Nor 
    "image/btn/Button_gunchange.png",							// eIMG_BTN_Change_Nor 
    "image/btn/Button_reload.png",								// eIMG_BTN_Reload_Nor 

    
    "image/btn/Button_pad_base.png",							// eIMG_BTN_Pad_BG     
    "image/btn/Button_pad_shootout.png",						// eIMG_BTN_Pad_Nor    
    "image/btn/Button_pad_shooton.png",                         // eIMG_BTN_Pad_Sel    
    
    "image/game/stamina_bar_red.png",							// eIMG_Player_HP_Red  
    "image/game/stamina_bar_blue.png",							// eIMG_Player_HP_Blue 
    
    "image/game/Icon_target_IN.png",							// eIMG_Target_In      
    "image/game/Icon_target_out.png",							// eIMG_Target_Out     
    
    "image/game/screeneffect_lowstamina.png",					// eIMG_Effect_Low_HP      
    "image/game/screeneffect_mydamage.png",						// eIMG_Effect_Damaged     
    
    
    
    "image/game/Icon_bullet.png",								// eIMG_Bullet         
    
    "image/game/Icon_weapon_1.png",								// eIMG_Icon_Weapons          
    
    "image/9patch/progress_bg.9.png",							// eIMG_9_Progress      
    "image/9patch/progress9.9.png",								// eIMG_9_Progress2     
    
	"image/btn/rodeboad_nor0.png",								// eIMG_BTN_Main_Nor	
	"image/btn/rodeboad_sel0.png",								// eIMG_BTN_Main_Sel

	"image/btn/cir_btn_nor_1.png",								// eIMG_BTN_Prev_Nor
	"image/btn/cir_btn_sel_1.png",								// eIMG_BTN_Prev_Sel

	"image/etc/lpad1.png",										// eIMG_ETC_L_Pad		
	"image/etc/rpad1.png",										// eIMG_ETC_R_Pad		
	"image/etc/rader1.png",										// eIMG_ETC_Rader		
};

CResManager::CResManager ( KDvoid )
{
	m_pMusic  = KD_NULL;
	m_pSounds = new CCDictionary ( );
}

CResManager::~CResManager ( KDvoid )
{
	removeMusic ( );
	CC_SAFE_RELEASE ( m_pSounds );
}

KDvoid CResManager::create  ( KDvoid )
{
	if ( !g_pResMgr )
	{
		g_pResMgr = new CResManager ( );
	}

	kdAssert ( g_pResMgr );
}

KDvoid CResManager::release ( KDvoid )
{
	CC_SAFE_DELETE ( g_pResMgr );
}

const KDchar* CResManager::getPath ( EPath eID, KDuint uIndex )
{
	std::string  sRet;
	KDsize		 nLen;

	sRet  = "/res/";
	sRet += l_szPath [ eID ];
    
    if ( uIndex > 0 )
    {
        nLen    = sRet.size ( );
        uIndex += sRet [ nLen - 5 ] - '0';
        
        sRet [ nLen - 5 ] = '0' + uIndex % 10;
        if ( uIndex > 9 )
        {
            sRet [ nLen - 6 ] = '0' + uIndex / 10;
        }	    
    }

	return CCString::create ( sRet.c_str ( ) )->getCString ( );
}

CCLabelBMFont* CResManager::getLabel ( CCNode* pParent, const KDchar* szText )
{
    CCLabelBMFont*  pRet = CCLabelBMFont::create ( szText, g_pResMgr->getPath ( eFNT_Bitmap ) );
    
    if ( pRet && pParent )
    {
        pParent->addChild ( pRet ); 
    }
    
    return pRet;
}

CCProgressTimer* CResManager::getProgress ( CCNode* pParent, EPath eID, KDuint uIndex  )
{
    CCProgressTimer*  pRet = CCProgressTimer::create ( getPath ( eID, uIndex ) );
    
    if ( pRet && pParent )
    {
        pParent->addChild ( pRet ); 
    }
    
    return pRet;    
}

CCSprite* CResManager::getSprite ( CCNode* pParent, EPath eID, KDuint uIndex )
{
    CCSprite*  pRet = CCSprite::create ( getPath ( eID, uIndex ) );

    if ( pRet && pParent )
    {
        pParent->addChild ( pRet ); 
    }
    
    return pRet;
}

CCMenuItemSprite* CResManager::getMenuItemSprite ( CCNode* pParent, EPath eNormalID, EPath eSelectedID, CCObject* pListener, SEL_MenuHandler pSelector )
{
    CCSprite*       pNormal = CCSprite::create ( getPath ( eNormalID ) );
    CCSprite*       pSelect = CCSprite::create ( getPath ( eSelectedID == ePath_Null ? eNormalID : eSelectedID ) );    
    CCMenuItemSprite*  pRet = CCMenuItemSprite::create ( pNormal, pSelect, pListener, pSelector );
    
    if ( pRet && pParent )
    {
        if ( eSelectedID == ePath_Null )
        {
            pSelect->setOpacity ( 128 );
        }
        
        pParent->addChild ( CCMenu::create ( pRet, KD_NULL ) );
    }
    
    return pRet;
}

CSound* CResManager::getMusic ( EPath eID, KDuint uIndex )
{    
	std::string  sPath = getPath ( eID, uIndex );

	if ( m_pMusic && !m_pMusic->sPath.compare ( sPath ) )
	{
		CC_SAFE_RELEASE ( m_pMusic );

		m_pMusic = new CSound ( sPath.c_str ( ), XM_SOUND_REPEAT );
	}

	return m_pMusic;
}

CSound* CResManager::getSound ( EPath eID, KDuint uIndex, KDuint uTag )
{
    CSound*      pSound;
	std::string  sPath; 
    KDchar       szKey[256];
 
    kdSprintf ( szKey, "%p%p%p", eID, uIndex, uTag );
	pSound = (CSound*) m_pSounds->objectForKey ( szKey );
	if ( !pSound )
	{
		sPath = getPath ( eID, uIndex );
		pSound = new CSound ( sPath.c_str ( ), XM_SOUND_EFFECT );
		if ( pSound )
		{		
			pSound->autorelease ( );
			m_pSounds->setObject ( pSound, szKey );
		}
	}
    
	return pSound;
}

KDvoid CResManager::playSound ( EPath eID, KDuint uIndex, KDuint uTag )
{
	CSound*  pSound;
    KDchar   szKey[256];

    kdSprintf ( szKey, "%p%p%p", eID, uIndex, uTag );
	pSound = (CSound*) m_pSounds->objectForKey ( szKey );
	if ( pSound )
	{
		pSound->Rewind ( );
	}
}

KDvoid CResManager::removeMusic ( KDvoid )
{
	if ( m_pMusic )
	{
		CC_SAFE_RELEASE ( m_pMusic );
	}
}

KDvoid CResManager::removeSound ( EPath eID, KDuint uIndex, KDuint uTag )
{
    KDchar  szKey[256];
 
    kdSprintf ( szKey, "%p%p%p", eID, uIndex, uTag );
	m_pSounds->removeObjectForKey ( szKey );
}