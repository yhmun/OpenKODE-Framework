/* --------------------------------------------------------------------------
 *
 *      File            CCControlButton.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Stewart Hamilton-Arrandale
 *      Copyright (c) 2012      Yannick Loriot ( Modified )
 *      Copyright (c) 2012      Angus C        ( Converted to c++ )
 *
 *         http://www.cocos2d-x.org   
 *         http://creativewax.co.uk
 *         http://yannickloriot.com
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
#include "extensions/CCControl/CCControlButton.h"
#include "actions/CCActionInterval.h"
#include "cocoa/CCString.h"
#include "label_nodes/CCLabelBMFont.h"
#include "label_nodes/CCLabelTTF.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

enum
{
	kZoomActionTag = 0xCCCB0001,
};

CCControlButton::CCControlButton ( KDvoid )
{
	m_pCurrentTitle					 = KD_NULL;
	m_tCurrentTitleColor			 = ccWHITE;
	m_bDoesAdjustBackgroundImage	 = KD_FALSE;
	m_pTitleLabel					 = KD_NULL;
	m_pBackgroundSprite				 = KD_NULL;
	m_bZoomOnTouchDown				 = KD_FALSE;
	m_bIsPushed						 = KD_FALSE;
	m_bParentInited					 = KD_FALSE;
	m_pTitleDispatchTable			 = KD_NULL;
	m_pTitleColorDispatchTable		 = KD_NULL;
	m_pTitleLabelDispatchTable		 = KD_NULL;
	m_pBackgroundSpriteDispatchTable = KD_NULL;
	m_nMarginH						 = CCControlButtonMarginLR;
	m_nMarginV						 = CCControlButtonMarginTB;
}

CCControlButton::~CCControlButton ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pCurrentTitle );
    CC_SAFE_RELEASE ( m_pTitleLabel );
	CC_SAFE_RELEASE ( m_pBackgroundSpriteDispatchTable );
	CC_SAFE_RELEASE ( m_pTitleLabelDispatchTable );
	CC_SAFE_RELEASE ( m_pTitleColorDispatchTable );
	CC_SAFE_RELEASE ( m_pTitleDispatchTable );
	CC_SAFE_RELEASE ( m_pBackgroundSprite );
}

// initialisers

KDbool CCControlButton::init ( KDvoid )
{
    return this->initWithLabelAndBackgroundSprite ( CCLabelTTF::create ( "", "Helvetica", 12 ), CCScale9Sprite::create ( ) );
}

KDbool CCControlButton::initWithLabelAndBackgroundSprite ( CCNode* pNode, CCScale9Sprite* pBackgroundSprite )
{
	if ( CCControl::init ( ) )
	{
		CCAssert ( pNode != KD_NULL, "Label must not be nil." );
		CCLabelProtocol*  pLabel = dynamic_cast<CCLabelProtocol*> ( pNode );
		CCRGBAProtocol*   pRgbaLabel = dynamic_cast<CCRGBAProtocol*> ( pNode );
		CCAssert ( pBackgroundSprite != NULL, "Background sprite must not be nil." );
        CCAssert ( pLabel != NULL || pRgbaLabel!=NULL || pBackgroundSprite != NULL, "" );      
		
		m_bParentInited = KD_TRUE;

		// Initialize the button state tables
        this->setTitleDispatchTable            ( CCDictionary::create ( ) );
        this->setTitleColorDispatchTable	   ( CCDictionary::create ( ) );
        this->setTitleLabelDispatchTable	   ( CCDictionary::create ( ) );
        this->setBackgroundSpriteDispatchTable ( CCDictionary::create ( ) );

		setTouchEnabled ( KD_TRUE );
		m_bIsPushed			= KD_FALSE;
		m_bZoomOnTouchDown	= KD_TRUE;
		m_pCurrentTitle		= KD_NULL;

        // Adjust the background image by default
		setAdjustBackgroundImage ( KD_TRUE );
        setPreferredSize ( CCSizeZero );

		// Zooming button by default
		m_bZoomOnTouchDown = KD_TRUE;

		// Set the default anchor point
		ignoreAnchorPointForPosition ( KD_FALSE );
		setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
        
        // Set the nodes
		setTitleLabel ( pNode );
        setBackgroundSprite ( pBackgroundSprite );       

        // Set the default color and opacity
		setColor ( ccc3 ( 255, 255, 255 ) );
		setOpacity ( 255 );
		setOpacityModifyRGB ( KD_TRUE );
        
        // Initialize the dispatch table
		
		CCString*  pTempString = CCString::create ( pLabel->getString ( ) );
		
		setTitleForState ( pTempString, kCCControlStateNormal );
		setTitleColorForState ( pRgbaLabel->getColor ( ), kCCControlStateNormal );
		setTitleLabelForState ( pNode, kCCControlStateNormal );
		setBackgroundSpriteForState ( pBackgroundSprite, kCCControlStateNormal );
		
		setLabelAnchorPoint ( ccp ( 0.5f, 0.5f ) );

        // Layout update
		needsLayout ( );

    	return KD_TRUE;
	}
	
	return KD_FALSE;
}

CCControlButton* CCControlButton::create ( CCNode* pLabel, CCScale9Sprite* pBackgroundSprite )
{
	CCControlButton*  pRet = new CCControlButton ( );

	if ( pRet && pRet->initWithLabelAndBackgroundSprite ( pLabel, pBackgroundSprite ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCControlButton::initWithTitleAndFontNameAndFontSize ( std::string sTitle, const KDchar* szFontName, KDfloat fFontSize )
{
	CCLabelTTF*  pLabel = CCLabelTTF::create ( sTitle.c_str ( ), szFontName, fFontSize );
	return initWithLabelAndBackgroundSprite ( pLabel, (CCScale9Sprite*) CCScale9Sprite::create ( ) );
}

CCControlButton* CCControlButton::create ( std::string sTitle, const KDchar* szFontName, KDfloat fFontSize )
{
	CCControlButton*  pRet = new CCControlButton ( );

	if ( pRet && pRet->initWithTitleAndFontNameAndFontSize ( sTitle, szFontName, fFontSize ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCControlButton::initWithBackgroundSprite ( CCScale9Sprite* pSprite )
{
	CCLabelTTF*  pLabel = CCLabelTTF::create ( "", "Arial", 30 );
	return initWithLabelAndBackgroundSprite ( pLabel, pSprite );
}

CCControlButton* CCControlButton::create ( CCScale9Sprite* pSprite )
{
	CCControlButton*  pRet = new CCControlButton ( );

	if ( pRet && pRet->initWithBackgroundSprite ( pSprite ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDvoid CCControlButton::setMargins ( KDint nMarginH, KDint nMarginV )
{
	m_nMarginV = nMarginV;
	m_nMarginH = nMarginH;
	needsLayout ( );
}

KDvoid CCControlButton::setEnabled ( KDbool bEnabled )
{
	CCControl::setEnabled ( bEnabled );
	needsLayout ( );
}

KDvoid CCControlButton::setSelected ( KDbool bEnabled )
{
	CCControl::setSelected ( bEnabled );
	needsLayout ( );
}

KDvoid CCControlButton::setHighlighted ( KDbool bEnabled )
{
    if ( bEnabled == KD_TRUE )
    {
        m_eState = kCCControlStateHighlighted;
    }
    else
    {
        m_eState = kCCControlStateNormal;
    }

	CCControl::setHighlighted ( bEnabled );

	CCAction*  pAction = getActionByTag ( kZoomActionTag );
    if ( pAction )
    {
		stopAction ( pAction );        
    }
	needsLayout ( );

	if ( m_bZoomOnTouchDown )
    {
		KDfloat    fScaleValue = ( isHighlighted ( ) && isEnabled ( ) && !isSelected ( ) ) ? 1.1f : 1.0f;
		CCAction*  pZoomAction = CCScaleTo::create ( 0.05f, fScaleValue );
		pZoomAction->setTag ( kZoomActionTag );
		runAction ( pZoomAction );
    }
}

KDvoid CCControlButton::setZoomOnTouchDown ( KDbool bZoomOnTouchDown )
{
    m_bZoomOnTouchDown = bZoomOnTouchDown;
}

KDbool CCControlButton::getZoomOnTouchDown ( KDvoid )
{
    return m_bZoomOnTouchDown;
}

KDvoid CCControlButton::setPreferredSize ( CCSize tSize )
{
    if ( tSize.cx == 0 && tSize.cy == 0 )
    {
        m_bDoesAdjustBackgroundImage = KD_TRUE;
    }
    else
    {
        m_bDoesAdjustBackgroundImage = KD_FALSE;
        CCDictElement*  pItem = KD_NULL;
        CCDICT_FOREACH ( m_pBackgroundSpriteDispatchTable, pItem )
        {
            CCScale9Sprite*  pSprite = (CCScale9Sprite*) pItem->getObject ( );
            pSprite->setPreferredSize ( tSize );
        }               
    }

	m_tPreferredSize = tSize;

    needsLayout ( );
}

CCSize CCControlButton::getPreferredSize ( KDvoid )
{
    return m_tPreferredSize;
}

KDvoid CCControlButton::setAdjustBackgroundImage ( KDbool bAdjustBackgroundImage )
{
	m_bDoesAdjustBackgroundImage = bAdjustBackgroundImage;
	needsLayout ( );
}

KDbool CCControlButton::doesAdjustBackgroundImage ( KDvoid )
{
	return m_bDoesAdjustBackgroundImage;
}

CCPoint CCControlButton::getLabelAnchorPoint ( KDvoid )
{
    return this->m_tLabelAnchorPoint;
}

KDvoid CCControlButton::setLabelAnchorPoint ( CCPoint tLabelAnchorPoint )
{
    this->m_tLabelAnchorPoint = tLabelAnchorPoint;

	if ( m_pTitleLabel != KD_NULL )
    {
        this->m_pTitleLabel->setAnchorPoint ( tLabelAnchorPoint );
    }
}

CCString* CCControlButton::getTitleForState ( CCControlState uState )
{
	if ( m_pTitleDispatchTable != KD_NULL )
    {
        CCString*  pTitle = (CCString*) m_pTitleDispatchTable->objectForKey ( uState );    
        if ( pTitle )
        {
            return pTitle;
        }
        return (CCString*) m_pTitleDispatchTable->objectForKey ( kCCControlStateNormal );
    }

    return CCString::create ( "" );
}

KDvoid CCControlButton::setTitleForState ( CCString* pTitle, CCControlState uState )
{
	m_pTitleDispatchTable->removeObjectForKey ( uState );

    if ( pTitle )
    {
		m_pTitleDispatchTable->setObject ( pTitle, uState );
    }
    
    // If the current state if equal to the given state we update the layout
    if ( getState ( ) == uState )
    {
        needsLayout ( );
    }
}

const ccColor3B CCControlButton::getTitleColorForState ( CCControlState uState )
{
    ccColor3B  tRetColor = ccWHITE;

	do
	{
		CC_BREAK_IF ( KD_NULL == m_pTitleColorDispatchTable );

		CCColor3bObject*  pColorObject = (CCColor3bObject*) m_pTitleColorDispatchTable->objectForKey ( uState );    
		if ( pColorObject )
		{
			tRetColor = pColorObject->m_tValue;
			return tRetColor;
		}

		pColorObject = (CCColor3bObject*) m_pTitleColorDispatchTable->objectForKey ( kCCControlStateNormal );    
		if ( pColorObject )
		{
			tRetColor = pColorObject->m_tValue;
		}

	} while ( 0 );

    return tRetColor;
}

KDvoid CCControlButton::setTitleColorForState ( ccColor3B tColor, CCControlState uState )
{
	m_pTitleColorDispatchTable->objectForKey ( uState ); 
	
	CCColor3bObject*  pColor3bObject = new CCColor3bObject ( tColor );
    pColor3bObject->autorelease ( );

	m_pTitleColorDispatchTable->setObject ( pColor3bObject, uState );
      
    // If the current state if equal to the given state we update the layout
    if ( getState ( ) == uState )
    {
        needsLayout ( );
    }
}

CCNode* CCControlButton::getTitleLabelForState ( CCControlState uState )
{
	CCNode*  pTitleLabel = (CCNode*) m_pTitleLabelDispatchTable->objectForKey ( uState );    
    
	if ( pTitleLabel )
    {
        return pTitleLabel;
    }

    return (CCNode*) m_pTitleLabelDispatchTable->objectForKey ( kCCControlStateNormal );
}

KDvoid CCControlButton::setTitleLabelForState ( CCNode* pTitleLabel, CCControlState uState )
{
	CCNode*  pPreviousLabel = (CCNode*) m_pTitleLabelDispatchTable->objectForKey ( uState );
	if ( pPreviousLabel )
    {
		removeChild ( pPreviousLabel, KD_TRUE );
		m_pTitleLabelDispatchTable->removeObjectForKey ( uState );        
    }

	m_pTitleLabelDispatchTable->setObject ( pTitleLabel, uState );
	pTitleLabel->setVisible ( KD_FALSE );
	pTitleLabel->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	addChild ( pTitleLabel, 1 );

    // If the current state if equal to the given state we update the layout
    if ( getState ( ) == uState )
    {
        needsLayout ( );
    }
}

KDvoid CCControlButton::setTitleTTFForState ( const KDchar* szFntFile, CCControlState uState )
{
    CCString*  pTitle = this->getTitleForState ( uState );

    if ( !pTitle )
	{
		pTitle = CCString::create ( "" );
	}

    this->setTitleLabelForState ( CCLabelTTF::create ( pTitle->getCString ( ), szFntFile, 12 ), uState );
}

const KDchar* CCControlButton::getTitleTTFForState ( CCControlState uState )
{
	CCLabelProtocol*  pLabel = dynamic_cast<CCLabelProtocol*> ( this->getTitleLabelForState ( uState ) );
    CCLabelTTF*    pLabelTTF = dynamic_cast<CCLabelTTF*> ( pLabel );
    if ( pLabelTTF != 0 )
    {
        return pLabelTTF->getFontName ( );
    }
    else
    {
        return "";
    }
}

KDvoid CCControlButton::setTitleTTFSizeForState ( KDfloat fSize, CCControlState uState )
{
    CCLabelProtocol*    pLabel = dynamic_cast<CCLabelProtocol*> ( this->getTitleLabelForState ( uState ) );
    if ( pLabel )
    {
        CCLabelTTF*  pLabelTTF = dynamic_cast<CCLabelTTF*> ( pLabel );
        if ( pLabelTTF != 0 )
        {
            return pLabelTTF->setFontSize ( fSize );
        }
    }
}

KDfloat CCControlButton::getTitleTTFSizeForState ( CCControlState uState )
{
    CCLabelProtocol* pLabel = dynamic_cast<CCLabelProtocol*> ( this->getTitleLabelForState ( uState ) );
    CCLabelTTF*   pLabelTTF = dynamic_cast<CCLabelTTF*> ( pLabel );
    if ( pLabelTTF != 0 )
    {
        return pLabelTTF->getFontSize ( );
    }
    else
    {
        return 0;
    }
}

KDvoid CCControlButton::setTitleBMFontForState ( const KDchar* szFntFile, CCControlState uState )
{
    CCString*  pTitle = this->getTitleForState ( uState );

    if ( !pTitle )
	{
		pTitle = CCString::create ( "" );
	}

    this->setTitleLabelForState ( CCLabelBMFont::create ( pTitle->getCString ( ), szFntFile ), uState );
}

const KDchar* CCControlButton::getTitleBMFontForState ( CCControlState uState )
{
    CCLabelProtocol*      pLabel = dynamic_cast<CCLabelProtocol*> ( this->getTitleLabelForState ( uState ) );
    CCLabelBMFont*  pLabelBMFont = dynamic_cast<CCLabelBMFont*> ( pLabel );
    if ( pLabelBMFont != 0 )
    {
        return pLabelBMFont->getFntFile ( );
    }
    else
    {
        return "";
    }
}


CCScale9Sprite* CCControlButton::getBackgroundSpriteForState ( CCControlState uState )
{
	CCScale9Sprite*  pBackgroundSprite = (CCScale9Sprite*) m_pBackgroundSpriteDispatchTable->objectForKey ( uState );    
    
	if ( pBackgroundSprite )
    {
        return pBackgroundSprite;
    }

    return (CCScale9Sprite*) m_pBackgroundSpriteDispatchTable->objectForKey ( kCCControlStateNormal );
}


KDvoid CCControlButton::setBackgroundSpriteForState ( CCScale9Sprite* pSprite, CCControlState uState )
{
	CCSize  tOldPreferredSize = m_tPreferredSize;

	CCScale9Sprite*  pPreviousBackgroundSprite = (CCScale9Sprite*) m_pBackgroundSpriteDispatchTable->objectForKey ( uState );

	if ( pPreviousBackgroundSprite )
    {
		removeChild ( pPreviousBackgroundSprite, KD_TRUE );
		m_pBackgroundSpriteDispatchTable->removeObjectForKey ( uState );        
    }

	m_pBackgroundSpriteDispatchTable->setObject ( pSprite, uState );
	pSprite->setVisible ( KD_FALSE );
	pSprite->setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
	addChild ( pSprite );

	if ( this->m_tPreferredSize.cx != 0 || this->m_tPreferredSize.cy != 0 )
    {
		if ( tOldPreferredSize.equals ( m_tPreferredSize ) )
        {
            // Force update of preferred size
            pSprite->setPreferredSize ( CCSize ( tOldPreferredSize.cx + 1, tOldPreferredSize.cy + 1 ) );
        }

        pSprite->setPreferredSize ( this->m_tPreferredSize );
    }

    // If the current state if equal to the given state we update the layout
    if ( getState ( ) == uState )
    {
        needsLayout ( );
    }
}

KDvoid CCControlButton::setBackgroundSpriteFrameForState ( CCSpriteFrame* pSpriteFrame, CCControlState uState )
{
    CCScale9Sprite*  pSprite = CCScale9Sprite::createWithSpriteFrame ( pSpriteFrame );
    this->setBackgroundSpriteForState ( pSprite, uState );
}

KDvoid CCControlButton::needsLayout ( KDvoid )
{
	if ( !m_bParentInited )
	{
        return;
    }

	// Hide the background and the label
	if ( m_pTitleLabel != KD_NULL )
	{
        m_pTitleLabel->setVisible ( KD_FALSE );
    }
    if ( m_pBackgroundSprite ) 
	{
        m_pBackgroundSprite->setVisible ( KD_FALSE );
    }
    
	// Update anchor of all labels
    this->setLabelAnchorPoint ( this->m_tLabelAnchorPoint );

    // Update the label to match with the current state
	CC_SAFE_RELEASE ( m_pCurrentTitle );
	m_pCurrentTitle = getTitleForState ( m_eState );
	CC_SAFE_RETAIN  ( m_pCurrentTitle );

	m_tCurrentTitleColor = getTitleColorForState ( m_eState );
	this->setTitleLabel	( getTitleLabelForState ( m_eState ) );
	
	CCLabelProtocol*  pLabel = dynamic_cast<CCLabelProtocol*> ( m_pTitleLabel );
	if ( pLabel && m_pCurrentTitle )
	{
		pLabel->setString ( m_pCurrentTitle->getCString ( ) );
	}

	CCRGBAProtocol*  pRgbaLabel = dynamic_cast<CCRGBAProtocol*> ( m_pTitleLabel );
	if ( pRgbaLabel )
	{
		pRgbaLabel->setColor ( m_tCurrentTitleColor );
	}

	if ( m_pTitleLabel != KD_NULL )
	{
		m_pTitleLabel->setPosition ( ccpMid ( getContentSize ( ) ) );
	}
    
    // Update the background sprite
	this->setBackgroundSprite ( this->getBackgroundSpriteForState ( m_eState ) );
	if ( m_pBackgroundSprite != KD_NULL )
	{
		m_pBackgroundSprite->setPosition ( ccpMid ( getContentSize ( ) ) );
	}

    // Get the title label size
	CCSize  tTitleLabelSize;
	if ( m_pTitleLabel != KD_NULL )
	{
		tTitleLabelSize = m_pTitleLabel->boundingBox ( ).size;
	}
    
    // Adjust the background image if necessary
	if ( m_bDoesAdjustBackgroundImage )
	{
        // Add the margins
		if ( m_pBackgroundSprite != KD_NULL )
		{
			m_pBackgroundSprite->setContentSize ( CCSizeMake ( tTitleLabelSize.width + m_nMarginH * 2, tTitleLabelSize.height + m_nMarginV * 2 ) );
		}
    } 
	else
    {		
		//TODO: should this also have margins if one of the preferred sizes is relaxed?
		if ( m_pBackgroundSprite != KD_NULL )
		{
			CCSize  tPreferredSize = m_pBackgroundSprite->getPreferredSize ( );
			if ( tPreferredSize.cx <= 0 )
			{
				tPreferredSize.cx = tTitleLabelSize.cx;
			}
			if ( tPreferredSize.cy <= 0 )
			{
				tPreferredSize.cy = tTitleLabelSize.cy;
			}
	        
			m_pBackgroundSprite->setContentSize ( tPreferredSize );
		}
    }
    
    // Set the content size
	CCRect  tRectTitle;
	if ( m_pTitleLabel != KD_NULL )
    {
        tRectTitle = m_pTitleLabel->boundingBox ( );
    }

    CCRect  tRectBackground;
    if ( m_pBackgroundSprite != KD_NULL )
    {
        tRectBackground = m_pBackgroundSprite->boundingBox ( );
    }

	CCRect  tMaxRect = CCControlUtils::CCRectUnion ( tRectTitle, tRectBackground );
	setContentSize ( tMaxRect.size );		
    
	if ( m_pTitleLabel != KD_NULL )
	{
		m_pTitleLabel->setPosition ( ccpMid ( getContentSize ( ) ) );
		// Make visible the background and the label
		m_pTitleLabel->setVisible ( KD_TRUE );
	}

	if ( m_pBackgroundSprite != KD_NULL )
	{
		m_pBackgroundSprite->setPosition ( ccpMid ( getContentSize ( ) ) );
		m_pBackgroundSprite->setVisible ( KD_TRUE );
	}
}

KDbool CCControlButton::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	if ( !isTouchInside ( pTouch ) || !isEnabled ( ) || !isVisible ( ) || !hasVisibleParents ( ) )
    {
		return KD_FALSE;
	}
    
	m_bIsPushed = KD_TRUE;
	this->setHighlighted ( KD_TRUE );
	sendActionsForControlEvents ( kCCControlEventTouchDown );
    return KD_TRUE;
}

KDvoid CCControlButton::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{	
	if ( !isEnabled ( ) || !isPushed ( ) || isSelected ( ) )
    {
		if ( isHighlighted ( ) )
        {
            setHighlighted ( KD_FALSE );
			return;
        }
	}
    
	KDbool  bIsTouchMoveInside = isTouchInside ( pTouch );
    if ( bIsTouchMoveInside && !isHighlighted ( ) )
    {
		setHighlighted ( KD_TRUE );
		sendActionsForControlEvents ( kCCControlEventTouchDragEnter );
    }
	else if ( bIsTouchMoveInside && isHighlighted ( ) )
    {
		sendActionsForControlEvents ( kCCControlEventTouchDragInside );
    }
	else if ( !bIsTouchMoveInside && isHighlighted ( ) )
    {
		setHighlighted ( KD_FALSE );
		sendActionsForControlEvents ( kCCControlEventTouchDragExit );        
    }
	else if ( !bIsTouchMoveInside && !isHighlighted ( ) )
    {
		sendActionsForControlEvents ( kCCControlEventTouchDragOutside );        
    }
}

KDvoid CCControlButton::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
    m_bIsPushed = KD_FALSE;
	setHighlighted ( KD_FALSE );
    
    if ( isTouchInside ( pTouch ) )
    {
		sendActionsForControlEvents ( kCCControlEventTouchUpInside );        
    }
	else
    {
		sendActionsForControlEvents ( kCCControlEventTouchUpOutside );        
    }
}

KDvoid CCControlButton::setOpacity ( GLubyte cOpacity )
{
    // XXX fixed me if not correct
    CCControl::setOpacity ( cOpacity );
    
    CCDictElement*  pItem = KD_NULL;
    CCDICT_FOREACH ( m_pBackgroundSpriteDispatchTable, pItem )
    {
        CCScale9Sprite*  pSprite = (CCScale9Sprite*) pItem->getObject ( );
        pSprite->setOpacity ( cOpacity );
    }
}

GLubyte CCControlButton::getOpacity ( KDvoid )
{
    return m_cRealOpacity;
}

CCControlButton* CCControlButton::create ( KDvoid )
{
    CCControlButton*  pRet = new CCControlButton ( );

    if ( pRet && pRet->init ( ) )
    {
        pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
    }
    
    return pRet;
}

NS_CC_END