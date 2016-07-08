/* -----------------------------------------------------------------------------------
 *
 *      File            TestBlade.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2011      Ngo Duc Hiep. All rights reserved.
 *
 *         http://precognitiveresearch.com      
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 * 
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 * 
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "TestBlade.h"

TestBlade::TestBlade ( KDvoid )
{
	m_pDictionary = KD_NULL;
}

TestBlade::~TestBlade ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pDictionary );
}

KDbool TestBlade::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

	m_pDictionary = new CCDictionary ( );

	this->setTouchEnabled ( KD_TRUE );

	return KD_TRUE;
}

const KDchar* TestBlade::subtitle ( KDvoid )
{
	return "drag screen";
}

KDvoid TestBlade::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	for ( CCSetIterator it = pTouches->begin ( ); it != pTouches->end ( ); it++ ) 
	{
		CCTouch*		pTouch	  = (CCTouch*) ( *it );
		CCPoint			tLocation = this->convertTouchToNodeSpace ( pTouch );
		
		CCBlade*		pBlade	  = CCBlade::create ( 50 );
		pBlade->setAutoDim ( KD_TRUE );

		KDint			nRand	  = kdRand ( ) % 3 + 1;
		CCTexture2D*	pTexture  = CCTextureCache::sharedTextureCache ( )->addImage ( ccszf ( "xm_supports/streak%d-hd.png", nRand ) );
		pBlade->setTexture ( pTexture );
		pBlade->push ( tLocation );

		this->addChild ( pBlade );	

		m_pDictionary->setObject ( pBlade, pTouch->getID ( ) );
	}
}

KDvoid TestBlade::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	for ( CCSetIterator it = pTouches->begin ( ); it != pTouches->end ( ); it++ ) 
	{
		CCTouch*		pTouch	  = (CCTouch*) ( *it );
		CCPoint			tLocation = this->convertTouchToNodeSpace ( pTouch );
		
		CCBlade*		pBlade	  = (CCBlade*) m_pDictionary->objectForKey ( pTouch->getID ( ) );
		pBlade->push ( tLocation );
	}
}

KDvoid TestBlade::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	for ( CCSetIterator it = pTouches->begin ( ); it != pTouches->end ( ); it++ ) 
	{
		CCTouch*		pTouch	  = (CCTouch*) ( *it );

		CCBlade*		pBlade	  = (CCBlade*) m_pDictionary->objectForKey ( pTouch->getID ( ) );
		pBlade->finish ( );

		m_pDictionary->removeObjectForKey ( pTouch->getID ( ) );
	}
}
