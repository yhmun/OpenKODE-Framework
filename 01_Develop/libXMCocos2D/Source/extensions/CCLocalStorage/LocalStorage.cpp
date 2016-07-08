/* --------------------------------------------------------------------------
 *
 *      File            LocalStorage.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Zynga Inc.
 *
 *         http://www.cocos2d-x.org      
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
#include "extensions/CCLocalStorage/LocalStorage.h"
#include "XMSqlite/sqlite3.h"

NS_CC_BEGIN

static KDint			l_nInitialized = 0;
static sqlite3*			l_pDB;
static sqlite3_stmt*	l_pStmtSelect;
static sqlite3_stmt*	l_pStmtRemove;
static sqlite3_stmt*	l_pStmtUpdate;


static KDvoid	localStorageLazyInit ( KDvoid );
static KDvoid	localStorageCreateTable ( KDvoid );

static KDvoid	localStorageCreateTable ( KDvoid )
{
	const KDchar*  szSqlCreatetable = "CREATE TABLE IF NOT EXISTS data(key TEXT PRIMARY KEY,value TEXT);";
	sqlite3_stmt*  pStmt;

	KDint  nOk = sqlite3_prepare_v2 ( l_pDB, szSqlCreatetable, -1, &pStmt, KD_NULL );
	nOk |= sqlite3_step ( pStmt );
	nOk |= sqlite3_finalize ( pStmt );
	
	if ( nOk != SQLITE_OK && nOk != SQLITE_DONE )
	{
		kdPrintf ( "Error in CREATE TABLE" );
	}
}

KDvoid	localStorageInit ( const KDchar* szFullpath )
{
	if ( ! l_nInitialized )
	{
		KDint  nRet = 0;
		
		if ( !szFullpath )
		{
			nRet = sqlite3_open ( ":memory:", &l_pDB );
		}
		else
		{
			nRet = sqlite3_open ( szFullpath, &l_pDB );
		}

		localStorageCreateTable ( );

		// SELECT
		const KDchar*  szSqlSelect = "SELECT value FROM data WHERE key=?;";
		nRet |= sqlite3_prepare_v2 ( l_pDB, szSqlSelect, -1, &l_pStmtSelect, KD_NULL );

		// REPLACE
		const KDchar*  szSqlUpdate = "REPLACE INTO data (key, value) VALUES (?,?);";
		nRet |= sqlite3_prepare_v2 ( l_pDB, szSqlUpdate, -1, &l_pStmtUpdate, KD_NULL );

		// DELETE
		const KDchar*  szSqlRemove = "DELETE FROM data WHERE key=?;";
		nRet |= sqlite3_prepare_v2 ( l_pDB, szSqlRemove, -1, &l_pStmtRemove, KD_NULL );

		if ( nRet != SQLITE_OK )
		{
			kdPrintf ( "Error initializing DB" );
			// report error
		}
		
		l_nInitialized = 1;
	}
}

KDvoid	localStorageFree ( KDvoid )
{
	if ( l_nInitialized )
	{
		sqlite3_finalize ( l_pStmtSelect );
		sqlite3_finalize ( l_pStmtRemove );
		sqlite3_finalize ( l_pStmtUpdate );		

		sqlite3_close ( l_pDB );
		
		l_nInitialized = 0;
	}
}

/** sets an item in the LS */
KDvoid	localStorageSetItem ( const KDchar* szKey, const KDchar* szValue )
{
	kdAssert( l_nInitialized );
	
	KDint  nOk = sqlite3_bind_text ( l_pStmtUpdate, 1, szKey, -1, SQLITE_TRANSIENT );
	nOk |= sqlite3_bind_text ( l_pStmtUpdate, 2, szValue, -1, SQLITE_TRANSIENT );

	nOk |= sqlite3_step ( l_pStmtUpdate );
	
	nOk |= sqlite3_reset ( l_pStmtUpdate );
	
	if ( nOk != SQLITE_OK && nOk != SQLITE_DONE )
	{
		kdPrintf ( "Error in localStorage.setItem()" );
	}
}

/** gets an item from the LS */
const KDchar*	localStorageGetItem ( const KDchar* szKey )
{
	kdAssert ( l_nInitialized );

	KDint  nOk = sqlite3_reset ( l_pStmtSelect );

	nOk |= sqlite3_bind_text ( l_pStmtSelect, 1, szKey, -1, SQLITE_TRANSIENT );
	nOk |= sqlite3_step ( l_pStmtSelect );
	const KDubyte*  pRet = sqlite3_column_text ( l_pStmtSelect, 0 );	

	if ( nOk != SQLITE_OK && nOk != SQLITE_DONE && nOk != SQLITE_ROW )
	{
		kdPrintf ( "Error in localStorage.getItem()" );
	}

	return (const KDchar*) pRet;
}

/** removes an item from the LS */
KDvoid	localStorageRemoveItem ( const KDchar* szKey )
{
	kdAssert ( l_nInitialized );

	KDint  nOk = sqlite3_bind_text ( l_pStmtRemove, 1, szKey, -1, SQLITE_TRANSIENT );
	
	nOk |= sqlite3_step ( l_pStmtRemove );
	
	nOk |= sqlite3_reset ( l_pStmtRemove );

	if ( nOk != SQLITE_OK && nOk != SQLITE_DONE )
	{
		kdPrintf ( "Error in localStorage.removeItem()" );
	}
}

NS_CC_END