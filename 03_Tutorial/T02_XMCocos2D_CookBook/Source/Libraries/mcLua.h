/* --------------------------------------------------------------------------
 *
 *      File            mcLua.h
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

#ifndef __mcLua_h__
#define __mcLua_h__

extern "C"
{
	#include "XMLua/luajit/lua.h"
	#include "XMLua/luajit/lualib.h"
	#include "XMLua/luajit/lauxlib.h"
}

class mcLuaScript;

class mcLuaManager
{
	public :
				 mcLuaManager ( KDvoid );
		virtual ~mcLuaManager ( KDvoid );
	
	public :

		KDbool				LuaOpenLibrary   ( const KDchar* szName, const luaL_reg* pLibs );

		KDvoid				SetGlobalNumber  ( const KDchar* szName, KDdouble		dVal );
		KDvoid				SetGlobalInteger ( const KDchar* szName, KDint		    nVal );
		KDvoid				SetGlobalString	 ( const KDchar* szName, const KDchar* szVal );
		
		mcLuaScript*		CreateScript ( KDvoid );
		KDvoid				DestroyScript ( mcLuaScript* s );
		
		// call this function at every frame with the number of seconds since the last call
		KDvoid				Update ( KDfloat fElapsedSeconds );

	private :

		lua_State*			m_pMainState;
		mcLuaScript*		m_pHead;
};

class mcLuaScript
{
	protected :
				
				 mcLuaScript ( lua_State* l, mcLuaManager* pMan, KDint nRegistryRef );
		virtual ~mcLuaScript ( KDvoid );

		friend class mcLuaManager;

	public :

		KDvoid				LoadFile ( const KDchar* szName );

		KDvoid				LoadString ( const KDchar* szBuffer );
		
		// used internally by library- not for end user use
		KDvoid				YieldFrames  ( KDint nNum );
		KDvoid				YieldSeconds ( KDfloat fSecs );
		KDvoid				YieldPause   ( KDvoid );
		KDvoid				YieldResume  ( KDvoid );
		
	protected :

		mcLuaScript*		Update ( KDfloat fElapsedSeconds );

	protected :

		mcLuaScript*		m_pNext;
		KDint				m_nRegistryRef;
		
	private :

		typedef enum 
		{
			YM_NONE		,
			YM_FRAME	,
			YM_TIME		,
			YM_PAUSE	,
		} YIELDMODE;
		
		lua_State*			m_pState;
		mcLuaManager*		m_pManager;
		YIELDMODE			m_eYieldMode;
		KDint				m_nWaitFrames;
		KDfloat				m_fWaitTime;
};

#endif	// __mcLua_h__
