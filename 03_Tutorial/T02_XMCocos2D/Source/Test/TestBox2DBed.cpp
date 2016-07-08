/* --------------------------------------------------------------------------
 *
 *      File            TestBox2DBed.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (C) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
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
#include "TestBox2DBed.h"
#include "TestBox2D/Box2DTest.h"

KDuint TestBox2DBed::count ( KDvoid )
{
	return 36;
}

const KDchar* TestBox2DBed::subtitle ( KDvoid )
{
	return g_testEntries [ s_nSubTestIndex ].name;
}

KDvoid TestBox2DBed::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	m_pEntry = g_testEntries + s_nSubTestIndex;
	m_pTest  = m_pEntry->createFcn ( );	

	this->setTouchEnabled ( KD_TRUE );
	this->setTouchMode	  ( kCCTouchesOneByOne );

	this->schedule ( schedule_selector ( TestBox2DBed::tick ), 0.02f );
}

KDvoid TestBox2DBed::onExit ( KDvoid )
{	
	CC_SAFE_DELETE ( m_pTest );

	TestBasic::onExit ( );
}

KDvoid TestBox2DBed::draw ( KDvoid )
{
	TestBasic::draw ( );

	const CCSize&  s = this->getContentSize ( );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

    kmGLPushMatrix ( );

	kmGLTranslatef ( s.cx / 2 , s.cy / 4, 0 );
	kmGLScalef ( 10, 10, 10 );

    m_pTest->m_world->DrawDebugData ( );

    kmGLPopMatrix  ( );

    CHECK_GL_ERROR_DEBUG ( );
}

KDbool TestBox2DBed::ccTouchBegan ( CCTouch* pTouch, CCEvent* event )
{
	const CCSize&  s = this->getContentSize ( );

	CCPoint  tPos = this->convertTouchToNodeSpace ( pTouch );
	tPos = ccp ( ( tPos.x - s.cx / 2 ) / 10, ( tPos.y - s.cy / 4 ) / 10 );

	return m_pTest->MouseDown ( b2Vec2 ( tPos.x, tPos.y ) );	
}

KDvoid TestBox2DBed::ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent )
{
	const CCSize&  s = this->getContentSize ( );

	CCPoint  tPos = this->convertTouchToNodeSpace ( pTouch );
	tPos = ccp ( ( tPos.x - s.cx / 2 ) / 10, ( tPos.y - s.cy / 4 ) / 10 );
	
	m_pTest->MouseMove ( b2Vec2 ( tPos.x, tPos.y ) );
}

KDvoid TestBox2DBed::ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent )
{
	const CCSize&  s = this->getContentSize ( );

	CCPoint  tPos = this->convertTouchToNodeSpace ( pTouch );
	tPos = ccp ( ( tPos.x - s.cx / 2 ) / 10, ( tPos.y - s.cy / 4 ) / 10 );
	
	m_pTest->MouseUp ( b2Vec2 ( tPos.x, tPos.y ) );
}

KDvoid TestBox2DBed::tick ( KDfloat fDelta )
{
	static Settings  tSettings;
	
	m_pTest->Step ( &tSettings );

}