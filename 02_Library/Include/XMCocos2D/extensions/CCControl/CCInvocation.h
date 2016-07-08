/* --------------------------------------------------------------------------
 *
 *      File            CCInvocation.h
 *      Author          Young-Hwan Mun
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

#ifndef __CCInvocation_h__
#define __CCInvocation_h__

#include "../../cocoa/CCObject.h"

NS_CC_BEGIN

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */

typedef KDuint  CCControlEvent;

typedef KDvoid  ( CCObject::*SEL_CCControlHandler ) ( CCObject*, CCControlEvent );

#define cccontrol_selector(_SELECTOR) (SEL_CCControlHandler)(&_SELECTOR)

/*
 *
 * Helper class to store targets and selectors (and eventually, params?) in the same CCMutableArray. Basically a very crude form of a NSInvocation
 *
 */
class CCInvocation : public CCObject
{
	public :

		CCInvocation ( CCObject* pTarget, SEL_CCControlHandler pAction, CCControlEvent uControlEvent );

		static CCInvocation*	create ( CCObject* pTarget, SEL_CCControlHandler pAction, CCControlEvent uControlEvent );

	public :

		CC_SYNTHESIZE_READONLY ( SEL_CCControlHandler, m_pAction	  , Action		 );
		CC_SYNTHESIZE_READONLY ( CCObject*			 , m_pTarget	  , Target		 );
		CC_SYNTHESIZE_READONLY ( CCControlEvent		 , m_uControlEvent, ControlEvent );
		
		KDvoid					invoke ( CCObject* pSender );
};

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif // __CCInvocation_h__