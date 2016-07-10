/* -----------------------------------------------------------------------------------
 *
 *      File            NodeChangeImgs.cpp
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
#include "NodeChangeImgs.h"
#include "MyData/MyData.h"

cNodeChangeImgs::cNodeChangeImgs ( Node* pNode, KDbool bIsEnlarge )
: m_pNode			( pNode			)
, m_pSprite			( nullptr		)
, m_bIsEnlarge		( bIsEnlarge	)
, m_fScale			( 0				)
, m_bIsSound		( false			)
, m_dSoundLeftTime	( 0.15f			)
{
	m_pSprite = new cSprite ( m_pNode, ccszf ( "NodeChange/node_chage_%d.png", kdRand ( ) % 3 ), LAYER_NODE_CHAGE, Point ( 0.5f, 0.5f ), Point ( _SCR_W_HARP, _SCR_H_HARP ) );

	if ( m_bIsEnlarge == true )
	{
		m_fScale = 0;
	}
	else
	{
		m_fScale = 20;
	}

	m_pSprite->get ( )->setScale ( m_fScale );
}

cNodeChangeImgs::~cNodeChangeImgs ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pSprite );
}

KDbool cNodeChangeImgs::update ( KDdouble dLeftTime )
{
	if ( m_bIsSound == false )
	{
		m_dSoundLeftTime -= dLeftTime;

		if ( m_dSoundLeftTime < 0 )
		{
			m_bIsSound = true;

			if ( cMyData::getObj ( )->m_bIsOptionSound != false )
			{
				SimpleAudioEngine::getInstance ( )->playEffect ( "move_page.wav" );
			}
		}
	}

	if ( m_bIsEnlarge == true )
	{
		m_fScale += dLeftTime * 8 + dLeftTime * m_fScale * 4;

		if ( m_fScale > 20 )
		{
			m_fScale = 20;
			m_pSprite->get ( )->setScale ( m_fScale );

			return true;
		}
	}
	else
	{
		m_fScale -= dLeftTime * 8 + dLeftTime * m_fScale * 4;

		if ( m_fScale < 0 )
		{
			m_fScale = 0;
			m_pSprite->get ( )->setScale ( m_fScale );

			return true;
		}
	}

	m_pSprite->get ( )->setScale ( m_fScale );

	return false;
}
