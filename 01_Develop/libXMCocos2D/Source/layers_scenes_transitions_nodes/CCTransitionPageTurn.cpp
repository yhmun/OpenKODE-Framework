/* -----------------------------------------------------------------------------------
 *
 *      File            CCTransitionPageTurn.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      Sindesso Pty Ltd
 *
 *         http://www.cocos2d-x.org      
 *         http://www.sindesso.com 
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
#include "layers_scenes_transitions_nodes/CCTransitionPageTurn.h"
#include "actions/CCActionInterval.h"
#include "actions/CCActionInstant.h"
#include "actions/CCActionGrid.h"
#include "actions/CCActionPageTurn3D.h"
#include "support/CCDirector.h"

NS_CC_BEGIN

CCTransitionPageTurn* CCTransitionPageTurn::create ( KDfloat fDuration, CCScene* pScene, KDbool bBackwards )
{
	CCTransitionPageTurn*  pRet = new CCTransitionPageTurn ( );

    if ( pRet && pRet->initWithDuration ( fDuration, pScene, bBackwards ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool CCTransitionPageTurn::initWithDuration ( KDfloat fDuration, CCScene* pScene, KDbool bBackwards )
{
	m_bBackwards = bBackwards;

	return CCTransitionScene::initWithDuration ( fDuration, pScene );
}

KDvoid CCTransitionPageTurn::sceneOrder ( KDvoid )
{
	m_bIsInSceneOnTop = m_bBackwards;
}

KDvoid CCTransitionPageTurn::onEnter ( KDvoid )
{
	CCTransitionScene::onEnter ( );

	const CCSize&   tSize = CCDirector::sharedDirector ( )->getWinSize ( );
	KDint			x, y;

	if ( tSize.cx > tSize.cy )
	{
		x = 16;
        y = 12;
	}
	else
	{
		x = 12;
        y = 16;
	}

	CCActionInterval*  pAction = this->actionWithSize ( ccs ( x, y ) );

	if ( !m_bBackwards )
	{
		m_pOutScene->runAction
		(
			CCSequence::create
			(
				pAction,
				CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::finish ) ),
				CCStopGrid::create ( ),
				KD_NULL
			)
		);
	}
	else
	{
		// to prevent initial flicker
		m_pInScene->setVisible ( KD_FALSE );
		m_pInScene->runAction
		(
			CCSequence::create
			(
			    CCShow::create ( ),
				pAction,
				CCCallFunc::create ( this, callfunc_selector ( CCTransitionScene::finish ) ),
				CCStopGrid::create ( ),
				KD_NULL
			)
		);
	}
}

CCActionInterval* CCTransitionPageTurn::actionWithSize ( const CCSize& tVector )
{
    // Get hold of the PageTurn3DAction
	if ( m_bBackwards )
	{
		return CCReverseTime::create ( CCPageTurn3D::create ( m_fDuration, tVector ) );
	}
	else
	{
		return CCPageTurn3D::create ( m_fDuration, tVector );
	}
}


NS_CC_END
