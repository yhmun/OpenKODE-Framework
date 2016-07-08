/* --------------------------------------------------------------------------
 *
 *      File            KWAnimation.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/05/30 
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. 
 *      Copyright (c) 2011-2013 Kawaz. All rights reserved.                             
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

#ifndef __KWAnimation_h__
#define __KWAnimation_h__

class KWSprite;

class KWAnimation 
{
	public :

		static KWSprite*	spriteWithArray ( CCArray* pTextures, KDfloat fAps );

		static KWSprite*	spriteWithFiles ( CCArray* pFiles, KDfloat fAps );

		static KWSprite*	spriteWithFile  ( const KDchar* szFilename, CCSize tSize, KDfloat fAps );

		static KWSprite*	spriteWithTextureAtlas ( CCTexture2D* pTexture, CCSize tSize, KDfloat fAps );

		static KWSprite*	spriteWithSpriteFrames ( CCArray* pFrames, KDfloat fAps );
};

#endif	// __KWAnimation_h__
