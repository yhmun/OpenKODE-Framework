/* -----------------------------------------------------------------------------------
 *
 *      File            BackImg.cpp
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
#include "BackImg.h"

cBackImg::cBackImg ( JBaseScene* pScene, const KDchar* pStrBack, const KDchar* pStrBackGray )
: m_pScene			( pScene  )
, m_pSprBack		( nullptr )
, m_pSprBackGray	( nullptr )
{
	// sprite background
	m_pSprBack		= new cSprite ( pStrBack	, _LAYER_BACKGROUND, Point::ZERO );
	m_pSprBackGray	= new cSprite ( pStrBackGray, _LAYER_BACKGROUND, Point::ZERO );
	
	m_pSprBack		->setIsStatic  ( true );
	m_pSprBackGray	->setIsStatic  ( true );
	m_pSprBackGray	->setIsVisible ( false );
	
	m_pScene->addDrawObject ( m_pSprBack );
	m_pScene->addDrawObject ( m_pSprBackGray );
}

cBackImg::~cBackImg ( KDvoid )
{	
	m_pSprBack		= nullptr;
	m_pSprBackGray	= nullptr;

	m_aSprBackDecos    .clear ( );
	m_aSprBackDecosGray.clear ( );
}

KDvoid cBackImg::addSprDeco ( const KDchar* pSprite, const KDchar* pSpriteGray, KDint z )
{
	// sprite background
	cSprite*	pSprBack	 = new cSprite ( pSprite	, z, Point ( 0, 95 ) );
	cSprite*	pSprBackGray = new cSprite ( pSpriteGray, z, Point ( 0, 95 ) );
	
	pSprBack	->setIsStatic  ( true );
	pSprBackGray->setIsStatic  ( true );
	pSprBackGray->setIsVisible ( false );
	
	m_aSprBackDecos	   .push_back ( pSprBack     );
	m_aSprBackDecosGray.push_back ( pSprBackGray );
	
	m_pScene->addDrawObject ( pSprBack     );
	m_pScene->addDrawObject ( pSprBackGray );
}

KDvoid cBackImg::setGray ( KDbool bIsGray )
{
	if ( bIsGray == true )
	{
		m_pSprBack	  ->setIsVisible ( false );
		m_pSprBackGray->setIsVisible ( true  );
		
		for ( auto pSpr :  m_aSprBackDecos     ) pSpr->setIsVisible ( false );
		for ( auto pSpr :  m_aSprBackDecosGray ) pSpr->setIsVisible ( true  );
	}
	else
	{
		m_pSprBack	  ->setIsVisible ( true  );
		m_pSprBackGray->setIsVisible ( false );

		for ( auto pSpr :  m_aSprBackDecos     ) pSpr->setIsVisible ( true  );
		for ( auto pSpr :  m_aSprBackDecosGray ) pSpr->setIsVisible ( false );
	}
}

KDvoid cBackImg::update ( KDdouble dLeftTime )
{

}

