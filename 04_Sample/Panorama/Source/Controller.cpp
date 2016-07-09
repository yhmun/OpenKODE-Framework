/* --------------------------------------------------------------------------
 *
 *      File            Main.cpp
 *      Description     Application Entrance.
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

KDbool Controller::init ( KDvoid )
{
	m_fLon = 0;
	m_fLat = 0;
	m_bMagnetometer = KD_TRUE;

	CCLayer::init ( );

	const CCSize&  tSize = CCDirector::sharedDirector ( )->getWinSize ( );	
	const CCSize&  tEGLSize = CCEGLView::sharedOpenGLView ( )->getFrameSize ( );

	CCMenu*  pMenu = CCMenu::create
	(
		CCMenuItemImage::create ( "/res/image/btn_prev_0.png"   , "/res/image/btn_prev_1.png"   , this, menu_selector ( Controller::onPrev ) ),
		CCMenuItemImage::create ( "/res/image/btn_restart_0.png", "/res/image/btn_restart_1.png", this, menu_selector ( Controller::onRestart ) ),
		CCMenuItemImage::create ( "/res/image/btn_next_0.png"   , "/res/image/btn_next_1.png"   , this, menu_selector ( Controller::onNext ) ),				
		KD_NULL
	);
	do
	{
		CC_BREAK_IF ( !pMenu );

		pMenu->setPosition ( ccp ( tSize.cx / 2, 35 ) );
		pMenu->alignItemsHorizontally ( 30 );

		this->addChild ( pMenu );

	} while ( 0 );	

	CCLabelTTF*  pTitle = CCLabelTTF::create ( "Panorama View", "/res/font/Schwarzwald Regular.ttf", 28, CCSize ( tSize.cx - 20.f, 0 ), kCCTextAlignmentCenter );
	do
	{
		CC_BREAK_IF ( !pTitle );

		pTitle->setPosition ( ccp ( tSize.cx / 2, tSize.cy - 50 ) );
		pTitle->setColor ( ccGREEN );

		this->addChild ( pTitle );
		
	} while ( 0 );	

	m_pLog = CCLabelTTF::create ( "", "/res/font/Schwarzwald Regular.ttf", 24, CCSize ( 300, 100 ), kCCTextAlignmentCenter );
	do
	{
		CC_BREAK_IF ( !m_pLog );
 
		m_pLog->setPosition ( ccp ( 50, tSize.cy - m_pLog->getContentSize ( ).cy - 100 ) );
		m_pLog->setColor ( ccRED );
        m_pLog->ignoreAnchorPointForPosition ( KD_TRUE );
        
		this->addChild ( m_pLog );
        
	} while ( 0 );	
    
	CCMenuItemToggle*  pToggle = CCMenuItemToggle::createWithTarget 
	(
		this, 
		menu_selector ( Controller::onUseMagnetometer ), 
		CCMenuItemImage::create ( "/res/image/s_load_nor05.png", "/res/image/s_load_sel05.png"	),
		CCMenuItemImage::create ( "/res/image/s_load_nor03.png", "/res/image/s_load_sel03.png" ),
		KD_NULL 
	);
	do
	{
		CC_BREAK_IF ( !pToggle );
        
		pToggle->setPosition ( ccp ( tSize.cx - 30, 30 ) );
        
		this->addChild ( CCMenu::create ( pToggle, KD_NULL ) );

	} while ( 0 );

	m_pJoypad = CCSprite::create ( "image/joypad_bg.png" );
	do
	{
		CC_BREAK_IF ( !m_pJoypad );

		m_pJoypad->setPosition ( ccp ( 75, 75 ) );

		this->addChild ( m_pJoypad );
		
	} while ( 0 );

	this->setTouchEnabled ( KD_TRUE );
    this->setTouchMode    ( kCCTouchesOneByOne );
    this->setEventEnabled ( KD_TRUE );
	
	m_pTexture = KD_NULL;
	m_pSphere  = KD_NULL;
	m_pView    = new XMGView ( );

	XMGMatrix4F	tMatrix;
	tMatrix.Perspective ( 45.0f, tSize.cx / tSize.cy, 0.1f, 100.0f );
	m_pView->SetViewport ( XMGRectF ( 0, 0, 0, tEGLSize.cx, tEGLSize.cy ) );
	m_pView->SetProject ( tMatrix );

	setPanorama ( 0 );

	return KD_TRUE;
}

KDvoid Controller::onExit ( KDvoid )
{    
	CC_SAFE_DELETE ( m_pTexture );
	CC_SAFE_DELETE ( m_pSphere );
	CC_SAFE_DELETE ( m_pView );
    
	CCLayer::onExit ( );
}

KDvoid Controller::draw ( KDvoid )
{
	if ( m_bTouched )
	{
		m_fLon += m_tIncrease.cx;
		m_fLat -= m_tIncrease.cy;
	}

	XMGMatrix4F	 tMatrix;
	
	tMatrix.Rotate ( m_fLat, 1.0f, 0.0f, 0.0f );	
	tMatrix.Rotate ( m_fLon, 0.0f, 1.0f, 0.0f );

	m_pSphere->SetMatrix ( tMatrix );

    KDchar  szMsg[256];
    kdSprintfKHR ( szMsg, "Lon = %.2f\nLat = %.2f", m_fLon, m_fLat );
    m_pLog->setString ( szMsg );
    
	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_None );

	m_pView->Update   ( );
	m_pSphere->Render ( );

	CCDirector::sharedDirector ( )->setGLDefaultValues ( );
}

KDbool Controller::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	const CCSize&   tSize = m_pJoypad->getContentSize ( );
	const CCPoint&  tPosition = m_pJoypad->getPosition ( );
	CCRect			tRect ( tPosition.x - tSize.cx / 2, tPosition.y - tSize.cy / 2, tSize.cx, tSize.cy );

	if ( tRect.containsPoint ( pTouch->getLocation ( ) ) )
	{
		m_tPosition = pTouch->getLocation ( );
		m_bTouched  = KD_TRUE;
		m_tIncrease.cx = 0;
		m_tIncrease.cy = 0;
        
        m_pJoypad->setOpacity ( 128 );
		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid Controller::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCPoint  tDiff = ccp ( pTouch->getLocation ( ).x - m_tPosition.x, pTouch->getLocation ( ).y - m_tPosition.y );

	m_tIncrease.cx = kdFabs ( tDiff.x ) > 10.f ? tDiff.x / 10.f : 0;
	m_tIncrease.cy = kdFabs ( tDiff.y ) > 10.f ? tDiff.y / 10.f : 0;
}

KDvoid Controller::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	m_bTouched  = KD_FALSE;
    m_pJoypad->setOpacity ( 255 );
}

KDvoid Controller::onUseMagnetometer ( CCObject* pSender )
{
	m_bMagnetometer = !m_bMagnetometer;
}

KDvoid Controller::onEvent ( const KDEvent* pEvent )
{
    if ( pEvent->type == KD_EVENT_MAGNETOMETER )
    {
        if ( m_bMagnetometer )
        {
            m_fLon = pEvent->data.magnetometer.y;
            m_fLat = pEvent->data.magnetometer.x;
        }
    }
}

KDvoid Controller::setPanorama ( KDint nIdx )
{
	const KDchar*  aFiles[] =
	{
		"/res/image/pano1_1.jpg",
		"/res/image/Chicago_Downtown_Panorama.jpg",
	};

	KDint    nCount  = sizeof ( aFiles ) / sizeof (KDchar*);
	GLfloat	 fRadius = 1.0f;

	if ( nIdx < 0 )
	{
		nIdx = nCount - 1;
	}
	else if ( nIdx >= nCount )
	{
		nIdx = 0;
	}
	
	CC_SAFE_DELETE ( m_pTexture );
	CC_SAFE_DELETE ( m_pSphere );

	m_pTexture = new XMGTexture ( aFiles [ nIdx ] );
	m_pSphere  = new XMGSphere ( );

	m_pSphere->SetVertexArray ( &fRadius );
	m_pSphere->SetPosition ( XMGVector3F ( 0.0f, 0.0f, -0.75f ) );
	m_pSphere->SetTexture ( m_pTexture ); 
	m_pSphere->SetCull ( XMG_CULL_FRONT );

	m_nIdx = nIdx;
}

KDvoid Controller::onRestart ( CCObject* pSender )
{
	setPanorama ( m_nIdx );
}

KDvoid Controller::onPrev ( CCObject* pSender )
{
	setPanorama ( m_nIdx - 1 );
}

KDvoid Controller::onNext ( CCObject* pSender )
{
	setPanorama ( m_nIdx + 1 );
}
