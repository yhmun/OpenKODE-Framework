/* --------------------------------------------------------------------------
 *
 *      File            cocos-ext.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or ( at your option ) any later version.
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

#ifndef __cocos_ext_h__
#define __cocos_ext_h__

#include "ExtensionMacros.h"

#include "GUI/CCControlExtension/CCControlExtensions.h"
#include "GUI/CCScrollView/CCScrollView.h"
#include "GUI/CCScrollView/CCTableView.h"
#include "GUI/CCEditBox/CCEditBox.h"

// Physics integration
#if CC_ENABLE_CHIPMUNK_INTEGRATION || CC_ENABLE_BOX2D_INTEGRATION
#include "physics-nodes/CCPhysicsDebugNode.h"
#include "physics-nodes/CCPhysicsSprite.h"
#include "physics-nodes/CCBox2DDebugDraw.h"
#endif

#include "assets-manager/AssetsManager.h"

#include "parallax-scroll/CCParallaxScrollNode.h"

#include "joystick/CCJoyStick.h"

// kwing
#include "kwing/kwing.h"

// CookBook
#include "cookbook/GameDPad.h"
#include "cookbook/GameAPad.h"
#include "cookbook/GameButton.h"


// Utility
#include "Utility/XMUtility.h"

#endif	// __cocos_ext_h__
