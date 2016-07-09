/* --------------------------------------------------------------------------
 *
 *      File            Controller.cpp
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
#include "Controller.h"
#include "CDemo.h"

static CDemo*  l_pDemo     = KD_NULL;
static KDuint  l_uSound[2] = { 0 };

KDvoid Controller::EventProc ( const KDEvent* event )
{
	switch ( event->type )
	{
		case KD_EVENT_RESIZE :
			
        #ifdef ANDROID
            CC_SAFE_DELETE ( l_pDemo );
            l_pDemo = new CDemo ( event->data.size.width, event->data.size.height );
        #endif	

			break;

		case KD_EVENT_KEY_RELEASED :
		case KD_EVENT_KEY_PRESSED :
			{
				KDint  nCommand = -1;
				switch ( event->data.keypad.code )
				{
					case KD_INPUT_GAMEKEYS_UP		:	nCommand = CMD_MOVE_FORWARD;		break;
					case KD_INPUT_GAMEKEYS_LEFT		:	nCommand = CMD_MOVE_STRAFE_LEFT;	break;
					case KD_INPUT_GAMEKEYS_RIGHT	:	nCommand = CMD_MOVE_STRAFE_RIGHT;	break;
					case KD_INPUT_GAMEKEYS_DOWN		:	nCommand = CMD_MOVE_BACKWARD;		break;
					case KD_INPUT_GAMEKEYS_FIRE		:	nCommand = CMD_FIRE;				break;
					case KD_INPUT_PHONEKEYPAD_0		:	nCommand = CMD_JUMP;				break;
					case KD_INPUT_PHONEKEYPAD_1		:	break;
					case KD_INPUT_PHONEKEYPAD_2		:	nCommand = CMD_MOVE_BACKWARD;		break;      
					case KD_INPUT_PHONEKEYPAD_3     :	break;
					case KD_INPUT_PHONEKEYPAD_4		:	nCommand = CMD_MOVE_STRAFE_LEFT;	break;
					case KD_INPUT_PHONEKEYPAD_5		:	nCommand = CMD_FIRE;				break;
					case KD_INPUT_PHONEKEYPAD_6		:	nCommand = CMD_MOVE_STRAFE_RIGHT;	break;
					case KD_INPUT_PHONEKEYPAD_7		:	nCommand = CMD_ROTATE_LEFT;			break;
					case KD_INPUT_PHONEKEYPAD_8		:	nCommand = CMD_MOVE_FORWARD;		break;
					case KD_INPUT_PHONEKEYPAD_9		:	nCommand = CMD_ROTATE_RIGHT;		break;
					case KD_INPUT_XMSOFT_ESCAPE		:	nCommand = CMD_ESCAPE;				break;
					case KD_INPUT_XMSOFT_PREV		:	nCommand = CMD_ESCAPE;				break;
				}

				if ( nCommand != -1 )
				{
					DispatchToDemo ( nCommand, event->type == KD_EVENT_KEY_PRESSED );
					return;
				}
			}
			break;
	}

	if ( l_pDemo )
	{
		l_pDemo->EventProc ( event );
	}
}

KDvoid Controller::DispatchToDemo ( KDint nCommand, KDbool bPress )
{
	SEvent  tEvent;

	switch ( nCommand )
	{
		case CMD_ROTATE_LEFT		:	tEvent.KeyInput.Key = KEY_KEY_Q;	break;
		case CMD_MOVE_FORWARD		:	tEvent.KeyInput.Key = KEY_UP;		break;
		case CMD_ROTATE_RIGHT		:	tEvent.KeyInput.Key = KEY_KEY_E;	break;
		case CMD_MOVE_STRAFE_LEFT	:	tEvent.KeyInput.Key = KEY_LEFT;		break;
		case CMD_MOVE_STRAFE_RIGHT  :	tEvent.KeyInput.Key = KEY_RIGHT;	break;
		case CMD_MOVE_BACKWARD		:	tEvent.KeyInput.Key = KEY_DOWN;		break;
		case CMD_JUMP				:	tEvent.KeyInput.Key = KEY_KEY_J;	break;
		case CMD_ESCAPE				:	tEvent.KeyInput.Key = KEY_ESCAPE;	break;
		case CMD_FIRE				:	tEvent.KeyInput.Key = KEY_SPACE;	
			if ( bPress )
			{
				CCSound::sharedSound ( )->Rewind ( l_uSound[1] );
			}
			break;
	}
	
	tEvent.EventType = EET_KEY_INPUT_EVENT;
	tEvent.KeyInput.PressedDown = bPress ? true : false; 

	l_pDemo->OnEvent ( tEvent );
}

KDvoid Controller::draw ( KDvoid )
{
    CCDirector*  pDirector = CCDirector::sharedDirector ( );
    
	if ( l_pDemo )
	{        
		if ( l_pDemo->Draw ( ) == KD_FALSE )
		{
            pDirector->end ( );
		}
	}

	pDirector->setGLDefaultValues ( );
}

KDbool Controller::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCSize  tSize = CCDirector::sharedDirector ( )->getWinSize ( );

	CCLabelTTF*  pTitle = CCLabelTTF::create ( "Demo : Mixed Cocos2D + Irrlicht", "Font/Abduction.ttf", 25 );
	do
	{
		CC_BREAK_IF ( !pTitle );

		pTitle->setPosition ( ccp ( tSize.cx / 2, tSize.cy - 50 ) );
		pTitle->setColor ( ccGREEN );

		this->addChild ( pTitle );

	} while ( 0 );

    CCSprite*  pGrossini = CCSprite::create ( "Image/grossini.png" ); 
	do
	{
		CC_BREAK_IF ( !pGrossini );

		const CCSize&  tSize2 = pGrossini->getContentSize ( );

		pGrossini->setPosition ( ccp ( tSize.cx - tSize2.cx, tSize.cy / 2 + 30 ) );

		pGrossini->runAction ( CCRepeatForever::create
		(
			(CCActionInterval *) CCSequence::create 
			(
				(CCActionInterval *) CCSpawn::create 
				(
					CCJumpBy::create ( 1, ccp ( 0, 0 ), 80, 1 ),
					CCRotateBy::create ( 1, 360 ),
					KD_NULL
				), 
				CCDelayTime::create ( 0.5f ),
				KD_NULL
			) 
		) );

		this->addChild ( pGrossini );

	} while ( 0 );

	CCMenuItemImage*  pClose = CCMenuItemImage::create
	(
		"Image/CloseNormal.png", "Image/CloseSelected.png", this, menu_selector ( Controller::onClose ) 
	);
	do
	{	
		CC_BREAK_IF ( !pClose );

		const CCSize&  tSize2 = pClose->getContentSize ( );

		pClose->setPosition ( ccp ( tSize.cx - tSize2.cx, tSize.cy - tSize2.cy ) );
		this->addChild ( CCMenu::create ( pClose, KD_NULL ) );

	} while ( 0 );

	CCMenu*  pJoypad = CCMenu::create ( ); 
    CCMenu*  pButton = CCMenu::create ( );
	do
	{
		CC_BREAK_IF ( !pJoypad || !pButton );

		this->addChild ( pJoypad );
        this->addChild ( pButton );

		for ( KDint i = 0; i < 9; i++ )
		{
			KDchar  szPath[2][256];

			kdSprintfKHR ( szPath[0], "Image/mapcur_nor%d.png", i );
			kdSprintfKHR ( szPath[1], "Image/mapcur_sel%d.png", i );
			
			RollerOverCheck*  pBtn = RollerOverCheck::create ( szPath[0], szPath[1], i );
			do
			{
				CC_BREAK_IF ( !pBtn );

				const KDfloat aPosition [9][2] =
				{
					{  30, 180 },
					{ 115, 180 },
					{ 205, 180 },
					{  30, 130 },
					{ 205, 130 },
					{ 115,  70 },
					{ tSize.cx - 120,   95 },
					{ tSize.cx -  40,   95 },
					{ tSize.cx -  40,  165 },
				};

				pBtn->setPosition ( ccp ( aPosition[i][0], aPosition[i][1] ) );
                
                if ( i < 6 )
                {
                    pJoypad->addChild ( pBtn, 0, i );
                }
                else
                {
                    pButton->addChild ( pBtn, 0, i );
                }

			} while ( 0 );
		}

	} while ( 0 );

	CCSound*  pSound = CCSound::sharedSound ( );
	
	l_uSound[0] = pSound->Open ( "Sound/BGM.mp3", CCSoundPlay | CCSoundRepeat );
	l_uSound[1] = pSound->Open ( "Sound/Bullet.wav", CCSoundEffect );

	l_pDemo = new CDemo ( (KDint) tSize.cx, (KDint) tSize.cy );

	return KD_TRUE;
}

KDvoid Controller::onClose ( CCObject* sender )
{
	CC_SAFE_DELETE ( l_pDemo );
	CCDirector::sharedDirector ( )->end ( );
}

RollerOverCheck* RollerOverCheck::create ( const KDchar* szNormalImage, const KDchar* szSelectedImage, KDint nTag )
{
	RollerOverCheck*  pNode = new RollerOverCheck ( );
	if ( pNode && pNode->initWithNormalImage ( szNormalImage, szSelectedImage, 0, 0, 0 ) )
	{
		pNode->autorelease ( );
		pNode->setTag ( nTag );
	}
	else
	{
		CC_SAFE_DELETE ( pNode );
	}

	return pNode;
}

KDvoid RollerOverCheck::selected ( KDvoid )
{
	CCMenuItemImage::selected ( );
	
	Controller::DispatchToDemo ( this->getTag ( ), 1 );
}

KDvoid RollerOverCheck::unselected ( KDvoid )
{
	CCMenuItemImage::unselected ( );

	Controller::DispatchToDemo ( this->getTag ( ), 0 );
}