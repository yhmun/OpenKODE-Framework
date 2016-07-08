/* --------------------------------------------------------------------------
 *
 *      File            TestInterval.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
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
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "TestInterval.h"

#define SID_STEP1     100
#define SID_STEP2     101
#define SID_STEP3     102

#define IDC_PAUSE     200

TestInterval::~TestInterval ( KDvoid )
{
	if ( CCDirector::sharedDirector ( )->isPaused ( ) )
    {
        CCDirector::sharedDirector ( )->resume ( );
    }
}

KDbool TestInterval::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	CCParticleSystem*  pSun = CCParticleSun::create ( );
	pSun->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( "Images/fire.png" ) );
	pSun->setPosition ( ccp ( tLyrSize.cx - 100, tLyrSize.cy - 100 ) );
	pSun->setTotalParticles ( 130 );
	pSun->setLife ( 0.6f );

	this->addChild ( pSun );

	CCSprite*  pSprite = CCSprite::create ( "Images/grossini.png" );
	this->addChild ( pSprite );

	pSprite->setPosition ( ccp ( 40, 50 ) );

	CCJumpBy*  pAction = CCJumpBy::create ( 3, ccp ( tLyrSize.cx - 80, 0 ), 50, 4 );
	pSprite->runAction ( CCRepeatForever::create 
	( 
		CCSequence::create ( pAction, pAction->reverse ( ), KD_NULL )
	) );
	
	CCMenuItemFont::setFontSize ( 28 );
	CCMenuItemFont::setFontName ( "fonts/Marker Felt.ttf" );
	CCMenuItemToggle*  pToggle = CCMenuItemToggle::createWithTarget
	(
		this, 
		menu_selector ( TestInterval::onPause ), 
		CCMenuItemFont::create ( "Pause" ),
		CCMenuItemFont::create ( "Resume" ),
		KD_NULL 
	);
	pToggle->setPosition ( ccp ( tLyrSize.cx / 2, + 50 ) );
	pToggle->setColor ( ccc3 ( 16, 255, 255 ) );

	this->addChild ( CCMenu::createWithItem ( pToggle ) );

	for ( KDuint i = 0; i < 5; i++ )
	{
		m_pLabels [ i ] = CCLabelBMFont::create ( "0.0", "fonts/bitmapFontTest4.fnt" );
		m_pLabels [ i ]->setPosition ( ccp ( tLyrSize.cx * ( i + 1 ) / 6, tLyrSize.cy / 2 ) );
		this->addChild ( m_pLabels [ i ] );
	}

    this->scheduleUpdate ( );
	this->schedule ( schedule_selector ( TestInterval::step1 ) );
	this->schedule ( schedule_selector ( TestInterval::step2 ), 0.2f );
	this->schedule ( schedule_selector ( TestInterval::step3 ), 1.0f );
    this->schedule ( schedule_selector ( TestInterval::step4 ), 2.0f );

	return KD_TRUE;
}

KDvoid TestInterval::onPause ( CCObject* pSender )
{
	if ( CCDirector::sharedDirector ( )->isPaused ( ) )
	{
		CCDirector::sharedDirector ( )->resume ( ); 
	}
	else
	{
		CCDirector::sharedDirector ( )->pause ( ); 
	}
}

KDvoid TestInterval::update ( KDfloat dt )
{
   refresh ( 0, dt ); 
}

KDvoid TestInterval::step1 ( KDfloat dt )
{
	refresh ( 1, dt ); 
}

KDvoid TestInterval::step2 ( KDfloat dt )
{
	refresh ( 2, dt ); 
}

KDvoid TestInterval::step3 ( KDfloat dt )
{
	refresh ( 3, dt );   
}

KDvoid TestInterval::step4 ( KDfloat dt )
{
    refresh ( 4, dt );   
}

KDvoid TestInterval::refresh ( KDuint index, KDfloat dt )
{
	m_fTimes [ index ] += dt;

	KDchar  str[10] = { 0 };
	kdSprintfKHR ( str, "%2.1f", m_fTimes [ index ] );
	
	m_pLabels [ index ]->setString ( str ); 
}