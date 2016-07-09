/* --------------------------------------------------------------------------
 *
 *      File            Ch1_RenderTexturedPolygon.cpp
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
#include "Ch1_RenderTexturedPolygon.h"
#include "Libraries/TexturedPolygon.h"

KDbool Ch1_RenderTexturedPolygon::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}
				
	CCPoint		aVertexArr [ ] =
	{
		ccp ( 248, 340 ), ccp ( 200, 226 ), ccp (  62, 202 ), ccp ( 156, 120 ), ccp ( 134,   2 ), 
		ccp ( 250,  64 ), ccp ( 360,   0 ), ccp ( 338, 128 ), ccp ( 434, 200 ), ccp ( 306, 230 ) 
	};

//	KDint		nNumVerts = 10;
	
	CCArray*	pVertices = CCArray::create ( );
	for ( KDint i = 0; i < 10; i++ )
	{
		pVertices->addObject ( CCPointValue::create ( aVertexArr [ i ] ) );
	}
			
	// Note: Your texture MUST be a product of 2 for this to work.
	ccTexParams			tParams = { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, GL_REPEAT, GL_REPEAT };
	TexturedPolygon*	pTexturedPoly = TexturedPolygon::create ( "bricks.jpg", pVertices );
	pTexturedPoly->getTexture ( )->setTexParameters ( tParams );
	pTexturedPoly->setPosition ( ccp ( 0, 0 ) );
	this->addChild ( pTexturedPoly, 1, 0 );

	return KD_TRUE;
}
