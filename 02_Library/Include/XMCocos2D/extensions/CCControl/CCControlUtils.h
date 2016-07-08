/* --------------------------------------------------------------------------
 *
 *      File            ColourUtils.h
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

#ifndef __CCControlUtils_h__
#define __CCControlUtils_h__

#include "../../sprite_nodes/CCSprite.h"

NS_CC_BEGIN

/**
 * @addtogroup GUI
 * @{
 * @addtogroup control_extension
 * @{
 */

typedef struct
{
    KDdouble	r;       // percent
    KDdouble	g;       // percent
    KDdouble	b;       // percent
    KDdouble	a;       // percent
} CCRgb;

typedef struct
{
    KDdouble	h;       // angle in degrees
    KDdouble	s;       // percent
    KDdouble	v;       // percent
} CCHsv;

//helper class to store ccColor3B's in mutable arrays
class CCColor3bObject : public CCObject
{
	public :

		CCColor3bObject ( ccColor3B tValue ) 
		{
			m_tValue = tValue;
		}

		ccColor3B			m_tValue;		
};

class CCControlUtils
{
	public :

		static CCSprite*	addSpriteToTargetWithPosAndAnchor ( const KDchar* szSpriteName, CCNode* pTarget, CCPoint tPos, CCPoint tAnchor );

		static CCHsv		HSVfromRGB ( CCRgb tValue );

		static CCRgb		RGBfromHSV ( CCHsv tValue );

		static CCRect		CCRectUnion ( const CCRect& tSrc1, const CCRect& tSrc2 );
};

// end of GUI group
/// @}
/// @}

NS_CC_END

#endif // __CCControlUtils_h__