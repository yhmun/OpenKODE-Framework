/* -----------------------------------------------------------------------------------
 *
 *      File            StartDirector.cpp
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
#include "StartDirector.h"

cStartDirector::cStartDirector ( JBaseScene* pScene )
: m_pSprReady		( nullptr	)
, m_pSprGo			( nullptr	)
, m_nStep			( -1		)
, m_dStepTime		( 0			)
, m_dStepLeftTime	( 0			)
{
	// sprite
	m_pSprReady	= new cSprite ( "ready.png"	, eLayerGame_ReadyGO, Point ( _SCR_W_HARP, _SCR_H_HARP ) );
	m_pSprGo	= new cSprite ( "go.png"	, eLayerGame_ReadyGO, Point ( _SCR_W_HARP, _SCR_H_HARP ) );
	
	m_pSprReady	->setAnchorPoint ( Point ( 0.5f, 0.5f ) );
	m_pSprGo	->setAnchorPoint ( Point ( 0.5f, 0.5f ) );
	
	m_pSprReady	->setIsVisible ( false );
	m_pSprGo	->setIsVisible ( false );
	
	pScene->addDrawObject ( m_pSprReady	);
	pScene->addDrawObject ( m_pSprGo	);
}

cStartDirector::~cStartDirector ( KDvoid )
{
	// sprite
	m_pSprReady	= nullptr;
	m_pSprGo	= nullptr;
}

KDvoid cStartDirector::start ( KDvoid )
{
	m_nStep			= 0;
	m_dStepTime		= 0.5f;
	m_dStepLeftTime	= m_dStepTime;
}

KDvoid cStartDirector::stop ( KDvoid )
{
	m_nStep = -1;
}

KDvoid cStartDirector::update ( KDdouble dLeftTime, KDbool& bIsEnd )
{
	// init
	bIsEnd = false;
	
	// end check
	if ( m_nStep == -1 )
	{
		bIsEnd = true;
		return;
	}
	
	if ( m_nStep == 0 )
	{
		// time
		m_dStepLeftTime -= dLeftTime;
		
		// time check
		if ( m_dStepLeftTime < 0 )
		{
			m_dStepTime		= 1;
			m_dStepLeftTime	= m_dStepTime;

			m_pSprReady	->setIsVisible ( true );
			m_pSprGo	->setIsVisible ( false );
			
			m_pSprReady	->setScale ( 1 );
			m_pSprGo	->setScale ( 1 );
			
			++m_nStep;
		}
	}
	else if ( m_nStep == 1 )
	{
		// time
		m_dStepLeftTime -= dLeftTime;
		
		// time check
		if(m_dStepLeftTime < 0)
		{
			m_pSprReady	->setIsVisible ( false );
			m_pSprGo	->setIsVisible ( true );
			
			m_pSprReady	->setScale ( 1 );
			m_pSprGo	->setScale ( 0 );

			m_dStepTime		= 1;
			m_dStepLeftTime	= m_dStepTime;

			++m_nStep;
		}
		else
		{
			// percent
			KDfloat		fPercent = ( m_dStepTime - m_dStepLeftTime ) / m_dStepTime;
			
			// scale
				 if ( fPercent < 0.1f )	m_pSprReady->setScale ( 1 +    ( 0.3f / 0.1f ) * fPercent );
			else if ( fPercent < 0.2f )	m_pSprReady->setScale ( 1.3f - ( 0.3f / 0.1f ) * ( fPercent - 0.1f ) );
			else						m_pSprReady->setScale ( 1 );
		}
	}
	else if ( m_nStep == 2 )
	{
		// time
		m_dStepLeftTime -= dLeftTime;
		
		// time check
		if ( m_dStepLeftTime < 0 )
		{
			m_pSprReady	->setIsVisible ( false );
			m_pSprGo	->setIsVisible ( false );

			m_dStepTime		= 0.6f;
			m_dStepLeftTime	= m_dStepTime;

			++m_nStep;
		}
		else
		{
			// percent
			KDfloat		fPercent = ( m_dStepTime - m_dStepLeftTime ) / m_dStepTime;
			
			// scale
				 if ( fPercent < 0.2f )	m_pSprGo->setScale ( ( 1.1f / 0.2f ) * fPercent );
			else if ( fPercent < 0.3f )	m_pSprGo->setScale ( 1.1f - ( 0.1f / 0.1f ) * ( fPercent - 0.2f ) );
			else						m_pSprGo->setScale ( 1 );
		}
	}
	else if ( m_nStep == 3 )
	{
		// time
		m_dStepLeftTime -= dLeftTime;
		
		// time check
		if ( m_dStepLeftTime < 0 )
		{
			m_nStep = -1;
		}
	}
}