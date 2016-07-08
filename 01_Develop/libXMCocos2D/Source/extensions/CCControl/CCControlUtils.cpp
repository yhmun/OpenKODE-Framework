/* --------------------------------------------------------------------------
 *
 *      File            ColourUtils.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Stewart Hamilton-Arrandale
 *      Copyright (c) 2012      Yannick Loriot ( Modified )
 *      Copyright (c) 2012      Angus C        ( Converted to c++ )
 *
 *         http://www.cocos2d-x.org   
 *         http://creativewax.co.uk
 *         http://yannickloriot.com
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
 *      but WITHOUT ANY WARRANTY; withtOut even the implied warranty of
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
#include "extensions/CCControl/CCControlUtils.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

CCSprite* CCControlUtils::addSpriteToTargetWithPosAndAnchor ( const KDchar* szSpriteName, CCNode* pTarget, CCPoint tPos, CCPoint tAnchor )
{
	CCSprite*  pSprite = CCSprite::createWithSpriteFrameName ( szSpriteName );
    
	if ( pSprite )
	{
		pSprite->setPosition ( tPos );
		pSprite->setAnchorPoint ( tAnchor );
		pTarget->addChild ( pSprite );
	}
		
	return pSprite;
}

CCHsv CCControlUtils::HSVfromRGB ( CCRgb tValue )
{
    CCHsv       tOut;
    KDdouble    dMin, dMax, dDelta;
    
    dMin = tValue.r < tValue.g ? tValue.r : tValue.g;
    dMin = dMin  < tValue.b ? dMin : tValue.b;
    
    dMax = tValue.r > tValue.g ? tValue.r : tValue.g;
    dMax = dMax  > tValue.b ? dMax : tValue.b;
    
    tOut.v = dMax;												// v
    dDelta = dMax - dMin;
    if ( dMax > 0.0f )
    {
        tOut.s = ( dDelta / dMax );								// s
    }
	else
    {
        // r = g = b = 0										// s = 0, v is undefined
        tOut.s = 0.0;
        tOut.h = -1;											// its now undefined (don't know if setting to NAN is a good idea)
        return tOut;
    }

    if ( tValue.r >= dMax )										// > is bogus, just keeps compilor happy
    {
        tOut.h = ( tValue.g - tValue.b ) / dDelta;				// between yellow & magenta
    } 
	else
    {
        if ( tValue.g >= dMax )
		{
            tOut.h = 2.0 + ( tValue.b - tValue.r ) / dDelta;	// between cyan & yellow
		}
        else
		{
            tOut.h = 4.0 + ( tValue.r - tValue.g ) / dDelta;	// between magenta & cyan
		}
    }
    
    tOut.h *= 60.0;												// degrees
    
    if ( tOut.h < 0.0 )
	{
        tOut.h += 360.0;
	}
    
    return tOut;
}

CCRgb CCControlUtils::RGBfromHSV ( CCHsv tValue )
{
    KDdouble     hh, p, q, t, ff;
    KDint        i;
    CCRgb        tOut;
    
	tOut.a = 1;
    
    if ( tValue.s <= 0.0 )			// < is bogus, just shuts up warnings
    {       
        if ( kdIsNan ( tValue.h ) ) // tValue.h == NAN
        {   
            tOut.r = tValue.v;
            tOut.g = tValue.v;
            tOut.b = tValue.v;
            return tOut;
        }
        
        // error - should never happen
        tOut.r = 0.0;
        tOut.g = 0.0;
        tOut.b = 0.0;
        return tOut;
    }
    
    hh = tValue.h;
    if ( hh >= 360.0 )
	{
		hh = 0.0;
	}

    hh /= 60.0;
    i = (KDint) hh;
    ff = hh - i;
    p = tValue.v * ( 1.0 - tValue.s );
    q = tValue.v * ( 1.0 - ( tValue.s * ff ) );
    t = tValue.v * ( 1.0 - ( tValue.s * ( 1.0 - ff ) ) );
    
    switch ( i )
    {
        case 0 :
            tOut.r = tValue.v;
            tOut.g = t;
            tOut.b = p;
            break;

        case 1 :
            tOut.r = q;
            tOut.g = tValue.v;
            tOut.b = p;
            break;

        case 2 :
            tOut.r = p;
            tOut.g = tValue.v;
            tOut.b = t;
            break;
            
        case 3 :
            tOut.r = p;
            tOut.g = q;
            tOut.b = tValue.v;
            break;

        case 4 :
            tOut.r = t;
            tOut.g = p;
            tOut.b = tValue.v;
            break;

        case 5 :
        default:
            tOut.r = tValue.v;
            tOut.g = p;
            tOut.b = q;
            break;
    }

    return tOut;     
}

CCRect CCControlUtils::CCRectUnion ( const CCRect& tSrc1, const CCRect& tSrc2 ) 
{
	KDfloat  fX1 = KD_MIN ( tSrc1.getMinX ( ), tSrc2.getMinX ( ) );
	KDfloat  fY1 = KD_MIN ( tSrc1.getMinY ( ), tSrc2.getMinY ( ) );

	KDfloat  fX2 = KD_MAX ( tSrc1.getMaxX ( ), tSrc2.getMaxX ( ) );
	KDfloat  fY2 = KD_MAX ( tSrc1.getMaxY ( ), tSrc2.getMaxY ( ) );
	
	return CCRect ( fX1, fX2, fX2 - fX1, fY2 - fY1 );
}

NS_CC_END