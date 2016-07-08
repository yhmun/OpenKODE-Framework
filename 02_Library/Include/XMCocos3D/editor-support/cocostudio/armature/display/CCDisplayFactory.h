/* -----------------------------------------------------------------------------------
 *
 *      File            CCDisplayFactory.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCDisplayFactory_h__
#define __CCDisplayFactory_h__

#include "../utils/CCArmatureDefine.h"
#include "../datas/CCDatas.h"

namespace cocostudio {

class Skin;
class Bone;
class DecorativeDisplay;
class DisplayData;

class DisplayFactory
{
	public :
			
		static KDvoid		addDisplay				( Bone* pBone, DecorativeDisplay* pDecoDisplay, DisplayData* pDisplayData );
		static KDvoid		createDisplay			( Bone* pBone, DecorativeDisplay* pDecoDisplay );
		static KDvoid		updateDisplay			( Bone* pBone, KDfloat dt, KDbool bDirty );

		static KDvoid		addSpriteDisplay		( Bone* pBone, DecorativeDisplay* pDecoDisplay, DisplayData* pDisplayData );
		static KDvoid		createSpriteDisplay		( Bone* pBone, DecorativeDisplay* pDecoDisplay );
		static KDvoid		initSpriteDisplay		( Bone* pBone, DecorativeDisplay* pDecoDisplay, const KDchar* pDisplayName, Skin* pSkin );


		static KDvoid		addArmatureDisplay		( Bone* pBone, DecorativeDisplay* pDecoDisplay, DisplayData* pDisplayData );
		static KDvoid		createArmatureDisplay	( Bone* pBone, DecorativeDisplay* pDecoDisplay );
		static KDvoid		updateArmatureDisplay	( Bone* pBone, cocos2d::Node* pDisplay, KDfloat dt );

		static KDvoid		addParticleDisplay		( Bone* pBone, DecorativeDisplay* pDecoDisplay, DisplayData* pDisplayData );
		static KDvoid		createParticleDisplay	( Bone* pBone, DecorativeDisplay* pDecoDisplay );
		static KDvoid		updateParticleDisplay	( Bone* pBone, cocos2d::Node* pDisplay, KDfloat dt) ;
};

}

#endif	// __CCDisplayFactory_h__
