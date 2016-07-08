/* --------------------------------------------------------------------------
 *
 *      File            mcLua.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created by      Robert Grzesek on 11/10/09
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2009      GRZ Software LLC. All rights reserved.
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
#include "mcLua.h"

// Below are the functions used for the yield funtions.

static KDint LuaYieldSeconds ( lua_State* l )
{
	mcLuaScript*	s;
	lua_pushlightuserdata ( l, l );
	lua_gettable ( l, LUA_GLOBALSINDEX );
	s = (mcLuaScript*) lua_touserdata ( l, -1 );
	
	KDfloat			n = (KDfloat) lua_tonumber ( l, 1 );
	s->YieldSeconds ( n );
//	kdPrintf ( "waitSeconds %f\n", n );
	
	return ( lua_yield ( l, 0 ) );
}

static KDint LuaYieldFrames ( lua_State* l )
{
	mcLuaScript*	s;
	lua_pushlightuserdata ( l, l );
	lua_gettable ( l, LUA_GLOBALSINDEX );
	s = (mcLuaScript*) lua_touserdata ( l, -1 );
	
	KDint			f = (KDint) lua_tonumber ( l, 1 );
	s->YieldFrames ( f );
//	kdPrintf ( "waitFrames %d\n", f );
	
	return ( lua_yield ( l, 0 ) );
}

static KDint LuaYieldPause ( lua_State* l )
{
	mcLuaScript*	s;
	lua_pushlightuserdata ( l, l );
	lua_gettable ( l, LUA_GLOBALSINDEX );
	s = (mcLuaScript*) lua_touserdata ( l, -1 );
	
	s->YieldPause ( );
//	kdPrintf ( "pause \n" );
	
	return ( lua_yield ( l, 0 ) );
}

//---------------------------------------------------------------

mcLuaManager::mcLuaManager ( KDvoid )
{
#if 0
	m_pHead		 = KD_NULL;
	m_pMainState = lua_open ( );
	
	// Add functions for waiting
	static const luaL_reg scriptLib [ ] = 
	{
		{ "waitSeconds"	, LuaYieldSeconds	},
		{ "waitFrames"	, LuaYieldFrames	},
		{ "pause"		, LuaYieldPause		},
		{ KD_NULL		, KD_NULL			}
	};
	LuaOpenLibrary ( "script", scriptLib );
	
	// add the base library
	luaopen_base	( m_pMainState );
	luaopen_math	( m_pMainState );
	luaopen_string	( m_pMainState );
	lua_settop		( m_pMainState, 0 );	
#endif
}

mcLuaManager::~mcLuaManager ( KDvoid )
{
#if 0
	mcLuaScript*		n = m_pHead;
	while ( n ) 
	{
		mcLuaScript*	i = n->m_pNext;
		delete ( n );
		n = i;
	}
	lua_close ( m_pMainState );
#endif
}

KDbool mcLuaManager::LuaOpenLibrary ( const KDchar* szName, const luaL_reg* pLibs )
{
#if 0
	if ( m_pHead != KD_NULL )
	{
		return 0;
	}
	
	luaL_openlib ( m_pMainState, szName, pLibs, 0 );
#endif
	return 1;	
}

KDvoid mcLuaManager::SetGlobalNumber ( const KDchar* szName, KDdouble dVal )
{
#if 0
	lua_pushnumber	( m_pMainState,   dVal );
	lua_setglobal	( m_pMainState, szName );
#endif
}

KDvoid mcLuaManager::SetGlobalInteger ( const KDchar* szName, KDint nVal )
{
#if 0
	lua_pushinteger ( m_pMainState,   nVal );
	lua_setglobal	( m_pMainState, szName );
#endif
}

KDvoid mcLuaManager::SetGlobalString ( const KDchar* szName, const KDchar* szVal )
{
#if 0
	lua_pushstring	( m_pMainState,  szVal );
	lua_setglobal	( m_pMainState, szName );
#endif
}
	
mcLuaScript* mcLuaManager::CreateScript ( KDvoid )
{
#if 0
	lua_State*		s = lua_newthread ( m_pMainState );
	
	// Add reference to the new thread in the Lua Registry so
	// will not be garbage collected
	KDint			r = luaL_ref ( m_pMainState, LUA_REGISTRYINDEX );
	
	// Create a new mcLuaScript object to hold the new thread
	mcLuaScript*	ns = new mcLuaScript ( s, this, r );
	
	// Add an entry to map the new lua_State to the 
	// new mcLuaScript object 
	lua_pushlightuserdata ( m_pMainState, s );
	lua_pushlightuserdata ( m_pMainState, ns );
	lua_settable ( m_pMainState, LUA_GLOBALSINDEX );
	
	// insert the new script into the list
	ns->m_pNext = m_pHead;
	m_pHead = ns;
	return ns;
#endif
	return nullptr;
}

KDvoid mcLuaManager::DestroyScript ( mcLuaScript* s )
{
#if 0
	if ( s == m_pHead )
	{
		m_pHead = s->m_pNext;
		delete ( s );
		return;
	}
	
	mcLuaScript*	pLast = m_pHead;
	for ( mcLuaScript* l = pLast->m_pNext; l != NULL; l = l->m_pNext )
	{
		if ( l == s )
		{
			pLast->m_pNext = l->m_pNext;
			delete ( l );
			return;
		}
		pLast = l;
	}
#endif
}

KDvoid mcLuaManager::Update ( KDfloat fElapsedSeconds )
{
#if 0
	mcLuaScript*	n = m_pHead;
	while ( n )
	{
		n = n->Update ( fElapsedSeconds );
	}
#endif
}

//---------------------------------------------------------------


mcLuaScript::mcLuaScript ( lua_State* l, mcLuaManager* pMan, KDint nRegistryRef )
{
#if 0
	m_pNext			= KD_NULL;
	m_nRegistryRef	= nRegistryRef;
	m_pState		= l;
	m_pManager		= pMan;
	m_eYieldMode	= YM_NONE;
	m_nWaitFrames	= 0;
	m_fWaitTime		= 0;
#endif
}

mcLuaScript::~mcLuaScript ( KDvoid )
{
#if 0
	luaL_unref ( m_pState, LUA_REGISTRYINDEX, m_nRegistryRef );
#endif
}
	
KDvoid mcLuaScript::LoadFile ( const KDchar* szName )
{
#if 0
	kdPrintf ( "mcLuaScript::LoadFile() - %s\n", szName );
	KDint	nErr = luaL_loadfile ( m_pState, szName );
	if ( nErr )
	{
		kdPrintf ( "luaL_loadfile Error- %s\n", lua_tostring ( m_pState, -1 ) );
		lua_pop ( m_pState, 1 );
	}
	
	lua_resume ( m_pState, 0 );
#endif
}

KDvoid mcLuaScript::LoadString ( const KDchar* szBuffer )
{
#if 0
	KDint	nErr = luaL_loadstring ( m_pState, szBuffer );
	if ( nErr )
	{
		kdPrintf ( "%s", lua_tostring ( m_pState, -1 ) );
		lua_pop ( m_pState, 1 );
	}

	lua_resume ( m_pState, 0 );
#endif
}

mcLuaScript* mcLuaScript::Update ( KDfloat fElapsedSeconds )
{
#if 0
	if ( m_eYieldMode == YM_TIME )
	{
		m_fWaitTime -= fElapsedSeconds;
		if ( m_fWaitTime > 0 )
		{
			return m_pNext;
		}
	} 
	
	if ( m_eYieldMode == YM_FRAME )
	{
		--m_nWaitFrames;
		if ( m_nWaitFrames > 0 )
		{
			return m_pNext;
		}
	}
	
	if ( m_eYieldMode == YM_PAUSE )
	{
		return m_pNext;
	}
	
	m_eYieldMode = YM_NONE;
	lua_resume ( m_pState, 0 );
	return m_pNext;
#endif
	return 0;
}
 
KDvoid mcLuaScript::YieldFrames ( KDint nNum )
{
	m_eYieldMode  = YM_FRAME;
	m_nWaitFrames = nNum;
}

KDvoid mcLuaScript::YieldSeconds ( KDfloat fSecs )
{
//	kdPrintf ( "YieldSeconds \n" );
	m_eYieldMode = YM_TIME;
	m_fWaitTime  = fSecs;
}

KDvoid mcLuaScript::YieldPause ( KDvoid )
{
	m_eYieldMode = YM_PAUSE;
}

KDvoid mcLuaScript::YieldResume ( KDvoid )
{
//	kdPrintf ( "resume \n" );
	m_eYieldMode = YM_NONE;
}