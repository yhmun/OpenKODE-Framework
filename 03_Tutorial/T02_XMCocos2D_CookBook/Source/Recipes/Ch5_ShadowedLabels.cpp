/* --------------------------------------------------------------------------
 *
 *      File            Ch5_ShadowedLabels.cpp
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
#include "Ch5_ShadowedLabels.h"
#include "Libraries/ShadowLabel.h"

KDbool Ch5_ShadowedLabels::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "" );

	// Draw four different shadowed labels using 4 different fonts 
	CCMenuItemFont::setFontSize ( 47 );
	CCMenuItemFont::setFontName ( "Georgia.ttf" );
	this->label ( "Label 1", ccp ( -120, 50 ), ccc3 ( 0, 50, 255 ), ccc3 ( 0, 200, 255 ), menu_selector ( Ch5_ShadowedLabels::labelTouched ), 1 );

	CCMenuItemFont::setFontSize ( 40 );
	CCMenuItemFont::setFontName ( "Marker Felt.ttf" );
	this->label ( "Label 2", ccp ( 120, 50 ), ccc3 ( 255, 128, 0 ), ccc3 ( 255, 255, 0 ), menu_selector ( Ch5_ShadowedLabels::labelTouched ), 2 );

	CCMenuItemFont::setFontSize ( 45 );
	CCMenuItemFont::setFontName ( "Arial.ttf" );
	this->label ( "Label 3", ccp ( -120, -50 ), ccc3 ( 0, 128, 0 ), ccc3 ( 0, 255, 0 ), menu_selector ( Ch5_ShadowedLabels::labelTouched ), 3 );

	CCMenuItemFont::setFontSize ( 50 );
	CCMenuItemFont::setFontName ( "Courier New.ttf" );
	this->label ( "Label 4", ccp ( 120, -50 ), ccc3 ( 255, 0, 0 ), ccc3 ( 255, 255, 0 ), menu_selector ( Ch5_ShadowedLabels::labelTouched ), 4 );

	return KD_TRUE;
}

// Label creation helper method
KDvoid Ch5_ShadowedLabels::label ( const KDchar* szStr, const CCPoint& tPoint, const ccColor3B& tColor, const ccColor3B& tActiveColor, SEL_MenuHandler pSelector, KDint nTag )
{
	ShadowLabel*	pLabel = ShadowLabel::create ( szStr, this, pSelector );
	pLabel->setPosition ( tPoint );
	pLabel->setColor ( tColor );
	pLabel->setActiveColor ( tActiveColor );
	pLabel->setTag ( nTag );
	
	CCMenu*			pMenu = CCMenu::create ( pLabel->getShadow ( ), pLabel, KD_NULL );
	pMenu->setPosition ( ccp ( 240, 160 ) );
	this->addChild ( pMenu );
}

// Label touch callback
KDvoid Ch5_ShadowedLabels::labelTouched ( CCObject* pSender )
{
	ShadowLabel*	pLabel = (ShadowLabel*) pSender;
	this->showMessage ( ccszf ( "Pressed label %d", pLabel->getTag ( ) ) );
}
