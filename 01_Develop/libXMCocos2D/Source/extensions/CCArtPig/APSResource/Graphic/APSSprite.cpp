/* --------------------------------------------------------------------------
 *
 *      File            APSSprite.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com  
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
#include "extensions/CCArtPig/APSSprite.h"
#include "extensions/CCArtPig/APSSpriteSheetHolder.h"
#include "extensions/CCArtPig/APSDeviceInfo.h"
#include "extensions/CCArtPig/APSSymbol.h"
#include "sprite_nodes/CCSpriteBatchNode.h"
#include "sprite_nodes/CCSpriteFrameCache.h"

NS_APS_BEGIN

const KDbool  kAPSSprite_defaultIsAnimatable = KD_FALSE;
const KDbool  kAPSSprite_defaultFlipX = KD_FALSE;
const KDbool  kAPSSprite_defaultFlipY = KD_FALSE;

const std::string& APSSprite::getFlipXKey ( KDvoid )
{
    static const std::string  sKey = "flipX";
    return sKey;
}

const std::string& APSSprite::getIsAnimatableKey ( KDvoid ) 
{
    static const std::string  sKey = "isAnimatable";
    return sKey;
}

const std::string& APSSprite::getFlipYKey ( KDvoid ) 
{
    static const std::string  sKey = "flipY";
    return sKey;
}

const std::string& APSSprite::getImageHolderCodeKey ( KDvoid )
{
    static const std::string  sKey = "imageHolderCode";
    return sKey;
}

APSSprite::APSSprite ( APSDictionary* pProperties ) 
{
	m_bIsAnimatable	= kAPSSprite_defaultIsAnimatable;
	m_pImageHolder	= KD_NULL;
	m_bFlipX		= kAPSSprite_defaultFlipX;
	m_bFlipY		= kAPSSprite_defaultFlipY;
	m_pSpriteNode	= KD_NULL;

    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

APSSprite::~APSSprite ( KDvoid )
{
    CC_SAFE_RELEASE ( m_pSpriteNode );
}

KDbool APSSprite::initWithProperties ( APSDictionary* pProperties )
{
    KDbool  bOk = APSGraphic::initWithProperties ( pProperties );
    
    APSNumber*  pNumber;
    
    // set isAnimatable
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getIsAnimatableKey ( ) );
    if ( pNumber )
	{
        this->setIsAnimatable ( pNumber->getBoolean ( ) );
    }
    
    // set flipX
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getFlipXKey ( ) );
    if ( pNumber ) 
	{
        this->setFlipX ( pNumber->getBoolean ( ) );
    }
    
    // set flipY
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getFlipYKey ( ) );
    if ( pNumber )
	{
        this->setFlipY ( pNumber->getBoolean ( ) );
    }
    
    // set imageHolderCode
    APSString*  pStr = (APSString*) pProperties->getObjectForKey ( this->getImageHolderCodeKey ( ) );
    if ( pStr )
	{
        this->setImageHolderCode ( pStr->c_str ( ) );
    }
    
    return bOk;
}

KDvoid APSSprite::updateAccOpacity ( KDvoid )
{
    APSGraphic::updateAccOpacity ( );
    if  ( this->m_pSpriteNode ) 
	{
        this->m_pSpriteNode->setOpacity ( this->m_uAccOpacity );
    }
}

APSImageHolder* APSSprite::getImageHolder ( KDvoid )
{
    if ( !m_pImageHolder ) 
	{
        m_pImageHolder = (APSImageHolder*) this->getMediumForCode ( this->getImageHolderCode ( ) );
    }
    return m_pImageHolder;
}

APSSpriteSheetHolder* APSSprite::getSpriteSheetHolder ( KDvoid )
{
    if ( this->getIsAnimatable ( ) )
	{
        return (APSSpriteSheetHolder*) this->getImageHolder ( );
    }
    return KD_NULL;
}

CCPoint APSSprite::getPositionInSymbol ( KDvoid )
{
    APSGraphic*  pParent = this->getParent ( );
    if ( pParent )
	{
        return accpAdd ( pParent->getOriginPosition ( ), 
						 accpCompMult ( accpRotate ( this->getPosition ( ), pParent->getRotation ( ) ), pParent->getScale ( ) ) );
    }
    return this->getPosition ( );
}

KDvoid APSSprite::assignSpriteProperties ( CCSprite* pSprite, KDfloat fScale )
{    
    CCSize  tScaledSize = accsMult ( this->getSize ( ), fScale );
    
    pSprite->setRotation ( this->getRotation ( ) );    
    pSprite->setVisible ( this->getVisible ( ) );
    pSprite->setFlipX ( this->getFlipX ( ) );
    pSprite->setFlipY ( this->getFlipY ( ) );
    
    CCSize  tSize = pSprite->getContentSize ( );
    pSprite->setScaleX ( tScaledSize.cx / tSize.cx );
    pSprite->setScaleY ( tScaledSize.cy / tSize.cy );
    
    APSGraphic*  pParent = this->getParent ( );
    
    // Flatten hierarchy.
    if ( pParent ) 
	{
        // Calculate vector from sprite position to parent position.
        CCPoint  tPosition = accpRotate ( accpSub ( pParent->getPosition ( ), this->getPositionInSymbol ( ) ), this->getRotation ( ) * KD_PI_F / 180.f );
    
        // Calculate anchor
        CCPoint  tAnchor = CCPoint ( 0.5f + tPosition.x / this->getSize ( ).cx, 0.5f + tPosition.y / this->getSize ( ).cy );
        
        pSprite->setOpacity     ( pParent->getOpacity ( ) );        
        pSprite->setAnchorPoint ( tAnchor );        
        pSprite->setPosition    ( accpMult ( pParent->getPosition ( ), fScale ) );
    }
}

CCNode* APSSprite::createNode ( KDvoid ) 
{
    CCNode*          pNode = APSGraphic::createNode ( );
    APSImageHolder*  pIH = this->getImageHolder ( );

    CCSize  tContentSize;
    CCSize  tTargetSize = this->getSize ( );
    
    if ( pIH ) 
	{
        this->m_pSpriteNode = new CCSprite;
        if  ( this->getIsAnimatable ( ) )
		{
            // Create animation sprite
            APSSpriteSheetHolder*    pSH = (APSSpriteSheetHolder*) pIH;
            CCSpriteFrame*  pSpriteFrame = pSH->getCCSpriteFrameAtIndex ( ( (APSGraphicGroup*) this->getParent ( ) )->getDisplayedFrameIndex ( ) );
            
            CCSpriteBatchNode*  pSpriteSheet = CCSpriteBatchNode::createWithTexture ( pIH->getTexture ( ), pSH->getCount ( ) );

            this->m_pNode->addChild ( pSpriteSheet );
            
            this->m_pSpriteNode->initWithSpriteFrame ( pSpriteFrame );
            pSpriteSheet->addChild ( this->m_pSpriteNode );
            
            tContentSize = this->m_pSpriteNode->getContentSize ( );
            
        } 
		else 
		{
            // Check if there is a sprite frame in sprite frame cache.
            CCSpriteFrame*  pFrame = CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( this->getImageHolder ( )->getRuntimeFilename ( ).c_str ( ) );
            if ( pFrame )
			{
                // Use the sprite frame
                this->m_pSpriteNode->initWithSpriteFrame ( pFrame );
                tContentSize = pFrame->getOriginalSize ( );
                pNode->addChild ( this->m_pSpriteNode );
            } 
			else
			{
                // Normal sprite
                this->m_pSpriteNode->initWithTexture ( pIH->getTexture ( ) );
                tContentSize = pIH->getTexture ( )->getContentSize ( );
                pNode->addChild ( this->m_pSpriteNode );
            }
        }
        
        // scale the sprite to match the sizes of texture and sprite
        this->m_pSpriteNode->setScaleX ( tTargetSize.cx / tContentSize.cx );
        this->m_pSpriteNode->setScaleY ( tTargetSize.cy / tContentSize.cy );
    }

    return pNode;
}

KDbool APSSprite::initNode ( KDvoid ) 
{
    if ( APSGraphic::initNode ( ) )
	{
        CCSprite*  pSprite = this->m_pSpriteNode;
        if ( pSprite ) 
		{
            pSprite->setAnchorPoint ( CCPointZero );
            pSprite->setPosition ( CCPointZero );
            pSprite->setFlipX ( this->m_bFlipX );
            pSprite->setFlipY ( this->m_bFlipY );
        }
        
        return KD_TRUE;
    }
	else
	{
        return KD_FALSE;
	}
}

KDvoid APSSprite::updateSpriteFrame ( KDvoid ) 
{
    if  ( this->getIsAnimatable ( ) )
	{
        APSSpriteSheetHolder*  pSH = this->getSpriteSheetHolder ( );
        if ( pSH ) 
		{
            CCSpriteFrame*  pFrame = pSH->getCCSpriteFrameAtIndex ( ( (APSGraphicGroup*) this->getParent ( ) )->getRunningFrameIndex ( ) );
            if ( pFrame )
			{
                if ( !this->m_pSpriteNode->isFrameDisplayed ( pFrame ) )
				{
                    this->m_pSpriteNode->setDisplayFrame ( pFrame );
                }
            }
        }
    }
}

NS_APS_END
