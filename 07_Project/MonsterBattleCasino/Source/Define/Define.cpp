/* -----------------------------------------------------------------------------------
 *
 *      File            Define.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "Define.h"
#include "ShopNode/ShopNode.h"

KDbool RectInPoint ( const Rect& tRect, const Point& tPoint )
{
	if ( tPoint.x < tRect.origin.x || tPoint.y < tRect.origin.y || tPoint.x > tRect.origin.x + tRect.size.width || tPoint.y > tRect.origin.y + tRect.size.height )		
	{
		return false;
	}
	
	return true;
}

KDint collisionBox ( const Rect& tRect1, const Rect& tRect2 )
{
	return ( tRect1.origin.x + tRect1.size.cx >= tRect2.origin.x &&
			 tRect2.origin.x + tRect2.size.cx >= tRect1.origin.x &&
			 tRect1.origin.y + tRect1.size.cy >= tRect2.origin.y && 
			 tRect2.origin.y + tRect2.size.cy >= tRect1.origin.y );
}

KDbool readToData ( const KDubyte* pSrcData, const KDint nSrcDataSize, KDint& nSeek, KDvoid* pDestData, const KDint nDestDataSize )
{
	if ( nSeek + nDestDataSize > nSrcDataSize )
	{
		return false;
	}

	kdMemcpy ( pDestData, &pSrcData [ nSeek ], nDestDataSize );

	nSeek += nDestDataSize;

	return true;
}


KDvoid buy_10000_coin ( KDvoid )
{
	ShopNode::getObj ( )->completeTransaction ( nullptr );
}

KDvoid buy_33000_coin ( KDvoid )
{
	ShopNode::getObj ( )->completeTransaction ( nullptr );
}

KDvoid buy_60000_coin ( KDvoid )
{
	ShopNode::getObj ( )->completeTransaction ( nullptr );
}

KDvoid buy_130000_coin ( KDvoid )
{
	ShopNode::getObj ( )->completeTransaction ( nullptr );
}

KDvoid buy_3_slot ( KDvoid )
{
	ShopNode::getObj ( )->completeTransaction ( nullptr );
}

KDvoid buy_9_slot ( KDvoid )
{
	ShopNode::getObj ( )->completeTransaction ( nullptr );
}

KDvoid buy_30_slot ( KDvoid )
{
	ShopNode::getObj ( )->completeTransaction ( nullptr );
}

KDvoid buy_machine_unusual ( KDvoid )
{
	ShopNode::getObj ( )->completeTransaction ( nullptr );
}

KDvoid buy_machine_prettiness ( KDvoid )
{
	ShopNode::getObj ( )->completeTransaction ( nullptr );
}

KDvoid buy_machine_regendOfHeroRink	( KDvoid )
{
	ShopNode::getObj ( )->completeTransaction ( nullptr );
}

KDvoid buy_machine_warvel_vs_hell ( KDvoid )
{
	ShopNode::getObj ( )->completeTransaction ( nullptr );
}
