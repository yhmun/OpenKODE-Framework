/* -----------------------------------------------------------------------------------
 *
 *      File            JBaseScene.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
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
#include "JBaseScene.h"
#include "Object/Camera/Camera.h"
#include "Object/Button/Button.h"

JBaseScene::JBaseScene ( KDvoid )
{
	m_pCamera = nullptr;
}

JBaseScene::~JBaseScene ( KDvoid )
{
	// btns
	for ( auto pButton : m_aButtons )
	{	
		CC_SAFE_RELEASE ( pButton );
	}
	m_aButtons.clear ( );

	// draws
	for ( auto pDraw : m_aDraws )
	{
		CC_SAFE_RELEASE ( pDraw );
	}
	m_aDraws.clear ( );

	CC_SAFE_DELETE ( m_pCamera );
}

KDbool JBaseScene::init ( KDvoid )
{
	if ( !Scene::init ( ) )
	{
		return false;
	}

	m_dTickCount = 0;
	m_dBeforeTickCount = 0;
	m_dLeftTickCount = 0;

	// camera
	m_pCamera = new cCamera ( );

    auto	pTouchListener = EventListenerTouchAllAtOnce::create ( );
	pTouchListener->onTouchesBegan = CC_CALLBACK_2 ( JBaseScene::onTouchesBegan, this );
    pTouchListener->onTouchesMoved = CC_CALLBACK_2 ( JBaseScene::onTouchesMoved, this );
	pTouchListener->onTouchesEnded = CC_CALLBACK_2 ( JBaseScene::onTouchesEnded, this );
    m_pEventDispatcher->addEventListenerWithSceneGraphPriority ( pTouchListener, this );

	// schedule
	this->scheduleUpdate ( );

	return true;
}

KDvoid JBaseScene::draw ( KDvoid )
{
	// loop
	for (std::vector<cDraw*>::iterator pNode = m_aDraws.begin ( ); pNode != m_aDraws.end ( ); ++pNode )
	{
		// draw
		( *pNode )->draw ( m_pCamera );
	}
}

KDvoid JBaseScene::update ( KDfloat fDelta )
{
	// clear draws & btns--------------------

//	[self clear_draws_A_btns];

	// tickCount-----------------------------

	m_dTickCount = kdGetMilliseconds ( ) / 1000;

	if ( m_dBeforeTickCount == 0 )
	{
		m_dBeforeTickCount = m_dTickCount;
	}
	m_dLeftTickCount = m_dTickCount - m_dBeforeTickCount;
	m_dBeforeTickCount = m_dTickCount;

	// draw objects--------------------------

	// draw loop
	for ( auto pDraw : m_aDraws )
	{
		pDraw->update ( m_dLeftTickCount );
	}
}

KDvoid JBaseScene::onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	for ( auto tTouch : aTouches )
	{
		for ( auto pBtn : m_aButtons )
		{
			pBtn->onTouchBegan ( tTouch, pEvent );
		}
	}
}

KDvoid JBaseScene::onTouchesMoved ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	for ( auto tTouch : aTouches )
	{
		for ( auto pBtn : m_aButtons )
		{
			pBtn->onTouchMoved ( tTouch, pEvent );
		}
	}
}

KDvoid JBaseScene::onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent )
{
	for ( auto tTouch : aTouches )
	{
		for ( auto pBtn : m_aButtons )
		{
			pBtn->onTouchEnded ( tTouch, pEvent );
		}
	}
}

KDvoid JBaseScene::setIsTouch ( KDbool bIsTouch )
{
	m_pEventDispatcher->setEnabled ( bIsTouch );
}

KDdouble JBaseScene::getLeftTime ( KDvoid )
{
	return m_dLeftTickCount;
}

cCamera* JBaseScene::getCamera_ ( KDvoid )
{
	return m_pCamera;
}

KDvoid JBaseScene::addDrawObject ( cDraw* pDraw )
{
	// check
	if ( pDraw->getObjectType ( ) == cDraw::eObjectType_Button )
	{		
		CCAssert ( false, "" );
	}

	pDraw->retain ( );

	// loop
	std::vector<cDraw*>::iterator	pNode = m_aDraws.begin ( );
	while ( pNode != m_aDraws.end ( ) )
	{
		// insert
		if ( pDraw->getZorder() < (*pNode)->getZorder ( ) )
		{
			m_aDraws.insert ( pNode, pDraw );
			return;
		}

		// next
		++pNode;
	}

	// push_back	
	m_aDraws.push_back ( pDraw );
}

KDvoid JBaseScene::addBtnObject ( cButton* pBtn )
{
	// init
	KDbool		bIsInsert = false;

	pBtn->retain ( );

	// switch
	bIsInsert = false;

	// loop
	std::vector<cButton*>::iterator		pBtnNode = m_aButtons.begin ( );
	while ( pBtnNode != m_aButtons.end ( ) )
	{
		//check
		if( pBtn->getZorder ( ) < (*pBtnNode)->getZorder ( ) )
		{
			// insert
			m_aButtons.insert ( pBtnNode, pBtn );

			// switch
			bIsInsert = true;

			// break
			break;
		}

		// next
		++pBtnNode;
	}

	// push back
	if ( bIsInsert == false )
	{
		m_aButtons.push_back ( pBtn );
	}

	// draw----------------------------------

	pBtn->retain ( );

	// switch
	bIsInsert = false;

	//loop
	std::vector<cDraw*>::iterator	pNodeDraw = m_aDraws.begin ( );
	while( pNodeDraw != m_aDraws.end ( ) )
	{
		// check
		if ( pBtn->getZorder ( ) < (*pNodeDraw)->getZorder ( ) )
		{
			// insert
			m_aDraws.insert ( pNodeDraw, pBtn );

			// switch
			bIsInsert = true;

			// break
			break;
		}

		// next
		++pNodeDraw;
	}

	// push back
	if ( bIsInsert == false )
	{
		m_aDraws.push_back ( pBtn );
	}
}

KDvoid JBaseScene::setDrawObjZorder ( cDraw* pDraw, KDint z )
{
	// error check
	for ( std::vector<cButton*>::iterator pNode = m_aButtons.begin ( ); pNode != m_aButtons.end ( ); ++pNode )
	{
		if ( *pNode == pDraw )
		{		
			CCAssert ( false, "" );
		}
	}
	
	// erase
	KDbool	bIsErase = false;
	for ( std::vector<cDraw*>::iterator pNode = m_aDraws.begin ( ); pNode != m_aDraws.end ( ); ++pNode )
	{
		if ( *pNode == pDraw )
		{
			m_aDraws.erase ( pNode );
			bIsErase = true;
			break;
		}
	}
	
	// error check
	if ( bIsErase == false )
	{		
		CCAssert ( false, "" );
	}
	
	// set z order
	pDraw->setZorder ( z );
	
	// insert
	this->addDrawObject ( pDraw );
}