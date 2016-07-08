/* -----------------------------------------------------------------------------------
 *
 *      File            CCScene.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *
 *         http://www.cocos2d-x.org      
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
#include "layers_scenes_transitions_nodes/CCScene.h"
#include "support/CCPointExtension.h"
#include "support/CCDirector.h"

NS_CC_BEGIN
    
CCScene::CCScene ( KDvoid )
{
	m_bIgnoreAnchorPointForPosition = KD_TRUE;
	setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
}

KDbool CCScene::init ( KDvoid )
{
	KDbool  bRet = KD_FALSE;

 	do 
 	{
 		CCDirector*     pDirector;
 		CC_BREAK_IF ( !( pDirector = CCDirector::sharedDirector ( ) ) );

		this->setContentSize ( pDirector->getWinSize ( ) );

 		bRet = KD_TRUE;
 	
	} while ( 0 );

 	return bRet;
}

CCScene* CCScene::create ( KDvoid )
{
	CCScene*  pRet = new CCScene ( );

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
    else
    {
	    CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

NS_CC_END