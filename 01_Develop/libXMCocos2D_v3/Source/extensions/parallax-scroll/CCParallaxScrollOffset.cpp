/* -----------------------------------------------------------------------------------
 *
 *      File            CCParallaxScrollOffset.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      Created by Thomas Broquist on 3/26/12. 
 *      Copyright (c) 2012      Little Wins LLC. All rights reserved.
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *      
 *      The above copyright notice and this permission notice shall be included in
 *       all copies or substantial portions of the Software.
 *      
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "extensions/parallax-scroll/CCParallaxScrollOffset.h"

NS_CC_EXT_BEGIN

ParallaxScrollOffset* ParallaxScrollOffset::create ( Node* pNode, const Point& tRatio, const Point& tPosition, const Point& tOffset )
{
	ParallaxScrollOffset*	 pRet = new ParallaxScrollOffset ( );
	
	if ( pRet && pRet->initWithNode ( pNode, tRatio, tPosition, tOffset ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

ParallaxScrollOffset* ParallaxScrollOffset::create ( Node* pNode, const Point& tRatio, const Point& tPosition, const Point& tOffset, const Point& tVelocity )
{
	ParallaxScrollOffset*	 pRet = new ParallaxScrollOffset ( );
	
	if ( pRet && pRet->initWithNode ( pNode, tRatio, tPosition, tOffset, tVelocity ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

ParallaxScrollOffset* ParallaxScrollOffset::create ( KDvoid )
{
	ParallaxScrollOffset*	 pRet = new ParallaxScrollOffset ( );
	
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

KDbool ParallaxScrollOffset::initWithNode ( Node* pNode, const Point& tRatio, const Point& tPosition, const Point& tOffset )
{
	return this->initWithNode ( pNode, tRatio, tPosition, tOffset, Point::ZERO );
}

KDbool ParallaxScrollOffset::initWithNode ( Node* pNode, const Point& tRatio, const Point& tPosition, const Point& tOffset, const Point& tVelocity )
{
	if ( !Layer::init ( ) )
	{
		return KD_FALSE;
	}

    this->setRatio		  ( tRatio );
    this->setScrollOffset ( tOffset );
    this->setRelVelocity  ( tVelocity );
    this->setOrigPosition ( tPosition );    

	pNode->setPosition	  ( tPosition );
    pNode->setAnchorPoint ( Point::ZERO );
    this->setTheChild     ( pNode );

	return KD_TRUE;
}

KDbool ParallaxScrollOffset::init ( KDvoid )
{
	if ( !Layer::init ( ) )
	{
		return KD_FALSE;
	}

    this->setRatio		  ( Point::ZERO );
    this->setScrollOffset ( Point::ZERO );
    this->setRelVelocity  ( Point::ZERO );
    this->setOrigPosition ( Point::ZERO );    

	return KD_TRUE;
}

NS_CC_EXT_END