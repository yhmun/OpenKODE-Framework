/* --------------------------------------------------------------------------
 *
 *      File            LocalStorage.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      Zynga Inc.
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

#ifndef __LocalStorage_h__
#define __LocalStorage_h__

#include "../../ccMacros.h"

NS_CC_BEGIN

/**
 *	Local Storage support for the JS Bindings for iOS.
 *	Works on cocos2d-iphone and cocos2d-x.
 */

/** Initializes the database. If path is null, it will create an in-memory DB */
KDvoid			localStorageInit ( const KDchar* szFullpath );

/** Frees the allocated resources */
KDvoid			localStorageFree ( KDvoid );

/** sets an item in the LS */
KDvoid			localStorageSetItem ( const KDchar* szKey, const KDchar* szValue );

/** gets an item from the LS */
const KDchar*	localStorageGetItem ( const KDchar* szKey );

/** removes an item from the LS */
KDvoid			localStorageRemoveItem ( const KDchar* szKey );

NS_CC_END

#endif	// __LocalStorage_h__
