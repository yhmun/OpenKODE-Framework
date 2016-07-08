/* --------------------------------------------------------------------------
 *
 *      File            Ch1_AnimateTexturedPolygon.cpp
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
#include "Ch1_AnimateTexturedPolygon.h"
#include "Libraries/TexturedPolygon.h"

KDbool Ch1_AnimateTexturedPolygon::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	ccTexParams		tParams = { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, GL_REPEAT };
	
	// Create grass animated textured polygon
	CCPoint			aGrassVertexArr [ ] = 
	{
		ccp (   0,   0 ),
		ccp ( 480,   0 ),
		ccp ( 480, 320 ),
		ccp (   0, 320 ) 
	};
	KDint			nGrassNumVerts = 4;
	CCArray*		pGrassVertices = CCArray::create ( );
	for ( KDint i = 0; i < nGrassNumVerts; i++ )
	{
		pGrassVertices->addObject ( CCPointValue::create ( ccp ( aGrassVertexArr [ i ].x * 1, aGrassVertexArr [ i ].y * 1 ) ) );
	}

	TexturedPolygon*	pGrassPoly = TexturedPolygon::create ( "grass_tile_01.png", pGrassVertices );
	pGrassPoly->getTexture ( )->setTexParameters ( tParams );
	pGrassPoly->setPosition ( ccp ( 0, 0 ) );
	this->addChild ( pGrassPoly, 1, 1 );

	// Create swaying grass animation
	CCArray*	pGrassAnimFrames = CCArray::create ( );

	// This is a two part animation with 'back' and 'forth' frames
	for ( KDint i = 0; i <= 6; i++ )
	{
		pGrassPoly->addAnimFrameWithFile ( ccszf ( "grass_tile_0%d.png", i ), pGrassAnimFrames );
	}

	for ( KDint i = 5; i > 0; i-- )
	{
		pGrassPoly->addAnimFrameWithFile ( ccszf ( "grass_tile_0%d.png", i ), pGrassAnimFrames );
	}

	CCAnimation*	pGrassAnimation = CCAnimation::create ( );
	pGrassAnimation->setDelayPerUnit ( 0.1f );
	pGrassAnimation->setRestoreOriginalFrame ( KD_FALSE );

	for ( KDuint i = 0; i < pGrassAnimFrames->count ( ); i++ )
	{
		pGrassAnimation->addSpriteFrame ( (CCSpriteFrame*) pGrassAnimFrames->objectAtIndex ( i ) );
	}

	pGrassPoly->runAction
	(
		CCRepeatForever::create 
		(
			CCSequence::create 
			(
				CCAnimate::create ( pGrassAnimation ),
				CCDelayTime::create ( 0.0f ),
				KD_NULL
			)				
		)
	);

	return KD_TRUE;
}
