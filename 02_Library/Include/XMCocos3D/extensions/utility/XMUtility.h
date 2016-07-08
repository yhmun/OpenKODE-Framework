/* --------------------------------------------------------------------------
 *
 *      File            XMUtility.h
 *      Created By      Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. All rights reserved.                      
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

#ifndef __XMUtility_h__
#define __XMUtility_h__

#include "../ExtensionMacros.h"
#include "../../2d/base_nodes/CCNode.h"
#include "../../2d/actions/CCActionInterval.h"
#include "../../2d/sprite_nodes/CCSprite.h"

#include <functional>

NS_CC_EXT_BEGIN

class XMUtility
{
	public :

		static ActionInterval*			createShakeAction ( const Point& tCenter, KDfloat fDuration, Size tRange, KDuint uCount = 60 );

		static KDvoid					runShakeAction ( Node* pNode, KDfloat fDuration, const Size& tRange, KDuint uCount = 60 );

		static KDvoid					vibrateDevice ( KDfloat fVibrateTime );

		static Texture2D*				createTexture ( std::vector<std::string> aPaths, std::string sKey, KDbool bVertically );

		class ScreenShot
		{
			public :

				static  Image*			takeAsImage		( const Rect& tRect, KDbool bIsGray = false );		
				static  Texture2D*		takeAsTexture2D ( const Rect& tRect, KDbool bIsGray = false );
				static  Sprite*			takeAsSprite	( const Rect& tRect, KDbool bIsGray = false );
		};
};

NS_CC_EXT_END

#endif	// __XMUtility_h__