/* --------------------------------------------------------------------------
 *
 *      File            CCBSelectorResolver.h
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

#ifndef __CCBSelectorResolver_h__
#define __CCBSelectorResolver_h__

#include "../../cocoa/CCObject.h"
#include "../CCControl/CCInvocation.h"

NS_CC_BEGIN

#define CCB_SELECTORRESOLVER_CCMENUITEM_GLUE( _TARGET, _SELECTORNAME, _METHOD )			\
	if ( pTarget == _TARGET && kdStrcmp ( pSelectorName, _SELECTORNAME ) == 0 )			\
	{																					\
		return menu_selector ( _METHOD );												\
	}

#define CCB_SELECTORRESOLVER_CCCONTROL_GLUE( _TARGET, _SELECTORNAME, _METHOD )			\
	if ( pTarget == _TARGET && kdStrcmp ( pSelectorName, _SELECTORNAME ) == 0 )			\
	{																					\
		return cccontrol_selector ( _METHOD );											\
	}

#define CCB_SELECTORRESOLVER_CALLFUNC_GLUE( TARGET, _SELECTORNAME, _METHOD )			\
	if ( pTarget == TARGET && kdStrcmp ( pSelectorName, _SELECTORNAME ) == 0 )			\
	{																					\
	    return callfuncN_selector ( _METHOD );											\
	}

class CCBSelectorResolver
{
    public :

        virtual SEL_MenuHandler			onResolveCCBCCMenuItemSelector ( CCObject* pTarget, const KDchar* szSelectorName ) = 0;
		virtual SEL_CallFuncN			onResolveCCBCCCallFuncSelector ( CCObject* pTarget, const KDchar* szSelectorName ) { return KD_NULL; }	
        virtual SEL_CCControlHandler	onResolveCCBCCControlSelector  ( CCObject* pTarget, const KDchar* szSelectorName ) = 0;
};

class CCBScriptOwnerProtocol 
{
	public :

		virtual ~CCBScriptOwnerProtocol ( KDvoid ) { };

		virtual  CCBSelectorResolver*	createNew ( KDvoid ) = 0;
};

NS_CC_END // __CCBSelectorResolver_h__

#endif
