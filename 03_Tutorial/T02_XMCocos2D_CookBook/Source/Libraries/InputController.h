/* --------------------------------------------------------------------------
 *
 *      File            InputController.h
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

#ifndef __InputController_h__
#define __InputController_h__

KDdouble	_icPointLineComparison		( const CCPoint& tPoint, const CCPoint& tLineA, const CCPoint& tLineB );

CCPoint		_icRotateRealWorld			( const CCPoint& tLocation );

KDfloat		icDistanceBetweenTwoPoints	( const CCPoint& tFromPoint, const CCPoint& tToPoint );

KDbool		icIsPointInside				( const CCPoint& tPoint, const CCRect& tRect );

KDint		icFingerCount				( CCSet* pTouches, CCEvent* pEvent );
CCPoint		icFingerLocation			( KDint nFinger, CCSet* pTouches, CCEvent* pEvent );

KDbool		icWasSwipeLeft				( CCSet* pTouches, CCEvent* pEvent );
KDbool		icWasSwipeRight				( CCSet* pTouches, CCEvent* pEvent );
KDbool		icWasSwipeUp				( CCSet* pTouches, CCEvent* pEvent );
KDbool		icWasSwipeDown				( CCSet* pTouches, CCEvent* pEvent );

KDbool		icWasDragLeft				( CCSet* pTouches, CCEvent* pEvent );
KDbool		icWasDragRight				( CCSet* pTouches, CCEvent* pEvent );
KDbool		icWasDragUp					( CCSet* pTouches, CCEvent* pEvent );
KDbool		icWasDragDown				( CCSet* pTouches, CCEvent* pEvent );

KDbool		icWasZoomIn					( CCSet* pTouches, CCEvent* pEvent );
KDbool		icWasZoomOut				( CCSet* pTouches, CCEvent* pEvent );

KDbool		icWasAClickGeneric			( CCSet* pTouches, CCEvent* pEvent, KDint nFingers, KDint nTaps );
KDbool		icWasAClick					( CCSet* pTouches, CCEvent* pEvent );
KDbool		icWasADoubleClick			( CCSet* pTouches, CCEvent* pEvent );

CCPoint		icDistance					( KDint nFinger, CCSet* pTouches, CCEvent* pEvent );

#endif	// __InputController_h__
