/* --------------------------------------------------------------------------
 *
 *      File            Ch7_LuaDecisionTree.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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
#include "Ch7_LuaDecisionTree.h"

enum 
{
	TAG_BL_BG					= 0,
	TAG_BL_DOOR					= 1,
	TAG_BL_GUY_1				= 2,
	TAG_BL_GUY_2				= 3,
	TAG_BL_LOUIE				= 4,
	TAG_BL_OFFICER				= 5,
	TAG_BL_YOUR_GUN				= 6,
	TAG_BL_GUN_POINTED_AT_YOU	= 7,
};

// Callback pointer
Ch7_LuaDecisionTree*			l_pRecipe = KD_NULL;

// Static C functions
static KDint ldtDesc ( lua_State* l )
{
	l_pRecipe->desc ( lua_tostring ( l, 1 ) );
	l_pRecipe->m_pScript->YieldPause ( );
	return ( lua_yield ( l, 0 ) );
}
static KDint ldtAnim ( lua_State* l )
{
	l_pRecipe->anim ( lua_tostring ( l, 1 ) );
	return 0;
}
static KDint ldtDialog ( lua_State* l )
{
	l_pRecipe->dialog ( lua_tostring ( l, 1 ) );
	l_pRecipe->m_pScript->YieldPause ( );
	return ( lua_yield ( l, 0 ) );
}
static KDint ldtLogic ( lua_State* l )
{
	KDint	nNum = l_pRecipe->logic ( lua_tostring ( l, 1 ) );
	lua_pushnumber ( l, nNum );
	return 1;
}
static KDint ldtDialogOption ( lua_State* l )
{
	l_pRecipe->dialogOption ( lua_tostring ( l, 1 ) );
	return 0;
}
static KDint ldtActionOption ( lua_State* l )
{
	l_pRecipe->actionOption ( lua_tostring ( l, 1 ) );
	return 0;
}

static KDint ldtGetResponse ( lua_State* l )
{
	lua_pushnumber ( l, l_pRecipe->m_nResponse );		// Push a number onto the stack
	return 1;											// We are returning 1 result
}

static KDint ldtPresentOptions ( lua_State* l )
{
	l_pRecipe->presentOptions ( );
	l_pRecipe->m_pScript->YieldPause ( );
	return ( lua_yield ( l, 0 ) );
}

KDbool Ch7_LuaDecisionTree::init ( KDvoid )
{	
	// Superclass initialization
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Initial variables
	m_nResponse			= 0;
	m_sText				= "";
	m_sBufferString		= "";
	m_fTimeElapsed		= 0;
	m_bDialogForward	= KD_FALSE;
	m_bWrapNext			= KD_FALSE;
	m_nOptionsOnScreen	= 0;
	m_bCanMoveForward	= KD_FALSE;
	m_bGunsDown			= KD_FALSE;
	
	// Add sprite plist file
	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "big_louie.plist" );
	
	// Add sprite frames
	this->addSpriteFrame ( "bl_bg.png"					, 0, TAG_BL_BG			, KD_TRUE  );
	this->addSpriteFrame ( "bl_door.png"				, 1, TAG_BL_DOOR		, KD_TRUE  );
	this->addSpriteFrame ( "bl_officer.png"				, 2, TAG_BL_OFFICER		, KD_FALSE ); 
	this->addSpriteFrame ( "bl_louie.png"				, 3, TAG_BL_LOUIE		, KD_TRUE  );
	this->addSpriteFrame ( "bl_guy_1.png"				, 3, TAG_BL_GUY_1		, KD_FALSE ); 
	this->addSpriteFrame ( "bl_guy_2.png"				, 2, TAG_BL_GUY_2		, KD_FALSE ); 
	this->addSpriteFrame ( "bl_your_gun.png"			, 4, TAG_BL_YOUR_GUN	, KD_FALSE ); 
	this->addSpriteFile  ( "bl_gun_pointed_at_you.png"	, 4, TAG_BL_GUN_POINTED_AT_YOU, KD_FALSE ); 
		
	// Options Node
	m_pOptionsNode = CCNode::create ( );
	m_pOptionsNode->setPosition ( ccp ( 0, 0 ) );
	m_pOptionsNode->setVisible ( KD_FALSE );
	this->addChild ( m_pOptionsNode, 6 );
			
	// Text label
	m_pTextLabel = CCLabelBMFont::create ( "", "eurostile_30.fnt" );
	m_pTextLabel->setPosition ( ccp ( 10, 115 ) );
	m_pTextLabel->setScale ( 0.65f );
	m_pTextLabel->setColor ( ccc3 ( 255, 255, 255 ) );
	m_pTextLabel->setAnchorPoint ( ccp ( 0, 1 ) );
	this->addChild ( m_pTextLabel, 6 );

	// Dialog background image
	CCSprite*	pDialogBg = CCSprite::create ( "dialog_bg.png" );
	pDialogBg->setPosition ( ccp ( 240, 61.5f ) );
	this->addChild ( pDialogBg, 5 );

	// Set callback pointer
	l_pRecipe = this;

	// Lua initialization
	m_pLua = new mcLuaManager;
	
	//Lua function wrapper library
	static const luaL_reg	aScriptLib [ ] = 
	{
		{ "desc"			, ldtDesc			},	
		{ "anim"			, ldtAnim			},
		{ "dialog"			, ldtDialog			},
		{ "logic"			, ldtLogic			},
		{ "dialogOption"	, ldtDialogOption	},
		{ "actionOption"	, ldtActionOption	},	
		{ "getResponse"		, ldtGetResponse	},
		{ "presentOptions"	, ldtPresentOptions },
		{ KD_NULL			, KD_NULL			}
	};
	m_pLua->LuaOpenLibrary ( "scene", aScriptLib );

	// Open Lua script
	m_pScript = m_pLua->CreateScript ( );
	KDchar*		szPath = xmGetNativePath ( "/res/Data/decision_tree.lua", KD_NULL );
	m_pScript->LoadFile ( szPath );
	kdFree ( szPath );

	// Set initial update method counter
	m_pLua->Update ( 0 );
	
	// Schedule step method
	this->schedule ( schedule_selector ( Ch7_LuaDecisionTree::step ) );

	return KD_TRUE;
}

KDvoid Ch7_LuaDecisionTree::onExit ( KDvoid )
{
	m_pLua->DestroyScript ( m_pScript );

	CC_SAFE_DELETE ( m_pLua );

	Recipe::onExit ( );
}

/// Add sprite frame helper method 
KDvoid Ch7_LuaDecisionTree::addSpriteFrame ( const KDchar* szName, KDint nZOrder, KDint nTag, KDbool bVisible )
{
	CCSprite*	pSprite = CCSprite::createWithSpriteFrameName ( szName ); 
	pSprite->setPosition ( ccp ( 240, 160 ) );
	pSprite->setVisible ( bVisible );
	this->addChild ( pSprite, nZOrder, nTag );
}

/// Add sprite file helper method 
KDvoid Ch7_LuaDecisionTree::addSpriteFile ( const KDchar* szName, KDint nZOrder, KDint nTag, KDbool bVisible )
{
	CCSprite*	pSprite = CCSprite::create ( szName );
	pSprite->setPosition ( ccp ( 240, 160 ) );
	pSprite->setVisible ( bVisible );
	this->addChild ( pSprite, nZOrder, nTag );
}

KDvoid Ch7_LuaDecisionTree::step ( KDfloat fDelta )
{
	// Update Lua script runner
	m_pLua->Update ( fDelta );
	
	// Increment elapsed time
	m_fTimeElapsed += fDelta;
		
	// If enough time has passed and there is still data in the buffer, we show another character
	if ( m_fTimeElapsed > 0.002f && m_sBufferString.size ( ) > 0 ) 
	{
		// Reset elapsed time
		m_fTimeElapsed = 0.0f;
	
		// Add next character		
		m_sText = ccszf ( "%s%c", m_sText.c_str ( ), m_sBufferString [ 0 ] );
	
		// Text wrapping
		if ( m_sText.size ( ) > 0 && m_sText.size ( ) % 40 == 0 )
		{
			m_bWrapNext = KD_TRUE;
		}
			
		// Wrap at next word break
		if ( m_bWrapNext && ( !( m_sText.substr ( m_sText.size ( ) - 1 ) ).compare ( " " ) ) )
		{
			m_sText += "\n";
			m_bWrapNext = KD_FALSE;
		}		
	
		// Chop off last letter in buffer
		if ( m_sBufferString.size ( ) > 1 )
		{
			m_sBufferString = m_sBufferString.substr ( 1 );
		}
		else
		{
			m_sBufferString = "";
		}

		// Finally, set text label
		m_pTextLabel->setString ( m_sText.c_str ( ) );		
	}
	else
	{
		// Move dialog forward as necessary
		if ( m_sBufferString.size ( ) == 0 )
		{
			if ( m_bDialogForward )
			{
				m_bDialogForward = KD_FALSE;
			}
			else
			{
				m_bCanMoveForward = KD_TRUE;
			}
		}		
	}
}

/// Show text callback 
KDvoid Ch7_LuaDecisionTree::showText ( const KDchar* szStr )
{
	m_sText = "";
	m_sBufferString = szStr;
}

/// Description callback 
KDvoid Ch7_LuaDecisionTree::desc ( const KDchar* szStr )
{
	this->showText ( ccszf ( "[%s]", szStr ) );
}

/// Animation callback 
KDvoid Ch7_LuaDecisionTree::anim ( const KDchar* szStr )
{
	if ( !kdStrcmp ( szStr, "Open door" ) )
	{
		this->getChildByTag ( TAG_BL_DOOR )->setVisible ( KD_FALSE );
	}
	else if ( !kdStrcmp ( szStr, "Enter officer" ) )
	{
		this->getChildByTag ( TAG_BL_OFFICER )->setVisible ( KD_TRUE );
	}
	else if ( !kdStrcmp ( szStr, "Pull guns" ) )
	{
		this->getChildByTag ( TAG_BL_YOUR_GUN )->setVisible ( KD_TRUE );
		this->getChildByTag ( TAG_BL_GUY_1 )->setVisible ( KD_TRUE );
		this->getChildByTag ( TAG_BL_GUY_2 )->setVisible ( KD_TRUE );
	}
	else if ( !kdStrcmp ( szStr, "Louie looks away" ) )
	{
		CCSprite*	pSprite = (CCSprite*) this->getChildByTag ( TAG_BL_LOUIE );
		pSprite->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "bl_louie_look_away.png" ) );
	}
 	else if ( !kdStrcmp ( szStr, "Louie looks at you" ) )
	{
		CCSprite*	pSprite = (CCSprite*) this->getChildByTag ( TAG_BL_LOUIE );
		pSprite->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "bl_louie.png" ) );
	}
	else if ( !kdStrcmp ( szStr, "Louie scowls" ) )
	{
		CCSprite*	pSprite = (CCSprite*) this->getChildByTag ( TAG_BL_LOUIE );
		pSprite->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "bl_louie_angry.png" ) );
	}
	else if ( !kdStrcmp ( szStr, "Officer shocked" ) )
	{
		CCSprite*	pSprite = (CCSprite*) this->getChildByTag ( TAG_BL_OFFICER );
		pSprite->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "bl_officer_shocked.png" ) );
	}
	else if ( !kdStrcmp ( szStr, "Put guns down" ) )
	{
		CCSprite*	pSprite1 = (CCSprite*) this->getChildByTag ( TAG_BL_GUY_1 );
		pSprite1->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "bl_guy_1_no_gun.png" ) );

		CCSprite*	pSprite2 = (CCSprite*) this->getChildByTag ( TAG_BL_GUY_2 );
		pSprite2->setDisplayFrame ( CCSpriteFrameCache::sharedSpriteFrameCache ( )->spriteFrameByName ( "bl_guy_2_no_gun.png" ) );
	}
	else if ( !kdStrcmp ( szStr, "Pull gun on Louie" ) )
	{
		CCSprite*	pSprite = (CCSprite*) this->getChildByTag ( TAG_BL_YOUR_GUN );
		pSprite->setPosition ( ccp ( 140, 160 ) );
	}
	else if ( !kdStrcmp ( szStr, "Pull gun on men" ) )
	{
		CCSprite*	pSprite = (CCSprite*) this->getChildByTag ( TAG_BL_YOUR_GUN );
		pSprite->setFlipX ( KD_TRUE );
		pSprite->setPosition ( ccp ( 340, 160 ) );		
	}
	else if ( !kdStrcmp ( szStr, "Gun pointed at you" ) )
	{
		CCSprite*	pSprite = (CCSprite*) this->getChildByTag ( TAG_BL_GUN_POINTED_AT_YOU );
		pSprite->setVisible ( KD_TRUE );
	}
}

/// Dialog callback 
KDvoid Ch7_LuaDecisionTree::dialog ( const KDchar* szStr )
{
	this->showText ( szStr );
}

/// Logic callback 
KDint Ch7_LuaDecisionTree::logic ( const KDchar* szStr )
{
	KDint	nNum = 0;
		
	if ( !kdStrcmp ( szStr, "Put guns down" ) )
	{
		m_bGunsDown = KD_TRUE;
	}
	else if ( !kdStrcmp ( szStr, "Are guns down?" ) )
	{
		if ( m_bGunsDown )
		{
			nNum = 1;
		}
		else
		{
			nNum = 0;
		}
	}
	else if ( !kdStrcmp ( szStr, "You win" ) )
	{
		this->showMessage ( "You WIN!!" );
	}
	
	return nNum;
}
 
/// Dialog option callback 
KDvoid Ch7_LuaDecisionTree::dialogOption ( const KDchar* szStr )
{
	CCMenuItemFont::setFontName ( "Arial.ttf" );
	CCMenuItemFont::setFontSize ( 16 );

	m_nOptionsOnScreen += 1;
	
	// Add dialog option to screen
	CCMenuItemFont*		pOptionItem = CCMenuItemFont::create ( ccszf ( "\"%s\"", szStr ), this, menu_selector ( Ch7_LuaDecisionTree::selectOption ) );
	pOptionItem->setTag ( m_nOptionsOnScreen );
	pOptionItem->setPosition ( ccp ( 10, 135 - m_nOptionsOnScreen * 20 ) );
	pOptionItem->setAnchorPoint ( ccp ( 0, 1 ) );

	CCMenu*				pMenu = CCMenu::create ( pOptionItem, KD_NULL );
	pMenu->setPosition ( ccp ( 0, 0 ) );
	m_pOptionsNode->addChild ( pMenu );	
}

/// Action option callback 
KDvoid Ch7_LuaDecisionTree::actionOption ( const KDchar* szStr )
{
	CCMenuItemFont::setFontName ( "Arial.ttf" );
	CCMenuItemFont::setFontSize ( 16 );
	
	m_nOptionsOnScreen += 1;
	
	// Add action option to screen
	CCMenuItemFont*		pOptionItem = CCMenuItemFont::create ( ccszf ( "[%s]", szStr ), this, menu_selector ( Ch7_LuaDecisionTree::selectOption ) );
	pOptionItem->setTag ( m_nOptionsOnScreen );
	pOptionItem->setPosition ( ccp ( 10, 135 - m_nOptionsOnScreen * 20 ) );
	pOptionItem->setAnchorPoint ( ccp ( 0, 1 ) );
	
	CCMenu*				pMenu = CCMenu::create ( pOptionItem, KD_NULL );
	pMenu->setPosition ( ccp ( 0, 0 ) );
	m_pOptionsNode->addChild ( pMenu );	
}

/// Present options callback 
KDvoid Ch7_LuaDecisionTree::presentOptions ( KDvoid )
{
	m_sText = "";
	m_pTextLabel->setString ( m_sText.c_str ( ) );
	m_pOptionsNode->setVisible ( KD_TRUE );
}

/// Select option callback 
KDvoid Ch7_LuaDecisionTree::selectOption ( CCObject* pSender )
{	
	CCMenuItemFont*		pItem = (CCMenuItemFont*) pSender;
	m_nResponse = pItem->getTag ( );
	
	// Remove all children
	CCObject*		pObject;
	CCARRAY_FOREACH ( m_pOptionsNode->getChildren ( ), pObject )
	{
		CCNode*		pNode = (CCNode*) pObject;
		m_pOptionsNode->removeChild ( pNode, KD_TRUE ); 
	}
	this->removeChild ( m_pOptionsNode, KD_TRUE );
	
	// Re-add optionsNode
	m_pOptionsNode = CCNode::create ( );
	m_pOptionsNode->setPosition ( ccp ( 0, 0 ) );
	m_pOptionsNode->setVisible ( KD_FALSE );
	this->addChild ( m_pOptionsNode, 6 );
	
	m_nOptionsOnScreen = 0;
	
	// Resume the script
	m_pScript->YieldResume ( );
}

KDvoid Ch7_LuaDecisionTree::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	CCPoint		tPoint = pTouch->getLocation ( );

	// Move the dialog forward
	if ( tPoint.y < 123.0f && m_bCanMoveForward )
	{
		m_sBufferString = "";
		m_bDialogForward = KD_TRUE;
		m_bWrapNext = KD_FALSE;
		m_pScript->YieldResume ( );
		m_bCanMoveForward = KD_FALSE;
	}
}