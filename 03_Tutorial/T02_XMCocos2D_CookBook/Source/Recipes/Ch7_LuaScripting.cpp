/* --------------------------------------------------------------------------
 *
 *      File            Ch7_LuaScripting.cpp
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
#include "Ch7_LuaScripting.h"

// Callback pointer
static Ch7_LuaScripting*	l_pLsRecipe = KD_NULL;

// Static append message C function
static KDint lsAppendMessage ( lua_State* l )
{
#if 0
	// Pass lua string into append message method
	l_pLsRecipe->appendMessage ( lua_tostring ( l, 1 ) );
#endif
	return 0;
}

KDbool Ch7_LuaScripting::init ( KDvoid )
{	
	// Superclass initialization and message
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}
	return TRUE;

	this->showMessage ( "Loading Lua script..." );

	// Set callback pointer
	l_pLsRecipe = this;

	// Lua initialization
	m_pLua = new mcLuaManager;
	
	//Lua function wrapper library
	static const luaL_reg	aScriptLib [ ] = 
	{
		{ "appendMessage", lsAppendMessage },		
		{ NULL, NULL }
	};
	m_pLua->LuaOpenLibrary ( "scene", aScriptLib );

	// Open Lua script
	m_pScript = m_pLua->CreateScript ( );

	KDchar*		szPath = xmGetNativePath ( "/res/Data/show_messages.lua", KD_NULL );
	m_pScript->LoadFile ( szPath );
	kdFree ( szPath );

	// Set initial update method counter
	m_pLua->Update ( 0 );
	
	// Schedule step method
	this->schedule ( schedule_selector ( Ch7_LuaScripting::step ) );

	// Resume button
	CCMenuItemFont*		pResumeItem = CCMenuItemFont::create ( "Resume Script", this, menu_selector ( Ch7_LuaScripting::resumeScript ) );
	CCMenu*				pMenu = CCMenu::create ( pResumeItem, KD_NULL );
	this->addChild ( pMenu );

	return KD_TRUE;
}

KDvoid Ch7_LuaScripting::onExit ( KDvoid )
{
	m_pLua->DestroyScript ( m_pScript );

	CC_SAFE_DELETE ( m_pLua );

	Recipe::onExit ( );
}

KDvoid Ch7_LuaScripting::step ( KDfloat fDelta )
{
	// Update Lua script runner
	m_pLua->Update ( fDelta );
}

// Resume script callback 
KDvoid Ch7_LuaScripting::resumeScript ( CCObject* pSender )
{
	m_pScript->YieldResume ( );
}