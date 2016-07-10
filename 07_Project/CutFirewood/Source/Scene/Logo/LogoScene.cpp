/* -----------------------------------------------------------------------------------
 *
 *      File            LogoScene.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "LogoScene.h"
#include "Scene/Main/MainScene.h"
#include "Object/Animation/Animation.h"
#include "Object/Sprite/Sprite.h"

KDbool LogoScene::init ( KDvoid )
{
	if ( !JBaseScene::init ( ) )
	{
		return false;
	}

	// schedule--------------------------------------
	this->scheduleOnce ( schedule_selector ( LogoScene::timeout ), 5.2f );

	// sprite & animation----------------------------
	// background

	auto	pBg = new cSprite ( "logo_bg.png", 0, Point::ZERO );
	pBg->setIsStatic ( true );
	this->addDrawObject ( pBg );

	// logo
	m_pAniLogo = new cAnimations ( "logo.spr", 2, Point ( 114, 139 ) );
	m_pAniLogo->setIsStatic ( true );
	this->addDrawObject ( m_pAniLogo );

	// nurungee
	m_pAniNurungee = new cAnimations ( "logo_nurungee.spr", 1, Point ( 415, -63 ) );
	m_pAniNurungee->setIsStatic ( true );
	m_pAniNurungee->selectAnimation ( 1 );
	m_pAniNurungee->setIsLoop ( false );
	this->addDrawObject ( m_pAniNurungee );
	
	// nurungee punch
	m_pAniNurungeePunch = new cAnimations ( "logo_nurung_punch.spr", 1, Point ( _SCR_W + 39, -229 ) );
	m_pAniNurungeePunch->setIsStatic ( true );
	this->addDrawObject ( m_pAniNurungeePunch );

	// step
	m_nStepLogo				= 0;
	m_nStepNurungee			= 0;
	m_nStepNurungeePunch	= 0;
	
	// other
	m_fNurungVibrate		= 0;
	m_dNurungPunchTime		= 0;
	m_fNurungPunchVibrate	= 0;
	m_dNextScentDelay		= 0;

	return true;
}

KDvoid LogoScene::onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	m_nStepNurungee = 3;

	this->setIsTouch ( false );
}

KDvoid LogoScene::update ( KDfloat fDelta )
{
	// super update
	JBaseScene::update ( fDelta );
	
	// init
	KDdouble	dLeftTime = this->getLeftTime ( );


	// update logo
	this->updateLogo ( dLeftTime );
	
	// update nurungee
	this->updateNurungee ( dLeftTime );
	
	// update nurungeePunch
	this->updateNurungeePunch ( dLeftTime );
	
	// check next scene
	if ( m_nStepLogo == -1 && m_nStepNurungee == -1 && m_nStepNurungeePunch == -1 )
	{
		m_dNextScentDelay += dLeftTime;
		
		if ( m_dNextScentDelay > 0.5f )
		{
			this->unscheduleAllSelectors ( );

			Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.5f, MainScene::create ( ) ) );
		}
	}
}

KDvoid LogoScene::updateLogo ( KDdouble dLeftTime )
{
	// update animation
	m_pAniLogo->update ( dLeftTime );
	
	if ( m_nStepLogo == 0 )
	{
		// init
		std::vector<Rect>	aLogoRects;
		std::vector<Rect>	aPunchRects;

		// get logo def rect
		m_pAniLogo->getNowAnimation ( )->getNowFrameDefRects ( aLogoRects );

		//get punch att rect
		m_pAniNurungeePunch->getNowAnimation ( )->getNowFrameAttRects ( aPunchRects );
		
		// assert
		if ( aLogoRects.size ( ) == 0 || aPunchRects.size ( ) == 0 )
		{
			CCAssert ( false, "" );
		}

		// collision check
		if ( collisionBox ( aLogoRects [ 0 ], aPunchRects [ 0 ] ) != false )
		{
			// sound
			if ( cGlobalData::getObj ( )->m_bIsEffectMusic )
			{
				SimpleAudioEngine::getInstance ( )->playEffect ( "punch.wav" );
			}
			
			// step
			++m_nStepLogo;
			m_dLogoTime = 0;
		}	
	}
	else if ( m_nStepLogo == 1 )
	{
		// init
		Point			tPoint		= m_pAniLogo->getPoint ( );
		KDfloat			fRotation	= 0;
		KDbool			bIsNextStep	= false;
		const KDfloat	fEndTime	= 0.5f;
		KDfloat			fPercent	= 0.0f;
		
		// pass time
		m_dLogoTime += dLeftTime;

		// end check
		if ( m_dLogoTime > fEndTime )
		{
			m_dLogoTime	= fEndTime;
			bIsNextStep	= true;
		}
		
		// percent
		fPercent = m_dLogoTime / fEndTime;
		
		// edit pt
		tPoint.x = 114 - ( 114 + 252 ) * fPercent;
		tPoint.y = 139 + ( 139 + 100 ) * fPercent;
		
		// set pt
		m_pAniLogo->setPoint ( tPoint );
		
		// edit rotation
		fRotation = fPercent * -240;
		
		// set rotate
		m_pAniLogo->setRotation ( fRotation );
		
		// next step
		if ( bIsNextStep == true )
		{
			m_nStepLogo = -1;
		}
	}
}

KDvoid LogoScene::updateNurungee ( KDdouble dLeftTime )
{
	// update animation
	m_pAniNurungee->update ( dLeftTime );

	if ( m_nStepNurungee == 0 )
	{
		// init
		Point	tPoint = m_pAniNurungee->getPoint ( );
		KDbool	bIsNextStep = false;
		
		// edit
		tPoint.y += dLeftTime * 200;
		
		// fence
		if ( tPoint.y > 0 )
		{
			tPoint.y = 0;
			bIsNextStep = true;
		}
		
		// set pt
		m_pAniNurungee->setPoint ( tPoint );
		
		// change step
		if ( bIsNextStep == true )
		{
			++m_nStepNurungee;
			m_fNurungVibrate = -2;
		}
	}
	else if ( m_nStepNurungee == 1 )
	{
		// init
		KDfloat		fAbsVibrate = kdFabsf ( m_fNurungVibrate );
		
		// set pt
		m_pAniNurungee->setPoint ( Point ( m_pAniNurungee->getPoint ( ).x, m_pAniNurungee->getPoint ( ).y + m_fNurungVibrate ) );
		
		// vibrate
		fAbsVibrate -= dLeftTime * 10;
	
		// check next step
		if ( fAbsVibrate < 0 )
		{
			// step
			++m_nStepNurungee;

			// animation start
			m_pAniNurungee->start ( );
		}
		else
		{
			// set vibrate
			if ( m_fNurungVibrate > 0 )	m_fNurungVibrate = -fAbsVibrate;
			else						m_fNurungVibrate =  fAbsVibrate;
		}
	}
	else if ( m_nStepNurungee == 2 )
	{
		if ( m_pAniNurungee->getIsPlay ( ) == false )
		{
			m_pAniNurungee->selectAnimation ( 0 );
			m_pAniNurungee->setIsLoop ( true );
			m_pAniNurungee->start ( );
		}
	}
	else if ( m_nStepNurungee == 3 )
	{
		// init
		Point	tPoint = m_pAniNurungee->getPoint ( );
		KDbool	bIsNextStep = false;
		
		// edit
		tPoint.y -= dLeftTime * 200;
		
		// fence
		if ( tPoint.y < -63 )
		{
			tPoint.y = -63;
			bIsNextStep = true;
		}
		
		// set pt
		m_pAniNurungee->setPoint ( tPoint );
		
		// change step
		if ( bIsNextStep == true )
		{
			//step off
			m_nStepNurungee = -1;
			
			m_nStepNurungeePunch = 1;
			m_dNurungPunchTime = 0;
		}
	}
}

KDvoid LogoScene::updateNurungeePunch ( KDdouble dLeftTime )
{
	// update animation
	m_pAniNurungeePunch->update ( dLeftTime );

	if ( m_nStepNurungeePunch == 1 )
	{
		// init
		Point			tPoint		= m_pAniNurungeePunch->getPoint ( );
		KDbool			bIsNextStep	= false;
		const KDfloat	fArriveTime	= 0.1f;
		
		// passTime
		m_dNurungPunchTime += dLeftTime;
		
		// time check
		if ( m_dNurungPunchTime > fArriveTime )
		{
			bIsNextStep = true;
			m_dNurungPunchTime = fArriveTime;
		}
		
		// edit
		tPoint.x = _SCR_W + 39	- ( m_dNurungPunchTime / fArriveTime ) * 229;
		tPoint.y = -229			+ ( m_dNurungPunchTime / fArriveTime ) * 229;
		
		// pt fence
		if ( tPoint.x < _SCR_W - 190 )	tPoint.x = _SCR_W - 190;
		if ( tPoint.y > 0			 )	tPoint.y = 0;
		
		// set pt
		m_pAniNurungeePunch->setPoint ( tPoint );
		
		// change step
		if ( bIsNextStep == true )
		{
			++m_nStepNurungeePunch;
			m_fNurungPunchVibrate = -20;
		}		
	}
	else if ( m_nStepNurungeePunch == 2 )
	{
		// init
		KDfloat		fAbsVibrate = kdFabsf ( m_fNurungPunchVibrate );
		
		// set pt
		m_pAniNurungeePunch->setPoint ( Point ( m_pAniNurungeePunch->getPoint ( ).x, m_pAniNurungeePunch->getPoint ( ).y + m_fNurungPunchVibrate ) );
		
		// vibrate
		fAbsVibrate -= dLeftTime * 100;
		
		// check next step
		if ( fAbsVibrate < 0 )
		{
			// step
			m_nStepNurungeePunch = -1;
		}
		else
		{
			// set vibrate
			if ( m_fNurungPunchVibrate > 0 )	m_fNurungPunchVibrate = -fAbsVibrate;
			else								m_fNurungPunchVibrate =  fAbsVibrate;
		}
	}
}

KDvoid LogoScene::timeout ( KDfloat fDelta )
{
	this->unscheduleAllSelectors ( );

	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.5f, MainScene::create ( ) ) );
}
