/* -----------------------------------------------------------------------------------
 *
 *      File            ManualNode.cpp
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
#include "ManualNode.h"
#include "TitleNode/Main/MainNode.h"

KDbool ManualNode::init ( KDvoid )
{
	if ( !Scene::init ( ) )
	{
		return false;
	}

	// back
	auto  pBg = Sprite::create ( "Manual/manual_bg.png" );
	pBg->setPosition ( Point ( m_tContentSize ) / 2 );
	this->addChild ( pBg, LAYER_BACK );

	// main menu
	m_pMenuLeft  = Menu::createWithItem ( MenuItemImage::create ( "Manual/manual_btn_left.png"	, "Manual/manual_btn_left_push.png"	, CC_CALLBACK_1 ( ManualNode::leftCallback  , this ) ) );
	m_pMenuRight = Menu::createWithItem ( MenuItemImage::create ( "Manual/manual_btn_right.png"	, "Manual/manual_btn_right_push.png", CC_CALLBACK_1 ( ManualNode::rightCallback , this ) ) );
	m_pMenuExit	 = Menu::createWithItem ( MenuItemImage::create ( "Manual/manual_btn_back.png"	, "Manual/manual_btn_back_push.png" , CC_CALLBACK_1 ( ManualNode::exitCallback  , this ) ) );

	m_pMenuLeft ->setPosition ( Point (       28, 272 + 21.5f ) );
	m_pMenuRight->setPosition ( Point ( 425 + 28, 272 + 21.5f ) );
	m_pMenuExit	->setPosition ( Point (  11 + 32,  10 + 12    ) );

	this->addChild ( m_pMenuLeft , LAYER_MENE );
	this->addChild ( m_pMenuRight, LAYER_MENE );
	this->addChild ( m_pMenuExit , LAYER_MENE );

	// page
	m_nNowPage = 0;
	
	// set page
	this->setPage ( 0 );
	
	return true;
}

KDvoid ManualNode::leftCallback ( Object* pSender )
{
	// page
	this->setPage ( --m_nNowPage );
}

KDvoid ManualNode::rightCallback ( Object* pSender )
{
	// page
	this->setPage ( ++m_nNowPage );
}

KDvoid ManualNode::exitCallback ( Object* pSender )
{
	Director::getInstance ( )->replaceScene ( TransitionFade::create ( 0.0f, MainNode::create ( false ) ) );
}

KDvoid ManualNode::setPage ( KDint nPage )
{
	// page
		 if ( m_nNowPage < 0 )	m_nNowPage = 9;
	else if ( m_nNowPage > 9 )	m_nNowPage = 0;
	else						m_nNowPage = nPage;
	
	// sprite release
	for ( auto pSprite : m_aSprites )
	{
		this->removeChild ( pSprite, true );		
	}
	m_aSprites.clear ( );
	
	// sprite load
	if ( m_nNowPage == 0 )
	{
		// back
		Sprite*			pSprFont		= Sprite::create ( "Manual/manual_bg_1.png"		);
		Sprite*			pSprPage		= Sprite::create ( "Manual/manual_page_1.png"	);
		Sprite*			pSprCharacter	= Sprite::create ( "Manual/manual_bg_2.png"		);
		Sprite*			pSprManual		= Sprite::create ( "Manual/manual_1.png"		);

		pSprFont		->setPosition ( Point ( 145, 285 ) );
		pSprPage		->setPosition ( Point ( 191,   0 ) );
		pSprCharacter	->setPosition ( Point ( 342,   0 ) );
		pSprManual		->setPosition ( Point (  63, 102 ) );

		pSprFont		->setAnchorPoint ( Point::ZERO );
		pSprPage		->setAnchorPoint ( Point::ZERO );
		pSprCharacter	->setAnchorPoint ( Point::ZERO );
		pSprManual		->setAnchorPoint ( Point::ZERO );
		
		this->addChild ( pSprFont		, LAYER_BACK );
		this->addChild ( pSprPage		, LAYER_BACK );
		this->addChild ( pSprCharacter	, LAYER_BACK );
		this->addChild ( pSprManual		, LAYER_BACK );
		
		m_aSprites.push_back ( pSprFont		 );
		m_aSprites.push_back ( pSprPage		 );
		m_aSprites.push_back ( pSprCharacter );
		m_aSprites.push_back ( pSprManual	 );
	}
	else if ( m_nNowPage == 1 )
	{
		// back
		Sprite*			pSprFont		= Sprite::create ( "Manual/manual_bg_1.png"		);
		Sprite*			pSprPage		= Sprite::create ( "Manual/manual_page_2.png"	);
		Sprite*			pSprCharacter	= Sprite::create ( "Manual/manual_bg_2.png"		);
		Sprite*			pSprManual		= Sprite::create ( "Manual/manual_2.png"		);
		
		pSprFont		->setPosition ( Point ( 145, 285 ) );
		pSprPage		->setPosition ( Point ( 191,   0 ) );
		pSprCharacter	->setPosition ( Point ( 342,   0 ) );
		pSprManual		->setPosition ( Point (  45,  47 ) );
		
		pSprFont		->setAnchorPoint ( Point::ZERO );
		pSprPage		->setAnchorPoint ( Point::ZERO );
		pSprCharacter	->setAnchorPoint ( Point::ZERO );
		pSprManual		->setAnchorPoint ( Point::ZERO );
		
		this->addChild ( pSprFont		, LAYER_BACK );
		this->addChild ( pSprPage		, LAYER_BACK );
		this->addChild ( pSprCharacter	, LAYER_BACK );
		this->addChild ( pSprManual		, LAYER_BACK );
		
		m_aSprites.push_back ( pSprFont		 );
		m_aSprites.push_back ( pSprPage		 );
		m_aSprites.push_back ( pSprCharacter );
		m_aSprites.push_back ( pSprManual	 );
	}
	else if ( m_nNowPage == 2 )
	{
		// back
		Sprite*			pSprFont		= Sprite::create ( "Manual/manual_bg_1.png"		);
		Sprite*			pSprPage		= Sprite::create ( "Manual/manual_page_3.png"	);
		Sprite*			pSprCharacter	= Sprite::create ( "Manual/manual_bg_2.png"		);
		Sprite*			pSprManual		= Sprite::create ( "Manual/manual_3.png"		);
		
		pSprFont		->setPosition ( Point ( 145, 285 ) );
		pSprPage		->setPosition ( Point ( 191,   0 ) );
		pSprCharacter	->setPosition ( Point ( 342,   0 ) );
		pSprManual		->setPosition ( Point (  35,  46 ) );
		
		pSprFont		->setAnchorPoint ( Point::ZERO );
		pSprPage		->setAnchorPoint ( Point::ZERO );
		pSprCharacter	->setAnchorPoint ( Point::ZERO );
		pSprManual		->setAnchorPoint ( Point::ZERO );
		
		this->addChild ( pSprFont		, LAYER_BACK );
		this->addChild ( pSprPage		, LAYER_BACK );
		this->addChild ( pSprCharacter	, LAYER_BACK );
		this->addChild ( pSprManual		, LAYER_BACK );
		
		m_aSprites.push_back ( pSprFont		 );
		m_aSprites.push_back ( pSprPage		 );
		m_aSprites.push_back ( pSprCharacter );
		m_aSprites.push_back ( pSprManual	 );
	}
	else if ( m_nNowPage == 3 )
	{
		// back
		Sprite*			pSprFont		= Sprite::create ( "Manual/manual_bg_1.png"		);
		Sprite*			pSprPage		= Sprite::create ( "Manual/manual_page_4.png"	);
		Sprite*			pSprCharacter	= Sprite::create ( "Manual/manual_bg_2.png"		);
		Sprite*			pSprManual		= Sprite::create ( "Manual/manual_4.png"		);
		
		pSprFont		->setPosition ( Point ( 145, 285) );
		pSprPage		->setPosition ( Point ( 191,   0) );
		pSprCharacter	->setPosition ( Point ( 342,   0) );
		pSprManual		->setPosition ( Point (  17,  60) );
		
		pSprFont		->setAnchorPoint ( Point::ZERO );
		pSprPage		->setAnchorPoint ( Point::ZERO );
		pSprCharacter	->setAnchorPoint ( Point::ZERO );
		pSprManual		->setAnchorPoint ( Point::ZERO );
		
		this->addChild ( pSprFont		, LAYER_BACK );
		this->addChild ( pSprPage		, LAYER_BACK );
		this->addChild ( pSprCharacter	, LAYER_BACK );
		this->addChild ( pSprManual		, LAYER_BACK );
		
		m_aSprites.push_back ( pSprFont		 );
		m_aSprites.push_back ( pSprPage		 );
		m_aSprites.push_back ( pSprCharacter );
		m_aSprites.push_back ( pSprManual	 );
	}
	else if ( m_nNowPage == 4 )
	{
		// back
		Sprite*			pSprFont		= Sprite::create ( "Manual/manual_bg_1.png"		);
		Sprite*			pSprPage		= Sprite::create ( "Manual/manual_page_5.png"	);
		Sprite*			pSprCharacter	= Sprite::create ( "Manual/manual_bg_2.png"		);
		Sprite*			pSprManual		= Sprite::create ( "Manual/manual_5.png"		);
		
		pSprFont		->setPosition ( Point ( 145, 285 ) );
		pSprPage		->setPosition ( Point ( 191,   0 ) );
		pSprCharacter	->setPosition ( Point ( 342,   0 ) );
		pSprManual		->setPosition ( Point (  17, 106 ) );
		
		pSprFont		->setAnchorPoint ( Point::ZERO );
		pSprPage		->setAnchorPoint ( Point::ZERO );
		pSprCharacter	->setAnchorPoint ( Point::ZERO );
		pSprManual		->setAnchorPoint ( Point::ZERO );
		
		this->addChild ( pSprFont		, LAYER_BACK );
		this->addChild ( pSprPage		, LAYER_BACK );
		this->addChild ( pSprCharacter	, LAYER_BACK );
		this->addChild ( pSprManual		, LAYER_BACK );
		
		m_aSprites.push_back ( pSprFont		 );
		m_aSprites.push_back ( pSprPage		 );
		m_aSprites.push_back ( pSprCharacter );
		m_aSprites.push_back ( pSprManual	 );
	}
	else if ( m_nNowPage == 5 )
	{
		// back
		Sprite*			pSprFont		= Sprite::create ( "Manual/manual_bg_1.png"		);
		Sprite*			pSprPage		= Sprite::create ( "Manual/manual_page_6.png"	);
		Sprite*			pSprCharacter	= Sprite::create ( "Manual/manual_bg_2.png"		);
		Sprite*			pSprManual		= Sprite::create ( "Manual/manual_6.png"		);
		
		pSprFont		->setPosition ( Point ( 145, 285 ) );
		pSprPage		->setPosition ( Point ( 191,   0 ) );
		pSprCharacter	->setPosition ( Point ( 342,   0 ) );
		pSprManual		->setPosition ( Point (  19,  62 ) );
		
		pSprFont		->setAnchorPoint ( Point::ZERO );
		pSprPage		->setAnchorPoint ( Point::ZERO );
		pSprCharacter	->setAnchorPoint ( Point::ZERO );
		pSprManual		->setAnchorPoint ( Point::ZERO );
		
		this->addChild ( pSprFont		, LAYER_BACK );
		this->addChild ( pSprPage		, LAYER_BACK );
		this->addChild ( pSprCharacter	, LAYER_BACK );
		this->addChild ( pSprManual		, LAYER_BACK );
		
		m_aSprites.push_back ( pSprFont		 );
		m_aSprites.push_back ( pSprPage		 );
		m_aSprites.push_back ( pSprCharacter );
		m_aSprites.push_back ( pSprManual	 );
	}
	else if ( m_nNowPage == 6 )
	{
		// back
		Sprite*			pSprFont		= Sprite::create ( "Manual/manual_bg_1.png"	);
		Sprite*			pSprPage		= Sprite::create ( "Manual/manual_page_7.png"	);
		Sprite*			pSprCharacter	= Sprite::create ( "Manual/manual_bg_3.png"	);
		Sprite*			pSprManual		= Sprite::create ( "Manual/manual_7.png"		);
		
		pSprFont		->setPosition ( Point ( 145, 285 ) );
		pSprPage		->setPosition ( Point ( 191,   0 ) );
		pSprCharacter	->setPosition ( Point ( 303,   0 ) );
		pSprManual		->setPosition ( Point (  39,  58 ) );
		
		pSprFont		->setAnchorPoint ( Point::ZERO );
		pSprPage		->setAnchorPoint ( Point::ZERO );
		pSprCharacter	->setAnchorPoint ( Point::ZERO );
		pSprManual		->setAnchorPoint ( Point::ZERO );
		
		this->addChild ( pSprFont		, LAYER_BACK );
		this->addChild ( pSprPage		, LAYER_BACK );
		this->addChild ( pSprCharacter  , LAYER_BACK );
		this->addChild ( pSprManual		, LAYER_BACK );
		
		m_aSprites.push_back ( pSprFont		 );
		m_aSprites.push_back ( pSprPage		 );
		m_aSprites.push_back ( pSprCharacter );
		m_aSprites.push_back ( pSprManual	 );
	}
	else if ( m_nNowPage == 7 )
	{
		// back
		Sprite*			pSprFont		= Sprite::create ( "Manual/manual_bg_1.png"		);
		Sprite*			pSprPage		= Sprite::create ( "Manual/manual_page_8.png"	);
		Sprite*			pSprCharacter	= Sprite::create ( "Manual/manual_bg_3.png"		);
		Sprite*			pSprManual		= Sprite::create ( "Manual/manual_8.png"		);
		
		pSprFont		->setPosition ( Point ( 145, 285 ) );
		pSprPage		->setPosition ( Point ( 191,   0 ) );
		pSprCharacter	->setPosition ( Point ( 303,   0 ) );
		pSprManual		->setPosition ( Point (  85,  26 ) );
		
		pSprFont		->setAnchorPoint ( Point::ZERO );
		pSprPage		->setAnchorPoint ( Point::ZERO );
		pSprCharacter	->setAnchorPoint ( Point::ZERO );
		pSprManual		->setAnchorPoint ( Point::ZERO );
		
		this->addChild ( pSprFont		, LAYER_BACK );
		this->addChild ( pSprPage		, LAYER_BACK );
		this->addChild ( pSprCharacter	, LAYER_BACK );
		this->addChild ( pSprManual		, LAYER_BACK );
		
		m_aSprites.push_back ( pSprFont		 );
		m_aSprites.push_back ( pSprPage		 );
		m_aSprites.push_back ( pSprCharacter );
		m_aSprites.push_back ( pSprManual	 );
	}
	else if ( m_nNowPage == 8 )
	{
		// back
		Sprite*		pSprFont		= Sprite::create ( "Manual/manual_bg_1.png"		);
		Sprite*		pSprPage		= Sprite::create ( "Manual/manual_page_9.png"	);
		Sprite*		pSprCharacter	= Sprite::create ( "Manual/manual_bg_3.png"		);
		Sprite*		pSprManual		= Sprite::create ( "Manual/manual_9.png"		);
		
		pSprFont		->setPosition ( Point ( 145, 285 ) );
		pSprPage		->setPosition ( Point ( 191,   0 ) );
		pSprCharacter	->setPosition ( Point ( 303,   0 ) );
		pSprManual		->setPosition ( Point (  85,  26 ) );
		
		pSprFont		->setAnchorPoint ( Point::ZERO );
		pSprPage		->setAnchorPoint ( Point::ZERO );
		pSprCharacter	->setAnchorPoint ( Point::ZERO );
		pSprManual		->setAnchorPoint ( Point::ZERO );
		
		this->addChild ( pSprFont		, LAYER_BACK );
		this->addChild ( pSprPage		, LAYER_BACK );
		this->addChild ( pSprCharacter	, LAYER_BACK );
		this->addChild ( pSprManual		, LAYER_BACK );
		
		m_aSprites.push_back ( pSprFont		 );
		m_aSprites.push_back ( pSprPage		 );
		m_aSprites.push_back ( pSprCharacter );
		m_aSprites.push_back ( pSprManual	 );
	}
	else if ( m_nNowPage == 9 )
	{
		// back
		Sprite*			pSprFont		= Sprite::create ( "Manual/manual_bg_1.png"		);
		Sprite*			pSprPage		= Sprite::create ( "Manual/manual_page_10.png"	);
		Sprite*			pSprCharacter	= Sprite::create ( "Manual/manual_bg_3.png"		);
		Sprite*			pSprManual		= Sprite::create ( "Manual/manual_10.png"		);
		
		pSprFont		->setPosition ( Point ( 145, 285 ) );
		pSprPage		->setPosition ( Point ( 191,   0 ) );
		pSprCharacter	->setPosition ( Point ( 303,   0 ) );
		pSprManual		->setPosition ( Point (  58,  26 ) );
		
		pSprFont		->setAnchorPoint ( Point::ZERO );
		pSprPage		->setAnchorPoint ( Point::ZERO );
		pSprCharacter	->setAnchorPoint ( Point::ZERO );
		pSprManual		->setAnchorPoint ( Point::ZERO );
		
		this->addChild ( pSprFont		, LAYER_BACK );
		this->addChild ( pSprPage		, LAYER_BACK );
		this->addChild ( pSprCharacter	, LAYER_BACK );
		this->addChild ( pSprManual		, LAYER_BACK );
		
		m_aSprites.push_back ( pSprFont		 );
		m_aSprites.push_back ( pSprPage		 );
		m_aSprites.push_back ( pSprCharacter );
		m_aSprites.push_back ( pSprManual	 );
	}
}
