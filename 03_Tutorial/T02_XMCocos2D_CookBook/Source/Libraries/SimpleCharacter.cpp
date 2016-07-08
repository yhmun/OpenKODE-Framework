/* --------------------------------------------------------------------------
 *
 *      File            SimpleCharacter.cpp
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
#include "SimpleCharacter.h"

SimpleCharacter* SimpleCharacter::create ( KDvoid )
{
	SimpleCharacter*	pRet = new SimpleCharacter ( );

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

KDbool SimpleCharacter::init ( KDvoid )
{
	m_sCharColor	= "Red";
	m_sCharClass	= "Wizard";
	m_nStrength		= 10;
	m_nDexterity	= 10;
	m_nConstitution = 10;
	m_nIntelligence	= 10;
	m_nWisdom		= 10;
	m_nCharisma		= 10;

	return KD_TRUE;
}

KDvoid SimpleCharacter::addCharacterToNode ( CCNode* pNode, const CCPoint& tPosition )
{
	ccColor3B	tColor;
	
	if ( !m_sCharColor.compare ( "Red" ) )
	{
		tColor = ccc3 ( 255, 0, 0 );
	}
	else if ( !m_sCharColor.compare ( "Blue" ) )
	{
		tColor = ccc3 ( 0, 0, 255 );
	}
	else if ( !m_sCharColor.compare ( "Green" ) )
	{
		tColor = ccc3 ( 0, 255, 0 );
	}
	else if ( !m_sCharColor.compare ( "Yellow" ) ) 
	{
		tColor = ccc3 ( 255, 255, 0 );
	}
	else if ( !m_sCharColor.compare ( "Orange" ) )
	{
		tColor = ccc3 ( 255, 150, 0 );
	}
	else if ( !m_sCharColor.compare ( "Purple" ) )
	{
		tColor = ccc3 ( 175, 0, 255 );
	}

	CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( "dnd_characters.plist" );
	
	CCSprite*	pDrawing = CCSprite::createWithSpriteFrameName ( ccszf ( "dnd_%s_drawing.png", m_sCharClass.c_str ( ) ) ); 
	CCSprite*	pColors  = CCSprite::createWithSpriteFrameName ( ccszf ( "dnd_%s_colors.png" , m_sCharClass.c_str ( ) ) ); 

	pDrawing->setPosition ( tPosition );
	pColors ->setPosition ( tPosition );
	
	pDrawing->setScale ( 1.5f );
	pColors ->setScale ( 1.5f );
	
	pColors ->setColor ( tColor );
	
	pNode->addChild ( pColors , 0, 0 );
	pNode->addChild ( pDrawing, 1, 1 );
}

const KDchar* SimpleCharacter::getCharColor ( KDvoid )
{
	return m_sCharColor.c_str ( );
}

KDvoid SimpleCharacter::setCharColor ( const KDchar* szText )
{
	m_sCharColor = szText;
}

const KDchar* SimpleCharacter::getCharClass ( KDvoid )
{
	return m_sCharClass.c_str ( );
}

KDvoid SimpleCharacter::setCharClass ( const KDchar* szText )
{
	m_sCharClass = szText;
}

KDint SimpleCharacter::getStrength ( KDvoid )
{
	return m_nStrength;
}

KDvoid SimpleCharacter::setStrength ( KDint nValue )
{
	m_nStrength = nValue;
}

KDint SimpleCharacter::getDexterity ( KDvoid )
{
	return m_nDexterity;
}

KDvoid SimpleCharacter::setDexterity ( KDint nValue )
{
	m_nDexterity = nValue;
}

KDint SimpleCharacter::getConstitution ( KDvoid )
{
	return m_nConstitution;
}

KDvoid SimpleCharacter::setConstitution ( KDint nValue )
{
	m_nConstitution = nValue;
}

KDint SimpleCharacter::getIntelligence ( KDvoid )
{
	return m_nIntelligence;
}

KDvoid SimpleCharacter::setIntelligence ( KDint nValue )
{
	m_nIntelligence = nValue;
}

KDint SimpleCharacter::getWisdom ( KDvoid )
{
	return m_nWisdom;
}

KDvoid SimpleCharacter::setWisdom ( KDint nValue )
{
	m_nWisdom = nValue;
}

KDint SimpleCharacter::getCharisma ( KDvoid )
{
	return m_nCharisma;
}

KDvoid SimpleCharacter::setCharisma ( KDint nValue )
{
	m_nCharisma = nValue;
}