/* --------------------------------------------------------------------------
 *
 *      File            Helpers.cpp
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
#include "Helpers.h"

KDfloat degreesToRadians ( KDfloat fDegrees )
{
	return fDegrees * ( KD_PI_F / 180.f );
}	

KDfloat radiansToDegrees ( KDfloat fRadians )
{
	return fRadians * ( 180.f / KD_PI_F );
}

KDfloat vectorToRadians ( CCPoint tVector )
{
	if ( tVector.y == 0 )
	{
		tVector.y = 0.000001f;
	}

	KDfloat		fBaseRadians = kdAtanf ( tVector.x / tVector.y );
	if ( tVector.y < 0 )
	{
		fBaseRadians += KD_PI_F;		// Adjust for -Y
	}	
	return fBaseRadians;
}

CCPoint radiansToVector ( KDfloat fRadians )
{
	return ccp ( kdSinf ( fRadians - KD_PI_F / 2 ), kdCosf ( fRadians - KD_PI_F / 2 ) );
}

KDfloat distanceBetweenPoints ( const CCPoint& tPoint1, const CCPoint& tPoint2 )
{
	return kdSqrtf ( kdPowf ( ( tPoint1.x - tPoint2.x ), 2 ) + kdPowf ( ( tPoint1.y - tPoint2.y ), 2 ) );
}

KDbool pointIsInRect ( const CCPoint& tPoint, const CCRect& tRect )
{
	KDbool		bIsInRect = KD_FALSE;
	if ( tPoint.x < tRect.origin.x + tRect.size.cx && 
	     tPoint.x > tRect.origin.x &&
	     tPoint.y < tRect.origin.y + tRect.size.cy &&
	     tPoint.y > tRect.origin.y )
	{
		bIsInRect = KD_TRUE;
	}
	return bIsInRect;
}

KDbool pointIsInCircle ( const CCPoint& tPoint, const CCPoint& tOrigin, KDfloat fRadius )
{
	KDbool		bIsInCircle = KD_FALSE;
	if ( distanceBetweenPoints ( tPoint, tOrigin ) <= fRadius )
	{
		bIsInCircle = KD_TRUE;
	}
	return bIsInCircle;
}

KDfloat angleDifference ( KDfloat fAngleA, KDfloat fAngleB )
{
	KDfloat		fDiff = kdFabsf ( fAngleA - fAngleB );
	if ( kdFabsf ( ( fAngleA + 360 ) - fAngleB ) < fDiff )
	{
		fDiff = kdFabsf ( ( fAngleA + 360 ) - fAngleB );
	}
	if ( kdFabsf ( ( fAngleA - 360 ) - fAngleB ) < fDiff )
	{
		fDiff = kdFabsf ( ( fAngleA - 360 ) - fAngleB );
	}
	if ( kdFabsf ( fAngleA - ( fAngleB + 360 ) ) < fDiff )
	{
		fDiff = kdFabsf ( fAngleA - ( fAngleB + 360 ) );
	}
	if ( kdFabsf ( fAngleA - ( fAngleB - 360 ) ) < fDiff )
	{
		fDiff = kdFabsf ( fAngleA - ( fAngleB - 360 ) );
	}
	return fDiff;
}

KDfloat absoluteValue ( KDfloat fNum )
{
	if ( fNum < 0 )
	{
		fNum = fNum * -1;
	}

	return fNum;
}

KDfloat convertPositionX ( const CCPoint& tAreaSize, KDfloat x )
{
	return x - ( tAreaSize.x / 2 );
}

KDfloat convertPositionY ( const CCPoint& tAreaSize, KDfloat y )
{
	return y - ( tAreaSize.y / 2);
}

CCPoint midPoint ( const CCPoint& tPoint1, const CCPoint& tPoint2 )
{
	return ccp ( ( tPoint1.x + tPoint2.x ) / 2, ( tPoint1.y + tPoint2.y ) / 2 );
}