/* --------------------------------------------------------------------------
 *
 *      File            APSGraphic.cpp
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
#include "extensions/CCArtPig/APSGraphic.h"
#include "extensions/CCArtPig/APSLinkerAction.h"
#include "extensions/CCArtPig/APSGraphicGroup.h"
#include "extensions/CCArtPig/APSSymbol.h"
#include "extensions/CCArtPig/APSResourceArray.h"

NS_APS_BEGIN

const CCPoint	kAPSGraphic_defaultAnchor		= CCPoint(0.5, 0.5);
const CCPoint	kAPSGraphic_defaultScale		= CCPoint(1, 1);
const KDfloat	kAPSGraphic_defaultRotation		= 0.f;
const KDuint	kAPSGraphic_defaultOpacity		= APS_MAX_OPACITY;
const KDbool	kAPSGraphic_defaultVisible		= KD_TRUE;
const KDuint	kAPSGraphic_defaultTouchOption	= kAPSTouchOptionRetain | kAPSTouchOptionAcceptsOnlyVisible;

const std::string& APSGraphic::getParentIndexKey ( KDvoid )
{
    static const std::string  sKey = "parentIndex";
    return sKey;
}

const std::string& APSGraphic::getTouchBeginEventLinkersKey ( KDvoid )
{
    static const std::string  sKey = "TBeginLinkers";
    return sKey;
}

const std::string& APSGraphic::getTouchMovedEventLinkersKey ( KDvoid )
{
    static const std::string  sKey = "TMovedLinkers";
    return sKey;
}

const std::string& APSGraphic::getTouchEndEventLinkersKey ( KDvoid )
{
    static const std::string  sKey = "TEndLinkers";
    return sKey;
}

const std::string& APSGraphic::getPositionKey ( KDvoid ) 
{
    static const std::string  sKey = "position";
    return sKey;
}

const std::string& APSGraphic::getSizeKey ( KDvoid )
{
    static const std::string  sKey = "size";
    return sKey;
}

const std::string& APSGraphic::getAnchorKey ( KDvoid )
{
    static const std::string  sKey = "anchor";
    return sKey;
}

const std::string& APSGraphic::getScaleKey ( KDvoid )
{
    static const std::string  sKey = "scale";
    return sKey;
}

const std::string& APSGraphic::getRotationKey ( KDvoid )
{
    static const std::string  sKey = "rotation";
    return sKey;
}

const std::string& APSGraphic::getOpacityKey ( KDvoid ) 
{
    static const std::string  sKey = "opacity";
    return sKey;
}

const std::string& APSGraphic::getVisibleKey ( KDvoid )
{
    static const std::string  sKey = "visible";
    return sKey;
}

const std::string& APSGraphic::getTouchOptionKey ( KDvoid )
{
    static const std::string  sKey = "touchOption";
    return sKey;
}

const std::string& APSGraphic::getLevelKey ( KDvoid )
{
    static const std::string  sKey = "level";
    return sKey;
}

APSGraphic::APSGraphic ( APSDictionary* pProperties )
{
	m_pParent				= KD_NULL;
	m_uParentIndex			= APS_NIL_INDEX;
	m_pNode					= KD_NULL;
	m_tPosition				= CCPointZero;
	m_tSize					= CCSizeZero;
	m_tAnchor				= kAPSGraphic_defaultAnchor;
	m_tScale				= kAPSGraphic_defaultScale; 
	m_fRotation				= kAPSGraphic_defaultRotation;
	m_uOpacity				= kAPSGraphic_defaultOpacity;
	m_fLevel				= 0;
	m_uRunningOpacity		= kAPSGraphic_defaultOpacity;
	m_bVisible				= kAPSGraphic_defaultVisible;
	m_uTouchOption			= kAPSGraphic_defaultTouchOption;
	m_pTouchEventLinkers	= KD_NULL;

    if ( pProperties )
	{
        this->initWithProperties ( pProperties );
    }
}

APSGraphic::~APSGraphic ( KDvoid )
{
    if ( m_pTouchEventLinkers )
	{
		APS_FOREACH ( std::vector<APSResourceArray*>, *m_pTouchEventLinkers, iter )
		{
            APS_SAFE_DELETE ( *iter );
        }
        APS_SAFE_DELETE ( m_pTouchEventLinkers );
    }

    CC_SAFE_RELEASE ( this->m_pNode );
}

KDbool APSGraphic::initWithProperties ( APSDictionary* pProperties )
{
    if ( !APSResource::initWithProperties ( pProperties ) )
	{
        return KD_FALSE;
	}
    
    // set parentIndex
    APSNumber*  pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getParentIndexKey ( ) );
    if ( pNumber ) 
	{
        this->m_uParentIndex = APSNumber::indexWithNumber ( pNumber );
    }
    
    // set position
    APSString*  pStr = (APSString*) pProperties->getObjectForKey ( this->getPositionKey ( ) );
    if ( pStr ) 
	{
        this->setPosition ( pStr->getPointValue ( ) );
    }
    
    // set size
    pStr = (APSString*) pProperties->getObjectForKey ( this->getSizeKey ( ) );
    if ( pStr )
	{
        this->setSize ( pStr->getSizeValue ( ) );
    }
    
    // set anchor
    pStr = (APSString*) pProperties->getObjectForKey ( this->getAnchorKey ( ) );
    if ( pStr )
	{
        this->setAnchor ( pStr->getPointValue ( ) );
    }
    
    // set scale
    pStr = (APSString*) pProperties->getObjectForKey ( this->getScaleKey ( ) );
    if ( pStr ) 
	{
        this->setScale ( pStr->getPointValue ( ) );
    }
    
    // set rotation
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getRotationKey ( ) );
    if ( pNumber )
	{
        this->m_fRotation = pNumber->getFloat ( );
    }
    
    // set opacity
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getOpacityKey ( ) );
    if ( pNumber )
	{
        this->m_uRunningOpacity = this->m_uOpacity = pNumber->getUnsignedInteger ( );
    }
        
    // set visible
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getVisibleKey ( ) );
    if ( pNumber )
	{
        this->m_bVisible = pNumber->getBoolean ( );
    }
    
    // set touchOption
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getTouchOptionKey ( ) );
    if ( pNumber )
	{
        this->m_uTouchOption = pNumber->getUnsignedInteger ( );
    }
    
    // set level
    pNumber = (APSNumber*) pProperties->getObjectForKey ( this->getLevelKey ( ) );
    if ( pNumber ) 
	{
        this->m_fLevel = (KDfloat) pNumber->getFloat ( );
    }
    
    APSString*  pLinkersString = (APSString*) pProperties->getObjectForKey ( this->getTouchBeginEventLinkersKey ( ) );
    if ( pLinkersString ) 
	{
        APSResourceArray*  pLinkers = this->getLinkersForEventWithCreation ( kAPSEventTypeTouchBegan );
        pLinkersString->getResourceArray ( pLinkers );
    }
    
    pLinkersString = (APSString*) pProperties->getObjectForKey ( this->getTouchMovedEventLinkersKey ( ) );
    if ( pLinkersString )
	{
        APSResourceArray*  pLinkers = this->getLinkersForEventWithCreation ( kAPSEventTypeTouchMoved );
        pLinkersString->getResourceArray ( pLinkers );
    }
    
    pLinkersString = (APSString*) pProperties->getObjectForKey ( this->getTouchEndEventLinkersKey ( ) );
    if ( pLinkersString )
	{
        APSResourceArray*  pLinkers = this->getLinkersForEventWithCreation ( kAPSEventTypeTouchEnded );
        pLinkersString->getResourceArray ( pLinkers );
    }
    
    return KD_TRUE;
}

KDvoid APSGraphic::updateAccOpacity ( KDvoid )
{    
    APSGraphic*  pParent = this->getParent ( );
    KDuint  uParentAccOpacity = pParent ? pParent->getAccOpacity ( ) : APS_MAX_OPACITY;

    this->m_uAccOpacity = uParentAccOpacity * this->m_uRunningOpacity / APS_MAX_OPACITY;
}

CCPoint APSGraphic::getOriginPosition ( KDvoid ) const 
{
    return accpSub ( this->getPosition ( ), 
					 accpRotate ( accpCompMult ( CCPoint ( this->getSize ( ).cx, this->getSize ( ).cy ),
								  accpCompMult ( this->getScale ( ), this->getAnchor ( ) ) ), this->getRotation ( ) ) );
}

APSResourceArray* APSGraphic::getLinkersForEventWithCreation ( APSEventType uType )
{
    if ( !m_pTouchEventLinkers )
	{
		m_pTouchEventLinkers = new std::vector<APSResourceArray*> ( 4, (APSResourceArray*) KD_NULL );
    }
    
    KDuint  uPos;
    switch ( uType ) 
	{
        case kAPSEventTypeTouchBegan :	uPos = 0;	break;
        case kAPSEventTypeTouchMoved :	uPos = 1;	break;
		case kAPSEventTypeTouchEnded :	uPos = 2;	break;
 
        default :	return KD_NULL;
    }
    
	std::vector<APSResourceArray*>*  pLinkers = m_pTouchEventLinkers;
    if ( !( *pLinkers ) [ uPos ] ) 
	{
          ( *pLinkers ) [ uPos ] = new APSResourceArray ( this );
    }
    return ( *pLinkers ) [ uPos ];
}

APSResourceArray* APSGraphic::getLinkersForEvent ( APSEventType uType ) const 
{
    if ( !m_pTouchEventLinkers )
	{
        return KD_NULL;
    }
    
    KDuint  uPos;
    switch ( uType ) 
	{
        case kAPSEventTypeTouchBegan :	uPos = 0;	break;
        case kAPSEventTypeTouchMoved : 	uPos = 1;	break;
        case kAPSEventTypeTouchEnded :	uPos = 2;	break;
        default :	return KD_NULL;
    }
    
    return ( *m_pTouchEventLinkers ) [ uPos ];
    
}

CCNode* APSGraphic::createNode ( KDvoid )
{
    if ( !m_pNode )
	{
        m_pNode = CCNode::create ( );
        m_pNode->retain ( );
    }
    
    return m_pNode;
}

KDbool APSGraphic::initNode ( KDvoid )
{
    m_pNode->setPosition	( this->getPosition	( ) );    
    m_pNode->setContentSize ( this->getSize		( ) );    
    m_pNode->setAnchorPoint ( this->getAnchor	( ) );
    m_pNode->setScaleX		( this->getScale	( ).x );
    m_pNode->setScaleY		( this->getScale	( ).y );
    m_pNode->setRotation	( this->getRotation	( ) );
    m_pNode->setVisible		( this->getVisible	( ) );

    // We assume that this APSGraphic object is APSSymbol's child or symbol itself, and symbol calls updateAccOpacity() at the end of initialization. If this graphic is used solely,  updateAccOpacity() should be called for this object.
    this->m_uRunningOpacity = this->m_uOpacity;
    
    return KD_TRUE;
}

CCAffineTransform APSGraphic::getNodeToSymbolTransform ( KDvoid )
{
    CCNode*  pNode = this->getNode ( );
    
    CCAffineTransform  t = pNode->nodeToParentTransform ( );
    APSGraphic*  pGraphic = this->getParent ( );
    while ( pGraphic != this->getParentSymbol ( ) )
	{
        pNode = pGraphic->getNode ( );
        t = CCAffineTransformConcat ( t, pNode->nodeToParentTransform ( ) );
        
        pGraphic = pGraphic->getParent ( );
    }
    
    return t;
}

CCAffineTransform APSGraphic::getSymbolToNodeTransform ( KDvoid )
{
	CCNode*  pNode = this->getNode ( );
    
    CCAffineTransform  t = pNode->parentToNodeTransform ( );
    APSGraphic*  pGraphic = this->getParent ( );
    while ( pGraphic != this->getParentSymbol ( ) )
	{
        pNode = pGraphic->getNode ( );
        t = CCAffineTransformConcat ( pNode->parentToNodeTransform ( ), t );
        
        pGraphic = pGraphic->getParent ( );
    }
    
    return t;
}

CCPoint APSGraphic::convertPositionFromParentNode ( const CCPoint& tPosition )
{
    return CCPointApplyAffineTransform ( tPosition, this->getNode ( )->parentToNodeTransform ( ) );
}

//
// #### Accessors ####
//
CCNode* APSGraphic::getNode ( KDvoid )
{
    if ( !this->m_pNode )
	{
        this->createNode ( );
        this->initNode   ( );
    }

    return this->m_pNode;
}

APSGraphic* APSGraphic::getParent ( KDvoid )
{
    if ( !m_pParent )
	{
        m_pParent = (APSGraphicGroup*) this->getResource ( this->m_uParentIndex );
    }
    return m_pParent;
}

KDbool APSGraphic::isNodeVisible ( KDvoid )
{
    return this->getAccOpacity ( ) > 0 && this->getNode ( )->isVisible ( );    
}

KDbool APSGraphic::getRetainsTouch ( KDvoid ) const 
{
    return m_uTouchOption & kAPSTouchOptionRetain;
}

KDvoid APSGraphic::setRetainsTouch ( KDbool bRetainsTouch )
{
    if ( bRetainsTouch )
	{
        m_uTouchOption |= kAPSTouchOptionRetain;
    } 
	else 
	{
        m_uTouchOption &= ~ kAPSTouchOptionRetain;
    }
}

KDbool APSGraphic::getAcceptsTouchMoveEnter ( KDvoid ) const
{
	return ( m_uTouchOption & kAPSTouchOptionAcceptMoveEnter ) ? KD_TRUE : KD_FALSE;
}

KDvoid APSGraphic::setAcceptsTouchMoveEnter ( KDbool bAcceptsMoveEnter )
{
    if ( bAcceptsMoveEnter ) 
	{
        m_uTouchOption |= kAPSTouchOptionAcceptMoveEnter;
    } 
	else 
	{
        m_uTouchOption &= ~ kAPSTouchOptionAcceptMoveEnter;
    }
}

KDbool APSGraphic::getAcceptsTouchOnlyVisible ( KDvoid ) const
{
	return ( m_uTouchOption & kAPSTouchOptionAcceptsOnlyVisible ) ? KD_TRUE : KD_FALSE;
}

KDvoid APSGraphic::setAcceptsTouchOnlyVisible ( KDbool bAcceptsTouchOnlyVisible )
{
    if ( bAcceptsTouchOnlyVisible )
	{
        m_uTouchOption |= kAPSTouchOptionAcceptsOnlyVisible;
    }
	else
	{
        m_uTouchOption &= ~ kAPSTouchOptionAcceptsOnlyVisible;
    }
}

NS_APS_END

