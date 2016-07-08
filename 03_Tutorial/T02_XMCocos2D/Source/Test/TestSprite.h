/* --------------------------------------------------------------------------
 *
 *      File            TestSprite.h
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

#ifndef __TestSprite_h__
#define __TestSprite_h__

#include "../TestBasic.h"

class TestSprite : public TestBasic
{
	public :

		static  TestSprite*		create ( KDvoid );

	protected :		

		virtual KDuint			count  ( KDvoid );
};

class Sprite1 : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid					addNewSpriteWithCoords ( CCPoint tPosition );		
};

class SpriteBatchNode1 : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid					addNewSpriteWithCoords ( CCPoint p );		
};

class SpriteColorOpacity : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		KDvoid					removeAndAddSprite ( KDfloat dt );
};

class SpriteBatchNodeColorOpacity : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		KDvoid					removeAndAddSprite ( KDfloat dt );
};

class SpriteZOrder : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		KDvoid					reorderSprite ( KDfloat dt );

	protected :

		KDint					m_nDir;
};

class SpriteBatchNodeZOrder : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		KDvoid					reorderSprite ( KDfloat dt );

	protected :

		KDint					m_nDir;
};

class SpriteBatchNodeReorder : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class SpriteZVertex: public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );

	protected :

		KDint					m_nDir;
		KDfloat					m_fTime;
};

class SpriteBatchNodeZVertex: public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );

	protected :

		KDint					m_nDir;
		KDfloat					m_fTime;
};

class SpriteAnchorPoint : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class SpriteBatchNodeAnchorPoint : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class Sprite6 : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class SpriteFlip : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		KDvoid					flipSprites ( KDfloat fDelta );
};

class SpriteBatchNodeFlip : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		KDvoid					flipSprites ( KDfloat dt );
};

class SpriteAliased : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteBatchNodeAliased : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteNewTexture : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );

		virtual	KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid					addNewSprite ( KDvoid );

	protected :

		KDbool					m_bUsingTexture1;
		CCTexture2D*			m_pTexture1;
		CCTexture2D*			m_pTexture2;
};

class SpriteBatchNodeNewTexture : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );

		virtual	KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid					addNewSprite ( KDvoid );

	protected :

		CCTexture2D*			m_pTexture1;
		CCTexture2D*			m_pTexture2;
};

class SpriteFrameTest : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );

	    KDvoid					startIn05Secs ( KDfloat fDelta );
		KDvoid					flipSprites   ( KDfloat fDelta );

	protected :

		CCSprite*				m_pSprite1;
		CCSprite*				m_pSprite2;
		KDint					m_nCounter;
};


class SpriteFrameAliasNameTest : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteOffsetAnchorRotation: public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteBatchNodeOffsetAnchorRotation : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteOffsetAnchorScale : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteBatchNodeOffsetAnchorScale : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteOffsetAnchorSkew : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteOffsetAnchorRotationalSkew : public TestSprite
{
	public :
		
		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteBatchNodeOffsetAnchorSkew : public TestSprite
{
	public :
		
		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteOffsetAnchorRotationalSkewScale : public TestSprite
{
	public :
		
		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteBatchNodeOffsetAnchorRotationalSkew : public TestSprite
{
	public :
		
		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteOffsetAnchorSkewScale : public TestSprite
{
	public :
		
 		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteBatchNodeOffsetAnchorSkewScale : public TestSprite
{
	public :
		
		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteBatchNodeOffsetAnchorRotationalSkewScale : public TestSprite
{
	public :
		
		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteOffsetAnchorFlip : public TestSprite
{
	public :
		
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteBatchNodeOffsetAnchorFlip : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteAnimationSplit : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteHybrid : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );

		KDvoid					reparentSprite ( KDfloat fDelta );

	protected :

		KDbool					m_bUsingSpriteBatchNode;
};

class SpriteBatchNodeChildren : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteBatchNodeChildrenZ : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteChildrenVisibility: public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteChildrenAnchorPoint: public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteBatchNodeChildrenAnchorPoint : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteBatchNodeChildrenScale : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class SpriteChildrenChildren : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class SpriteBatchNodeChildrenChildren : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class SpriteNilTexture: public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class SpriteSubclass : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class AnimationCache : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class NodeSort : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		KDvoid					reorderSprite ( KDfloat fDelta );

	private :

		CCNode*					m_pNode;
		CCSprite*				m_pSprite1;
		CCSprite*				m_pSprite2;
		CCSprite*				m_pSprite3;
		CCSprite*				m_pSprite4;
		CCSprite*				m_pSprite5;
};

class SpriteBatchNodeReorderSameIndex : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		KDvoid					reorderSprite ( KDfloat fDelta );

	private :
		CCSpriteBatchNode*		m_pBatchNode;
		CCSprite*				m_pSprite1;
		CCSprite*				m_pSprite2;
		CCSprite*				m_pSprite3;
		CCSprite*				m_pSprite4;
		CCSprite*				m_pSprite5;
};

class SpriteBatchNodeReorderOneChild : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		KDvoid					reorderSprite ( KDfloat fDelta );

	private :

		CCSpriteBatchNode*		m_pBatchNode;
		CCSprite*				m_pReorderSprite;
};

class SpriteBatchNodeSkewNegativeScaleChildren : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteBatchNodeRotationalSkewNegativeScaleChildren : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class SpriteSkewNegativeScaleChildren : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteRotationalSkewNegativeScaleChildren : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class SpriteDoubleResolution : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class AnimationCacheFile : public TestSprite
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

#endif	// __TestSprite_h__
