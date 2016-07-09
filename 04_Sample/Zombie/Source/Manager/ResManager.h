/* --------------------------------------------------------------------------
 *
 *      File            ResManager.h
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

#ifndef __ResManager_h__
#define __ResManager_h__

enum EPath
{
    ePath_Null          ,
    
	eFNT_Gothic			,
    eFNT_Bitmap         ,

    eSND_BGM1           ,
    eSND_Bullet         ,
    eSND_Ripple         ,
    eSND_Machine_Gun    ,
    eSND_Enemy_Spawn    ,
    eSND_Enemy_Harm     ,
    eSND_Enemy_Dead     ,
    
    eSND_Player_Harm    ,
    eSND_Player_Dead    ,
    
    eSND_Effect_Heart   ,
    
    eSND_Effect_Change  ,
    eSND_Effect_Reload  ,
    
	eMAP_Stage1			,
	eMAP_Stage2			,

    eMDL_Zombie1        ,
    eMDL_Zombie2        ,
    eMDL_Zombie3        ,
    eMDL_Zombie4        ,
    eMDL_Zombie5        ,
    
	eMDL_Enemy1			,
	eMDL_Enemy2			,
	eMDL_Enemy3			,
	eMDL_Enemy4			,
	eMDL_Enemy5			,

	eMDL_Gun1			,
    eMDL_Gun2           ,

	eTEX_Wall			,
    eTEX_Rock           ,
    eTEX_Smoke          ,
    eTEX_Harm           ,    
    eTEX_Spawn          ,
    
    eTEX_Pointing       ,

	eTEX_Enemy3			,
	eTEX_Enemy4			,
	eTEX_Enemy5			,

	eTEX_Gun1			,
    eTEX_Gun2           ,
    eTEX_Bullet1        ,

    eTEX_Effect_Fires   ,
    
	eIMG_BG_Splash		,
	eIMG_BG_Intro		,
	eIMG_BG_Main		,
	eIMG_BG_Stage		,
    
    eIMG_BTN_Pause_Nor  ,
    eIMG_BTN_Resume_Nor ,
    eIMG_BTN_Mode_Nor   ,
    eIMG_BTN_Change_Nor ,
    eIMG_BTN_Reload_Nor ,
    
    eIMG_BTN_Pad_BG     ,
    eIMG_BTN_Pad_Nor    ,
    eIMG_BTN_Pad_Sel    ,
    
    eIMG_Player_HP_Red  ,
    eIMG_Player_HP_Blue ,
    
    eIMG_Target_In      ,
    eIMG_Target_Out     ,
    
    eIMG_Effect_Low_HP  ,
    eIMG_Effect_Damaged ,
    
    eIMG_Bullet         ,
    
    eIMG_Icon_Weapons   ,
    
    eIMG_9_Progress     ,
    eIMG_9_Progress2    ,

	eIMG_BTN_Main_Nor	,
	eIMG_BTN_Main_Sel	,

	eIMG_BTN_Prev_Nor	,

	eIMG_BTN_Prev_Sel	,

	eIMG_ETC_L_Pad		,
	eIMG_ETC_R_Pad		,
	eIMG_ETC_Rader		,
};

class CSound : public CCObject
{
	public :

		CSound ( KDvoid )
		{
			pSound = KD_NULL;
			sPath  = "";
		}

		CSound ( const KDchar* szPath, KDint nFlag )
		{
			pSound = xmSoundOpen ( szPath, nFlag );
			sPath  = szPath;
		}

		virtual ~CSound ( KDvoid )
		{
			if ( pSound )
			{
				xmSoundClose ( pSound );
			}
		}

		KDvoid  Rewind ( KDvoid )
		{
			if ( pSound )
			{
				xmSoundRewind ( pSound );
			}
		}

		std::string		sPath;
		XMSound*		pSound;
};

class CResManager
{
	protected :

		 CResManager ( KDvoid );
		~CResManager ( KDvoid );

	public :

		static KDvoid  create  ( KDvoid );
		static KDvoid  release ( KDvoid );

		const KDchar*    getPath ( EPath eID, KDuint uIndex = 0 );
    
        CCLabelBMFont*   getLabel ( CCNode* pParent, const KDchar* szText );
    
        CCSprite*        getSprite ( CCNode* pParent, EPath eID, KDuint uIndex = 0 );

        CCProgressTimer* getProgress ( CCNode* pParent, EPath eID, KDuint uIndex = 0 );
    
        CCMenuItemSprite* getMenuItemSprite ( CCNode* pParent, EPath eNormalID, EPath eSelectedID, CCObject* pListener, SEL_MenuHandler pSelector );

    
        CSound*  getMusic  ( EPath eID, KDuint uIndex = 0 );
        CSound*  getSound  ( EPath eID, KDuint uIndex = 0, KDuint uTag = 0 ); 
        KDvoid   playSound ( EPath eID, KDuint uIndex = 0, KDuint uTag = 0 );
    
        KDvoid    removeMusic ( KDvoid );
        KDvoid    removeSound ( EPath eID, KDuint uIndex = 0, KDuint uTag = 0 );
    
    private :
    
		CSound*				m_pMusic;
		CCDictionary*		m_pSounds;
};

extern CResManager*  g_pResMgr;

#endif