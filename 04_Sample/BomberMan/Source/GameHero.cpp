/* --------------------------------------------------------------------------
 *
 *      File            GameHero.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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
#include "GameHero.h"

GameHero* GameHero::create ( const CCPoint& tPoint, CCLayer* pLayer )
{
	GameHero*	pRet = new GameHero ( );

	if ( pRet && pRet->initAtPoint ( tPoint, pLayer ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool GameHero::initAtPoint ( const CCPoint& tPoint, CCLayer* pLayer )
{
	if ( !CCSprite::initWithFile ( "down.png" ) )
	{
		return KD_FALSE;
	}

	this->setPosition ( tPoint );

	m_eAct		 = kStay;
	m_fSpeed	 = 8;
	m_pTileArray = KD_NULL;

	pLayer->addChild ( this );

	return KD_TRUE;		
}

KDvoid GameHero::setTileArray ( CCArray* pTileArray )
{
	m_pTileArray = pTileArray;
}

HeroAct GameHero::getAct ( KDvoid )
{
	return m_eAct;
}

KDvoid GameHero::setAct ( HeroAct eAct )
{
	m_eAct = eAct;
}

KDvoid GameHero::moveLeft ( KDvoid )
{
	this->changWithSpriteFile ( "left.png" );

    m_eAct = kLeft;
    m_eOrg = kl;

    for ( KDuint i = 0; i < m_pTileArray->count ( ); i++ ) 
    {
        CCSprite*	pTile = (CCSprite*) m_pTileArray->objectAtIndex ( i );
        
        if ( ( this->getPosition ( ).x > pTile->getPosition ( ).x ) && this->getPosition ( ).x - pTile->getPosition ( ).x == TileW ) 
        {            
            if ( pTile->getPosition ( ).y == this->getPosition ( ).y ) 
            {
                return;
            }
            if ( pTile->getPosition ( ).y > this->getPosition ( ).y && pTile->getPosition ( ).y - this->getPosition ( ).y < TileH ) 
            {
                this->setPosition ( ccp ( this->getPosition ( ).x, pTile->getPosition ( ).y - TileH ) );
                break;
            }
            if ( pTile->getPosition ( ).y < this->getPosition ( ).y && this->getPosition ( ).y - pTile->getPosition ( ).y < TileH ) 
            {
                this->setPosition ( ccp ( this->getPosition ( ).x, pTile->getPosition ( ).y + TileH ) );
                break;
            }
        }

        if ( this->getPosition ( ).x <= 1.5 * TileW ) 
        {
            this->setPosition ( ccp ( 1.5 * TileW, this->getPosition ( ).y ) );
            return;
        }     
    }   

	this->setPositionX ( this->getPositionX ( ) - m_fSpeed );
}

KDvoid GameHero::moveRight ( KDvoid )
{
	this->changWithSpriteFile ( "right.png" );

    m_eAct = kRight;
    m_eOrg = kr;

    for ( KDuint i = 0; i < m_pTileArray->count ( ); i++ ) 
    {
        CCSprite*	pTile = (CCSprite*) m_pTileArray->objectAtIndex ( i );
        
        if ( ( this->getPosition ( ).x < pTile->getPosition ( ).x ) && pTile->getPosition ( ).x - this->getPosition ( ).x == TileW ) 
        {
            if ( pTile->getPosition ( ).y == this->getPosition ( ).y ) 
            {
                return;
            }
            if ( pTile->getPosition ( ).y > this->getPosition ( ).y && pTile->getPosition ( ).y - this->getPosition ( ).y < TileH ) 
            {
                this->setPosition ( ccp ( this->getPosition ( ).x, pTile->getPosition ( ).y - TileH ) );
                break;
            }
            if ( pTile->getPosition ( ).y < this->getPosition ( ).y && this->getPosition ( ).y - pTile->getPosition ( ).y < TileH ) 
            {
                this->setPosition ( ccp ( this->getPosition ( ).x, pTile->getPosition ( ).y + TileH ) );
                break;
            }
        }

        if ( this->getPosition ( ).x >= 13.5 * TileW ) 
        {
            this->setPosition ( ccp ( 13.5* TileW, this->getPosition ( ).y ) );
            return;
        }
    }       

	this->setPositionX ( this->getPositionX ( ) + m_fSpeed );
}

KDvoid GameHero::moveUp ( KDvoid )
{
	this->changWithSpriteFile ( "up.png" );

    m_eAct = kUp;
    m_eOrg = ku;

    for ( KDuint i = 0; i < m_pTileArray->count ( ); i++ ) 
    {
        CCSprite*	pTile = (CCSprite*) m_pTileArray->objectAtIndex ( i );

        if ( ( this->getPosition ( ).y  < pTile->getPosition ( ).y ) && pTile->getPosition ( ).y - this->getPosition ( ).y == TileH ) 
        {
            if ( pTile->getPosition ( ).x ==  this->getPosition ( ).x ) 
            {
                return;
            }
            if ( pTile->getPosition ( ).x >  this->getPosition ( ).x && pTile->getPosition ( ).x - this->getPosition ( ).x < TileW ) 
            {
                this->setPosition ( ccp ( pTile->getPosition ( ).x - TileW, this->getPosition ( ).y ) );
                break;
            }
            if ( pTile->getPosition ( ).x< this->getPosition ( ).x && this->getPosition ( ).x - pTile->getPosition ( ).x < TileW ) 
            { 
                this->setPosition ( ccp ( pTile->getPosition ( ).x + TileW, this->getPosition ( ).y ) );
                break;
            }
        }
   
        if ( this->getPosition ( ).y >= ( 9 - 0.5 ) * TileH )  
        {
            this->setPosition ( ccp ( this->getPosition ( ).x, ( 9 - 0.5 ) * TileH ) );
            return;
        }

    }   
            
    this->setPositionY ( this->getPositionY ( ) + m_fSpeed );
}

KDvoid GameHero::moveDown ( KDvoid )
{
	this->changWithSpriteFile ( "down.png" );

    m_eAct = kDown;
    m_eOrg = kd;

    for ( KDuint i = 0; i < m_pTileArray->count ( ); i++ ) 
    {
        CCSprite*	pTile = (CCSprite*) m_pTileArray->objectAtIndex ( i );

        if ( ( this->getPosition ( ).y > pTile->getPosition ( ).y ) && this->getPosition ( ).y - pTile->getPosition ( ).y == TileH ) 
        {
            if ( pTile->getPosition ( ).x ==  this->getPosition ( ).x ) 
            {
                return;
            }
            if ( pTile->getPosition ( ).x > this->getPosition ( ).x && pTile->getPosition ( ).x - this->getPosition ( ).x < TileW ) 
            {
                this->setPosition ( ccp ( pTile->getPosition ( ).x - TileW, this->getPosition ( ).y ) );
                break;
            }
            if ( pTile->getPosition ( ).x < this->getPosition ( ).x && this->getPosition ( ).x - pTile->getPosition ( ).x < TileW ) 
            {
                this->setPosition ( ccp ( pTile->getPosition ( ).x + TileW, this->getPosition ( ).y ) );
                break;
            }
        }
        
        if ( this->getPosition ( ).y <= 0.5 * TileH ) 
        {
            this->setPosition ( ccp ( this->getPosition ( ).x, 0.5 * TileH ) );
            return;
        }
    }         

	this->setPositionY ( this->getPositionY ( ) - m_fSpeed );
}

KDvoid GameHero::onFire ( CCSprite* pBomb )
{
	m_eAct = kFire;
}	
 
KDvoid GameHero::onStay ( KDvoid )
{
	m_eAct = kStay;
}

KDvoid GameHero::changWithSpriteFile ( const KDchar* szFilename )
{
	this->setTexture ( CCTextureCache::sharedTextureCache ( )->addImage ( szFilename ) );
}

