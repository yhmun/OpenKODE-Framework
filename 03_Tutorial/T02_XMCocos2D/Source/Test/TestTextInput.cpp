/* --------------------------------------------------------------------------
 *
 *      File            TestTextInput.cpp
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
#include "TestTextInput.h"

#define FONT_NAME	"xm_supports/NanumGothicBold.ttf"
#define FONT_SIZE	36

TestTextInput* TestTextInput::create ( KDvoid )
{
	TestTextInput*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0 : pLayer = new TextFieldTTFDefaultTest ( ); break;
        case  1 : pLayer = new TextFieldTTFActionTest  ( ); break;
	}

	if ( pLayer && pLayer->init ( ) )
	{
		pLayer->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

	return pLayer;
}

KDuint TestTextInput::count ( KDvoid )
{
	return 2;
}

KDbool TestTextInput::init ( KDvoid )
{
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );

	m_pTextField = CCTextFieldTTF::create ( "<click here for input>", FONT_NAME, FONT_SIZE );
	m_pTextField->setPosition ( ccp ( tLyrSize.cx / 2.f, tLyrSize.cy / 2.f + 50.f ) );
	this->addChild ( m_pTextField );

	this->setTouchEnabled ( KD_TRUE );
    this->setTouchMode    ( kCCTouchesOneByOne );
	return KD_TRUE;
}

KDvoid TestTextInput::onExit ( KDvoid )
{
    m_pTextField->detachWithIME ( );

    TestBasic::onExit ( );
}

KDbool TestTextInput::ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent )
{
	CCRect  tRect = CCRect ( CCPointZero, m_pTextField->getContentSize ( ) );

	if ( tRect.containsPoint ( m_pTextField->convertToNodeSpace ( pTouch->getLocation ( ) ) ) )
	{
		m_pTextField->attachWithIME ( );
	}
	else
	{
		m_pTextField->detachWithIME ( );
	}

    return KD_TRUE;
}

//////////////////////////////////////////////////////////////////////////
// implement TextFieldTTFDefaultTest
//////////////////////////////////////////////////////////////////////////
const KDchar* TextFieldTTFDefaultTest::subtitle ( KDvoid )
{
    return "TextFieldTTF with default behavior test";
}

//////////////////////////////////////////////////////////////////////////
// implement TextFieldTTFActionTest
//////////////////////////////////////////////////////////////////////////
KDvoid TextFieldTTFActionTest::onEnter ( KDvoid )
{
	TestTextInput::onEnter ( );

	m_uLimit	= 12;
	m_uLength	= 0;
	m_pAction	= KD_NULL;

    m_pAction = CCRepeatForever::create
	( 
		CCSequence::create
		(
			CCFadeOut::create ( 0.25f ),
			CCFadeIn ::create ( 0.25f ),
			KD_NULL
		) 
	);

	m_pAction->retain ( );

	m_pTextField->setDelegate ( this );
}

KDvoid TextFieldTTFActionTest::onExit ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pAction );

	TestTextInput::onExit ( );
}

const KDchar* TextFieldTTFActionTest::subtitle ( KDvoid )
{
    return "CCTextFieldTTF with action and char limit test";
}

// CCTextFieldDelegate protocol
KDbool TextFieldTTFActionTest::onTextFieldAttachWithIME ( CCTextFieldTTF* pSender )
{
	if ( m_pTextField->numberOfRunningActions ( ) == 0 )
	{
		m_pTextField->runAction ( m_pAction );
	}

    return KD_FALSE;
}

KDbool TextFieldTTFActionTest::onTextFieldDetachWithIME ( CCTextFieldTTF* pSender )
{
	if ( m_pTextField->numberOfRunningActions ( ) != 0 )
	{
		m_pTextField->stopAction ( m_pAction );
		m_pTextField->setOpacity ( 255 );
	}

    return KD_FALSE;
}

KDbool TextFieldTTFActionTest::onTextFieldInsertText ( CCTextFieldTTF* pSender, const KDchar* szText, KDsize uLen )
{
    // if insert enter, treat as default to detach with ime
    if ( '\n' == *szText )
    {
        return KD_FALSE;
    }
    
    // if the textfield's char count more than m_nCharLimit, doesn't insert text anymore. 
    if ( pSender->getCharCount ( ) >= m_uLimit )
    {
        return KD_TRUE;
    }

	const CCSize&  tLyrSize = this->getContentSize ( );

	m_uLength = pSender->getCharCount ( );

    // create a insert text sprite and do some action
    CCLabelTTF*  pLabel = CCLabelTTF::create ( szText, FONT_NAME, FONT_SIZE );
	this->addChild ( pLabel );
	pLabel->setColor ( ccc3 ( 226, 121, 7 ) );

	// move the sprite from top to position
	CCPoint  tEnd = pSender->getPosition ( );
	if ( pSender->getCharCount ( ) )
	{
		tEnd.x += pSender->getContentSize ( ).cx / 2;
	}

	CCSize   tSize = pLabel->getContentSize ( );
	CCPoint  tBegin = ccp ( tEnd.x, tLyrSize.cy - tSize.cy * 2 ); 

	pLabel->setPosition ( tBegin );
	pLabel->setScale ( 8 );
	
	CCAction*  pSeq = CCSequence::create	
	(
		CCSpawn::create 
		(
			CCMoveTo ::create ( 0.5f, tEnd ),
			CCScaleTo::create ( 0.5f, 1 ),
			CCFadeOut::create ( 0.5f ),
			KD_NULL
		),
		CCCallFuncN::create ( this, callfuncN_selector ( TextFieldTTFActionTest::callbackRemoveNodeWhenDidAction ) ),
		KD_NULL
	);
	pLabel->runAction ( pSeq );

    return KD_FALSE;
}

KDbool TextFieldTTFActionTest::onTextFieldDeleteBackward ( CCTextFieldTTF* pSender, const KDchar* szText, KDsize uLen )
{
	const CCSize&  tLyrSize = this->getContentSize ( );

    // create a delete text sprite and do some action
    CCLabelTTF*  pLabel = CCLabelTTF::create ( szText, FONT_NAME, FONT_SIZE );
	this->addChild ( pLabel );

	// move the sprite to fly out
	CCPoint  tBegin = pSender->getPosition ( );
	tBegin.x += ( pSender->getContentSize ( ).cx - pLabel->getContentSize ( ).cx ) / 2.0f;

	CCPoint  tEnd = ccp ( -tLyrSize.cx / 4.0f, tLyrSize.cy * ( 0.5f + (KDfloat) kdRand ( ) / ( 2.0f * KD_RAND_MAX ) ) );
	pLabel->setPosition ( tBegin );

	CCAction*  pSeq = CCSequence::create
	(
		CCSpawn::create
		(
			CCMoveTo ::create ( 0.5f, tEnd ),
			CCRepeat ::create ( CCRotateBy::create ( 0.2f, ( kdRand ( ) % 2 ) ? 360.f : -360.f ), 5 ),
			CCFadeOut::create ( 0.5f ),
			KD_NULL
		),
		CCCallFuncN::create ( this, callfuncN_selector ( TextFieldTTFActionTest::callbackRemoveNodeWhenDidAction ) ),
		KD_NULL
	);

	pLabel->runAction ( pSeq );

    return KD_FALSE;
}

KDvoid TextFieldTTFActionTest::callbackRemoveNodeWhenDidAction ( CCNode* pNode )
{
    this->removeChild ( pNode, KD_TRUE );
}
