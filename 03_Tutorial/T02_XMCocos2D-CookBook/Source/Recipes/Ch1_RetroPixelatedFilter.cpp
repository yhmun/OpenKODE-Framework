/* --------------------------------------------------------------------------
 *
 *      File            Ch1_RetroPixelatedFilter.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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
#include "Ch1_RetroPixelatedFilter.h"

static const KDchar*	l_aMode [ ] =
{
	"NO SCAN LINES"		,
	"VERT SCAN LINES"	,
	"HORIZ SCAN LINES"	,
	"FULL SCAN LINES"	,
};

KDbool Ch1_RetroPixelatedFilter::init ( KDvoid )
{	
	if ( !Ch1_DrawingSprites::init ( ) )
	{
		return KD_FALSE;
	}

	// Vertical scan lines
	m_pScanLinesVert = CCSprite::create ( "scanlines_vert.png" );
	m_pScanLinesVert->setPosition ( ccp ( 240, 160 ) );
	m_pScanLinesVert->setOpacity ( 120 );
	this->addChild ( m_pScanLinesVert, 3 );
	
	// Horizontal scan lines
	m_pScanLinesHoriz = CCSprite::create ( "scanlines_horiz.png" );
	m_pScanLinesHoriz->setPosition ( ccp ( 240, 160 ) );
	m_pScanLinesHoriz->setOpacity ( 120 );
	this->addChild ( m_pScanLinesHoriz, 3 );

	this->initSubMenus ( sizeof ( l_aMode ) / sizeof ( l_aMode [ 0 ] ) );
	this->nextSubCallback ( );

	return KD_TRUE;
}

const KDchar* Ch1_RetroPixelatedFilter::runSubAction ( KDint nIndex )
{
	KDbool		bScanVert;
	KDbool		bScanHoriz;

	switch ( nIndex )
	{
		case 0 :	bScanVert = KD_FALSE;	bScanHoriz = KD_FALSE;	break;
		case 1 :	bScanVert = KD_TRUE;	bScanHoriz = KD_FALSE;	break;
		case 2 :	bScanVert = KD_FALSE;	bScanHoriz = KD_TRUE;	break;
		case 3 :	bScanVert = KD_TRUE;	bScanHoriz = KD_TRUE;	break;
	}

	m_pScanLinesVert ->setVisible ( bScanVert );
	m_pScanLinesHoriz->setVisible ( bScanHoriz );

	return l_aMode [ nIndex ];
}
