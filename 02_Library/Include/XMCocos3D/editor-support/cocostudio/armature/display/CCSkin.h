/* -----------------------------------------------------------------------------------
 *
 *      File            CCSkin.h
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

#ifndef __CCSkin_h__
#define __CCSkin_h__

#include "../utils/CCArmatureDefine.h"
#include "../CCBone.h"
#include "../../../../2d/sprite_nodes/CCSprite.h"

namespace cocostudio {

class Skin : public cocos2d::Sprite
{
	public :

		static Skin*				create ( KDvoid );
		static Skin*				createWithSpriteFrameName ( const KDchar* pszSpriteFrameName );
		static Skin*				create ( const KDchar* pszFileName );

	public :

		Skin ( KDvoid );

		virtual KDbool				initWithSpriteFrameName ( const std::string& sSpriteFrameName ) override;
		virtual KDbool				initWithFile ( const std::string& sFilename ) override;

		KDvoid						updateArmatureTransform ( KDvoid );
		KDvoid						updateTransform ( KDvoid ) override;

		cocos2d::AffineTransform	getNodeToWorldTransform ( KDvoid ) const override;
		cocos2d::AffineTransform	getNodeToWorldTransformAR ( KDvoid ) const;

		virtual KDvoid				setSkinData ( const BaseData& tData );
		virtual const BaseData&		getSkinData ( KDvoid ) const;

		virtual KDvoid				setBone ( Bone* pBone );
		virtual Bone*				getBone ( KDvoid ) const;

		virtual const std::string&	getDisplayName ( KDvoid ) const { return m_sDisplayName; }

	protected :

		BaseData					m_tSkinData;
		Bone*						m_pBone;
		Armature*					m_pArmature;
		cocos2d::AffineTransform	m_tSkinTransform;
		std::string					m_sDisplayName;
};

}

#endif	// __CCSkin_h__
