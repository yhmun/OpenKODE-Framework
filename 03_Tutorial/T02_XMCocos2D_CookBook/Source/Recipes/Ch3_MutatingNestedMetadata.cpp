/* --------------------------------------------------------------------------
 *
 *      File            Ch3_MutatingNestedMetadata.cpp
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
#include "Ch3_MutatingNestedMetadata.h"
#include "Libraries/CCJson.h"

KDbool Ch3_MutatingNestedMetadata::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	m_pMessage->setString ( "" );

	// Initialize a read-only dictionary from our file
	m_pDictMutable = CCJson::create ( "data_to_mutate.json" );
	m_pDictMutable->retain ( );

	// Show JSON data
	this->showJsonData ( m_pDictMutable );

	// Add randomize button
	CCMenuItemFont::setFontSize ( 30 );

	CCMenuItemFont*		pRandomizeItem = CCMenuItemFont::create ( "Randomize Data", this, menu_selector ( Ch3_MutatingNestedMetadata::randomizeData ) );
	CCMenu*				pMenu = CCMenu::create ( pRandomizeItem, KD_NULL );
	pMenu->setPosition ( ccp ( 240, 140 ) );
	this->addChild ( pMenu, 1 );	

	return KD_TRUE;
}

KDvoid Ch3_MutatingNestedMetadata::onExit ( KDvoid )
{
	Recipe::onExit ( );
}

KDvoid Ch3_MutatingNestedMetadata::showJsonData ( CCDictionary* pDict )
{
	this->showMessage ( "" );

	// Loop through all dictionary nodes to process individual types
	CCArray*	pNodes  = (CCArray*) pDict->objectForKey ( "people" );
	CCObject*	pObject = KD_NULL;
	CCARRAY_FOREACH ( pNodes, pObject )
	{
		CCDictionary*	pNode = (CCDictionary*) pObject;

		KDint			nHeight = ( (CCString*) pNode->objectForKey ( "height" ) )->intValue ( );
		KDint			nWeight = ( (CCString*) pNode->objectForKey ( "weight" ) )->intValue ( );
		const KDchar*	szName	= ( (CCString*) pNode->objectForKey ( "name" ) )->getCString ( );

		this->appendMessage ( ccszf ( "%s: %din %dlbs", szName, nHeight, nWeight ) );
	}
}

KDvoid Ch3_MutatingNestedMetadata::randomizeData ( CCObject* pSender )
{
	// Randomize some data in 'dictMutable'
	CCArray*	pNodes  = (CCArray*) m_pDictMutable->objectForKey ( "people" );
	CCObject*	pObject = KD_NULL;
	CCARRAY_FOREACH ( pNodes, pObject )
	{
		CCDictionary*	pNode = (CCDictionary*) pObject;

		( (CCString*) pNode->objectForKey ( "height" ) )->initWithFormat ( "%d", kdRand ( ) % 48 + 30 );
		( (CCString*) pNode->objectForKey ( "weight" ) )->initWithFormat ( "%d", kdRand ( ) % 100 + 100 );
	}

	this->showJsonData ( m_pDictMutable );
}