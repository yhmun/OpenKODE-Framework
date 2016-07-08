/* --------------------------------------------------------------------------
 *
 *      File            TestDrawPrimitive.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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
#include "TestDrawPrimitives.h"

TestDrawPrimitives* TestDrawPrimitives::create ( KDvoid )
{
	TestDrawPrimitives*  pLayer = KD_NULL;

	switch ( s_nSubTestIndex )
	{
		case  0 : pLayer = new DrawPrimitives ( ); break;
		case  1 : pLayer = new DrawNodeTest	  ( ); break;
	}

	if ( pLayer && pLayer->init ( ) )
	{
		pLayer->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pLayer );
	}

	return pLayer;
}

KDuint TestDrawPrimitives::count ( KDvoid )
{
	return 2;
}

//------------------------------------------------------------------
//
// DrawPrimitives
//
//------------------------------------------------------------------
const KDchar* DrawPrimitives::subtitle ( KDvoid )
{
	return "Drawing Primitives. Use CCDrawNode instead";
}

KDvoid DrawPrimitives::draw ( KDvoid )
{
	TestBasic::draw ( );

	const CCSize&  tLyrSize = this->getContentSize ( );

 	// draw a simple line
	// The default state is:
	// Line Width: 1
	// color: 255,255,255,255 (white, non-transparent)
	// Anti-Aliased	
	ccDrawLine ( ccp ( 0, 0 ), ccpMid ( tLyrSize ) );
	
	// line: color, width, aliased
	// glLineWidth > 1 and GL_LINE_SMOOTH are not compatible
	// GL_SMOOTH_LINE_WIDTH_RANGE = (1,1) on iPhone
	ccLineWidth ( 5.0f );	
	ccDrawColor4B ( 255, 0, 0, 255 );
	ccDrawLine ( ccp ( 0, tLyrSize.cy ), ccp ( tLyrSize.cx, 0 ) );

	// TIP:
	// If you are going to use always the same color or width, you don't
	// need to call it before every draw
	//
	// Remember: OpenGL is a state-machine.
	
	// draw big point in the center
	ccPointSize ( 64 );	
	ccDrawColor4B ( 0, 0, 255, 128 );
	ccDrawPoint ( ccpMid ( tLyrSize ) );
	
	// draw 4 small points
	CCPoint  aPoints [ ] =
	{
		ccp ( 60, 60 ), 
		ccp ( 70, 70 ),
		ccp ( 60, 70 ),
		ccp ( 70, 60 )
	};
    ccPointSize ( 4 );
    ccDrawColor4B ( 0, 255, 255, 255 );
	ccDrawPoints ( aPoints, 4 );
	
	// draw a green circle with 10 segments
	ccLineWidth ( 16 );	
	ccDrawColor4B ( 0, 255, 0, 255 );
	ccDrawCircle ( ccpMid ( tLyrSize ), 100, 0, 10, KD_FALSE );

	// draw a green circle with 50 segments with line to center
	ccLineWidth ( 2 );	
	ccDrawColor4B ( 0, 255, 255, 255 );
	ccDrawCircle ( ccpMid ( tLyrSize ), 50, CC_DEGREES_TO_RADIANS ( 90 ), 50, KD_TRUE );	
	
	// open yellow poly	
	ccDrawColor4B ( 255, 255, 0, 255 );
	ccLineWidth ( 10 );
	CCPoint  aVertices [] = 
	{
		ccp (   0,   0 ), 
		ccp (  50,  50 ), 
		ccp ( 100,  50 ),
		ccp ( 100, 100 ),
		ccp (  50, 100 ) 
	};
	ccDrawPoly ( aVertices, 5, KD_FALSE );
	
    // filled poly
	ccLineWidth ( 1 );
	CCPoint  aFilledVertices [ ] = 
	{
		ccp (  0, 120 ),
		ccp ( 50, 120 ),
		ccp ( 50, 170 ),
		ccp ( 25, 200 ),
		ccp (  0, 170 )
	};
	ccDrawSolidPoly ( aFilledVertices, 5, ccc4F ( 0.5f, 0.5f, 1, 1 ) );

	// closed purble poly	
	ccDrawColor4B ( 255, 0, 255, 255 );
	ccLineWidth ( 2 );
	CCPoint  aVertices2 [] =
	{ 
		ccp ( 30, 130 ), 
		ccp ( 30, 230 ), 
		ccp ( 50, 200 )
	};
	ccDrawPoly ( aVertices2, 3, KD_TRUE );
	
	// draw quad bezier path
	ccDrawQuadBezier ( ccp ( 0, tLyrSize.cy ), ccpMid ( tLyrSize ), CCPoint ( tLyrSize ), 50 );

	// draw cubic bezier path
	ccDrawCubicBezier ( ccpMid ( tLyrSize ), ccp ( tLyrSize.cx / 2 + 30, tLyrSize.cy / 2 + 50 ), ccp ( tLyrSize.cx / 2 + 60, tLyrSize.cy / 2 - 50 ), ccp ( tLyrSize.cx, tLyrSize.cy / 2 ), 100 );
	
    // draw a solid polygon
    CCPoint  aVertices3 [ ] =
	{
		ccp (  60, 160 ),
		ccp (  70, 190 ),
		ccp ( 100, 190 ),
		ccp (  90, 160 )
	};
    ccDrawSolidPoly ( aVertices3, 4, ccc4F ( 1, 1, 0, 1 ) );

	// restore original values
	ccDrawColor4B ( 255, 255, 255, 255 );
    ccPointSize ( 1 );
	ccLineWidth ( 1 );
}

//------------------------------------------------------------------
//
// DrawNodeTest
//
//------------------------------------------------------------------
const KDchar* DrawNodeTest::subtitle ( KDvoid )
{
	return "Testing DrawNode - batched draws.\nConcave polygons are BROKEN";
}

KDvoid DrawNodeTest::onEnter ( KDvoid )
{
	TestDrawPrimitives::onEnter ( );

    const CCSize&  tSize = this->getContentSize ( );
    
    CCDrawNode*  pDraw = CCDrawNode::create ( );
    this->addChild ( pDraw );
    
    // Draw 10 circles
    for ( KDint i = 0; i < 10; i++ )
    {
        pDraw->drawDot ( ccp ( tSize.cx / 2, tSize.cy / 2 ), 10.f * ( 10 - i ), ccc4F ( CCRANDOM_0_1 ( ), CCRANDOM_0_1 ( ), CCRANDOM_0_1 ( ), 1 ) );
    }
    
    // Draw polygons
    CCPoint  aPoints [ ] = 
	{
		ccp ( tSize.cy / 4, 0 ), 
		ccp ( tSize.cx, tSize.cy / 5 ),
		ccp ( tSize.cx / 3 * 2, tSize.cy ) 
	};
    pDraw->drawPolygon ( aPoints, sizeof ( aPoints ) / sizeof ( aPoints [ 0 ] ), ccc4f ( 1, 0, 0, 0.5f ), 4, ccc4F ( 0, 0, 1, 1 ) );
  
    // star poly (triggers buggs)
	{
		const KDfloat  o = 100;
		const KDfloat  w = 40;
		const KDfloat  h = 100;
		CCPoint  aStar [ ] = 
		{
			ccp ( o + w        , o - h ), ccp ( o + w * 2, o         ),		// lower spike
			ccp ( o + w * 2 + h, o + w ), ccp ( o + w * 2, o + w * 2 ),		// right spike
		};

		pDraw->drawPolygon ( aStar, sizeof ( aStar ) / sizeof ( aStar [ 0 ] ), ccc4F ( 0, 1, 0, 0.5f ), 1, ccc4F ( 0, 0, 1, 1 ) );
	}
    
    // star poly (doesn't trigger bug... order is important un tesselation is supported.    
	{
		const KDfloat  o = 180;
		const KDfloat  w = 40;
		const KDfloat  h = 100;
		
		CCPoint  aStar [ ] = 
		{
			ccp ( o            , o             ), ccp ( o + w    , o - h     ), ccp ( o + w * 2, o ),		// lower spike
			ccp ( o + w * 2 + h, o + w         ), ccp ( o + w * 2, o + w * 2 ),								// right spike
			ccp ( o + w        , o + w * 2 + h ), ccp ( o        , o + w * 2 ),								// top spike
			ccp ( o - h        , o + w         ),															// left spike
		};

		pDraw->drawPolygon ( aStar, sizeof ( aStar ) / sizeof ( aStar [ 0 ] ), ccc4F ( 1, 0, 0, 0.5f ), 1, ccc4F ( 0, 0, 1, 1 ) );
	}
    
    // Draw segment
    pDraw->drawSegment ( ccp ( 20, tSize.cy ), ccp ( 20, tSize.cy / 2 ), 10, ccc4F ( 0, 1, 0, 0.5f ) );

    pDraw->drawSegment ( ccp ( 10, tSize.cy / 2 ), ccp ( tSize.cx / 2 - 100.f, tSize.cy / 2 ), 40, ccc4F ( 1, 0, 1, 0.5f ) );
	
}
