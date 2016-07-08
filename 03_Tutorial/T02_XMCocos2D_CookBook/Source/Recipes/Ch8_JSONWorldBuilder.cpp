/* --------------------------------------------------------------------------
 *
 *      File            Ch8_JSONWorldBuilder.cpp
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
#include "Ch8_JSONWorldBuilder.h"
#include "Libraries/CCJson.h"
#include "Libraries/TexturedPolygon.h"

KDbool Ch8_JSONWorldBuilder::init ( KDvoid )
{	
	// Load our map file
	this->loadMap ( "world.json" );

	return KD_TRUE;
}

KDvoid Ch8_JSONWorldBuilder::onExit ( KDvoid )
{
	CC_SAFE_DELETE ( m_pLineVerticesA );
	CC_SAFE_DELETE ( m_pLineVerticesB );
	CC_SAFE_DELETE ( m_pPoints );

	GameArea2D::onExit ( );
}

/// Draw our lines and our points 
KDvoid Ch8_JSONWorldBuilder::drawLayer ( KDvoid )
{
	// Draw lines
	ccDrawColor4B ( 255, 0, 0, 255 );

	const CCSize&	tGameSize = m_pGameNode->getContentSize ( );

	for ( KDuint i = 0; i < m_pLineVerticesA->count ( ); i++ )
	{
		CCPoint		tPointA = ( (CCPointValue*) m_pLineVerticesA->objectAtIndex ( i ) )->getValue ( ); 
		CCPoint		tPointB = ( (CCPointValue*) m_pLineVerticesB->objectAtIndex ( i ) )->getValue ( ); 
		
		CCPoint		a = ccp ( tPointA.x - tGameSize.cx / 2, tPointA.y - tGameSize.cy / 2 );
		CCPoint		b = ccp ( tPointB.x - tGameSize.cx / 2, tPointB.y - tGameSize.cy / 2 );
		
		ccDrawLine ( a, b );
	}
	
	// Draw points
	for ( KDuint i = 0; i < m_pPoints->count ( ); i++ )
	{
		CCPoint		tPoint = ( (CCPointValue*) m_pPoints->objectAtIndex ( i ) )->getValue ( ); 
		CCPoint		p = ccp ( tPoint.x - tGameSize.cx / 2, tPoint.y - tGameSize.cy / 2 );
		
		ccDrawCircle ( p, 16.0f, 0, 16, KD_FALSE );
	}
}

// Called after the map has been loaded into a container but before assets have been loaded
KDvoid Ch8_JSONWorldBuilder::finishInit ( KDvoid )
{
	// Superclass initialization and message
	GameArea2D::init ( );

	this->showMessage ( "Scroll to move the camera around the world.\nPinch to zoom." );

	// Add a draw layer for line drawing
	this->addDrawLayer ( );
	
	// Initial variables
	m_fCameraZoom = 0.85f;
	
	// Init line/point containers
	m_pLineVerticesA = new CCArray ( );
	m_pLineVerticesB = new CCArray ( );
	m_pPoints		 = new CCArray ( );
	
	// Center camera
	this->centerCameraOnGameCoord ( ccp ( 0, 0 ) );
}

/// Our load map method 
KDvoid Ch8_JSONWorldBuilder::loadMap ( const KDchar* szMapStr )
{
	CCObject*	pObject;

	// Deserialize the string into a nested CCDictionary
	m_pMapData = CCJson::create ( szMapStr );
	
	//Add all sprite frames for listed plist files
	CCArray*	pPlistFiles = (CCArray*) m_pMapData->objectForKey ( "plistFiles" );
	CCARRAY_FOREACH ( pPlistFiles, pObject )
	{
		CCString*	pPlistFile = (CCString*) pObject;
		CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFramesWithFile ( pPlistFile->getCString ( ) );
	}

	// List of PNG files is also available
//	CCArray*	pPngFiles = (CCArray*) m_pMapData->objectForKey ( "pngFiles" );
		
	// Pre process data
	this->preProcessMapData ( );
	
	//Process map nodes
	CCDictionary*	pMapNodes = (CCDictionary*) m_pMapData->objectForKey ( "mapNodes" ); 
	CCArray*		pMapNodeKeys = pMapNodes->allKeys ( );
	CCARRAY_FOREACH ( pMapNodeKeys, pObject )
	{
		CCString*		pMapNodeKey = (CCString*) pObject;
		CCDictionary*	pMapNode    = (CCDictionary*) pMapNodes->objectForKey ( pMapNodeKey->getCString ( ) ); 		
		CCString*		pNodeType   = (CCString*) pMapNode->objectForKey ( "type" );

		if ( !pNodeType )
		{
			continue;
		}

		const KDchar*	szNodeType  = pNodeType->getCString ( );
		
		// Process node types
		if ( !kdStrcmp ( szNodeType, "sprite" ) )
		{
			this->processSprite ( pMapNode );
		}
		else if ( !kdStrcmp ( szNodeType, "tiledSprite" ) )	
		{
			this->processTiledSprite ( pMapNode );
		}
		else if ( !kdStrcmp ( szNodeType, "line" ) )
		{
			this->processLine ( pMapNode );
		} 
		else if ( !kdStrcmp ( szNodeType, "point" ) )
		{
			this->processPoint ( pMapNode );
		}
	} 
}

KDvoid Ch8_JSONWorldBuilder::preProcessMapData ( KDvoid )
{
	// Set canvasSize and gameAreaSize from map file
	m_tCanvasSize = ccp
	(
		( (CCString*) m_pMapData->objectForKey ( "canvasWidth"  ) )->floatValue ( ),
		( (CCString*) m_pMapData->objectForKey ( "canvasHeight" ) )->floatValue ( ) 
	);
	
	m_tGameAreaSize = ccs ( m_tCanvasSize.x / PTM_RATIO, m_tCanvasSize.y / PTM_RATIO );

	//Finish map initialization
	this->finishInit ( );
}

/// Process a sprite node. This represents a single sprite onscreen 
KDvoid Ch8_JSONWorldBuilder::processSprite ( CCDictionary* pMapNode )
{
	// Get node information
	CCString*	pTexture = (CCString*) pMapNode->objectForKey ( "selectedSpriteY" );
	KDfloat		fOriginX = ( (CCString*) pMapNode->objectForKey ( "originX" ) )->floatValue ( );
	KDfloat		fOriginY = ( (CCString*) pMapNode->objectForKey ( "originY" ) )->floatValue ( );
	KDfloat		fIndexZ	 = ( (CCString*) pMapNode->objectForKey ( "zIndex"  ) )->floatValue ( );
	CCString*	pFlipX	 = (CCString*) pMapNode->objectForKey ( "flipX" );
	CCString*	pFlipY	 = (CCString*) pMapNode->objectForKey ( "flipY" );
	KDfloat		fTag	 = 0;
		
	// Get metadata
	CCDictionary*	pMetaPairs = (CCDictionary*) pMapNode->objectForKey ( "meta" ); 
	CCArray*		pMetaKeys  = pMetaPairs->allKeys ( );
	CCObject*		pObject;

	CCARRAY_FOREACH ( pMetaKeys, pObject )
	{
		CCString*	pMetaKey   = (CCString*) pObject;
//		CCString*	pMetaValue = (CCString*) pMetaPairs->objectForKey ( pMetaKey->getCString ( ) );
		
		// Check for key "tag"
		if ( !kdStrcmp ( pMetaKey->getCString ( ), "tag" ) )
		{
			fTag = 0; //((int)[metaValue dataUsingEncoding:NSUTF8StringEncoding]);
		}
	}
	
	// Either load sprite with file or with sprite frame name
	CCSprite*	pSprite = CCSprite::create ( pTexture->getCString ( ) );
	if ( !pSprite )
	{
		pSprite = CCSprite::createWithSpriteFrameName ( pTexture->getCString ( ) );
	}

	// Set sprite size
	KDfloat		fWidth  = pSprite->getTextureRect ( ).size.cx;
	KDfloat		fHeight = pSprite->getTextureRect ( ).size.cy;
	
	// Set sprite position (some conversion required)
	KDfloat		x = fOriginX - ( m_tCanvasSize.x / 2 );
	KDfloat		y = m_tCanvasSize.y - fOriginY - ( m_tCanvasSize.y / 2 );
	pSprite->setPosition ( ccp ( x + fWidth / 2, y - fHeight / 2 ) );
	
	// Flip sprite if necessary
	if ( !kdStrcmp ( pFlipX->getCString ( ), "Y" ) ) 
	{
		pSprite->setFlipX ( KD_TRUE ); 
	}

	if ( !kdStrcmp ( pFlipY->getCString ( ), "Y" ) )
	{
		pSprite->setFlipY ( KD_TRUE ); 
	}
	
	// Finally, add the sprite
	m_pGameNode->addChild ( pSprite, (KDint) fIndexZ - 24995, (KDint) fTag );
}

/// Process a tiled sprite. 
KDvoid Ch8_JSONWorldBuilder::processTiledSprite ( CCDictionary* pMapNode )
{
	// Get node information
	CCString*		pTexture = (CCString*) pMapNode->objectForKey ( "selectedSpriteY" );
	CCDictionary*	pFrames	 = CCDictionary::create ( );
	KDfloat			fOriginX = ( (CCString*) pMapNode->objectForKey ( "originX" ) )->floatValue ( );
	KDfloat			fOriginY = ( (CCString*) pMapNode->objectForKey ( "originY" ) )->floatValue ( );
	KDfloat			fWidth	 = ( (CCString*) pMapNode->objectForKey ( "width"   ) )->floatValue ( );
	KDfloat			fHeight	 = ( (CCString*) pMapNode->objectForKey ( "height"	) )->floatValue ( );
	KDfloat			fIndexZ	 = ( (CCString*) pMapNode->objectForKey ( "zIndex"	) )->floatValue ( );
	KDfloat			fTag	 = 0;

	// Get metadata
	CCDictionary*	pMetaPairs = (CCDictionary*) pMapNode->objectForKey ( "meta" ); 
	CCArray*		pMetaKeys  = pMetaPairs->allKeys ( );
	CCObject*		pObject;

	CCARRAY_FOREACH ( pMetaKeys, pObject )
	{
		CCString*	pMetaKey   = (CCString*) pObject;
		CCString*	pMetaValue = (CCString*) pMetaPairs->objectForKey ( pMetaKey->getCString ( ) );

		// Check for key "tag" or key "frame" (for animation)
		if ( !kdStrcmp ( pMetaKey->getCString ( ), "tag" ) )
		{
			fTag = 0; //((int)[metaValue dataUsingEncoding:NSUTF8StringEncoding]);
		}
		else if ( !kdStrcmp ( pMetaKey->getCString ( ), "frame" ) )	//"frame"].location != NSNotFound )
		{
			pFrames->setObject ( pMetaValue, pMetaKey->getCString ( ) );
		}
	}
	
	// Get any masks to be applied to this tiled sprite
	CCArray*		pMasks = (CCArray*) pMapNode->objectForKey ( "masks" );
	
	// OpenGL texture parameters
	ccTexParams		tParams = { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, GL_REPEAT };
		
	// If a mask exists, apply it
	if ( pMasks && pMasks->count ( ) > 0 )
	{
		// Create containers
		CCArray*		pVertices	= CCArray::create ( );
		CCArray*		pTriangles	= CCArray::create ( );
	
		// Get mask information
		CCDictionary*	pMask		= (CCDictionary*) pMasks->objectAtIndex ( 0 );
		CCArray*		pPointsX	= (CCArray*) pMask->objectForKey ( "pointsX" );
		CCArray*		pPointsY	= (CCArray*) pMask->objectForKey ( "pointsY" );
		CCArray*		pTriPoints	= (CCArray*) pMask->objectForKey ( "triPoints" );

		// Set vertices
		for ( KDuint i = 0; i < pPointsX->count ( ); i += 1 )
		{
			CCPoint		tVertex = ccp
			(
						  ( (CCString*) pPointsX->objectAtIndex ( i ) )->floatValue ( ),
				fHeight - ( (CCString*) pPointsY->objectAtIndex ( i ) )->floatValue ( )
			);
			pVertices->addObject ( CCPointValue::create ( tVertex ) );
		}
	
		// Set triangle points
		for ( KDuint i = 0; i < pTriPoints->count ( ); i += 1 )
		{
			CCArray*	pTri = (CCArray*) pTriPoints->objectAtIndex ( i );

			Vector3D*	pVect = Vector3D::create
			(
				( (CCString*) pTri->objectAtIndex ( 0 ) )->floatValue ( ), 
				( (CCString*) pTri->objectAtIndex ( 1 ) )->floatValue ( ), 
				( (CCString*) pTri->objectAtIndex ( 2 ) )->floatValue ( )
			);
			pTriangles->addObject ( pVect );
		}		
					
		// Create TexturedPolygon object
		TexturedPolygon*	pTexPolygon = TexturedPolygon::create ( pTexture->getCString ( ), pVertices, pTriangles );
		pTexPolygon->getTexture ( )->setTexParameters ( &tParams );
		
		// Set position
		KDfloat		x = fOriginX - ( m_tCanvasSize.x / 2 );
		KDfloat		y = m_tCanvasSize.y - fOriginY - ( m_tCanvasSize.y / 2 );
		pTexPolygon->setPosition ( ccp ( x, y - fHeight ) );
			
		// If meta key "frames" is specified apply some animation
		if ( pFrames->count ( ) > 0 )
		{			
			// Create container for frames
			CCArray*	pAnimFrames = CCArray::create ( );
					
			// Create sprite frames from other textures
			for ( KDuint f = 0; f < pFrames->count ( ); f = f + 1 ) 
			{
				const KDchar*	szKey = ccszf ( "frame0%d", f );
				
				if ( f > 9 )
				{
					szKey = ccszf ( "frame%d", f );
				}
				
				CCTexture2D*	pFrameTexture = CCTextureCache::sharedTextureCache ( )->addImage ( ( (CCString*) pFrames->objectForKey ( szKey ) )->getCString ( ) );
				pFrameTexture->setTexParameters ( &tParams );
				
				CCSpriteFrame*	pFrame = CCSpriteFrame::createWithTexture ( pFrameTexture, pTexPolygon->getTextureRect ( ) );				
				CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFrame ( pFrame, szKey ); 				
				pAnimFrames->addObject ( pFrame );						
			}
			
			// Create repeated animation with frames
			CCAnimation*		pAnimation = CCAnimation::createWithSpriteFrames ( pAnimFrames, 0.1f );
			pAnimation->setRestoreOriginalFrame ( KD_FALSE );
	
			CCActionInterval*	pAnimate = CCAnimate::create ( pAnimation );
			CCActionInterval*	pRepeatAnimation = CCRepeatForever::create ( pAnimate ); 
			pTexPolygon->runAction ( pRepeatAnimation );			
		}
						
		// Finally, add the node
		m_pGameNode->addChild ( pTexPolygon, (KDint) fIndexZ - 24995 );
	}
	else if ( pFrames->count ( ) > 0 )
	{

		// If we have a non-masked tiled animated sprite 
					
		// Create container for frames
		CCArray*	pAnimFrames = CCArray::create ( );
				
		//Create sprite frames from other textures
		for ( KDuint f = 0; f < pFrames->count ( ); f = f + 1 )
		{
			const KDchar*	szKey = ccszf ( "frame0%d", f );
			
			if ( f > 9 )
			{
				szKey = ccszf ( "frame%d", f );
			}
			
			CCTexture2D*	pFrameTexture = CCTextureCache::sharedTextureCache ( )->addImage ( ( (CCString*) pFrames->objectForKey ( szKey ) )->getCString ( ) );
			pFrameTexture->setTexParameters ( &tParams );
			
			CCSpriteFrame*	pFrame = CCSpriteFrame::createWithTexture ( pFrameTexture, CCRect ( 0, 0, fWidth, fHeight ) );
			CCSpriteFrameCache::sharedSpriteFrameCache ( )->addSpriteFrame ( pFrame, szKey ); 			
			pAnimFrames->addObject ( pFrame );
		}
		
		// Init our sprite with the first frame and add it
		CCSprite*	pSprite = CCSprite::createWithSpriteFrame ( (CCSpriteFrame*) pAnimFrames->objectAtIndex ( 0 ) );
		pSprite->getTexture ( )->setTexParameters ( &tParams );
		
		// Set position
		KDfloat		x = fOriginX - ( m_tCanvasSize.x / 2 );
		KDfloat		y = m_tCanvasSize.y - fOriginY - ( m_tCanvasSize.y / 2 );
		pSprite->setPosition ( ccp ( x + fWidth / 2, y - fHeight / 2 ) );
		
		// Add the node
		m_pGameNode->addChild ( pSprite, (KDint) fIndexZ - 24999 );
		
		// Create repeated animation with frames
		CCAnimation*		pAnimation = CCAnimation::createWithSpriteFrames ( pAnimFrames, 0.1f );
		pAnimation->setRestoreOriginalFrame ( KD_FALSE );

		CCActionInterval*	pAnimate = CCAnimate::create ( pAnimation );
		CCActionInterval*	pRepeatAnimation = CCRepeatForever::create ( pAnimate ); 
		pSprite->runAction ( pRepeatAnimation );	
	}
	else
	{		
		// Use a regular Sprite
		CCSprite*	pSprite = CCSprite::create ( pTexture->getCString ( ), CCRect ( 0, 0, fWidth, fHeight ) );
		pSprite->getTexture ( )->setTexParameters ( &tParams );
		
		// Set position
		KDfloat		x = fOriginX - ( m_tCanvasSize.x / 2 );
		KDfloat		y = m_tCanvasSize.y - fOriginY - ( m_tCanvasSize.y / 2 );
		pSprite->setPosition ( ccp ( x + fWidth / 2, y - fHeight / 2 ) );
		
		// Add the node
		m_pGameNode->addChild ( pSprite, (KDint) fIndexZ - 24999 );	
	}
}

/// Process a line 
KDvoid Ch8_JSONWorldBuilder::processLine ( CCDictionary* pMapNode )
{
	// Get line information
	CCArray*		pDrawnLines = (CCArray*) pMapNode->objectForKey ( "drawnLines" );
	CCDictionary*	pDrawnLine	= (CCDictionary*) pDrawnLines->objectAtIndex ( 0 );
	KDfloat			fFromX		= ( (CCString*) pDrawnLine->objectForKey ( "fromX" ) )->floatValue ( );
	KDfloat			fFromY		= ( (CCString*) pDrawnLine->objectForKey ( "fromY" ) )->floatValue ( );
	KDfloat			fToX		= ( (CCString*) pDrawnLine->objectForKey ( "toX"   ) )->floatValue ( );
	KDfloat			fToY		= ( (CCString*) pDrawnLine->objectForKey ( "toY"   ) )->floatValue ( );

	// Add information to our line containers
	m_pLineVerticesA->addObject ( CCPointValue::create ( ccp ( fFromX, m_tCanvasSize.y - fFromY ) ) );
	m_pLineVerticesB->addObject ( CCPointValue::create ( ccp ( fToX  , m_tCanvasSize.y - fToY   ) ) );
}

/// Process a point 
KDvoid Ch8_JSONWorldBuilder::processPoint ( CCDictionary* pMapNode )
{
	// Get point information
	KDfloat		fOriginX = ( (CCString*) pMapNode->objectForKey ( "originX" ) )->floatValue ( );
	KDfloat		fOriginY = ( (CCString*) pMapNode->objectForKey ( "originY" ) )->floatValue ( );
	fOriginY = m_tCanvasSize.y - fOriginY;

	// If metadata is appropriate, add point to container
	CCDictionary*	pMetaPairs = (CCDictionary*) pMapNode->objectForKey ( "meta" ); 
	CCArray*		pMetaKeys  = pMetaPairs->allKeys ( );
	CCObject*		pObject;

	CCARRAY_FOREACH ( pMetaKeys, pObject )
	{
		const KDchar*	szMetaKey   = ( (CCString*) pObject )->getCString ( );
		const KDchar*	szMetaValue = ( (CCString*) pMetaPairs->objectForKey ( szMetaKey ) )->getCString ( );

		if ( !kdStrcmp ( szMetaKey, "type" ) && !kdStrcmp ( szMetaValue, "example" ) )
		{
			m_pPoints->addObject ( CCPointValue::create ( ccp ( fOriginX, fOriginY ) ) );
		}
	}
}

/// Add draw layer 
KDvoid Ch8_JSONWorldBuilder::addDrawLayer ( KDvoid )
{
	m_pDrawLayer = DrawLayer::create ( );
	m_pDrawLayer->setPosition ( ccp ( 0, 0 ) );
	m_pDrawLayer->setRecipe ( this );
	m_pGameNode->addChild ( m_pDrawLayer, 10 );
}