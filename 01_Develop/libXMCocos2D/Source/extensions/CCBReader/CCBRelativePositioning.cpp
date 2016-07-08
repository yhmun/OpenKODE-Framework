/* --------------------------------------------------------------------------
 *
 *      File            CCBRelativePositioning.cpp
 *      Author          Y.H Mun
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
 *      version 2 of the License, or (at your otPositionion) any later version.
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
#include "extensions/CCBReader/CCBRelativePositioning.h"
#include "extensions/CCBReader/CCBReader.h"

NS_CC_BEGIN

CCPoint getAbsolutePosition ( const CCPoint& tPosition, KDint nType, const CCSize& tContainerSize, const KDchar* szPropName )
{
    CCPoint  tAbsPosition = ccp ( 0, 0 );

	switch ( nType )
	{
		case kCCBPositionTypeRelativeBottomLeft : 
			tAbsPosition = tPosition;
			break;

		case kCCBPositionTypeRelativeTopLeft :
			tAbsPosition.x = tPosition.x;
			tAbsPosition.y = tContainerSize.cy - tPosition.y;
			break;

		case kCCBPositionTypeRelativeTopRight :
			tAbsPosition.x = tContainerSize.cx - tPosition.x;
			tAbsPosition.y = tContainerSize.cy - tPosition.y;
			break;

		case kCCBPositionTypeRelativeBottomRight :
			tAbsPosition.x = tContainerSize.cx - tPosition.x;
			tAbsPosition.y = tPosition.y;
			break;

		case kCCBPositionTypePercent :
			tAbsPosition.x = (KDfloat) ( (KDint) ( tContainerSize.cx * tPosition.x / 100.0f ) );
			tAbsPosition.y = (KDfloat) ( (KDint) ( tContainerSize.cy * tPosition.y / 100.0f ) );
			break;

		case kCCBPositionTypeMultiplyResolution :			
			tAbsPosition = ccpMult ( tPosition, CCBReader::getResolutionScale ( ) );		
			break;

	}
    
    return tAbsPosition;
}

KDvoid setRelativeScale ( CCNode* pNode, KDfloat fScaleX, KDfloat fScaleY, KDint nType, const KDchar* szPropName )
{
    CCAssert ( pNode, "pNode should not be null" );
    
    if ( nType == kCCBScaleTypeMultiplyResolution )
    {
        KDfloat  fResolutionScale = CCBReader::getResolutionScale ( );
        
        fScaleX *= fResolutionScale;
        fScaleY *= fResolutionScale;
    }
    
    pNode->setScaleX ( fScaleX );
    pNode->setScaleY ( fScaleY );
}

NS_CC_END
