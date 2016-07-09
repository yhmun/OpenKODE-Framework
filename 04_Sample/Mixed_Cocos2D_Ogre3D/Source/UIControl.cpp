/* --------------------------------------------------------------------------
 *
 *      File            UIControl.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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
#include "UIControl.h"
#include "Sinbad.h"

namespace Ogre
{
    void RenderStartPlugin ( void ) throw ( );
    void RenderStopPlugin  ( void );
}

typedef struct 
{
	CCPoint	  tPos;
	KDint     nKey;
} RollButton;

const RollButton  c_tRollButton [ ] =
{
	{ ccp (       115, 130 ), KD_INPUT_GAMEKEYSNC_UP    },
	{ ccp (        30,  80 ), KD_INPUT_GAMEKEYSNC_LEFT  },
	{ ccp (       205,  80 ), KD_INPUT_GAMEKEYSNC_RIGHT },
	{ ccp (       115,  20 ), KD_INPUT_GAMEKEYSNC_DOWN  },
	{ ccp ( 800 - 205,  45 ), KD_INPUT_GAMEKEYSNC_FIRE  },
	{ ccp ( 800 - 125,  45 ), KD_INPUT_GAMEKEYSNC_A     },
	{ ccp ( 800 -  45,  45 ), KD_INPUT_GAMEKEYSNC_B		},
	{ ccp ( 800 - 125, 115 ), KD_INPUT_GAMEKEYSNC_D		},
	{ ccp ( 800 -  45, 115 ), KD_INPUT_GAMEKEYSNC_C		},
};

KDvoid UIControl::draw ( KDvoid )
{   
	if ( m_pRoot )
	{
		m_pRoot->renderOneFrame ( KD_GET_UST2MSEC * 1000.f );
	}

	CCDirector::sharedDirector ( )->setGLDefaultValues ( );
}

KDbool UIControl::init ( KDvoid )
{
	if ( !CCLayer::init ( ) )
	{
		return KD_FALSE;
	}

	CCSize  tWinSize = CCDirector::sharedDirector ( )->getWinSize ( );
	CCSize  tSize;

	CCLabelTTF*  pTitle = CCLabelTTF::create 
	(
		"Demo - Mixed Cocos2D and Ogre3D", "Font/Abduction.ttf", 25 
	);

	this->addChild ( pTitle );
	pTitle->setPosition ( ccp ( tWinSize.cx / 2, tWinSize.cy - 50 ) );
	pTitle->setColor ( ccGREEN );

    CCSprite*  pGrossini = CCSprite::create ( "Image/grossini.png" ); 

	this->addChild ( pGrossini );
	pGrossini->setPosition ( ccp ( tWinSize.cx - pGrossini->getContentSize ( ).cx, tWinSize.cy / 2 + 20 ) );
	pGrossini->runAction ( CCRepeatForever::create
	(
		CCSequence::create 
		(
			CCSpawn::create 
			(
				CCJumpBy::create ( 0.7f, ccp ( 0, 0 ), 100, 1 ),
				CCRotateBy::create ( 0.7f, 360 ),
				KD_NULL
			), 
			CCDelayTime::create ( 0.5f ),
			KD_NULL
		) 
	) );

	CCMenuItemImage*  pClose = CCMenuItemImage::create
	(
		"Image/CloseNormal.png", "Image/CloseSelected.png", this, menu_selector ( UIControl::onClose ) 
	);

	this->addChild ( CCMenu::createWithItem ( pClose ) );
	pClose->setPosition ( ccpSub ( tWinSize, pClose->getContentSize ( ) ) );
	
	CCMenu*  pJoypad = CCMenu::create ( ); 
    CCMenu*  pButton = CCMenu::create ( );

	this->addChild ( pJoypad );
    this->addChild ( pButton );

	for ( KDint i = 0; i < 9; i++ )
	{		
		RollerOver*  pBtn = RollerOver::create
		(
			CCString::createWithFormat ( "Image/btn_n%02d.png", i )->getCString ( ), 
			CCString::createWithFormat ( "Image/btn_s%02d.png", i )->getCString ( ), 
			c_tRollButton [ i ].nKey 
		);

		pBtn->setPosition ( c_tRollButton [ i ].tPos );

        if ( i < 4 )
        {
            pJoypad->addChild ( pBtn, i );
        }
        else
        {
            pButton->addChild ( pBtn, i );
        }
	}

	this->setKeypadEnabled ( KD_TRUE );

	initOgre3D ( );

	return KD_TRUE;
}

KDvoid UIControl::keyReleased ( KDint nID )
{
	KDEventKeypad  tEvent = { nID };

	m_pSinbad->injectKeyUp ( tEvent );
}

KDvoid UIControl::keyPressed  ( KDint nID )
{
	KDEventKeypad  tEvent = { nID };

	m_pSinbad->injectKeyDown ( tEvent );
}

KDvoid UIControl::onClose ( CCObject* sender )
{
	MeshManager::getSingleton ( ).remove ( "floor" );

	CC_SAFE_DELETE ( m_pSinbad );
	CC_SAFE_DELETE ( m_pRoot );

	RenderStopPlugin ( );

	CCDirector::sharedDirector ( )->end ( );
}

KDbool UIControl::initOgre3D ( KDvoid )
{
	m_pRoot = new Root ( );
	RenderStartPlugin ( );

	m_pRoot->restoreConfig ( );
    m_pRoot->addFrameListener ( this );

	m_pWindow = m_pRoot->initialise ( true );

    ResourceGroupManager::getSingleton ( ).addResourceLocation
    (
        "/res/Media/packs/Sinbad.zip", "Zip", "Sinbad"
    );
    
	ResourceGroupManager::getSingleton ( ).addResourceLocation
    (
        "/res/Media/materials/scripts", "FileSystem", "Common"
    );
    
    ResourceGroupManager::getSingleton ( ).addResourceLocation
    (
        "/res/Media/materials/textures", "FileSystem", "Common"
    );
    
	ResourceGroupManager::getSingleton ( ).initialiseAllResourceGroups ( );

	TextureManager::getSingleton ( ).setDefaultNumMipmaps ( 5 );

	SceneManager*   pSceneMgr = m_pRoot->createSceneManager ( ST_GENERIC, "SinbadScene" );
    Camera*         pCamera   = pSceneMgr->createCamera ( "MainCamera" );    
    Viewport*       pViewport = m_pWindow->addViewport ( pCamera );

    pCamera->setAspectRatio ( (Real) pViewport->getActualWidth ( ) / (Real) pViewport->getActualHeight ( ) );
    pCamera->setNearClipDistance ( 5 );
    pViewport->setBackgroundColour ( ColourValue ( 1.0f, 1.0f, 0.8f ) );
    pSceneMgr->setFog ( FOG_LINEAR, ColourValue ( 1.0f, 1.0f, 0.8f ), 0, 15, 100 );
    
    // use a small amount of ambient lighting
    pSceneMgr->setAmbientLight ( ColourValue ( 0.3, 0.3, 0.3 ) );
    
    // add a bright light above the scene
    Light*  pLight = pSceneMgr->createLight ( );
    pLight->setType ( Light::LT_POINT );
    pLight->setPosition ( -10, 40, 20 );
    pLight->setSpecularColour ( ColourValue::White );

    // create a floor mesh resource
    MeshManager::getSingleton ( ).createPlane
    (
        "floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        Plane ( Vector3::UNIT_Y, 0 ), 100, 100, 10, 10, true, 1, 10, 10, Vector3::UNIT_Z
    );
    
    // create a floor entity, give it a material, and place it at the origin
    Entity*  pFloor = pSceneMgr->createEntity ( "Floor", "floor" );
    pFloor->setMaterialName ( "Examples/Rockwall" );
    pSceneMgr->getRootSceneNode ( )->attachObject ( pFloor );
    
	m_pSinbad = new Sinbad ( pCamera );
    
	return KD_TRUE;
}

bool UIControl::frameRenderingQueued ( const FrameEvent& tEvent )
{
    m_pSinbad->addTime ( tEvent.timeSinceLastEvent );
    
    return true;
}

RollerOver* RollerOver::create ( const KDchar* szNormalImage, const KDchar* szSelectedImage, KDint nTag )
{
	RollerOver*  pRet = new RollerOver ( );
	if ( pRet && pRet->initWithNormalImage ( szNormalImage, szSelectedImage, 0, 0, 0 ) )
	{
		pRet->autorelease ( );
		pRet->setTag ( nTag );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDvoid RollerOver::selected ( KDvoid )
{
	CCMenuItemImage::selected ( );
	
	UIControl*  pControl = (UIControl*) this->getParent ( )->getParent ( );
	
	pControl->keyPressed ( this->getTag ( ) );
}

KDvoid RollerOver::unselected ( KDvoid )
{
	CCMenuItemImage::unselected ( );

	UIControl*  pControl = (UIControl*) this->getParent ( )->getParent ( );

	pControl->keyReleased ( this->getTag ( ) );
}
