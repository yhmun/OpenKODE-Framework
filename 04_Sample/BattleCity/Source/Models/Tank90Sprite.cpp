/* --------------------------------------------------------------------------
 *
 *      File            Tank90Sprite.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2012      喆 肖 (12/08/10 ). All rights reserved.
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
#include "Tank90Sprite.h"

Tank90Sprite* Tank90Sprite::create ( Tank90SpriteDelegate* pDelegate, KDint nLife, Tank90Kind eKind, const CCSize& tSize )
{
	Tank90Sprite*	pRet = new Tank90Sprite ( );

	if ( pRet && pRet->initWithDelegate ( pDelegate, nLife, eKind, tSize ) )
	{
		pRet->autorelease ( );	
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool Tank90Sprite::initWithDelegate ( Tank90SpriteDelegate* pDelegate, KDint nLife, Tank90Kind eKind, const CCSize& tSize )
{
	const KDchar*	szFrameName;

    switch ( eKind ) 
	{            
        case eBorn			:	szFrameName = "p1.png";		m_fSpeed = 1;		break;         
        case ePlusStarOne	:	szFrameName = "p1-a.png";	m_fSpeed = 1.5f;	break;   
		case ePlusStarTwo	:	szFrameName = "p1-b.png";	m_fSpeed = 1.5f;	break; 
		case ePlusStarThree	:	szFrameName = "p1-c.png";	m_fSpeed = 1.5f;	break; 
    }

	if ( !CCSprite::initWithSpriteFrameName ( szFrameName ) )
	{
		return KD_FALSE;
	}

	m_pDelegate			= pDelegate;
	m_nLife				= nLife;
	m_tMapSize			= tSize;
	m_nKind				= eKind;
	m_eAction			= eUp;
	m_bIsCanFire		= KD_TRUE;
	m_bIsHomeProtect	= KD_FALSE;
	m_bIsHomeDone		= KD_FALSE;
	m_bIsButtleDone		= KD_FALSE;
	m_pButtleSprite		= KD_NULL;
	m_bIsTankDone		= KD_FALSE;

	this->setScale ( 0.7f );

	this->bronInformation ( );

	return KD_TRUE;
}

KDvoid Tank90Sprite::moveUp ( KDvoid )
{
	this->setRotation ( 0 );
	m_eAction = eUp;

	CCSize		tTS = this->boundingBox ( ).size;
	CCPoint		tTP = this->getPosition ( );
	CCPoint		tTNP = ccp ( tTP.x, tTP.y + tTS.cy / 2 + m_fSpeed );
    
    if ( ( tTP.y + tTS.cy / 2 + m_fSpeed ) > m_tMapSize.cy ) 
	{
		return;
	}

	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
    
    tTNP = ccp ( tTP.x - tTS.cx / 2, tTP.y + tTS.cy / 2 + m_fSpeed );
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
    
	tTNP = ccp ( tTP.x - tTS.cx / 3, tTP.y + tTS.cy / 2 + m_fSpeed );
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
    
	tTNP = ccp ( tTP.x + tTS.cx / 2, tTP.y + tTS.cy / 2 + m_fSpeed );
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
    
	tTNP = ccp ( tTP.x + tTS.cx / 3, tTP.y + tTS.cy / 2 + m_fSpeed );
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
    
	if ( this->checkTankPosition ( ) )
	{
        return;
    }
	this->checkTool ( );
    
	this->setPosition ( ccp ( tTP.x, tTP.y + m_fSpeed ) );
}

KDvoid Tank90Sprite::moveDown ( KDvoid )
{
    this->setRotation ( 180 );
	m_eAction = eDown;

	CCSize		tTS = this->boundingBox ( ).size;
	CCPoint		tTP = this->getPosition ( );
	CCPoint		tTNP = ccp ( tTP.x, tTP.y - tTS.cy / 2 - m_fSpeed );
    
    if ( ( tTP.y - tTS.cy / 2 - m_fSpeed ) < 0 )
	{
		return;
	}

	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
    
    tTNP = ccp ( tTP.x - tTS.cx / 2, tTP.y - tTS.cy / 2 - m_fSpeed );
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
    
    tTNP = ccp ( tTP.x - tTS.cx / 3, tTP.y - tTS.cy / 2 - m_fSpeed );
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
    
    tTNP = ccp ( tTP.x + tTS.cx / 2, tTP.y - tTS.cy / 2 - m_fSpeed );
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
    
    tTNP = ccp ( tTP.x + tTS.cx / 3, tTP.y - tTS.cy / 2 - m_fSpeed );
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}

	if ( this->checkTankPosition ( ) )
	{
        return;
    }

	this->checkTool ( );

	this->setPosition ( ccp ( tTP.x, tTP.y - m_fSpeed ) );   
}

KDvoid Tank90Sprite::moveLeft ( KDvoid )
{
    this->setRotation ( -90 );
	m_eAction = eLeft;

 	CCSize		tTS = this->boundingBox ( ).size;
	CCPoint		tTP = this->getPosition ( );
    CCPoint		tTNP = ccp ( tTP.x - tTS.cx / 2 - m_fSpeed, tTP.y );
    
    if ( ( tTP.x - tTS.cx / 2 - m_fSpeed ) < 0 )
	{
		return;
	}
   
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
   
    tTNP = ccp ( tTP.x - tTS.cx / 2 - m_fSpeed, tTP.y + tTS.cy / 2 - 2 );
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
    
    tTNP = ccp ( tTP.x - tTS.cx / 2 - m_fSpeed, tTP.y + tTS.cy / 3 );
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
    
    tTNP = ccp ( tTP.x - tTS.cx / 2 - m_fSpeed, tTP.y - tTS.cy / 2 + 2 );
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
    
    tTNP = ccp ( tTP.x - tTS.cx / 2 - m_fSpeed, tTP.y - tTS.cy / 3 );
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
	
	if ( this->checkTankPosition ( ) )
	{
        return;
    }

	this->checkTool ( );

	this->setPosition ( ccp ( tTP.x - m_fSpeed, tTP.y ) );   
}

KDvoid Tank90Sprite::moveRight ( KDvoid )
{
    this->setRotation ( 90 );
	m_eAction = eRight;

 	CCSize		tTS = this->boundingBox ( ).size;
	CCPoint		tTP = this->getPosition ( );
	CCPoint		tTNP = ccp ( tTP.x + tTS.cx / 2 + m_fSpeed, tTP.y );
    if ( ( tTP.x + tTS.cx / 2 + m_fSpeed ) > m_tMapSize.cx )
	{
		return; 
	}
   
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
    
    tTNP = ccp ( tTP.x + tTS.cx / 2 + m_fSpeed, tTP.y + tTS.cy / 2 - 2 );
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
    
    tTNP = ccp ( tTP.x + tTS.cx / 2 + m_fSpeed, tTP.y + tTS.cy / 3 );
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
    
    tTNP = ccp ( tTP.x + tTS.cx / 2 + m_fSpeed, tTP.y - tTS.cy / 2 + 2 );
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
    
    tTNP = ccp ( tTP.x + tTS.cx / 2 + m_fSpeed, tTP.y - tTS.cy / 3 );
	if ( this->checkPoint ( tTNP ) )
	{
		return;
	}
        
	if ( this->checkTankPosition ( ) )
	{
        return;
    }

	this->checkTool ( );

	this->setPosition ( ccp ( tTP.x + m_fSpeed, tTP.y ) );   
}

KDbool Tank90Sprite::checkPoint ( const CCPoint& tTNP )
{
	KDuint		uTID;

    uTID = m_pDelegate->tileIDFromPosition ( tTNP, this );
    if ( uTID != 0 && uTID != 9 && uTID != 10 && uTID != 37 && uTID != 38 )
	{
		return KD_TRUE;
	}
    
    uTID = m_pDelegate->tileIDFromPositionLayer2 ( tTNP, this );
    if ( uTID != 0 )
	{
		return KD_TRUE;
	}

	return KD_FALSE;
}

KDvoid Tank90Sprite::onFire ( KDfloat fDelta )
{
    if ( m_nLife == 0 )
	{
        return;            
    }
    
    if ( m_nKind == ePlusStarTwo || m_nKind == ePlusStarThree )
	{        
        if ( m_nButtleCount <= 2 )
		{            
            m_nButtleOrientation = m_eAction;     
			
			CCSprite*	pButtle = CCSprite::createWithSpriteFrameName ( "bullet.png" ); 
            pButtle->setVisible ( KD_FALSE );
            
			m_pDelegate->initButtleDidFinish ( this, pButtle );
            
			this->fire  ( pButtle, m_eAction );
            m_nButtleCount++;

            if ( m_nButtleCount > 2 )
			{
				this->scheduleOnce ( schedule_selector ( Tank90Sprite::canFire2 ), 1 );
            }            
        }
		else 
		{
            return;
        }        
    }
	else if ( m_nKind == eBorn || m_nKind == ePlusStarOne )
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

        this->fire  ( pButtle, m_eAction );
    }
}

KDvoid Tank90Sprite::makeCanFire ( KDvoid )
{
	 m_bIsCanFire = KD_TRUE;
}

KDvoid Tank90Sprite::canFire2 ( KDfloat fDelta )
{
	m_nButtleCount = 0;
}

KDvoid Tank90Sprite::fire ( CCSprite* pButtle, Tank90Action eButtleOrientation )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sounds/bullet.wav" );

	CCSize		tTS = this->boundingBox ( ).size;
	CCPoint		tTP = this->getPosition ( );
    CCPoint		tPos;
    
    switch ( m_nButtleOrientation )
	{            
		case eUp :
            pButtle->setRotation ( 0 );
			pButtle->setPosition ( ccp ( tTP.x, tTP.y + tTS.cy / 2 ) ); 
            
            tPos = ccp ( tTP.x, tTP.y + m_tMapSize.cx );
            break;
        
        case eDown :
			pButtle->setRotation ( 180 );
			pButtle->setPosition ( ccp ( tTP.x, tTP.y - tTS.cy / 2 ) );

            tPos = ccp ( tTP.x, tTP.y - m_tMapSize.cx );
            break;
        
		case eLeft :
			pButtle->setRotation ( -90 );
			pButtle->setPosition ( ccp ( tTP.x - tTS.cx / 2, tTP.y ) );

            tPos = ccp ( tTP.x - m_tMapSize.cx, tTP.y );
            break;

        case eRight :
			pButtle->setRotation ( 90 );
			pButtle->setPosition ( ccp ( tTP.x + tTS.cx / 2, tTP.y ) );

            tPos = ccp ( tTP.x + m_tMapSize.cx, tTP.y );
            break;        
    }

	pButtle->runAction
	(
		CCSequence::create 
		(
			CCDelayTime::create ( 1 / 60.f ),
			CCShow::create ( ),			
			CCMoveTo::create ( 2, tPos ),
			CCCallFunc::create ( this, callfunc_selector ( Tank90Sprite::makeCanFire ) ),
			KD_NULL
		)
	);
	
	this->schedule ( schedule_selector ( Tank90Sprite::checkBang ), 1 / 60.f );
}

KDvoid Tank90Sprite::checkBang ( KDfloat fDelta )
{
	CCSprite*	pButtle = m_pButtleSprite;
	KDuint		uGID = m_pDelegate->tileIDFromPosition ( pButtle->getPosition ( ), this );

    if ( uGID == -1 )
	{
		goto remove;
    }

    if ( this->checkLayer2 ( pButtle ) )
	{
        goto remove;
    }
    
    if ( this->checkHome ( pButtle ) )
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

	{
		CCArray*	pEnemies = m_pDelegate->enemyArray ( this );
		CCObject*	pObject;
		CCPoint		tBP = pButtle->getPosition ( );
		CCSize		tBS = pButtle->boundingBox ( ).size;
		CCRect		tButtleRect = CCRect ( tBP.x - tBS.cx / 2, tBP.y - tBS.cy / 2, tBS.cx, tBS.cy );
		CCRect		tTankEnemy;

		CCARRAY_FOREACH ( pEnemies, pObject )
		{			
			Tank90Sprite*	pTank = (Tank90Sprite*) pObject;

			if ( pTank->getButtleSprite ( ) != KD_NULL )
			{
				if ( tButtleRect.containsPoint ( pTank->getButtleSprite ( )->getPosition ( ) ) )
				{
					pTank->setButtleDone ( KD_TRUE );  
					goto remove;
				}
			}

			CCPoint		tTP = pTank->getPosition ( );
			CCSize		tTS = pTank->boundingBox ( ).size;
			tTankEnemy = CCRect ( tTP.x - tTS.cx / 2, tTP.y - tTS.cy / 2, tTS.cx, tTS.cy );

			if ( tTankEnemy.containsPoint ( tBP ) ) 
			{
				m_pTmpTank = pTank;

				this->checkEnemyKind ( pTank );

				this->unschedule ( schedule_selector ( Tank90Sprite::checkBang ) );      
			 
				pButtle->removeFromParent ( );

				m_pButtleSprite = KD_NULL;  
				m_bIsCanFire	= KD_TRUE;
			}
		}
	}

    if ( m_bIsTankDone )
	{
		SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sounds/blast.wav" );
        m_pTmpTank->animationBang ( );        
        m_pDelegate->removeSprite ( m_pTmpTank );
        m_bIsTankDone = KD_FALSE;
        return;
    }

    if ( this->checkBound ( pButtle ) )
	{
        goto remove;
    }

	return ;

remove :

	this->unschedule ( schedule_selector ( Tank90Sprite::checkBang ) );      
 
	pButtle->removeFromParent ( );

	m_pButtleSprite = KD_NULL;  
	m_bIsCanFire	= KD_TRUE;
}

KDbool Tank90Sprite::checkHome ( CCSprite* pButtle )
{
	if ( m_tHomeRect.containsPoint ( pButtle->getPosition ( ) ) )
	{
        if ( m_bIsHomeProtect ) 
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

KDvoid Tank90Sprite::checkEnemyKind ( Tank90Sprite* pTank )
{
    switch ( pTank->getKind ( ) )
	{            
        case eSlow :
            m_bIsTankDone = KD_TRUE;
            break;
        
        case eQuike : 
            m_bIsTankDone = KD_TRUE;
            break;
        
        case eStrong : 
            m_bIsTankDone = KD_TRUE;
            break;
        
        case eStrongYellow :			
            if ( pTank->getLife ( ) == 2 )
			{
                pTank->setLife ( 1 );
				pTank->setKind ( eStrong );
				pTank->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "en6.png" ) );    
            }
			else 
			{
                m_bIsTankDone = KD_TRUE;
            }
            break;

        case eStrongRedLife3 :             
            if ( pTank->isRead ( ) ) 
			{
                m_pDelegate->createTool ( );
                pTank->setRead ( KD_FALSE );
            }
            
			if ( pTank->getLife ( ) == 3 )
			{
				pTank->setLife ( 2 );
				pTank->setKind ( eStrongYellow );            
				pTank->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "en5.png" ) );    
            }
			else if ( pTank->getLife ( ) == 2 )
			{
                pTank->setLife ( 1 );
                pTank->setKind ( eStrong );
				pTank->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "en6.png" ) );    
            }
            else 
			{
                m_bIsTankDone = KD_TRUE;
            }			
            break;
		
        case eStrongRed :            
            m_pDelegate->createTool ( );
            m_bIsTankDone = KD_TRUE;
            break;        
		
        case eStrongGreen :
            if ( pTank->getLife ( ) == 3 )
			{
				pTank->setLife ( 2 );
                pTank->setKind ( eStrongYellow );
				pTank->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "en5.png" ) );
            }
			else if ( pTank->getLife ( ) == 2 )
			{
				pTank->setLife ( 1 );
				pTank->setKind ( eStrong );
                pTank->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "en6.png" ) );
            }
            else 
			{
                m_bIsTankDone = KD_TRUE;
            }
            break;            
        
        case eQuikeR :           
            m_pDelegate->createTool ( );
            m_bIsTankDone = KD_TRUE;
            break;
        
        case eSlowR :            
            m_pDelegate->createTool ( );
            m_bIsTankDone = KD_TRUE;
            break;        		
    }
}

KDvoid Tank90Sprite::checkTool ( KDvoid )
{
	CCArray*	pArray = m_pDelegate->toolsArray ( this ); //CCArray::createWithArray ( m_pDelegate->toolsArray ( this ) );
	CCPoint		tTP = this->getPosition ( );
	CCSize		tTS = this->boundingBox ( ).size;
	CCRect		tTankRect, tPropRect;
	CCObject*	pObject;
    
    tTankRect = CCRect ( tTP.x - tTS.cx / 2, tTP.y - tTS.cy / 2, tTS.cx, tTS.cy );
   
    CCARRAY_FOREACH ( pArray, pObject )
	{        
		ToolSprite*		pProp = (ToolSprite*) pObject;

		tTP = pProp->getPosition ( );
		tTS = pProp->getContentSize ( );

        tPropRect = CCRect ( tTP.x - tTS.cx / 2, tTP.y - tTS.cy / 2, tTS.cx, tTS.cy );

        if ( pProp->isVisible ( ) == KD_FALSE )
		{
			continue;
		}
        
		if ( tTankRect.containsPoint ( pProp->getPosition ( ) ) )
		{
			pProp->setVisible ( KD_FALSE );  
            m_pTmpTool = pProp;
			this->changeTankStatus ( pProp );
            m_pDelegate->removeTool ( pProp );   
		}
    }    
}

KDvoid Tank90Sprite::changeTankStatus ( ToolSprite* pTool )
{	
	ToolSpriteKind	eKind = pTool->getKind ( );

    if ( eKind == eStart )
	{        		
        if ( m_nKind == eBorn )
		{
			SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sounds/award.wav" );
			m_nKind = ePlusStarOne;
			m_fSpeed = 1.5f;
			this->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "p1-a.png" ) );
        }
		else if ( m_nKind == ePlusStarOne )
		{
			SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sounds/award.wav" ); 
			m_nKind = ePlusStarTwo;
			m_fSpeed = 1.5f;
			this->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "p1-b.png" ) );
        }
		else if ( m_nKind == ePlusStarTwo )
		{
			SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sounds/award.wav" );
			m_nKind = ePlusStarThree;
			m_fSpeed = 1.5f;
			this->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "p1-c.png" ) );       
        }
		else 
		{
            return;
        }		
    }
	else if ( eKind == eMine ) 
	{
		SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sounds/tankbomb.wav" );
		m_pDelegate->plusBoon ( this );
    }
	else if ( eKind == eLife )
	{
		SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sounds/addlife.wav" );
		m_nLife++;
		m_pDelegate->changeLife ( this );
    }
	else if ( eKind == ePass )
	{
		SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sounds/award.wav" );
		m_pDelegate->plusPass ( this );
    }
	else if ( eKind == eSafe ) 
	{
		SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sounds/award.wav" );
		
        if ( m_bIsProtect )  
		{          
            return;
        }
        this->animationShield ( 0 );
		this->scheduleOnce ( schedule_selector ( Tank90Sprite::removeShield ), 10 );
    }
	else if ( eKind == eWall )
	{
		SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sounds/award.wav" );
		m_bIsHomeProtect = KD_TRUE;		      
        m_pDelegate->homeProtect ( KD_TRUE, this );
		this->scheduleOnce ( schedule_selector ( Tank90Sprite::removeHomeProtect ), 10 );
    }	
}

KDvoid Tank90Sprite::removeHomeProtect ( KDfloat fDelta )
{
    m_bIsHomeProtect = KD_FALSE;
    m_pDelegate->homeProtect ( KD_FALSE, this );
}

KDbool Tank90Sprite::checkBound ( CCSprite* pButtle )
{
	const CCPoint&	tPos  = pButtle->getPosition ( );
	const CCSize&	tSize = pButtle->getContentSize ( );

    if ( ( tPos.y + tSize.cy / 2 ) > m_tMapSize.cy )
	{  
        return KD_TRUE;        
    }   
    else if ( ( tPos.y - tSize.cy / 2 ) < 0 )
	{             
        return KD_TRUE;
    }  
    else if ( ( tPos.x - tSize.cx / 2 ) < 0 )
	{          
        return KD_TRUE;
    }    
    else if ( ( tPos.x + tSize.cx / 2 ) > m_tMapSize.cx )
	{
        return KD_TRUE;
    }

	return KD_FALSE;
}

KDbool Tank90Sprite::checkWall ( CCSprite* pButtle )
{
	CCPoint		tBtPos = pButtle->getPosition ( );
	KDuint		uGID, uGID1, uGID2, uGID3, uGID4;

	uGID = m_pDelegate->tileIDFromPosition ( tBtPos, this );

	if ( m_nButtleOrientation == eUp || m_nButtleOrientation == eDown )
	{
		uGID1 = m_pDelegate->tileIDFromPosition ( ccp ( tBtPos.x -  8, tBtPos.y ), this );
		uGID2 = m_pDelegate->tileIDFromPosition ( ccp ( tBtPos.x +  8, tBtPos.y ), this );
		uGID3 = m_pDelegate->tileIDFromPosition ( ccp ( tBtPos.x - 16, tBtPos.y ), this );
		uGID4 = m_pDelegate->tileIDFromPosition ( ccp ( tBtPos.x + 16, tBtPos.y ), this );

		if ( uGID == 29 || uGID == 30 || uGID == 2 || uGID == 1 ) 
		{
			m_pDelegate->destroyTile ( tBtPos, this );
	        	       
			if ( ( uGID1 == 29 || uGID1 == 30 || uGID1 == 2 || uGID1 == 1 ) && ( uGID2 == 29 || uGID2 == 30 || uGID2 == 2 || uGID2 == 1 ) ) 
			{	            	            
				if ( uGID4 == 29 || uGID4 == 30 || uGID4 == 2 || uGID4 == 1 ) 
				{
					m_pDelegate->destroyTile ( ccp ( tBtPos.x -  8, tBtPos.y ), this );
					m_pDelegate->destroyTile ( ccp ( tBtPos.x +  8, tBtPos.y ), this );
					m_pDelegate->destroyTile ( ccp ( tBtPos.x + 16, tBtPos.y ), this );
					return KD_TRUE;
				}
				else 
				{
					if ( uGID3 == 0 ) 
					{
						m_pDelegate->destroyTile ( ccp ( tBtPos.x - 8, tBtPos.y ), this );
						m_pDelegate->destroyTile ( ccp ( tBtPos.x + 8, tBtPos.y ), this );
						return KD_TRUE;
					} 
					else if ( uGID3 == 29 || uGID3 == 30 || uGID3 == 2 || uGID3 == 1 )
					{
						m_pDelegate->destroyTile ( ccp ( tBtPos.x -  8, tBtPos.y ), this );
						m_pDelegate->destroyTile ( ccp ( tBtPos.x - 16, tBtPos.y ), this );
						m_pDelegate->destroyTile ( ccp ( tBtPos.x +  8, tBtPos.y ), this );
						return KD_TRUE;
					}
					else
					{
						m_pDelegate->destroyTile ( ccp ( tBtPos.x - 8, tBtPos.y ), this );
						m_pDelegate->destroyTile ( ccp ( tBtPos.x + 8, tBtPos.y ), this );
						return KD_TRUE;
					}
				}	            
			}
	        	       
			if ( ( uGID1 == 29 || uGID1 == 30 || uGID1 == 2 || uGID1 == 1 ) && uGID2 == 0 ) 
			{
				m_pDelegate->destroyTile ( ccp ( tBtPos.x - 8, tBtPos.y ), this );
				return KD_TRUE;
			}
	        	        
			if ( uGID1 == 0 && ( uGID2 == 29 || uGID2 == 30 || uGID2 == 2 || uGID2 == 1 ) ) 
			{
				m_pDelegate->destroyTile ( ccp ( tBtPos.x + 8, tBtPos.y ), this );
				return KD_TRUE;
			}
	        	        
			if ( ( uGID1 != 1 && uGID1 != 2 && uGID1 != 29 && uGID1 != 30 && uGID1 != 0 ) && ( uGID2 == 29 || uGID2 == 30 || uGID2 == 2 || uGID2 == 1 ) ) 
			{
				m_pDelegate->destroyTile ( ccp ( tBtPos.x + 8, tBtPos.y ), this );
				return KD_TRUE;
			}
	        	        
			if ( ( uGID1 != 1 && uGID1 != 2 && uGID1 != 29 && uGID1 != 30 && uGID1 != 0 ) && uGID2 == 0 ) 
			{
				return KD_TRUE;
			}
	         
			if ( ( uGID2 != 1 && uGID2 != 2 && uGID2 != 29 && uGID2 != 30 && uGID2 != 0 ) && ( uGID1 == 29 || uGID1 == 30 || uGID1 == 2 || uGID1 == 1 ) ) 
			{
				m_pDelegate->destroyTile ( ccp ( tBtPos.x - 8, tBtPos.y ), this );
				return KD_TRUE;
			}
	        	        
			if ( ( uGID2 != 1 && uGID2 != 2 && uGID2 != 29 && uGID2 != 30 && uGID2 != 0 ) && uGID1 == 0 )
			{
				return KD_TRUE;
			}
	        	        
			if ( uGID1 == 0 && uGID2 == 0 )
			{
				return KD_TRUE;
			}
		}		
	}

	if ( m_nButtleOrientation == eLeft || m_nButtleOrientation == eRight )
	{
		uGID1 = m_pDelegate->tileIDFromPosition ( ccp ( tBtPos.x, tBtPos.y -  8 ), this );
		uGID2 = m_pDelegate->tileIDFromPosition ( ccp ( tBtPos.x, tBtPos.y +  8 ), this );
		uGID3 = m_pDelegate->tileIDFromPosition ( ccp ( tBtPos.x, tBtPos.y - 16 ), this );
		uGID4 = m_pDelegate->tileIDFromPosition ( ccp ( tBtPos.x, tBtPos.y + 16 ), this );

		if ( uGID == 29 || uGID == 30 || uGID == 2 || uGID == 1 ) 
		{	        
			m_pDelegate->destroyTile ( tBtPos, this );
	        	        
			if ( ( uGID1 == 29 || uGID1 == 30 || uGID1 == 2 || uGID1 == 1 ) && ( uGID2 == 29 || uGID2 == 30 || uGID2== 2 || uGID2 == 1 ) ) 
			{	            	            
				if ( uGID4 == 29 || uGID4 == 30 || uGID4 == 2 || uGID4 == 1 ) 
				{
					m_pDelegate->destroyTile ( ccp ( tBtPos.x, tBtPos.y -  8 ), this );
					m_pDelegate->destroyTile ( ccp ( tBtPos.x, tBtPos.y +  8 ), this );
					m_pDelegate->destroyTile ( ccp ( tBtPos.x, tBtPos.y + 16 ), this );
					return KD_TRUE;
				}
				else 
				{
					if ( uGID3 == 0 ) 
					{
						m_pDelegate->destroyTile ( ccp ( tBtPos.x, tBtPos.y - 8 ), this );
						m_pDelegate->destroyTile ( ccp ( tBtPos.x, tBtPos.y + 8 ), this );
						return KD_TRUE;
					}
					else if ( uGID3 == 29 || uGID3 == 30 || uGID3 == 2 || uGID3 == 1 )
					{
						m_pDelegate->destroyTile ( ccp ( tBtPos.x, tBtPos.y -  8 ), this );
						m_pDelegate->destroyTile ( ccp ( tBtPos.x, tBtPos.y - 16 ), this );
						m_pDelegate->destroyTile ( ccp ( tBtPos.x, tBtPos.y +  8 ), this );
						return KD_TRUE;
					}
					else
					{
						m_pDelegate->destroyTile ( ccp ( tBtPos.x, tBtPos.y - 8 ), this );
						m_pDelegate->destroyTile ( ccp ( tBtPos.x, tBtPos.y + 8 ), this );
						return KD_TRUE;
					}
				}	            
			}	        
	        
			if ( ( uGID1 == 29 || uGID1 == 30 || uGID1 == 2 || uGID1 == 1 ) && uGID2 == 0 ) 
			{
				m_pDelegate->destroyTile ( ccp ( tBtPos.x, tBtPos.y - 8 ), this );
				return KD_TRUE;
			}
	        	        
			if ( uGID1 == 0 && ( uGID2 == 29 || uGID2 == 30 || uGID2== 2 || uGID2 == 1 ) ) 
			{
				m_pDelegate->destroyTile ( ccp ( tBtPos.x, tBtPos.y + 8 ), this );
				return KD_TRUE;
			}	        
	        
			if ( ( uGID1 != 1 && uGID1 != 2 && uGID1 != 29 && uGID1 != 30 && uGID1 != 0 ) && ( uGID2 == 29 || uGID2 == 30 || uGID2== 2 || uGID2 == 1 ) ) 
			{
				m_pDelegate->destroyTile ( ccp ( tBtPos.x, tBtPos.y + 8 ), this );
				return KD_TRUE;
			}
	        	        
			if ( ( uGID1 != 1 && uGID1 != 2 && uGID1 != 29 && uGID1 != 30 && uGID1 != 0 ) && uGID2 == 0 ) 
			{
				return KD_TRUE;
			}
	        	        
			if ( ( uGID2 != 1 && uGID2 != 2 && uGID2 != 29 && uGID2 != 30 && uGID2 != 0 ) && ( uGID1 == 29 || uGID1 == 30 || uGID1 == 2 || uGID1 == 1 ) ) 
			{
				m_pDelegate->destroyTile ( ccp ( tBtPos.x, tBtPos.y - 8 ), this );
				return KD_TRUE;
			}
	        	        
			if ( ( uGID2 != 1 && uGID2 != 2 && uGID2 != 29 && uGID2 != 30 && uGID2 != 0 ) && uGID1 == 0 )
			{
				return KD_TRUE;
			}
	        	        
			if ( uGID1 == 0 && uGID2 == 0 ) 
			{
				return KD_TRUE;
			}	        
		}
	}

	return KD_FALSE;
}

KDbool Tank90Sprite::checkStrongWall ( CCSprite* pButtle )
{
    CCPoint		tBtPos = pButtle->getPosition ( );
    KDuint		uGID = m_pDelegate->tileIDFromPosition ( tBtPos, this );

    if ( uGID == 5 || uGID ==6 ||uGID ==33 ||uGID == 34 ) 
	{        
        if ( m_nKind == ePlusStarThree )
		{            
            m_pDelegate->destroyTile ( ccp ( tBtPos.x,tBtPos.y ), this );
           
            m_bIsCanFire = KD_TRUE;
            if ( m_nButtleOrientation == eUp )
			{
                if ( uGID == 33 )
				{                    
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x    , tBtPos.y + 8 ), this );
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x + 8, tBtPos.y     ), this );
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x + 8, tBtPos.y + 8 ), this );
                }
				else if ( uGID == 34 )
				{                    
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x    , tBtPos.y + 8 ), this );
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x - 8, tBtPos.y     ), this );
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x - 8, tBtPos.y + 8 ), this );
                }
                return KD_TRUE;
                
            }
			else if ( m_nButtleOrientation == eDown )
			{
                if ( uGID == 5 )
				{                    
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x    , tBtPos.y - 8 ), this );
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x + 8, tBtPos.y     ), this );
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x + 8, tBtPos.y - 8 ), this );
                }
				else if ( uGID == 6 )
				{                    
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x    , tBtPos.y - 8 ), this );
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x - 8, tBtPos.y     ), this );
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x - 8, tBtPos.y - 8 ), this );
                }
                return KD_TRUE;
                
            }
			else if ( m_nButtleOrientation == eLeft )
			{
                if ( uGID == 34 )
				{                    
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x - 8, tBtPos.y     ), this );
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x    , tBtPos.y + 8 ), this );
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x - 8, tBtPos.y + 8 ), this );
                }
				else if ( uGID == 6 )
				{                    
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x - 8, tBtPos.y     ), this );
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x    , tBtPos.y - 8 ), this );
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x - 8, tBtPos.y - 8 ), this );
                }
                return KD_TRUE;
            }
			else if ( m_nButtleOrientation == eRight )
			{                
                if ( uGID == 5 )
				{                    
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x + 8, tBtPos.y     ), this );
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x    , tBtPos.y - 8 ), this );
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x + 8, tBtPos.y - 8 ), this );
                }
				else if ( uGID == 33 )
				{                    
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x    , tBtPos.y + 8 ), this );
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x + 8, tBtPos.y     ), this );
                    m_pDelegate->destroyTile ( ccp ( tBtPos.x + 8, tBtPos.y + 8 ), this );
                }
                return KD_TRUE;
            }            
        }
		else 
		{
            return KD_TRUE;  
        }        
    }

	return KD_FALSE;
}

KDvoid Tank90Sprite::topButtle ( KDvoid )
{
	SimpleAudioEngine::sharedEngine ( )->playEffect ( "Sounds/blast.wav" );

	this->animationBang ( );
   
    m_bIsProtect = KD_TRUE; 
    
	this->schedule ( schedule_selector ( Tank90Sprite::checkLife ), 0.3f );
}

KDvoid Tank90Sprite::checkLife ( KDfloat fDelta )
{
	this->unschedule ( schedule_selector ( Tank90Sprite::checkLife ) );

	this->setVisible ( KD_FALSE );

    if ( m_nLife > 1 )
	{
        m_nLife--;
        this->setPosition ( m_tBornPosition );
       
        this->bronInformation ( );
        this->setRotation ( 0 );
        m_eAction = eUp;

		this->setVisible ( KD_TRUE );      
        m_pDelegate->changeLife ( this );
    }
	else 
	{
        m_nLife--;
        m_pDelegate->gameOver ( this );
        m_pDelegate->changeLife ( this );
    }
}

KDvoid Tank90Sprite::bronInformation ( KDvoid )
{
	this->animationBorn ( );

	this->scheduleOnce ( schedule_selector ( Tank90Sprite::animationShield ), 1 / 2.f );
	this->scheduleOnce ( schedule_selector ( Tank90Sprite::removeShield ), 5.f );
}

KDvoid Tank90Sprite::animationBorn ( KDvoid )
{
	CCArray*	pArray = CCArray::create ( );  

    for ( KDint i = 1; i < 5; i++ )
	{
		CCSpriteFrame*		pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( ccszf ( "xing%1d.png", i ) ); 
		pArray->addObject ( pFrame );
    }
    
	CCAnimation*	pAnimation = CCAnimation::createWithSpriteFrames ( pArray, 0.2f );
	pAnimation->setRestoreOriginalFrame ( KD_TRUE );
   
	this->runAction ( CCAnimate::create ( pAnimation ) );
}
 
KDvoid Tank90Sprite::animationBang ( KDvoid )
{
    CCArray*	pArray = CCArray::create ( );  

    for ( KDint i = 1; i < 3; i++ )
	{
		CCSpriteFrame*		pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( ccszf ( "explode%1d.png", i ) ); 
		pArray->addObject ( pFrame );
    }
    
	CCAnimation*	pAnimation = CCAnimation::createWithSpriteFrames ( pArray, 0.1f );
	pAnimation->setRestoreOriginalFrame ( KD_TRUE );

	this->runAction ( CCAnimate::create ( pAnimation ) );   
}

KDvoid Tank90Sprite::animationShield ( KDfloat fDelta )
{
	this->unschedule ( schedule_selector ( Tank90Sprite::animationShield ) );
    m_bIsProtect = KD_TRUE; 
    
	CCArray*	pArray = CCArray::create ( );  

    for ( KDint i = 1; i < 3; i++ )
	{
		CCSpriteFrame*		pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( ccszf ( "shield%1d.png", i ) ); 
		pArray->addObject ( pFrame );
    }

	CCSprite*		pBackAni = CCSprite::create ( );
	CCAnimation*	pAnimation = CCAnimation::createWithSpriteFrames ( pArray, 0.2f );
	pBackAni->setPosition ( ccpMid ( this->getContentSize ( ) ) );
	pBackAni->runAction ( CCRepeatForever::create ( CCAnimate::create ( pAnimation ) ) );
	this->addChild ( pBackAni, 1, 1001 );
}

KDvoid Tank90Sprite::removeShield ( KDfloat fDelta )
{
	CCSprite*		pBackAni = (CCSprite*) this->getChildByTag ( 1001 );
	pBackAni->removeFromParent ( );

	m_bIsProtect = KD_FALSE;
}

KDbool Tank90Sprite::checkTankPosition ( KDvoid )
{	
    CCArray*	pArray = m_pDelegate->enemyArray ( this );
    CCPoint		tPoint;

	CCSize		tTS = this->boundingBox ( ).size;
    CCPoint		tTP = this->getPosition ( );

    if      ( m_eAction == eUp )
	{
        tPoint = ccp ( tTP.x, tTP.y + tTS.cy / 2 + m_fSpeed );
    }
	else if ( m_eAction == eDown )
	{
        tPoint = ccp ( tTP.x, tTP.y - tTS.cy / 2 - m_fSpeed );
    }
	else if ( m_eAction == eLeft ) 
	{
       tPoint = ccp ( tTP.x - tTS.cx / 2 - m_fSpeed, tTP.y ); 
    }
	else if ( m_eAction == eRight ) 
	{
        tPoint = ccp ( tTP.x + tTS.cx / 2 + m_fSpeed, tTP.y ); 
    }    
	
	CCObject*	pObject;
	CCARRAY_FOREACH ( pArray, pObject )
	{
		Tank90Sprite*	pTank = (Tank90Sprite*) pObject;

		tTP = pTank->getPosition ( );
		tTS = pTank->boundingBox ( ).size;

		CCRect			tTankEnemy = CCRect ( tTP.x - tTS.cx / 2, tTP.y - tTS.cy / 2, tTS.cx, tTS.cy );

		if ( tTankEnemy.containsPoint ( tPoint ) )
		{
			return KD_TRUE;
		}
	}

    return KD_FALSE;
}

KDbool Tank90Sprite::checkLayer2 ( CCSprite* pButtle )
{
    if ( !m_bIsHomeProtect )
	{
		return KD_FALSE;
	}

	KDuint	uGID = m_pDelegate->tileIDFromPositionLayer2 ( pButtle->getPosition ( ), this );

    if ( 0 != uGID )
	{
        return KD_TRUE;
    }

    return KD_FALSE;
}

KDvoid Tank90Sprite::setDelegate ( Tank90SpriteDelegate* pDelegate )
{
	m_pDelegate = pDelegate;
}

KDvoid Tank90Sprite::setMapSize ( const CCSize& tSize )
{
	m_tMapSize = tSize;
}

KDvoid Tank90Sprite::setBornPosition ( const CCPoint& tPosition )
{
	m_tBornPosition = tPosition;
}

KDvoid Tank90Sprite::setHomeRect ( const CCRect& tRect )
{
	m_tHomeRect = tRect;
}

KDbool Tank90Sprite::isHomeProtect ( KDvoid )
{
	return m_bIsHomeProtect;
}

KDbool Tank90Sprite::isProtect ( KDvoid )
{
	return m_bIsProtect;
}

KDint Tank90Sprite::getKind ( KDvoid )
{
	return m_nKind;
}

KDvoid Tank90Sprite::setKind ( KDint nKind )
{
	m_nKind = nKind;
}

KDvoid Tank90Sprite::setSpeed ( KDfloat fSpeed )
{
	m_fSpeed = fSpeed;
}

KDint Tank90Sprite::getLife ( KDvoid )
{
	return m_nLife;
}

KDvoid Tank90Sprite::setLife ( KDint nLife )
{
	m_nLife = nLife; 
}

KDint Tank90Sprite::getEnemyKindForScore ( KDvoid )
{
	return m_nEnemyKindForScore;
}

CCSprite* Tank90Sprite::getButtleSprite ( KDvoid )
{
	return m_pButtleSprite;
}

KDvoid Tank90Sprite::setButtleDone ( KDbool bDone )
{
	m_bIsButtleDone = bDone;
}

KDbool Tank90Sprite::isRead ( KDvoid )
{
	return m_bIsRead;
}

KDvoid Tank90Sprite::setRead ( KDbool bIsRead )
{
	m_bIsRead = bIsRead;
}

