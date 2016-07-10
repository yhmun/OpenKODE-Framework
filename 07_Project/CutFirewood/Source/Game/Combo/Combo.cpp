/* -----------------------------------------------------------------------------------
 *
 *      File            Combo.cpp
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
#include "Combo.h"
#include "Object/Sprite/Sprite.h"
#include "Object/Label/LabelAtlasCostom.h"

cCombo::cCombo ( JBaseScene* pScene )
: m_pScene		( pScene	)
, m_pSprite		( nullptr	)
, m_pLabel		( nullptr	)
, m_nComboCount	( 0			)
, m_bIsScale	( false		)
, m_bIsScaleAdd	( false		)
, m_fScale		( 0			)
{
	// sprite
	m_pSprite = new cSprite ( "combo.png", eLayerGame_Combo, Point ( 2, 260 ) );
	m_pSprite->setIsVisible ( false );
	pScene->addDrawObject ( m_pSprite );
	
	// label
	m_pLabel = new cLabelAtlasCostom ( "combo_num.png", 31, eLayerGame_Score, Point ( 43, 240 ) );
	m_pLabel->addCharInfo ( '1',   0, 22, 18 );	m_pLabel->addCharInfo ( '2',  24, 35, 28 );	m_pLabel->addCharInfo ( '3',  61, 35, 28 );
	m_pLabel->addCharInfo ( '4',  98, 35, 28 );	m_pLabel->addCharInfo ( '5', 135, 35, 28 );	m_pLabel->addCharInfo ( '6', 173, 35, 28 );
	m_pLabel->addCharInfo ( '7', 210, 35, 28 );	m_pLabel->addCharInfo ( '8', 246, 36, 28 );	m_pLabel->addCharInfo ( '9', 283, 35, 28 );
	m_pLabel->addCharInfo ( '0', 320, 35, 28 );
	
	m_pLabel->setIsVisible ( false );
	m_pLabel->setAnchorPoint ( Point ( 0.5f, 0.5f ) );
	m_pLabel->setString	( "" );
	
	pScene->addDrawObject ( m_pLabel );
}

cCombo::~cCombo ( KDvoid )
{
	m_pSprite = nullptr;
	m_pLabel  = nullptr;
}

KDvoid cCombo::setAdd ( KDvoid )
{
	// count
	++m_nComboCount;
	
	// check
	if ( m_nComboCount <= 1 )
	{
		return;
	}
	
	// sprite
	m_pSprite->setIsVisible ( true );
	
	//label
	m_pLabel->setIsVisible ( true );
	m_pLabel->setString ( String::createWithFormat ( "%d", m_nComboCount )->getCString ( ) );
	Point		tPoint = m_pLabel->getPoint ( );
	if ( 43 - (KDint) ( m_pLabel->getSize ( ).width / 2 ) < 3 )	tPoint.x = 3 + (KDint) ( m_pLabel->getSize ( ).width / 2 );
	else														tPoint.x = 43;
	m_pLabel->setPoint ( tPoint );

	// scale
	m_bIsScale	  = true;
	m_bIsScaleAdd = true;
	m_fScale	  = 1;
}

KDvoid cCombo::setZero ( KDvoid )
{
	// count
	m_nComboCount = 0;
	
	// sprite
	m_pSprite->setIsVisible ( false );
	
	//label
	m_pLabel->setIsVisible ( false );

	// scale
	m_bIsScale	  = false;
	m_bIsScaleAdd = false;
}

KDvoid cCombo::update ( KDdouble dLeftTime )
{
	if ( m_bIsScale == true )
	{
		if ( m_bIsScaleAdd == true )
		{
			m_fScale += dLeftTime * 3;

			if ( m_fScale > 1.5f )
			{
				m_fScale = 1.5f;
				m_bIsScaleAdd = false;
			}
		}
		else
		{
			m_fScale -= dLeftTime * 3;
			
			if ( m_fScale < 1 )
			{
				m_fScale = 1;
				m_bIsScale = false;
			}
		}
		
		m_pLabel->setScale ( m_fScale );
	}
	else
	{
		m_pLabel->setScale ( 1 );
	}
}
