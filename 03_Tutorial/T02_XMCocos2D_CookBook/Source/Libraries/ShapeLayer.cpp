/* --------------------------------------------------------------------------
 *
 *      File            ShapeLayer.cpp
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
#include "ShapeLayer.h"

ShapeLayer* ShapeLayer::create ( KDvoid )
{
	ShapeLayer*		pRet = new ShapeLayer ( );

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

KDvoid ShapeLayer::draw ( KDvoid )
{	
	// Set line width.
	ccLineWidth ( 4.0f );
	
	// Set point size
	ccPointSize ( 16 );
		
	// Draw a blue quadratic bezier curve
	ccDrawColor4B ( 0, 0, 255, 255 );
	ccDrawQuadBezier ( ccp ( 100, 0 ), ccp ( 240, 70 ), ccp ( 380, 0 ), 10 );
		
	// Draw a hollow purple circle
	ccDrawColor4B ( 255, 0, 255, 255 );
	ccDrawCircle ( ccp ( 240, 160 ), 125.0f, 0.0f, 100, KD_FALSE );
		
	// Draw a solid red lines
	ccDrawColor4B ( 255, 0, 0, 255 );
	ccDrawLine ( ccp ( 170, 220 ), ccp ( 220, 190 ) );
	ccDrawLine ( ccp ( 260, 190 ), ccp ( 310, 220 ) );
	
	// Draw a green point
	ccDrawColor4B ( 0, 255, 0, 255 );
	ccDrawPoint ( ccp ( 200, 180 ) );
	ccDrawPoint ( ccp ( 280, 180 ) );
	
	// Draw a turquoise solid circle
	ccDrawColor4B ( 0, 128, 255, 50 );
	ccDrawSolidCircle ( ccp ( 200, 180 ), 25.0f, 0.0f, 20, KD_FALSE );
	ccDrawSolidCircle ( ccp ( 280, 180 ), 25.0f, 0.0f, 20, KD_FALSE );
	
	// Draw a brown hollow circle
	ccDrawColor4B ( 64, 32, 0, 255 );
	ccDrawCircle ( ccp ( 200, 180 ), 25.0f, 0.0f, 100, KD_FALSE );
	ccDrawCircle ( ccp ( 280, 180 ), 25.0f, 0.0f, 100, KD_FALSE );
	
	//Draw brown lines
	ccDrawColor4B ( 64, 32, 0, 255 );
	ccDrawLine ( ccp ( 225, 180 ), ccp ( 255, 180 ) );
	ccDrawLine ( ccp ( 305, 180 ), ccp ( 370, 160 ) );
	ccDrawLine ( ccp ( 175, 180 ), ccp ( 110, 160 ) );
	
	// Draw an orange polygon
	ccDrawColor4B ( 255, 128, 0, 255 );
	CCPoint		aVertices [ 5 ] = 
	{
		ccp ( 230, 150 ),
		ccp ( 240, 160 ),
		ccp ( 250, 150 ),
		ccp ( 245, 140 ),
		ccp ( 235, 140 ) 
	};
	ccDrawPoly ( aVertices, 5, KD_TRUE );

	// Draw a yellow cubic bezier curve
	ccDrawColor4B ( 255, 255, 0, 255 );
	ccDrawCubicBezier ( ccp ( 170, 90 ), ccp ( 220, 150 ), ccp ( 260, 50 ), ccp ( 320, 100 ), 10 );
	
	// Restore original values
	ccLineWidth ( 1 );	
	ccDrawColor4B ( 255, 255, 255, 255 );
	ccPointSize ( 1 );		
}