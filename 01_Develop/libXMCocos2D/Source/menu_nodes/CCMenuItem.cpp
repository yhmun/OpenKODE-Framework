/* -----------------------------------------------------------------------------------
 *
 *      File            CCMenuItem.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
 *
 *         http://www.cocos2d-x.org      
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
#include "menu_nodes/CCMenuItem.h"
#include "actions/CCActionInterval.h"
#include "label_nodes/CCLabelAtlas.h"
#include "label_nodes/CCLabelTTF.h"
#include "sprite_nodes/CCSprite.h"
#include "script_support/CCScriptSupport.h"
#include "support/CCFileUtils.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN
    
static KDuint           l_uFontSize = kCCItemSize;
static std::string      l_sFontPath = "Marker Felt";
static KDbool           l_bFontPathRelease = KD_FALSE;

const  KDuint           kCurrentItem   = 0xc0c05001;
const  KDuint           kZoomActionTag = 0xc0c05002;

const  KDuint           kNormalTag	 = 0x1;
const  KDuint           kSelectedTag = 0x2;
const  KDuint           kDisableTag  = 0x3;


//
// CCMenuItem
//

CCMenuItem* CCMenuItem::create ( KDvoid )
{
    return CCMenuItem::create ( KD_NULL, KD_NULL );
}

CCMenuItem* CCMenuItem::create ( CCObject* pTarget, SEL_MenuHandler pSelector )
{
	CCMenuItem*  pRet = new CCMenuItem ( );

    if ( pRet && pRet->initWithTarget ( pTarget, pSelector ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCMenuItem::initWithTarget ( CCObject* pTarget, SEL_MenuHandler pSelector )
{
	setAnchorPoint ( ccp ( 0.5f, 0.5f ) );

	m_pListener	= pTarget;
	m_pSelector	= pSelector;
	m_bEnabled	= KD_TRUE;
	m_bSelected	= KD_FALSE;
	
	return KD_TRUE;
}

CCMenuItem::CCMenuItem ( KDvoid )
{
    m_bSelected	= KD_FALSE;
    m_bEnabled	= KD_FALSE;      
    m_pListener	= KD_NULL;	
    m_pSelector	= KD_NULL;
	m_nScriptTapHandler = 0;
}

CCMenuItem::~CCMenuItem ( KDvoid )
{
	unregisterScriptTapHandler ( );
}

KDvoid CCMenuItem::selected ( KDvoid )
{
	m_bSelected = KD_TRUE;
}

KDvoid CCMenuItem::unselected ( KDvoid )
{
	m_bSelected = KD_FALSE;
}

KDvoid CCMenuItem::registerScriptTapHandler ( KDint nHandler )
{
	unregisterScriptTapHandler ( );
	
	m_nScriptTapHandler = nHandler;

	LUALOG ( "[LUA] Add CCMenuItem script handler: %d", m_nScriptTapHandler );
}

KDvoid CCMenuItem::unregisterScriptTapHandler ( KDvoid )
{
	if ( m_nScriptTapHandler )
	{
		CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->removeScriptHandler ( m_nScriptTapHandler );

		LUALOG ( "[LUA] Remove CCMenuItem script handler: %d", m_nScriptTapHandler );

		m_nScriptTapHandler = 0;
	}
}

KDvoid CCMenuItem::activate ( KDvoid )
{
	if ( m_bEnabled )
	{
		if ( m_pListener && m_pSelector )
		{
			( m_pListener->*m_pSelector ) ( this );
		}

		if ( kScriptTypeNone != m_eScriptType )
		{
			CCScriptEngineManager::sharedManager ( )->getScriptEngine ( )->executeMenuItemEvent ( this );
		}
	}
}

KDvoid CCMenuItem::setEnabled ( KDbool bEnabled )
{
	m_bEnabled = bEnabled;
}

KDbool CCMenuItem::isEnabled ( KDvoid )
{
	return m_bEnabled;
}

CCRect CCMenuItem::rect ( KDvoid )
{
	return CCRect ( m_tPosition.x - m_tAnchorPointInPoints.x, 
					m_tPosition.y - m_tAnchorPointInPoints.y,
					m_tContentSize.cx,
					m_tContentSize.cy );
}

KDbool CCMenuItem::isSelected ( KDvoid )
{
	return m_bSelected;
}

KDvoid CCMenuItem::setTarget ( CCObject* pTarget, SEL_MenuHandler pSelector )
{
    m_pListener   = pTarget;
    m_pSelector = pSelector;
}


//
// CCMenuItemLabel
//
CCMenuItemLabel::CCMenuItemLabel ( KDvoid )
{
    m_pLabel = KD_NULL;
	m_fOriginalScale = 0.0f;
}

CCMenuItemLabel::~CCMenuItemLabel ( KDvoid )
{

}

const ccColor3B& CCMenuItemLabel::getDisabledColor ( KDvoid )
{
	return m_tDisabledColor;
}

KDvoid CCMenuItemLabel::setDisabledColor ( const ccColor3B& tDisabledColor )
{
	m_tDisabledColor = tDisabledColor;
}

CCNode* CCMenuItemLabel::getLabel ( KDvoid )
{
	return m_pLabel;
}

KDvoid CCMenuItemLabel::setLabel ( CCNode* pLabel )
{
    if ( pLabel )
    {
        addChild ( pLabel );
        pLabel->setAnchorPoint ( ccp ( 0, 0 ) );
        setContentSize ( pLabel->getContentSize ( ) );
    }

    if ( m_pLabel )
    {
        removeChild ( m_pLabel, KD_TRUE );
    }
    
	m_pLabel = pLabel;
}

CCMenuItemLabel* CCMenuItemLabel::create ( CCNode* pLabel, CCObject* pTarget, SEL_MenuHandler pSelector )
{
	CCMenuItemLabel*  pRet = new CCMenuItemLabel ( );

    if ( pRet && pRet->initWithLabel ( pLabel, pTarget, pSelector ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

CCMenuItemLabel* CCMenuItemLabel::create ( CCNode* pLabel )
{
	CCMenuItemLabel*  pRet = new CCMenuItemLabel ( );

    if ( pRet && pRet->initWithLabel ( pLabel, KD_NULL, KD_NULL ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }
    
	return pRet;
}

KDbool CCMenuItemLabel::initWithLabel ( CCNode* pLabel, CCObject* pTarget, SEL_MenuHandler pSelector )
{
	CCMenuItem::initWithTarget ( pTarget, pSelector );

	m_fOriginalScale = 1.0f;
	m_tColorBackup   = ccWHITE;
	setDisabledColor ( ccc3 ( 126, 126, 126 ) );
	this->setLabel ( pLabel );

	setCascadeColorEnabled   ( KD_TRUE );
    setCascadeOpacityEnabled ( KD_TRUE );

	return KD_TRUE;
}

KDvoid CCMenuItemLabel::setString ( const KDchar* szText )
{
	dynamic_cast<CCLabelProtocol*> ( m_pLabel )->setString ( szText );
    
	this->setContentSize ( m_pLabel->getContentSize ( ) );
}

KDvoid CCMenuItemLabel::activate ( KDvoid )
{
	if ( m_bEnabled )
	{
		this->stopAllActions ( );
		this->setScale ( m_fOriginalScale );
        
		CCMenuItem::activate ( );
	}
}

KDvoid CCMenuItemLabel::selected ( KDvoid )
{
	if ( m_bEnabled )
	{
		CCMenuItem::selected ( );

		CCAction*  pAction = getActionByTag ( kZoomActionTag );
		if ( pAction ) 
		{
			this->stopAction ( pAction );
		}
		else
		{
			m_fOriginalScale = this->getScale ( );
		}

		CCAction*  pZoomAction = CCScaleTo::create ( 0.05f, m_fOriginalScale * 1.2f );
   	 	pZoomAction->setTag ( kZoomActionTag );
		this->runAction ( pZoomAction );
	}
}

KDvoid CCMenuItemLabel::unselected ( KDvoid )
{
	// subclass to change the default action
	if ( m_bEnabled )
	{
		CCMenuItem::unselected ( );
		this->stopActionByTag ( kZoomActionTag );
        
		CCAction*  pZoomAction = CCScaleTo::create ( 0.05f, m_fOriginalScale );
		pZoomAction->setTag ( kZoomActionTag );
		this->runAction ( pZoomAction );
	}
}

KDvoid CCMenuItemLabel::setEnabled ( KDbool bEnabled )
{
	if ( m_bEnabled != bEnabled ) 
	{
		if ( bEnabled == KD_FALSE )
		{
			m_tColorBackup = dynamic_cast<CCRGBAProtocol*> ( m_pLabel )->getColor ( );
			dynamic_cast<CCRGBAProtocol*> ( m_pLabel )->setColor ( m_tDisabledColor );
		}
		else
		{
			dynamic_cast<CCRGBAProtocol*> ( m_pLabel )->setColor ( m_tColorBackup );
		}
	}

	CCMenuItem::setEnabled ( bEnabled );
}


//
// CCMenuItemAtlasFont
//

CCMenuItemAtlasFont* CCMenuItemAtlasFont::create ( const KDchar* szText, const KDchar* szAtlasPath, KDint uTileWidth, KDint uTileHeight, KDubyte cStartChar )
{
	return CCMenuItemAtlasFont::create ( szText, szAtlasPath, uTileWidth, uTileHeight, cStartChar, KD_NULL, KD_NULL );
}

CCMenuItemAtlasFont* CCMenuItemAtlasFont::create ( const KDchar* szText, const KDchar* szAtlasPath, KDint uTileWidth, KDint uTileHeight, KDubyte cStartChar, CCObject* pListener, SEL_MenuHandler pSelector )
{
	CCMenuItemAtlasFont*  pRet = new CCMenuItemAtlasFont ( );

    if ( pRet && pRet->initWithString ( szText, szAtlasPath, uTileWidth, uTileHeight, cStartChar, pListener, pSelector ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCMenuItemAtlasFont::initWithString ( const KDchar* szText, const KDchar* szAtlasPath, KDint uTileWidth, KDint uTileHeight, KDubyte cStartChar, CCObject* pListener, SEL_MenuHandler pSelector )
{
	CCAssert ( szText && kdStrlen ( szText ) != 0, "value length must be greater than 0" );

	CCLabelAtlas*  pRet = new CCLabelAtlas ( );
    
    if ( pRet && pRet->initWithString ( szText, szAtlasPath, uTileWidth, uTileHeight, cStartChar ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
        
        return KD_FALSE;
    }

	return CCMenuItemLabel::initWithLabel ( pRet, pListener, pSelector );
}

//
// CCMenuItemFont
//
KDvoid CCMenuItemFont::setFontSize ( KDuint uFontSize )
{
	l_uFontSize = uFontSize;
}

KDuint CCMenuItemFont::getFontSize ( KDvoid )
{
	return l_uFontSize;
}

KDvoid CCMenuItemFont::setFontName ( const KDchar* szFontPath )
{
	if ( l_bFontPathRelease )
	{
		l_sFontPath.clear ( );
	}

	l_sFontPath = CCFileUtils::sharedFileUtils ( )->fullPathForFilename ( szFontPath );
	l_bFontPathRelease = KD_TRUE;
}

const KDchar* CCMenuItemFont::getFontName ( KDvoid )
{
	return l_sFontPath.c_str ( );
}

CCMenuItemFont* CCMenuItemFont::create ( const KDchar* szText, CCObject* pListener, SEL_MenuHandler pSelector )
{
	CCMenuItemFont*  pRet = new CCMenuItemFont ( );

    if ( pRet && pRet->initWithString ( szText, pListener, pSelector ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }
    
	return pRet;
}

CCMenuItemFont* CCMenuItemFont::create ( const KDchar* szText )
{
	CCMenuItemFont*  pRet = new CCMenuItemFont ( );

    if ( pRet && pRet->initWithString ( szText, KD_NULL, KD_NULL ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCMenuItemFont::initWithString ( const KDchar* szText, CCObject* pListener, SEL_MenuHandler pSelector )
{
	CCAssert ( szText != KD_NULL && kdStrlen ( szText ) != 0, "Value length must be greater than 0" );

    m_sFontName = l_sFontPath;
    m_uFontSize = l_uFontSize;

	CCLabelTTF*  pLabel = CCLabelTTF::create ( szText, m_sFontName.c_str ( ), (KDfloat) m_uFontSize );

    if ( pLabel )
    {
        return CCMenuItemLabel::initWithLabel ( pLabel, pListener, pSelector );
    }
    else
    {
        return KD_FALSE;
    }
}

KDvoid CCMenuItemFont::recreateLabel ( KDvoid )
{
	CCLabelTTF*  pLabel = CCLabelTTF::create 
	(
		dynamic_cast<CCLabelProtocol*> ( m_pLabel )->getString ( ), 
		m_sFontName.c_str ( ), (KDfloat) m_uFontSize 
	);
	this->setLabel ( pLabel );
}

KDvoid CCMenuItemFont::setFontSizeObj ( KDuint uFontSize )
{
	m_uFontSize = uFontSize;
	recreateLabel ( );
}

KDuint CCMenuItemFont::fontSizeObj ( KDvoid )
{
	return m_uFontSize;
}

KDvoid CCMenuItemFont::setFontNameObj ( const KDchar* szFontName )
{
	m_sFontName = szFontName;
	recreateLabel ( );
}

const KDchar* CCMenuItemFont::fontNameObj ( KDvoid )
{
	return m_sFontName.c_str ( );
}


//
// CCMenuItemSprite
//
CCMenuItemSprite::CCMenuItemSprite ( KDvoid )
{
    m_pNormalImage   = KD_NULL;
    m_pSelectedImage = KD_NULL;
    m_pDisabledImage = KD_NULL;
}

CCNode* CCMenuItemSprite::getNormalImage ( KDvoid )
{
	return m_pNormalImage;
}

KDvoid CCMenuItemSprite::setNormalImage ( CCNode* pImage )
{
	if ( pImage != m_pNormalImage )
    {
        if ( pImage )
        {
            addChild ( pImage, 0, kNormalTag );
            pImage->setAnchorPoint ( ccp ( 0, 0 ) );
        }

        if ( m_pNormalImage )
        {
            removeChild ( m_pNormalImage, KD_TRUE );
        }

        m_pNormalImage = pImage;
        this->setContentSize ( m_pNormalImage->getContentSize ( ) );
        this->updateImagesVisibility ( );
	}
}

CCNode* CCMenuItemSprite::getSelectedImage ( KDvoid )
{
	return m_pSelectedImage;
}

KDvoid CCMenuItemSprite::setSelectedImage ( CCNode* pImage )
{
	if ( pImage != m_pSelectedImage )
    {
        if ( pImage )
        {
            addChild ( pImage, 0, kSelectedTag );
            pImage->setAnchorPoint ( ccp ( 0, 0 ) );
        }

        if ( m_pSelectedImage )
        {
            removeChild ( m_pSelectedImage, KD_TRUE );
        }

        m_pSelectedImage = pImage;
        this->setContentSize ( m_pSelectedImage->getContentSize ( ) );
        this->updateImagesVisibility ( );
	}
}

CCNode* CCMenuItemSprite::getDisabledImage ( KDvoid )
{
	return m_pDisabledImage;
}

KDvoid CCMenuItemSprite::setDisabledImage ( CCNode* pImage )
{
	if ( pImage != m_pDisabledImage )
    {
        if ( pImage )
        {
            addChild ( pImage, 0, kDisableTag );
            pImage->setAnchorPoint ( ccp ( 0, 0 ) );
        }

        if ( m_pDisabledImage )
        {
            removeChild ( m_pDisabledImage, KD_TRUE );
        }

        m_pDisabledImage = pImage;
        this->setContentSize ( m_pDisabledImage->getContentSize ( ) );
        this->updateImagesVisibility ( );
	}
}

CCMenuItemSprite* CCMenuItemSprite::create ( CCNode* pNormalSprite, CCNode* pSelectedSprite )
{
	return CCMenuItemSprite::create ( pNormalSprite, pSelectedSprite, KD_NULL, KD_NULL, KD_NULL );
}

CCMenuItemSprite* CCMenuItemSprite::create ( CCNode* pNormalSprite, CCNode* pSelectedSprite, CCObject* pListener, SEL_MenuHandler pSelector )
{
	return CCMenuItemSprite::create ( pNormalSprite, pSelectedSprite, KD_NULL, pListener, pSelector );
}

CCMenuItemSprite* CCMenuItemSprite::create ( CCNode* pNormalSprite, CCNode* pSelectedSprite, CCNode* pDisabledSprite, CCObject* pListener, SEL_MenuHandler pSelector )
{
	CCMenuItemSprite*  pRet = new CCMenuItemSprite ( );

    if ( pRet && pRet->initWithNormalSprite ( pNormalSprite, pSelectedSprite, pDisabledSprite, pListener, pSelector ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCMenuItemSprite::initWithNormalSprite ( CCNode* pNormalSprite, CCNode* pSelectedSprite, CCNode* pDisabledSprite, CCObject* pListener, SEL_MenuHandler pSelector )
{
	CCMenuItem::initWithTarget ( pListener, pSelector ); 

    setNormalImage   ( pNormalSprite   );
    setSelectedImage ( pSelectedSprite );
    setDisabledImage ( pDisabledSprite );

	if ( m_pNormalImage )
	{
		this->setContentSize ( m_pNormalImage->getContentSize ( ) );
	}

    setCascadeColorEnabled ( KD_TRUE );
    setCascadeOpacityEnabled ( KD_TRUE );

	return KD_TRUE;
}

KDvoid CCMenuItemSprite::selected ( KDvoid )
{
    CCMenuItem::selected ( );

	if ( m_pNormalImage )
	{
		if ( m_pDisabledImage )
		{
			m_pDisabledImage->setVisible ( KD_FALSE );
		}

		if ( m_pSelectedImage )
		{
			m_pSelectedImage->setVisible ( KD_TRUE );
			m_pNormalImage  ->setVisible ( KD_FALSE );
		}
		else
		{
			m_pNormalImage->setVisible ( KD_TRUE );
		}
	}
}

KDvoid CCMenuItemSprite::unselected ( KDvoid )
{
    CCMenuItem::unselected ( );

	if ( m_pNormalImage )
	{
		m_pNormalImage->setVisible ( KD_TRUE );

		if ( m_pSelectedImage )
		{
			m_pSelectedImage->setVisible ( KD_FALSE );
		}

		if ( m_pDisabledImage )
		{
			m_pDisabledImage->setVisible ( KD_FALSE );
		}
	}
}

KDvoid CCMenuItemSprite::setEnabled ( KDbool bEnabled )
{
    if ( m_bEnabled != bEnabled ) 
    {
        CCMenuItem::setEnabled ( bEnabled );
        this->updateImagesVisibility ( );
    }
}

// Helper 
KDvoid CCMenuItemSprite::updateImagesVisibility ( )
{
    if ( m_bEnabled )
    {
        if (   m_pNormalImage )   m_pNormalImage->setVisible ( KD_TRUE  );
        if ( m_pSelectedImage ) m_pSelectedImage->setVisible ( KD_FALSE );
        if ( m_pDisabledImage ) m_pDisabledImage->setVisible ( KD_FALSE );
    }
    else
    {
        if ( m_pDisabledImage )
        {
            if (   m_pNormalImage )   m_pNormalImage->setVisible ( KD_FALSE );
            if ( m_pSelectedImage ) m_pSelectedImage->setVisible ( KD_FALSE );
            if ( m_pDisabledImage ) m_pDisabledImage->setVisible ( KD_TRUE  );
        }
        else
        {
            if (   m_pNormalImage )   m_pNormalImage->setVisible ( KD_TRUE  );
            if ( m_pSelectedImage ) m_pSelectedImage->setVisible ( KD_FALSE );
            if ( m_pDisabledImage ) m_pDisabledImage->setVisible ( KD_FALSE );
        }
    }
}


//
// CCMenuItemImage
//

CCMenuItemImage* CCMenuItemImage::create ( KDvoid )
{
	CCMenuItemImage*  pRet = new CCMenuItemImage ( );

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

KDbool CCMenuItemImage::init ( KDvoid )
{
    return initWithNormalImage ( KD_NULL, KD_NULL, KD_NULL, KD_NULL, KD_NULL );
}

CCMenuItemImage* CCMenuItemImage::create ( const KDchar* szNormalImage, const KDchar* szSelectedImage )
{
	return CCMenuItemImage::create ( szNormalImage, szSelectedImage, KD_NULL, KD_NULL, KD_NULL );
}

CCMenuItemImage* CCMenuItemImage::create ( const KDchar* szNormalImage, const KDchar* szSelectedImage, CCObject* pListener, SEL_MenuHandler pSelector )
{
	return CCMenuItemImage::create ( szNormalImage, szSelectedImage, KD_NULL, pListener, pSelector );
}

CCMenuItemImage* CCMenuItemImage::create ( const KDchar* szNormalImage, const KDchar* szSelectedImage, const KDchar* szDisabledImage, CCObject* pListener, SEL_MenuHandler pSelector )
{
	CCMenuItemImage*  pRet = new CCMenuItemImage ( );

	if ( pRet && pRet->initWithNormalImage ( szNormalImage, szSelectedImage, szDisabledImage, pListener, pSelector ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCMenuItemImage* CCMenuItemImage::create ( const KDchar* szNormalImage, const KDchar* szSelectedImage, const KDchar* szDisabledImage )
{
	CCMenuItemImage*  pRet = new CCMenuItemImage ( );

	if ( pRet && pRet->initWithNormalImage ( szNormalImage, szSelectedImage, szDisabledImage, KD_NULL, KD_NULL ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool CCMenuItemImage::initWithNormalImage ( const KDchar* szNormalImage, const KDchar* szSelectedImage, const KDchar* szDisabledImage, CCObject* pListener, SEL_MenuHandler pSelector )
{
	CCNode*   pNormalSprite   = KD_NULL;
	CCNode*   pSelectedSprite = KD_NULL;
	CCNode*   pDisabledSprite = KD_NULL;

	if ( szNormalImage )
	{
		pNormalSprite = CCSprite::create ( szNormalImage );
	}

    if ( szSelectedImage )
    {
        pSelectedSprite = CCSprite::create ( szSelectedImage );
    }

	if ( szDisabledImage )
	{
		pDisabledSprite = CCSprite::create ( szDisabledImage );
	}

	return initWithNormalSprite ( pNormalSprite, pSelectedSprite, pDisabledSprite, pListener, pSelector );
}

//
// Setter of sprite frames
//
KDvoid CCMenuItemImage::setNormalSpriteFrame ( CCSpriteFrame* pFrame )
{
    setNormalImage ( CCSprite::createWithSpriteFrame ( pFrame ) );
}

KDvoid CCMenuItemImage::setSelectedSpriteFrame ( CCSpriteFrame* pFrame )
{
    setSelectedImage ( CCSprite::createWithSpriteFrame ( pFrame ) );
}

KDvoid CCMenuItemImage::setDisabledSpriteFrame ( CCSpriteFrame* pFrame )
{
    setDisabledImage ( CCSprite::createWithSpriteFrame ( pFrame ) );
}


//
// MenuItemToggle
//
CCMenuItemToggle::CCMenuItemToggle ( KDvoid )
{
    m_uSelectedIndex = 0;
    m_pSubItems      = KD_NULL;	
}

CCMenuItemToggle::~CCMenuItemToggle ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pSubItems );
}

KDvoid CCMenuItemToggle::setSubItems ( CCArray* pSubItems )
{
	CC_SAFE_RETAIN  ( pSubItems );
	CC_SAFE_RELEASE ( m_pSubItems );
	m_pSubItems = pSubItems;
}

CCArray* CCMenuItemToggle::getSubItems ( KDvoid )
{
	return m_pSubItems;
}

CCMenuItemToggle* CCMenuItemToggle::createWithTarget ( CCObject* pTarget, SEL_MenuHandler pSelector, CCArray* pMenuItems )
{
	CCMenuItemToggle*	pRet = new CCMenuItemToggle ( );
	pRet->CCMenuItem::initWithTarget ( pTarget, pSelector );
	pRet->m_pSubItems = CCArray::create ( );
	pRet->m_pSubItems->retain ( );

	for ( KDuint z = 0; z < pMenuItems->count ( ); z++ )
	{
		CCMenuItem*		pMenuItem = (CCMenuItem*) pMenuItems->objectAtIndex ( z );
		pRet->m_pSubItems->addObject ( pMenuItem );
	}

	pRet->m_uSelectedIndex = KDUINT_MAX;
	pRet->setSelectedIndex ( 0 );
	return pRet;
}

CCMenuItemToggle* CCMenuItemToggle::createWithTarget ( CCObject* pListener, SEL_MenuHandler pSelector, CCMenuItem* pItem, ... )
{
	KDVaListKHR       pArgs;
	KD_VA_START_KHR ( pArgs, pItem );

	CCMenuItemToggle*  pRet = new CCMenuItemToggle ( );
	
    if ( pRet && pRet->initWithTarget ( pListener, pSelector, pItem, pArgs ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	KD_VA_END_KHR ( pArgs );

	return pRet;
}

CCMenuItemToggle* CCMenuItemToggle::create ( KDvoid )
{
    CCMenuItemToggle*  pRet = new CCMenuItemToggle ( );
    pRet->initWithItem ( KD_NULL );
    pRet->autorelease ( );
    return pRet;
}

KDbool CCMenuItemToggle::initWithTarget ( CCObject* pListener, SEL_MenuHandler pSelector, CCMenuItem* pItem, KDVaListKHR pArgs )
{
	CCMenuItem::initWithTarget ( pListener, pSelector );
    
	this->m_pSubItems = CCArray::create ( );
    this->m_pSubItems->retain ( );

	CCMenuItem*  pNemItem = pItem;

	while ( pNemItem ) 
	{
		m_pSubItems->addObject ( pNemItem );
		pNemItem = va_arg ( pArgs, CCMenuItem* );
	}

	m_uSelectedIndex = KDUINT_MAX;
	this->setSelectedIndex ( 0 );

	return KD_TRUE;
}

CCMenuItemToggle* CCMenuItemToggle::create ( CCMenuItem* pItem )
{
	CCMenuItemToggle*  pRet = new CCMenuItemToggle ( );
    
    if ( pRet && pRet->initWithItem ( pItem ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCMenuItemToggle::initWithItem ( CCMenuItem* pItem )
{
	CCMenuItem::initWithTarget ( KD_NULL, KD_NULL );

	setSubItems ( CCArray::create ( ) );

	if ( pItem ) 
	{
		m_pSubItems->addObject ( pItem );
	}

	m_uSelectedIndex = KDUINT_MAX;
	this->setSelectedIndex ( 0 );

	return KD_TRUE;
}

KDvoid CCMenuItemToggle::addSubItem ( CCMenuItem* pItem )
{
	m_pSubItems->addObject ( pItem );
}

KDvoid CCMenuItemToggle::setSelectedIndex ( KDuint uSelectedIndex )
{
	if ( uSelectedIndex != m_uSelectedIndex && m_pSubItems->count ( ) > 0 )
	{
		m_uSelectedIndex = uSelectedIndex;
        CCMenuItem*  pCurrentItem = (CCMenuItem*) getChildByTag ( kCurrentItem );
        if ( pCurrentItem )
        {
            pCurrentItem->removeFromParentAndCleanup ( KD_FALSE );
        }

        CCMenuItem*  pItem = (CCMenuItem*) m_pSubItems->objectAtIndex ( m_uSelectedIndex );		
		this->addChild ( pItem, 0, kCurrentItem );

		const CCSize&  tSize = pItem->getContentSize ( );
		this->setContentSize ( tSize );
		pItem->setPosition ( ccpMid ( tSize ) );
	}
}

KDuint CCMenuItemToggle::getSelectedIndex ( KDvoid )
{
	return m_uSelectedIndex;
}

KDvoid CCMenuItemToggle::selected ( KDvoid )
{
	CCMenuItem::selected ( );
    
	( (CCMenuItem*) ( m_pSubItems->objectAtIndex ( m_uSelectedIndex ) ) )->selected ( );
}

KDvoid CCMenuItemToggle::unselected ( KDvoid )
{
	CCMenuItem::unselected ( );
    
	( (CCMenuItem*) ( m_pSubItems->objectAtIndex ( m_uSelectedIndex ) ) )->unselected ( );
}

KDvoid CCMenuItemToggle::activate ( KDvoid )
{
	// update index
	if ( m_bEnabled ) 
	{
		KDuint  uNewIndex = ( m_uSelectedIndex + 1 ) % m_pSubItems->count ( );
		this->setSelectedIndex ( uNewIndex );
	}

	CCMenuItem::activate ( );
}

KDvoid CCMenuItemToggle::setEnabled ( KDbool bEnabled )
{
	if ( m_bEnabled != bEnabled )
	{
		CCMenuItem::setEnabled ( bEnabled );

		if ( m_pSubItems && m_pSubItems->count ( ) > 0 )
		{
			CCObject*  pObject = KD_NULL;
			CCARRAY_FOREACH ( m_pSubItems, pObject )
			{
				CCMenuItem*  pItem = (CCMenuItem*) pObject;
				pItem->setEnabled ( bEnabled );
			}
		}
	}
}

CCMenuItem* CCMenuItemToggle::selectedItem ( KDvoid )
{
	return (CCMenuItem*) m_pSubItems->objectAtIndex ( m_uSelectedIndex );
}

NS_CC_END