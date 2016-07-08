/* --------------------------------------------------------------------------
 *
 *      File            TestSqlite.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
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
#include "TestSqlite.h"
#include "XMSqlite/CppSQLite3.h"

KDbool TestSqlite::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	m_pLabel = CCLabelTTF::create 
	(
		"Touch and Sqlite test will be.", "fonts/Marker Felt.ttf", 32, CCSizeZero, kCCTextAlignmentLeft 
	);	
	this->addChild ( m_pLabel );
	m_pLabel->setPositionWithParent ( kCCAlignmentCenter );

    this->setTouchEnabled ( KD_TRUE );  

	return KD_TRUE;
}

KDuint TestSqlite::count ( KDvoid )
{
	return 2;
}

KDvoid TestSqlite::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( s_nSubTestIndex == 0 )
	{
		testSqlite ( );
	}
	else
	{
		testCppSqlite ( );
	}
}

KDvoid TestSqlite::testSqlite ( KDvoid )
{
	KDchar			szLog [ 256 ];
	std::string		sRet   = "";
	KDchar*			szPath = xmGetNativePath ( "/data/TestSqlite.db", KD_NULL );
	KDchar*			szErr  = KD_NULL;
	const KDchar*	szSql  = KD_NULL;
	const KDchar*	szTail = KD_NULL;
	sqlite3*		pDB    = KD_NULL;
	sqlite3_stmt*	pStmt  = KD_NULL;
	KDint			nRet   = 0;
	KDint			nCols  = 0;

	if ( szPath )
	{
		nRet = sqlite3_open ( szPath, &pDB );
		if ( nRet != SQLITE_OK )
		{
			kdSprintfKHR ( szLog, "Can't open database: %s", sqlite3_errmsg ( pDB ) );
			m_pLabel->setString ( szLog );
			goto finish;
		}

		szSql = "create table dramaheros(id int, name text)";
		nRet  = sqlite3_exec ( pDB, szSql, KD_NULL, KD_NULL, &szErr );
		if ( nRet != SQLITE_OK )
		{
			kdSprintfKHR ( szLog, "Can't create table: %s", szErr );
			m_pLabel->setString ( szLog );
			goto finish;
		}

		szSql = "insert into dramaheros values (7, 'dr gregory house')";
		nRet  = sqlite3_exec ( pDB, szSql, KD_NULL, KD_NULL, &szErr );
		if ( nRet != SQLITE_OK )
		{
			kdSprintfKHR ( szLog, "Can't insert into: %s", szErr );
			m_pLabel->setString ( szLog );
			goto finish;
		}

		szSql = "insert into dramaheros values (14, 'dr james wilson')";
		nRet  = sqlite3_exec ( pDB, szSql, KD_NULL, KD_NULL, &szErr );
		if ( nRet != SQLITE_OK )
		{
			kdSprintfKHR ( szLog, "Can't insert into: %s", szErr );
			m_pLabel->setString ( szLog );
			goto finish;
		}

		szSql = "select * from dramaheros;";
		nRet  = sqlite3_prepare ( pDB, szSql, kdStrlen ( szSql ), &pStmt, &szTail );
		if ( nRet != SQLITE_OK )
		{
			kdSprintfKHR ( szLog, "SQL error: %s", sqlite3_errmsg ( pDB ) );
			m_pLabel->setString ( szLog );
			goto finish;
		}

		nRet  = sqlite3_step ( pStmt );
		nCols = sqlite3_column_count ( pStmt );
		while ( nRet == SQLITE_ROW ) 
		{
			for ( KDint  i = 0; i < nCols; i++ )
			{
				kdSprintfKHR ( szLog, "'%s' ", sqlite3_column_text ( pStmt, i ) );
				sRet += szLog;
			}
			
			sRet += "\n";
			nRet = sqlite3_step ( pStmt );
		}

		m_pLabel->setString ( sRet.c_str ( ) );
	}

finish :

	if ( szPath )
	{
		kdFree ( szPath );
	}

	if ( pDB )
	{
		sqlite3_exec  ( pDB, "drop table dramaheros", KD_NULL, KD_NULL, &szErr );
		sqlite3_close ( pDB );
	}
}

KDvoid TestSqlite::testCppSqlite ( KDvoid )
{
	m_pLabel->setString ( "See console log." );

	CCLOG ( "SQLite Header  Version        : %s", CppSQLite3DB::SQLiteHeaderVersion		   ( ) );
	CCLOG ( "SQLite Library Version        : %s", CppSQLite3DB::SQLiteLibraryVersion	   ( ) );
	CCLOG ( "SQLite Library Version Number : %d", CppSQLite3DB::SQLiteLibraryVersionNumber ( ) );

	const KDchar*  szPath = "/data/TestCppSqlite.db";
	CppSQLite3DB    tDB;
	CppSQLite3Query tQuery;
	KDtime  nTimeStart;
	KDtime  nTimeEnd;
	KDint   nRows;
	KDint   nRowsToCreate;
	KDint   i, fld;

	kdRemove ( szPath );
	tDB.open ( szPath );
	
	CCLOG ( "\nemp table exists = %s", tDB.tableExists ( "emp" ) ? "TRUE":"FALSE" );

	CCLOG ( "\nCreating emp table" );
	tDB.execDML ( "create table emp(empno int, empname char(20));" );
	CCLOG ( "emp table exists = %s", tDB.tableExists ( "emp" ) ? "TRUE":"FALSE" );

	////////////////////////////////////////////////////////////////////////////////
	// Execute some DML, and print number of rows affected by each one
	////////////////////////////////////////////////////////////////////////////////
	CCLOG ( "\nDML tests" );
	nRows = tDB.execDML ( "insert into emp values (7, 'David Beckham');" );
	CCLOG ( "%d rows inserted", nRows );

	nRows = tDB.execDML ( "update emp set empname = 'Christiano Ronaldo' where empno = 7;" );
	CCLOG ( "%d rows updated", nRows );

	nRows = tDB.execDML ( "delete from emp where empno = 7;" );
	CCLOG ( "%d rows deleted", nRows );

	////////////////////////////////////////////////////////////////////////////////
	// Transaction Demo
	// The transaction could just as easily have been rolled back
	////////////////////////////////////////////////////////////////////////////////
	nRowsToCreate = 25000;
	CCLOG ( "\nTransaction test, creating %d rows please wait...", nRowsToCreate );
	nTimeStart = kdTime ( 0 );
	CCLOG ( "PRE-TXN AUTO COMMIT = %s", tDB.IsAutoCommitOn ( ) ? "Y" : "N" );
	tDB.execDML ( "begin transaction;" );
	CCLOG ( "IN-TXN AUTO COMMIT = %s", tDB.IsAutoCommitOn ( ) ? "Y" : "N" );

    for ( i = 0; i < nRowsToCreate; i++)
    {
        KDchar  buf[128];
        kdSprintf ( buf, "insert into emp values (%d, 'Empname%06d');", i, i );
        tDB.execDML ( buf );
    }

	tDB.execDML ( "commit transaction;" );
	CCLOG ( "POST-TXN AUTO COMMIT = %s", tDB.IsAutoCommitOn ( ) ? "Y" : "N" );
	nTimeEnd = kdTime ( 0 );

    ////////////////////////////////////////////////////////////////////////////////
    // Demonstrate CppSQLiteDB::execScalar()
    ////////////////////////////////////////////////////////////////////////////////
    CCLOG ( "%d rows in emp table in %d seconds (that was fast!)", tDB.execScalar ( "select count(*) from emp;" ), nTimeEnd - nTimeStart );

	////////////////////////////////////////////////////////////////////////////////
	// Re-create emp table with auto-increment field
	////////////////////////////////////////////////////////////////////////////////
	CCLOG ( "\nAuto increment test" );
	tDB.execDML ("drop table emp;" );
	tDB.execDML ("create table emp(empno integer primary key, empname char(20));" );
	CCLOG ( "%d rows deleted", nRows );

    for ( i = 0; i < 5; i++)
    {
        KDchar  buf[128];
        kdSprintf ( buf, "insert into emp (empname) values ('Empname%06d');", i + 1 );
        tDB.execDML ( buf );
        CCLOG ( " primary key : %d", (KDint) tDB.lastRowId ( ) );
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Query data and also show results of inserts into auto-increment field
    ////////////////////////////////////////////////////////////////////////////////
    CCLOG ( "\nSelect statement test" );
    tQuery = tDB.execQuery ( "select * from emp order by 1;" );

    for ( fld = 0; fld < tQuery.numFields ( ); fld++ )
    {
		CCLOG ( "%s ( %s ) |", tQuery.fieldName ( fld ), tQuery.fieldDeclType ( fld ) );
    }

    while ( !tQuery.eof ( ) )
    {
        CCLOG ( "%s | %s |", tQuery.fieldValue ( 0 ), tQuery.fieldValue ( 1 ) );
        tQuery.nextRow ( );
    }

    ////////////////////////////////////////////////////////////////////////////////
    // SQLite's printf() functionality. Handles embedded quotes and NULLs
    ////////////////////////////////////////////////////////////////////////////////
    CCLOG ( "\nSQLite sprintf test" );
    CppSQLite3Buffer  tBufSQL;
    tBufSQL.format ( "insert into emp (empname) values (%Q);", "He's bad" );
    CCLOG ( (const KDchar*) tBufSQL );
    tDB.execDML ( tBufSQL );

    tBufSQL.format ( "insert into emp (empname) values (%Q);", KD_NULL );
    CCLOG ( (const KDchar*) tBufSQL );
    tDB.execDML ( tBufSQL );

    ////////////////////////////////////////////////////////////////////////////////
    // Fetch table at once, and also show how to use CppSQLiteTable::setRow() method
    ////////////////////////////////////////////////////////////////////////////////
    CCLOG ( "\ngetTable() test" );
    CppSQLite3Table  tTable = tDB.getTable ( "select * from emp order by 1;" );

    for ( fld = 0; fld < tTable.numFields ( ); fld++ )
    {
        CCLOG ( "%s | ", tTable.fieldName ( fld ) );
    }
    
    for ( KDint row = 0; row < tTable.numRows ( ); row++ )
    {
        tTable.setRow ( row );
        for ( KDint fld = 0; fld < tTable.numFields ( ); fld++ )
        {
            if ( !tTable.fieldIsNull ( fld ) )
			{
                CCLOG ( "%s | ", tTable.fieldValue ( fld ) );
			}
            else
			{
                CCLOG ( "NULL |" );
			}
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Test CppSQLiteBinary by storing/retrieving some binary data, checking
    // it afterwards to make sure it is the same
    ////////////////////////////////////////////////////////////////////////////////
    CCLOG ( "\nBinary data test" );
    tDB.execDML ( "create table bindata(desc char(10), data blob);" );
    
    KDubyte  cBin[256];
    CppSQLite3Binary  tBlob;

    for ( i = 0; i < sizeof ( cBin ); i++ )
    {
        cBin[i] = i;
    }

    tBlob.setBinary ( cBin, sizeof ( cBin ) );

    tBufSQL.format ( "insert into bindata values ('testing', %Q);", tBlob.getEncoded ( ) );
    tDB.execDML ( tBufSQL );
    CCLOG ( "Stored binary Length: %d", sizeof ( cBin ) );

    tQuery = tDB.execQuery ( "select data from bindata where desc = 'testing';" );

    if ( !tQuery.eof ( ) )
    {
        tBlob.setEncoded ( (KDubyte*) tQuery.fieldValue ( "data" ) );
        CCLOG ( "Retrieved binary Length : %d", tBlob.getBinaryLength ( ) );
    }
	tQuery.finalize ( );

    const KDubyte* pbin = tBlob.getBinary ( );
    for ( i = 0; i < sizeof ( cBin ); i++ )
    {
        if ( pbin[i] != i )
        {
            CCLOG ( "Problem: i: %d, bin[i]: ", i, pbin[i] );
        }
    }

	////////////////////////////////////////////////////////////////////////////////
    // Pre-compiled Statements Demo
    ////////////////////////////////////////////////////////////////////////////////
    CCLOG ( "\nTransaction test, creating %d rows please wait...", nRowsToCreate * 2 );
    tDB.execDML ( "drop table emp;" );
    tDB.execDML ( "create table emp(empno int, empname char(20));" );
    nTimeStart = kdTime ( 0 );
    tDB.execDML ( "begin transaction;" );

    CCLOG ( "\nCreating with bind by number" );
    CppSQLite3Statement  tStmt = tDB.compileStatement ( "insert into emp values (?, ?);" );
    for ( i = 0; i < nRowsToCreate; i++ )
    {
        KDchar  szBuf[16];
        kdSprintf ( szBuf, "EmpName%06d", i );
        tStmt.bind ( 1, i );
        tStmt.bind ( 2, szBuf );
        tStmt.execDML ( );
        tStmt.reset ( );
    }

    CCLOG ( "\nCreating with bind by name" );
	CppSQLite3Statement  tStmt2 = tDB.compileStatement ( "insert into emp values (:p1, @p2);" );
    for ( i = 0; i < nRowsToCreate; i++ )
    {
        KDchar  szBuf[16];
        kdSprintf ( szBuf, "EmpName%06d", i );
		tStmt2.bind ( ":p1", i );
		tStmt2.bind ( "@p2", szBuf );
        tStmt2.execDML ( );
        tStmt2.reset ( );
    }

	tDB.execDML ( "commit transaction;" );
    nTimeEnd = kdTime ( 0 );

    CCLOG ( "%d rows in emp table in %d seconds (that was even faster!)", tDB.execScalar ( "select count(*) from emp;" ), nTimeEnd - nTimeStart );
    CCLOG ( "\nEnd of tests" );
}