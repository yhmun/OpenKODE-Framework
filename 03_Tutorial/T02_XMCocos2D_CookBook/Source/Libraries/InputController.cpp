/* --------------------------------------------------------------------------
 *
 *      File            InputController.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      João Caxaria on 4/28/09
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2009      Feel Fine Games. All rights reserved.
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
#include "InputController.h"

KDdouble _icPointLineComparison ( const CCPoint& tPoint, const CCPoint& tLineA, const CCPoint& tLineB )
{
	return 0.5f * ( tLineA.x * tLineB.y - tLineA.y * tLineB.x - tPoint.x * tLineB.y + tPoint.y * tLineB.x + tPoint.x * tLineA.y - tPoint.y * tLineA.x );
}

CCPoint _icRotateRealWorld ( const CCPoint& tLocation )
{
	return CCDirector::sharedDirector ( )->convertToGL ( tLocation );
}

KDfloat icDistanceBetweenTwoPoints ( const CCPoint& tFromPoint, const CCPoint& tToPoint )
{
	return ccpDistance ( tFromPoint, tToPoint );
}

KDbool icIsPointInside ( const CCPoint& tPoint, const CCRect& tRect )
{
	return tRect.containsPoint ( tPoint ); 
}

KDint icFingerCount ( CCSet* pTouches, CCEvent* pEvent )
{
	return pTouches->count ( );
}

CCPoint icFingerLocation ( KDint nFinger, CCSet* pTouches, CCEvent* pEvent )
{
	if ( nFinger == 0 || nFinger > pTouches->count ( ) )
	{		
		return ccpz;
	}
	
	KDint	nCount = 0;
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++, nCount++ ) 
	{
		if ( nCount == ( nFinger - 1 ) )
		{
			CCTouch*	pTouch = (CCTouch*) ( *iter );
			return pTouch->getLocation ( );
		}
	}

	return ccpz;
}

CCPoint icPreviousFingerLocation ( KDint nFinger, CCSet* pTouches, CCEvent* pEvent )
{
	if ( nFinger == 0 || nFinger > pTouches->count ( ) )
	{		
		return ccpz;
	}
	
	KDint	nCount = 0;
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++, nCount++ ) 
	{
		if ( nCount == ( nFinger - 1 ) )
		{
			CCTouch*	pTouch = (CCTouch*) ( *iter );
			return pTouch->getPreviousLocation ( );
		}
	}

	return ccpz;
}

KDbool icWasSwipeLeft ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( 1 != pTouches->count ( ) )
	{
		return KD_FALSE;
	}
	
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );

	CCPoint		tStart = pTouch->getPreviousLocation ( );
	CCPoint		tEnd   = pTouch->getLocation ( );
	
	if ( tStart.x > tEnd.x )
	{
		return KD_TRUE;
	}
	
	return KD_FALSE;
}

KDbool icWasSwipeRight ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( 1 != pTouches->count ( ) )
	{
		return KD_FALSE;
	}
	
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );

	CCPoint		tStart = pTouch->getPreviousLocation ( );
	CCPoint		tEnd   = pTouch->getLocation ( );
	
	if ( tStart.x < tEnd.x )
	{
		return KD_TRUE;
	}
	
	return KD_FALSE;
}

KDbool icWasSwipeUp ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( 1 != pTouches->count ( ) )
	{
		return KD_FALSE;
	}
	
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );

	CCPoint		tStart = pTouch->getPreviousLocation ( );
	CCPoint		tEnd   = pTouch->getLocation ( );
	
	if ( tStart.y < tEnd.y )
	{
		return KD_TRUE;
	}
	
	return KD_FALSE;
}

KDbool icWasSwipeDown ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( 1 != pTouches->count ( ) )
	{
		return KD_FALSE;
	}
	
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );

	CCPoint		tStart = pTouch->getPreviousLocation ( );
	CCPoint		tEnd   = pTouch->getLocation ( );
	
	if ( tStart.y > tEnd.y )
	{
		return KD_TRUE;
	}
	
	return KD_FALSE;	
}

KDbool icWasDragLeft ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( 2 != pTouches->count ( ) )
	{
		return KD_FALSE;
	}

	CCPoint		aStarts [ 2 ];
	CCPoint		aEnds   [ 2 ];

	KDint	i = 0;
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++, i++ ) 
	{
		CCTouch*	pTouch = (CCTouch*) ( *iter );

		aStarts [ i ] = pTouch->getPreviousLocation ( );
		aEnds   [ i ] = pTouch->getLocation ( );
	}	
	
	if ( aStarts [ 0 ].x > aEnds [ 0 ].x && aStarts [ 1 ].x > aEnds [ 1 ].x )
	{
		return KD_TRUE;
	}
	
	return KD_FALSE;
}

KDbool icWasDragRight ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( 2 != pTouches->count ( ) )
	{
		return KD_FALSE;
	}

	CCPoint		aStarts [ 2 ];
	CCPoint		aEnds   [ 2 ];

	KDint	i = 0;
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++, i++ ) 
	{
		CCTouch*	pTouch = (CCTouch*) ( *iter );

		aStarts [ i ] = pTouch->getPreviousLocation ( );
		aEnds   [ i ] = pTouch->getLocation ( );
	}	
	
	if ( aStarts [ 0 ].x < aEnds [ 0 ].x && aStarts [ 1 ].x < aEnds [ 1 ].x )
	{
		return KD_TRUE;
	}
	
	return KD_FALSE;
}

KDbool icWasDragUp ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( 2 != pTouches->count ( ) )
	{
		return KD_FALSE;
	}

	CCPoint		aStarts [ 2 ];
	CCPoint		aEnds   [ 2 ];

	KDint	i = 0;
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++, i++ ) 
	{
		CCTouch*	pTouch = (CCTouch*) ( *iter );

		aStarts [ i ] = pTouch->getPreviousLocation ( );
		aEnds   [ i ] = pTouch->getLocation ( );
	}	
	
	if ( aStarts [ 0 ].y < aEnds [ 0 ].y && aStarts [ 1 ].y < aEnds [ 1 ].y )
	{
		return KD_TRUE;
	}
	
	return KD_FALSE;	
}

KDbool icWasDragDown ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( 2 != pTouches->count ( ) )
	{
		return KD_FALSE;
	}

	CCPoint		aStarts [ 2 ];
	CCPoint		aEnds   [ 2 ];

	KDint	i = 0;
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++, i++ ) 
	{
		CCTouch*	pTouch = (CCTouch*) ( *iter );

		aStarts [ i ] = pTouch->getPreviousLocation ( );
		aEnds   [ i ] = pTouch->getLocation ( );
	}	
	
	if ( aStarts [ 0 ].y > aEnds [ 0 ].y && aStarts [ 1 ].y > aEnds [ 1 ].y )
	{
		return KD_TRUE;
	}
	
	return KD_FALSE;
}

KDbool icWasZoomIn ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( 2 != pTouches->count ( ) )
	{
		return KD_FALSE;
	}

	CCPoint		aStarts [ 2 ];
	CCPoint		aEnds   [ 2 ];

	KDint	i = 0;
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++, i++ ) 
	{
		CCTouch*	pTouch = (CCTouch*) ( *iter );

		aStarts [ i ] = pTouch->getPreviousLocation ( );
		aEnds   [ i ] = pTouch->getLocation ( );
	}	
	
	KDfloat		fInitialDistance = icDistanceBetweenTwoPoints ( aStarts [ 0 ], aStarts [ 1 ] );
	KDfloat		fEndDistance     = icDistanceBetweenTwoPoints ( aEnds   [ 0 ], aEnds   [ 1 ] );
	
	if ( fEndDistance > fInitialDistance )
	{
		return KD_TRUE;
	}
	
	return KD_FALSE;
}

KDbool icWasZoomOut ( CCSet* pTouches, CCEvent* pEvent )
{
	if ( 2 != pTouches->count ( ) )
	{
		return KD_FALSE;
	}

	CCPoint		aStarts [ 2 ];
	CCPoint		aEnds   [ 2 ];

	KDint	i = 0;
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++, i++ ) 
	{
		CCTouch*	pTouch = (CCTouch*) ( *iter );

		aStarts [ i ] = pTouch->getPreviousLocation ( );
		aEnds   [ i ] = pTouch->getLocation ( );
	}	
	
	KDfloat		fInitialDistance = icDistanceBetweenTwoPoints ( aStarts [ 0 ], aStarts [ 1 ] );
	KDfloat		fEndDistance     = icDistanceBetweenTwoPoints ( aEnds   [ 0 ], aEnds   [ 1 ] );
	
	if ( fEndDistance < fInitialDistance )
	{
		return KD_TRUE;
	}
	
	return KD_FALSE;
}

KDbool icWasAClickGeneric ( CCSet* pTouches, CCEvent* pEvent, KDint nFingers, KDint nTaps )
{
	if ( nFingers == pTouches->count ( ) )
	{		
		return KD_FALSE;
	}
	
//	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
	
	//return [touch phase] == UITouchPhaseEnded && [touch tapCount] == taps;

	return KD_TRUE;
}

KDbool icWasAClick ( CCSet* pTouches, CCEvent* pEvent )
{
	return icWasAClickGeneric ( pTouches, pEvent, 1, 1 );
}

KDbool icWasADoubleClick ( CCSet* pTouches, CCEvent* pEvent )
{
	return icWasAClickGeneric ( pTouches, pEvent, 1, 2 );
}

CCPoint icDistance ( KDint nFinger, CCSet* pTouches, CCEvent* pEvent )
{
	if ( nFinger == 0 || nFinger > pTouches->count ( ) )
	{		
		return ccpz;
	}
	
	KDint	nCount = 0;
	for ( CCSetIterator iter = pTouches->begin ( ); iter != pTouches->end ( ); iter++, nCount++ ) 
	{
		if ( nCount == ( nFinger - 1 ) )
		{
			CCTouch*	pTouch = (CCTouch*) ( *iter );
			CCPoint		tStart = pTouch->getPreviousLocation ( );
			CCPoint		tEnd   = pTouch->getLocation ( );

			KDfloat		fDistX = ( kdFabsf ( tStart.x - tEnd.x ) );
			KDfloat		fDistY = ( kdFabsf ( tStart.y - tEnd.y ) );

			return ccp ( fDistX, fDistY );
		}
	}

	return ccpz;
}