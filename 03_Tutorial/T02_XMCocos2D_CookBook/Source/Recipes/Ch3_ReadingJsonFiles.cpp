/* --------------------------------------------------------------------------
 *
 *      File            Ch3_ReadingJsonFiles.cpp
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
#include "Ch3_ReadingJsonFiles.h"
#include "Libraries/TexturedPolygon.h"

KDbool Ch3_ReadingJsonFiles::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Initialize a read-only dictionary from our file
	KDsize			uSize   = 0;
	KDubyte*		pBuffer = CCFileUtils::sharedFileUtils ( )->getFileData ( "scene2.json", "rb", &uSize );
	std::string		sBuffer = (KDchar*) pBuffer;
	CC_SAFE_DELETE_ARRAY ( pBuffer );

	Json::Value		tRoot;
	KDbool			bRet = Json::Reader ( ).parse ( sBuffer, tRoot );
	if ( bRet )
	{
		this->processMap ( tRoot );
	}

	return KD_TRUE;
}

KDvoid Ch3_ReadingJsonFiles::processMap ( const Json::Value& tRoot )
{
	const Json::Value&	tNodes = tRoot [ "nodes" ];
	
	// Loop through all json nodes to process individual types
	for ( KDuint i = 0; i < tNodes.size ( ); i++ )
	{
		const Json::Value&	tNode = tNodes [ i ];
		const KDchar*		szType = tNode [ "type" ].asCString ( );

		if ( !kdStrcmp ( szType, "spriteFile" ) )
		{
			this->processSpriteFile ( tNode );
		}
		else if ( !kdStrcmp ( szType, "texturedPolygon" ) )
		{	
			this->processTexturedPolygon ( tNode );
		}
	}
}

/// Process the 'spriteFile' type 
KDvoid Ch3_ReadingJsonFiles::processSpriteFile ( const Json::Value& tNode )
{
	// Init the sprite
	CCSprite*	pSprite = CCSprite::create ( tNode [ "file" ].asCString ( ) );
	
	// Set sprite position
	pSprite->setPosition ( ccp ( tNode [ "position" ][ "x" ].asDouble ( ), tNode [ "position" ][ "y" ].asDouble ( ) ) );

	// Each numeric value is an NSString or NSNumber that must be cast into a float
	pSprite->setScale ( (KDfloat) tNode [ "scale" ].asDouble ( ) ); 

	// Set the anchor point so objects are positioned from the bottom-up
	pSprite->setAnchorPoint ( ccp ( 0.5, 0 ) );

	// We set the sprite Z according to its Y to produce an isometric perspective
	KDfloat		fZOrder = this->getZFromY ( pSprite->getPositionY ( ) );
	if ( !tNode [ "z" ] == KD_FALSE )
	{
		fZOrder = (KDfloat) tNode [ "z" ].asDouble ( );
	}

	// Finally, add the sprite
	this->addChild ( pSprite, (KDint) fZOrder );
}

/// Process the 'texturedPolygon' type 
KDvoid Ch3_ReadingJsonFiles::processTexturedPolygon ( const Json::Value& tNode )
{
	// Process vertices
	CCArray*				pVertices = CCArray::create ( );
	const Json::Value&		tVertexData = tNode [ "vertices" ];

	for ( KDuint i = 0; i < tVertexData.size ( ); i++ )
	{
		const Json::Value&	tVData = tVertexData [ i ];

		KDdouble	x = tVData [ "x" ].asDouble ( );
		KDdouble	y = tVData [ "y" ].asDouble ( );

		pVertices->addObject ( CCPointValue::create ( ccp ( x, y ) ) );
	}
	
	// Init our textured polygon
	ccTexParams			tParams = { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, GL_REPEAT };

	TexturedPolygon*	pTexturedPoly = TexturedPolygon::create ( tNode [ "file" ].asCString ( ), pVertices );
	pTexturedPoly->getTexture ( )->setTexParameters ( tParams );

	// Set position
	pTexturedPoly->setPosition ( ccp ( tNode [ "position" ][ "x" ].asDouble ( ), tNode [ "position" ][ "y" ].asDouble ( ) ) );

	// Add the texturedPolygon behind any sprites
	this->addChild ( pTexturedPoly, 0 );
}

/// Our simple method used to order sprites by depth 
KDfloat Ch3_ReadingJsonFiles::getZFromY ( KDfloat y )
{
	return 320 - y;
}