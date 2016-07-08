/* --------------------------------------------------------------------------
 *
 *      File            ShadowLabel.cpp
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
#include "ShadowLabel.h"

ShadowLabel* ShadowLabel::create ( const KDchar* szText, CCObject* pListener, SEL_MenuHandler pSelector )
{
	ShadowLabel*		pRet = new ShadowLabel ( );

	if ( pRet && pRet->initWithText ( szText, pListener, pSelector ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

ShadowLabel* ShadowLabel::create ( const KDchar* szText )
{
	ShadowLabel*		pRet = new ShadowLabel ( );

	if ( pRet && pRet->initWithText ( szText, KD_NULL, KD_NULL ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

ShadowLabel::ShadowLabel ( KDvoid )
{
	m_pShadow = KD_NULL;
}

ShadowLabel::~ShadowLabel ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pShadow );
}

KDbool ShadowLabel::initWithText ( const KDchar* szText, CCObject* pListener, SEL_MenuHandler pSelector )
{
	CCLabelTTF*		pLabel = CCLabelTTF::create ( szText, CCMenuItemFont::getFontName ( ), (KDfloat) CCMenuItemFont::getFontSize ( ) );

	if ( !CCMenuItemLabel::initWithLabel ( pLabel, pListener, pSelector ) )
	{
		return KD_FALSE;
	}

	m_tShadowColor = ccc3 ( 0, 0, 0 );

	this->setShadow ( CCMenuItemFont::create ( szText ) );

	m_pShadow->setColor ( m_tShadowColor );
	m_pShadow->setDisabledColor ( m_tShadowColor );
	m_pShadow->setEnabled ( KD_FALSE );

	return KD_TRUE;
}

KDvoid ShadowLabel::setPosition ( const CCPoint& tPosition )
{
	m_pShadow->setPosition ( ccp ( tPosition.x - 2, tPosition.y - 2 ) );

	CCMenuItemLabel::setPosition ( tPosition );
}

KDvoid ShadowLabel::activate ( KDvoid )
{
	m_pShadow->setEnabled ( KD_TRUE );
	m_pShadow->activate   ( );
	m_pShadow->setEnabled ( KD_FALSE );

	CCMenuItemLabel::activate ( );
}

KDvoid ShadowLabel::selected ( KDvoid )
{
	m_pShadow->setEnabled ( KD_TRUE );
	m_pShadow->selected   ( );
	m_pShadow->setEnabled ( KD_FALSE );
	
	m_tColorBackup = this->getColor ( );
	this->setColor ( m_tActiveColor );

	CCMenuItemLabel::selected ( );
}

KDvoid ShadowLabel::unselected ( KDvoid )
{
	m_pShadow->setEnabled ( KD_TRUE );
	m_pShadow->unselected ( );
	m_pShadow->setEnabled ( KD_FALSE );
	
	this->setColor ( m_tColorBackup );
	
	CCMenuItemLabel::unselected ( );
}

CCMenuItemLabel* ShadowLabel::getShadow ( KDvoid )
{
	return m_pShadow;
}

KDvoid ShadowLabel::setShadow ( CCMenuItemLabel* pShadow )
{
	CC_SAFE_RELEASE ( m_pShadow );
	m_pShadow = pShadow;
	CC_SAFE_RETAIN  ( m_pShadow );
}

const ccColor3B& ShadowLabel::getShadowColor ( KDvoid )
{
	return m_tShadowColor;
}

KDvoid ShadowLabel::setShadowColor ( const ccColor3B& tShadowColor )
{
	m_tShadowColor = tShadowColor;
}

const ccColor3B& ShadowLabel::getActiveColor ( KDvoid )
{
	return m_tActiveColor;
}

KDvoid ShadowLabel::setActiveColor ( const ccColor3B& tActiveColor )
{
	m_tActiveColor = tActiveColor;
}