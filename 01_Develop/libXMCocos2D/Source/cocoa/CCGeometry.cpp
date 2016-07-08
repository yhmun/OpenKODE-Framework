/* -----------------------------------------------------------------------------------
 *
 *      File            CCGeometry.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
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
#include "cocoa/CCGeometry.h"
#include "cocoa/CCString.h"

struct cpVect
{
	cpFloat x;
	cpFloat y;
};

NS_CC_BEGIN
    
//
// implementation of CCPoint
//
CCPoint::CCPoint ( KDvoid )
{
	setPoint ( 0.0f, 0.0f );
}

CCPoint::CCPoint ( KDfloat fX, KDfloat fY )
{
	setPoint ( fX, fY );
}

CCPoint::CCPoint ( const CCPoint& tOther )
{
    setPoint ( tOther.x, tOther.y );
}

CCPoint::CCPoint ( const CCSize& tSize )
{
	setPoint ( tSize.cx, tSize.cy );
}

CCPoint::CCPoint ( const cpVect& tVect )
{
	setPoint ( tVect.x, tVect.y );
}

CCPoint& CCPoint::operator= ( const CCPoint& tOther )
{
    setPoint ( tOther.x, tOther.y );
    return *this;
}

CCPoint& CCPoint::operator= ( const CCSize& tSize )
{
	setPoint ( tSize.cx, tSize.cy );
	return *this;
}

CCPoint& CCPoint::operator= ( const cpVect& tVect )
{
    setPoint ( tVect.x, tVect.y );
    return *this;
}

CCPoint CCPoint::operator+ ( const CCPoint& tRight ) const
{
	return CCPoint ( this->x + tRight.x, this->y + tRight.y );
}

CCPoint CCPoint::operator- ( const CCPoint& tRight ) const
{
	return CCPoint ( this->x - tRight.x, this->y - tRight.y );
}

CCPoint CCPoint::operator- ( KDvoid ) const
{
	return CCPoint ( -x, -y );
}

CCPoint CCPoint::operator* ( KDfloat fA ) const
{
	return CCPoint ( this->x * fA, this->y * fA );
}

CCPoint CCPoint::operator/ ( KDfloat fA ) const
{
	CCAssert ( fA, "CCPoint division by 0." );
	return CCPoint ( this->x / fA, this->y / fA );
}

KDvoid CCPoint::setPoint ( KDfloat fX, KDfloat fY )
{
    this->x = fX;
    this->y = fY;
}

KDbool CCPoint::equals ( const CCPoint& tTarget ) const
{
	return ( kdFabsf ( this->x - tTarget.x ) < KD_FLT_EPSILON )
        && ( kdFabsf ( this->y - tTarget.y ) < KD_FLT_EPSILON );
}

KDbool CCPoint::fuzzyEquals ( const CCPoint& tTarget, KDfloat fVariance ) const
{
	if ( x - fVariance <= tTarget.x && tTarget.x <= x + fVariance )
	{
		if ( y - fVariance <= tTarget.y && tTarget.y <= y + fVariance )
		{
			return KD_TRUE;
		}
	}

	return KD_FALSE;
}

KDfloat CCPoint::getAngle ( const CCPoint& tOther ) const
{
	CCPoint		tA2 = normalize();
	CCPoint		tB2 = tOther.normalize();
	KDfloat		fAngle = kdAtan2f ( tA2.cross ( tB2 ), tA2.dot ( tB2 ) );

	if ( kdFabsf ( fAngle ) < KD_FLT_EPSILON )
	{
		return 0.f;
	}

	return fAngle;
}

CCPoint CCPoint::rotateByAngle ( const CCPoint& tPivot, KDfloat fAngle ) const
{
	return tPivot + ( *this - tPivot ).rotate ( CCPoint::forAngle ( fAngle ) );
}

KDbool CCPoint::containsPolygon ( const CCPoint* pPoints, KDuint uPoints ) const
{
	KDbool   bInside = KD_FALSE;

	CCPoint  tNewPoint;
	CCPoint  tOldPoint;
	
	CCPoint  tPoint1;
	CCPoint  tPoint2;

    if ( uPoints < 3 ) 
	{
        return KD_FALSE;
    }

	tOldPoint = pPoints [ uPoints - 1 ];

    for ( KDuint i = 0 ; i < uPoints ; i++ )
	{
		tNewPoint = pPoints [ i ];

		if ( tNewPoint.x > tOldPoint.x )
		{
			tPoint1 = tOldPoint;
			tPoint2 = tNewPoint;
        }
        else
		{
			tPoint1 = tNewPoint;
			tPoint2 = tOldPoint;			
        }

		if ( ( tNewPoint.x < this->x ) == ( this->x <= tOldPoint.x )          // edge "open" at one end //
		  && ( this->y - tPoint1.y ) * ( tPoint2.x - tPoint1.x ) < ( tPoint2.y - tPoint1.y )* ( this->x - tPoint1.x ) )
		{
            bInside = !bInside;
        }

        tOldPoint = tNewPoint;
    }

    return bInside;
}

//
// implementation of CCSize
//
CCSize::CCSize ( KDvoid )
{
	setSize ( 0.0f, 0.0f );
}

CCSize::CCSize ( KDfloat fWidth, KDfloat fHeight )
{
	setSize ( fWidth, fHeight );
}

CCSize::CCSize ( const CCSize& tOther )
{
	setSize ( tOther.cx, tOther.cy );
}

CCSize::CCSize ( const CCPoint& tPoint )
{
	setSize ( tPoint.x, tPoint.y );
}

CCSize& CCSize::operator= ( const CCSize& tSize )
{
	setSize ( tSize.cx, tSize.cy );
    return *this;
}

CCSize& CCSize::operator= ( const CCPoint& tPoint )
{
	setSize ( tPoint.x, tPoint.y );
	return *this;
}

CCSize CCSize::operator+ ( const CCSize& tRight ) const
{
	return CCSize ( this->width + tRight.width, this->height + tRight.height );
}

CCSize CCSize::operator- ( const CCSize& tRight ) const
{
	return CCSize ( this->width - tRight.width, this->height - tRight.height );
}

CCSize CCSize::operator- ( KDvoid ) const
{
	return CCSize ( -this->width, -this->height );
}

CCSize CCSize::operator* ( KDfloat fA ) const
{
	return CCSize ( this->width * fA, this->height * fA );
}

CCSize CCSize::operator/ ( float fA ) const
{
	CCAssert ( fA, "CCSize division by 0.");
	return CCSize ( this->width / fA, this->height / fA );
}

KDvoid CCSize::setSize ( KDfloat fWidth, KDfloat fHeight )
{
	this->cx = fWidth;
	this->cy = fHeight;
}

KDbool CCSize::equals ( const CCSize& tTarget ) const
{
	return ( kdFabsf ( this->cx - tTarget.cx ) < KD_FLT_EPSILON )
        && ( kdFabsf ( this->cy - tTarget.cy ) < KD_FLT_EPSILON );
}

//
// implementation of CCRect
//
CCRect::CCRect ( KDvoid )
{
	setRect ( 0.0f, 0.0f, 0.0f, 0.0f );
}

CCRect::CCRect ( KDfloat fX, KDfloat fY, KDfloat fWidth, KDfloat fHeight )
{
	setRect ( fX, fY, fWidth, fHeight );
}

CCRect::CCRect ( const CCPoint& tPoint, const CCSize& tSize )
{
	setRect ( tPoint.x, tPoint.y, tSize.cx, tSize.cy );
}

CCRect::CCRect ( const CCRect& tRect )
{
	setRect ( tRect.origin.x, tRect.origin.y, tRect.size.cx, tRect.size.cy );
}

CCRect& CCRect::operator= ( const CCRect& tRect )
{
	setRect ( tRect.origin.x, tRect.origin.y, tRect.size.cx, tRect.size.cy );
    return *this;
}

KDvoid CCRect::setRect ( KDfloat fX, KDfloat fY, KDfloat fWidth, KDfloat fHeight )
{
	// Only support that, the width and height > 0
	CCAssert ( fWidth >= 0.0f && fHeight >= 0.0f, "width and height of Rect must not less than 0." );

	origin.x = fX;
	origin.y = fY;

	size.cx  = fWidth;
	size.cy  = fHeight;
}

KDbool CCRect::equals ( const CCRect& tRect ) const
{
    return ( origin.equals ( tRect.origin ) && size.equals ( tRect.size ) );
}

KDfloat CCRect::getMaxX ( KDvoid ) const
{
    return origin.x + size.cx;
}

KDfloat CCRect::getMidX ( KDvoid ) const
{
    return origin.x + size.cx / 2.0f;
}

KDfloat CCRect::getMinX ( KDvoid ) const
{
    return origin.x;
}

KDfloat CCRect::getMaxY ( KDvoid ) const
{
    return origin.y + size.cy;
}

KDfloat CCRect::getMidY ( KDvoid ) const
{
    return origin.y + size.cy / 2.0f;
}

KDfloat CCRect::getMinY ( KDvoid ) const
{
    return origin.y;
}

KDbool CCRect::containsPoint ( const CCPoint& tPoint ) const
{
    KDbool  bRet = KD_FALSE;

    if ( tPoint.x >= getMinX ( ) && tPoint.x <= getMaxX ( )
      && tPoint.y >= getMinY ( ) && tPoint.y <= getMaxY ( ) )
    {
        bRet = KD_TRUE;
    }

    return bRet;
}

KDbool CCRect::intersectsRect ( const CCRect& tRect ) const
{
    return !(       getMaxX ( ) < tRect.getMinX ( ) ||
              tRect.getMaxX ( ) <       getMinX ( ) ||
                    getMaxY ( ) < tRect.getMinY ( ) ||
              tRect.getMaxY ( ) <       getMinY ( ) );
}

KDbool CCRect::overlaps ( const CCRect& tRect ) const
{
	if ( origin.x + size.cx < tRect.origin.x ) { return KD_FALSE; }
	if ( origin.y + size.cy < tRect.origin.y ) { return KD_FALSE; }

    if ( tRect.origin.x + tRect.size.cx < origin.x ) { return KD_FALSE; }
    if ( tRect.origin.y + tRect.size.cy < origin.y ) { return KD_FALSE; }

    return KD_TRUE;
}

CCRect& CCRect::unions ( const CCRect& tRect )
{
	CCRect  tTemp = CCRect ( 0, 0, 0, 0 );

    tTemp.origin.x = KD_MIN ( origin.x, tRect.origin.x );
    tTemp.origin.y = KD_MIN ( origin.y, tRect.origin.y );
    tTemp.size.cx  = KD_MAX ( origin.x + size.cx, tRect.origin.x + tRect.size.cx ) - tTemp.origin.x;
    tTemp.size.cy  = KD_MAX ( origin.y + size.cy, tRect.origin.y + tRect.size.cy ) - tTemp.origin.y;

	*this = tTemp;
	return *this;
}

NS_CC_END

cpVect cpv ( const cocos2d::CCPoint& tPoint )
{
	cpVect  tVect = { tPoint.x, tPoint.y };
	return tVect;
}