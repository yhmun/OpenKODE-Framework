/* --------------------------------------------------------------------------
 *
 *      File            TankEnemySprite.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      喆 肖 (12/08/10). All rights reserved.
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
#include "TankEnemySprite.h"

TankEnemySprite* TankEnemySprite::create ( KDint nKind )
{
	TankEnemySprite*	pRet = new TankEnemySprite ( );

	if ( pRet && pRet->initWithKind ( nKind ) )
	{
		pRet->autorelease ( );	
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool TankEnemySprite::initWithKind ( KDint nKind )
{
	const KDchar*	szFrameName = KD_NULL;	

	m_bIsRead = KD_FALSE;

    switch ( nKind ) 
	{  
        case eSlow : 
			szFrameName	= "en1.png";
			m_nLife		= 1;
			m_fSpeed	= 2;
			m_nScore	= 500;
            break;
        
        case eQuike : 
			szFrameName	= "en2.png";
			m_nLife		= 1;
			m_fSpeed	= 2;
			m_nScore	= 1000;
            break;

		case eStrong :
			szFrameName	= "en6.png";
			m_nLife		= 1;
			m_fSpeed	= 1;
			m_nScore	= 1000;
            break;

		case eStrongYellow :
			szFrameName	= "en5.png";
			m_nLife		= 2;
			m_fSpeed	= 1;
			m_nScore	= 1500;
            break;

		case eStrongRedLife3 :
			szFrameName	= "en7.png";
			m_nLife		= 3;
			m_fSpeed	= 1;
			m_nScore	= 2000;
            break;

		case eStrongRed :
			szFrameName	= "en7.png";
			m_nLife		= 1;
			m_fSpeed	= 1;
			m_nScore	= 2500;
			m_bIsRead	= KD_TRUE;
            break;

		case eStrongGreen :
			szFrameName	= "en3.png";
			m_nLife		= 3;
			m_fSpeed	= 1;
			m_nScore	= 3000;
            break;

		case eQuikeR :
			szFrameName	= "en2r.png";
			m_nLife		= 1;
			m_fSpeed	= 2;
			m_nScore	= 1000;
			m_bIsRead	= KD_TRUE;
            break;

		case eSlowR :
			szFrameName	= "en1r.png";
			m_nLife		= 1;
			m_fSpeed	= 1;
			m_nScore	= 500;
			m_bIsRead	= KD_TRUE;
            break;
    }

	if ( !this->initWithSpriteFrameName ( szFrameName ) )
	{
		return KD_FALSE;
	}

	m_nKind					= nKind;
	m_nEnemyKindForScore	= nKind;
	m_eAction				= eDown;
	m_bIsCanFire			= KD_TRUE;
	m_bIsHomeProtect		= KD_FALSE;
	m_bIsHomeDone			= KD_FALSE;
	m_bIsButtleDone			= KD_FALSE;
	m_pButtleSprite			= KD_NULL;

	this->setScale ( 0.8f );
	this->setRotation ( 180 );

	this->animationBorn ( );

	this->scheduleOnce ( schedule_selector ( TankEnemySprite::initAction ), 1 );

	return KD_TRUE;
}

KDvoid TankEnemySprite::setTank ( Tank90Sprite* pTank )
{
	m_pTank = pTank;
}

KDvoid TankEnemySprite::initAction ( KDfloat fDelta )
{
	this->unschedule ( schedule_selector ( TankEnemySprite::initAction ) );

	this->schedule ( schedule_selector ( TankEnemySprite::doRandomAction ), 1 );
	this->schedule ( schedule_selector ( TankEnemySprite::keepMove ), 1 / 30.f );
	this->schedule ( schedule_selector ( TankEnemySprite::rodmoFire ), 1 );
}

KDvoid TankEnemySprite::doRandomAction ( KDfloat fDelta )
{
	KDfloat		fRand = CCRANDOM_0_1 ( );
    
    if		( fRand < 0.4f ) m_eAction = eDown;
    else if ( fRand < 0.6f ) m_eAction = eLeft;
    else if ( fRand < 0.9f ) m_eAction = eRight;
    else					 m_eAction = eUp;
}

KDvoid TankEnemySprite::keepMove ( KDfloat fDelta )
{
    switch ( m_eAction )
	{
        case eUp	:	this->moveUp	( );	break;
        case eDown	:	this->moveDown	( );	break;
		case eLeft	:	this->moveLeft	( );	break;
		case eRight	:	this->moveRight ( );	break;
        default : ;
    }
}

KDvoid TankEnemySprite::stopTankAction ( KDvoid )
{
	this->unschedule ( schedule_selector ( TankEnemySprite::doRandomAction ) );
	this->unschedule ( schedule_selector ( TankEnemySprite::keepMove ) );
	this->unschedule ( schedule_selector ( TankEnemySprite::rodmoFire ) );
}

KDvoid TankEnemySprite::removeSelfFromMap ( KDfloat fDelta )
{
	this->removeFromParent ( );
}

KDvoid TankEnemySprite::rodmoFire ( KDfloat fDelta )
{
    for ( KDint i = 0 ;i < 4; i++ )
	{
		KDint		nRand = kdRand ( ) % 4;
		KDfloat		fDelay = 0;
        
		switch ( nRand )
		{
			case 0 :	fDelay = 0;		break;
			case 1 :	fDelay = 0.2f;	break;
			case 2 :	fDelay = 0.4f;	break;
			case 3 :	fDelay = 0.7f;	break;
		}

		this->unschedule   ( schedule_selector ( TankEnemySprite::onFire ) );
		this->scheduleOnce ( schedule_selector ( TankEnemySprite::onFire ), fDelay );
    }
}

KDvoid TankEnemySprite::onFire ( KDfloat fDelta )
{
    if ( m_bIsCanFire == KD_FALSE )
	{
		return;
	}

    m_nButtleOrientation = m_eAction;     

	CCSprite*	pButtle = CCSprite::createWithSpriteFrameName ( "bullet.png" );
	m_pButtleSprite = pButtle;
    
	m_pDelegate->initButtleDidFinish ( this, pButtle );
	pButtle->setVisible ( KD_FALSE );
	m_bIsCanFire = KD_FALSE;

	this->fire ( pButtle, m_eAction );   
}

KDvoid TankEnemySprite::checkBang ( KDfloat fDelta )
{
    CCSprite*	pButtle = m_pButtleSprite;
    
    if ( this->checkLayer2 ( pButtle ) )
	{
        goto remove;
    }

    if ( this->checkHome ( pButtle ) )
	{
        goto remove;
    }

    if ( m_bIsButtleDone )
	{  
		m_bIsButtleDone = KD_FALSE;
        goto remove;
    }

    if ( this->checkTank ( pButtle ) )
	{
        goto remove;
    }

    if ( this->checkWall ( pButtle ) )
	{
        goto remove;
    }

    if ( this->checkStrongWall ( pButtle ) )
	{
        goto remove;
    }

	if ( this->checkBound ( pButtle ) )
	{
        goto remove;
    }

	return;

remove :

	if ( this->isScheduled ( schedule_selector ( Tank90Sprite::checkBang ) ) )
	{
		this->unschedule ( schedule_selector ( Tank90Sprite::checkBang ) );      
	}
 
	pButtle->removeFromParent ( );

	m_pButtleSprite = KD_NULL;  
	m_bIsCanFire	= KD_TRUE;
}

KDbool TankEnemySprite::checkHome ( CCSprite* pButtle )
{
	if ( m_tHomeRect.containsPoint ( pButtle->getPosition ( ) ) )
	{
        if ( m_pTank->isHomeProtect ( ) )
		{
			return KD_TRUE;
		}

        if ( !m_bIsHomeDone )
		{
            m_bIsHomeDone = KD_TRUE;

			m_pDelegate->gameOver ( this );                  
        }

        return KD_TRUE;
    }

    return KD_FALSE;
}

KDbool TankEnemySprite::checkTank ( CCSprite* pButtle )
{	 
	if ( m_pTank->isVisible ( ) != KD_FALSE )
	{	    
		CCPoint		tTankPos  = m_pTank->getPosition ( );
		CCSize		tTankSize = m_pTank->boundingBox ( ).size;
		CCRect		tTankRect = CCRect ( tTankPos.x - tTankSize.cx / 2, tTankPos.y - tTankSize.cy / 2, tTankSize.cx, tTankSize.cy );

		if ( tTankRect.containsPoint ( pButtle->getPosition ( ) ) )
		{    
			if ( m_pTank->isProtect ( ) ) 
			{ 
				return KD_TRUE;
			}
			else 
			{								
				if ( m_pTank->getKind ( ) == ePlusStarThree )
				{					
					m_pTank->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "p1-b.png" ) );
					m_pTank->setSpeed ( 1.5f );
					m_pTank->setKind ( ePlusStarTwo );
					return KD_TRUE;
				}
				else if ( m_pTank->getKind ( ) == ePlusStarTwo )
				{
					m_pTank->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "p1-a.png" ) );
					m_pTank->setSpeed ( 1.5f );
					m_pTank->setKind ( ePlusStarOne );
					return KD_TRUE;
				}
				else 
				{	                
					m_pTank->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "p1.png" ) );
					m_pTank->setSpeed ( 1.f );
					m_pTank->setKind ( eBorn );
					m_pTank->topButtle ( );
				}
			}			
		}		
	}	

	return KD_FALSE;
}

KDbool TankEnemySprite::checkTankPosition ( KDvoid )
{
	CCPoint		tTP = this->getPosition ( );
	CCSize		tTS = this->boundingBox ( ).size;
    CCRect		tTankEnemy;
    CCPoint		tPoint;
 
    if ( m_eAction == eUp )
	{
        tPoint = ccp ( tTP.x, tTP.y + tTS.cy / 2 + m_fSpeed );
    }
	else if ( m_eAction == eDown )
	{
        tPoint = ccp ( tTP.x, tTP.y - tTS.cy / 2 - m_fSpeed );
    }
	else if ( m_eAction == eLeft ) 
	{
        tPoint =  ccp ( tTP.x - tTS.cx / 2 - m_fSpeed, tTP.y ); 
    }
	else if ( m_eAction == eRight )
	{
        tPoint = ccp ( tTP.x + tTS.cx / 2 + m_fSpeed, tTP.y ); 
    }

	tTP = m_pTank->getPosition ( );
	tTS = m_pTank->boundingBox ( ).size;

	tTankEnemy = CCRect ( tTP.x - tTS.cx / 2, tTP.y - tTS.cy / 2, tTS.cx, tTS.cy ); 
	if ( tTankEnemy.containsPoint ( tPoint ) )
	{
        return KD_TRUE;
    }

    return KD_FALSE;
}

