/* --------------------------------------------------------------------------
 *
 *      File            Ch3_ReadingPlistFiles.cpp
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
#include "Ch3_ReadingPlistFiles.h"
#include "Libraries/TexturedPolygon.h"

KDbool Ch3_ReadingPlistFiles::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Initialize a read-only dictionary from our file
	CCDictionary*	pDict = CCDictionary::createWithContentsOfFile ( "scene1_xml.plist" );
	
	// Process this dictionary
	this->processMap ( pDict );

	return KD_TRUE;
}

KDvoid Ch3_ReadingPlistFiles::processMap ( CCDictionary* pDict )
{
	// Loop through all dictionary nodes to process individual types
	CCArray*	pNodes  = (CCArray*) pDict->objectForKey ( "nodes" );
	CCObject*	pObject = KD_NULL;

	CCARRAY_FOREACH ( pNodes, pObject ) 
	{
		CCDictionary*	pNode = (CCDictionary*) pObject;
		CCString*		pType = (CCString*) pNode->objectForKey ( "type" );

		if ( !pType->compare ( "spriteFile" ) )
		{
			this->processSpriteFile ( pNode );
		}
		else if ( !pType->compare ( "texturedPolygon" ) )
		{
			this->processTexturedPolygon ( pNode );
		}
	}
}

KDvoid Ch3_ReadingPlistFiles::processSpriteFile ( CCDictionary* pNode )
{
	// Init the sprite
	const KDchar*		szFile  = ( (CCString*) pNode->objectForKey ( "file" ) )->getCString ( );
	CCSprite*			pSprite = CCSprite::create ( szFile );
	
	// Set sprite position
	CCDictionary*		pPosDict = (CCDictionary*) pNode->objectForKey ( "position" );
	pSprite->setPosition 
	(
		ccp ( ( (CCString*) pPosDict->objectForKey ( "x" ) )->floatValue ( ), ( (CCString*) pPosDict->objectForKey ( "y" ) )->floatValue ( ) )
	);

	// Each numeric value is an CCString that must be cast into a float
	CCString*	pScale = (CCString*) pPosDict->objectForKey ( "scale" );
	if ( pScale )
	{
		pSprite->setScale ( pScale->floatValue ( ) );
	}
	
	// Set the anchor point so objects are positioned from the bottom-up
	pSprite->setAnchorPoint ( ccp ( 0.5f, 0 ) );
		
	// We set the sprite Z according to its Y to produce an isometric perspective
	KDfloat		fZOrder = this->getZFromY ( pSprite->getPositionY ( ) );
	CCString*	pZOrder = (CCString*) pPosDict->objectForKey ( "z" );
	if ( pZOrder )
	{
		fZOrder = pZOrder->floatValue ( );
	}
	
	// Finally, add the sprite	
	this->addChild ( pSprite, (KDint) fZOrder ); 
}

KDvoid Ch3_ReadingPlistFiles::processTexturedPolygon ( CCDictionary* pNode )
{
	// Process vertices
	CCArray*	pVertices = CCArray::create ( );
	CCArray*	pVertexData = (CCArray*) pNode->objectForKey ( "vertices" );
	CCObject*	pObject;

	CCARRAY_FOREACH ( pVertexData, pObject )
	{
		CCDictionary*	pData = (CCDictionary*) pObject;

		KDfloat			x = ( (CCString*) pData->objectForKey ( "x" ) )->floatValue ( );
		KDfloat			y = ( (CCString*) pData->objectForKey ( "y" ) )->floatValue ( );

		pVertices->addObject ( CCPointValue::create ( ccp ( x, y ) ) );
	}

	const KDchar*		szFile = ( (CCString*) pNode->objectForKey ( "file" ) )->getCString ( );
	ccTexParams			tParams = { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, GL_REPEAT };
	TexturedPolygon*	pTexturedPoly = TexturedPolygon::create ( szFile, pVertices );
	pTexturedPoly->getTexture ( )->setTexParameters ( &tParams );
	
	// Set position
	CCDictionary*		pPosDict = (CCDictionary*) pNode->objectForKey ( "position" );
	pTexturedPoly->setPosition 
	(
		ccp ( ( (CCString*) pPosDict->objectForKey ( "x" ) )->floatValue ( ), ( (CCString*) pPosDict->objectForKey ( "y" ) )->floatValue ( ) )
	);

	// Add the texturedPolygon behind any sprites
	this->addChild ( pTexturedPoly, 0 ); 
}

/// Our simple method used to order sprites by depth 
KDfloat Ch3_ReadingPlistFiles::getZFromY ( KDfloat y )
{
	return 320 - y;
}