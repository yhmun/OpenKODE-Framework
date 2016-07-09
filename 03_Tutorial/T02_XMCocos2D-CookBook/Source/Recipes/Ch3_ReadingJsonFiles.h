/* --------------------------------------------------------------------------
 *
 *      File            Ch3_ReadingJsonFiles.h
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

#ifndef __Ch3_ReadingJsonFiles_h__
#define __Ch3_ReadingJsonFiles_h__

#include "XMJson/json.h"

class Ch3_ReadingJsonFiles : public Recipe
{
	public :

		SCENE_FUNC ( Ch3_ReadingJsonFiles );

	protected :		

		virtual KDbool		init ( KDvoid );

		KDvoid				processMap ( const Json::Value& tRoot );

		KDvoid				processSpriteFile ( const Json::Value& tNode );

		KDvoid				processTexturedPolygon ( const Json::Value& tNode );

		KDfloat				getZFromY ( KDfloat y );
};

#endif	// __Ch3_ReadingJsonFiles_h__
