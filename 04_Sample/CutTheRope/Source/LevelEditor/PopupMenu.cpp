/* --------------------------------------------------------------------------
 *
 *      File            PopupMenu.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2013      guanghui on 5/11/13. All rights reserved.
 *
 *         https://github.com/andyque/CutTheRope-x.git
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
#include "PopupMenu.h"

PopupMenu::PopupMenu ( CCNode* parent )
{
    CCSprite*			pPineappleSprite		 = CCSprite::create ( "pineappleitem.png" );
    CCSprite*			pPineappleSpriteSelected = CCSprite::create ( "pineappleitem.png" );
    pPineappleSpriteSelected->setColor ( ccc3 ( 100, 0, 0 ) );
    
	CCMenuItemSprite*	pPineappleItem		= CCMenuItemSprite::create ( pPineappleSprite, pPineappleSpriteSelected, KD_NULL, this, menu_selector ( PopupMenu::createPineapple ) );
    
    CCSprite*			pRopeSprite			= CCSprite::create ( "ropeitem.png" );
    CCSprite*			pRopeSpriteSelected	= CCSprite::create ( "ropeitem.png" );
    CCSprite*			pRopeSprite3		= CCSprite::create ( "ropeitem.png" );
    pRopeSpriteSelected	->setColor ( ccc3 ( 100, 0, 0 ) );
    pRopeSprite3		->setColor ( ccc3 ( 100, 100, 100 ) );

    m_pRopeItem		= CCMenuItemSprite::create ( pRopeSprite, pRopeSpriteSelected, pRopeSprite3, this, menu_selector ( PopupMenu::createRope ) );
    m_pMenu			= CCMenu::create ( pPineappleItem, m_pRopeItem, NULL );
    m_pBackground	= CCSprite::create ( "menu.png" );
    m_pBackground->addChild ( m_pMenu, 150 );
    this->addChild ( m_pBackground );
    
    parent->addChild ( this, 1000 );
    this->setMenuEnabled ( KD_FALSE );
}

PopupMenu::~PopupMenu ( KDvoid )
{
    
}

KDvoid PopupMenu::setPopupPositioin ( CCPoint tPosition )
{
    m_tTapPosition = tPosition;
    
    // load defaultBackground and use its size to determine whether the popup still fits there

    CCSprite*	pDefaultBackground		= CCSprite::create ( "menu.png" );
    CCSize		tDefaultBackgroundSize	= pDefaultBackground->getContentSize ( );
    KDfloat		fContentScaleFactor		= CCDirector::sharedDirector ( )->getContentScaleFactor ( );
    KDfloat		fPadding				= tDefaultBackgroundSize.cx * 0.1f * fContentScaleFactor;
    m_pMenu->alignItemsHorizontallyWithPadding ( fPadding );
    
    CCPoint		tAnchorPoint			= ccp ( 0.5f, 0.0f );
    CCPoint		tMenuPosition			= ccp ( tDefaultBackgroundSize.cx / 2, tDefaultBackgroundSize.cy * 0.7f );
    
    // Menu horizontal alignment
    CCSize		tWinSize				= CCDirector::sharedDirector ( )->getWinSize ( );

    const KDchar*	szHorizontalAlignment;
    if ( tPosition.x < tDefaultBackgroundSize.cx / 2 )
	{
        // left
        szHorizontalAlignment	= "left";
        tAnchorPoint.x			= 0.0f;
        m_pMenu->alignItemsVerticallyWithPadding ( fPadding );
        tMenuPosition.x			= tDefaultBackgroundSize.cy * 0.7f;
        tMenuPosition.y			= tDefaultBackgroundSize.cx * 0.5f;
    } 
	else if ( tWinSize.cx - tPosition.x < tDefaultBackgroundSize.cx / 2 )
	{
        // right
        szHorizontalAlignment	= "right";
        tAnchorPoint.x			= 1.0f;
        m_pMenu->alignItemsVerticallyWithPadding ( fPadding );
        tMenuPosition.x			= tDefaultBackgroundSize.cy * 0.3f;
        tMenuPosition.y			= tDefaultBackgroundSize.cx * 0.5f;
    } 
	else 
	{
        // center
        szHorizontalAlignment	= "center";
        m_pMenu->alignItemsHorizontallyWithPadding ( fPadding );
    }
    
    // Menu vertical alignment
    const KDchar*	szVerticalAlignment;
    if ( tPosition.y < tDefaultBackgroundSize.cy ) 
	{
        // bottom
        szVerticalAlignment = "bottom";
        if ( kdStrcmp ( szHorizontalAlignment, "center" ) == 0 ) 
		{
            tAnchorPoint.y = 0.0f;
        } 
		else
		{
            tAnchorPoint.y = 0.25f;
        }
    } 
	else if ( tWinSize.cy - tPosition.y < tDefaultBackgroundSize.cy ) 
	{
        // top
        szVerticalAlignment = "top";
        if ( kdStrcmp ( szHorizontalAlignment, "center" ) == 0 )
		{
            tAnchorPoint.y  = 1.0f;
            tMenuPosition.y = tDefaultBackgroundSize.cy * 0.3f;
        } 
		else 
		{
            tAnchorPoint.y = 0.75f;
        }
    }
	else 
	{
        // middle
        szVerticalAlignment = "middle";
        if ( kdStrcmp ( szHorizontalAlignment, "center" ) == 0 ) 
		{
            tAnchorPoint.y = 0.0f;
        }
		else 
		{
            tAnchorPoint.y = 0.5f;
        }
    }
    
    // Draw the menu
    CCString*		pFilesname = CCString::createWithFormat ( "menu-%s-%s.png", szVerticalAlignment, szHorizontalAlignment );
    CCTexture2D*	pTexture   = CCTextureCache::sharedTextureCache ( )->addImage ( pFilesname->getCString ( ) );
    if ( !pTexture ) 
	{
        pTexture = CCTextureCache::sharedTextureCache ( )->addImage ( "menu.png" );
    }
    m_pBackground->setTexture		( pTexture );
    m_pBackground->setTextureRect	( CCRectMake ( 0, 0, pTexture->getContentSize ( ).cx, pTexture->getContentSize ( ).cy ) );       
    m_pBackground->setAnchorPoint	( tAnchorPoint );
    m_pBackground->setPosition		( tPosition );
    m_pBackground->setOpacity		( m_pMenu->getOpacity ( ) );
    
    m_pMenu->setPosition ( tMenuPosition );
}

KDvoid PopupMenu::setMenuEnabled ( KDbool bEnable )
{
	GLubyte		cOpacity = bEnable ? 255 : 0;

    CCObject*	pObject;    
    CCARRAY_FOREACH ( m_pMenu->getChildren ( ), pObject )
    {
        CCMenuItem*	 pItem = (CCMenuItem*) pObject;
        pItem->setEnabled ( bEnable );
    }
    
    m_bIsEnabled = bEnable;
    
    m_pBackground->setOpacity ( cOpacity );
    m_pMenu		 ->setOpacity ( cOpacity );
}

KDvoid PopupMenu::setRopeItemEnabled ( KDbool bEnable )
{
    m_pRopeItem->setEnabled ( m_bIsEnabled );
}

KDbool PopupMenu::isEnabled ( KDvoid )
{
    return m_bIsEnabled;
}

KDvoid PopupMenu::createPineapple ( CCObject* pSender )
{
    m_pDelegate->createPineappleAt ( m_tTapPosition );
}

KDvoid PopupMenu::createRope ( CCObject* pSender )
{
    m_pDelegate->createRopeAt ( m_tTapPosition );
}