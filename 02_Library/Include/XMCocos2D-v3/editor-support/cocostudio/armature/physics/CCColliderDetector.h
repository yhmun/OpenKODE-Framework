/* -----------------------------------------------------------------------------------
 *
 *      File            CCColliderDetector.h
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

#ifndef __CCColliderDetector_h__
#define __CCColliderDetector_h__

#include "../utils/CCArmatureDefine.h"
#include "../datas/CCDatas.h"

#ifndef PT_RATIO
#define PT_RATIO			32
#endif

struct cpBody;
struct cpShape;

class b2Body;
class b2Fixture;
struct b2Filter;

namespace cocostudio {

class Bone;

class ColliderFilter
{
	public :

				 ColliderFilter ( KDvoid );
		virtual ~ColliderFilter ( KDvoid ) { }

	public :

		ColliderFilter ( KDushort uCategoryBits, KDushort uMaskBits, KDshort m_nGroupIndex );	// 0x0001, 0xFFFF, 0

		KDvoid							updateB2Shape ( b2Fixture* pFixture );

		virtual KDvoid					setCategoryBits ( KDushort uCategoryBits ) { m_uCategoryBits = uCategoryBits; }
		virtual KDushort				getCategoryBits ( KDvoid ) const { return m_uCategoryBits; }

		virtual KDvoid					setMaskBits ( KDushort uMaskBits ) { m_uMaskBits = uMaskBits; }
		virtual KDushort				getMaskBits ( KDvoid ) const { return m_uMaskBits; }

		virtual KDvoid					setGroupIndex ( KDshort nGroupIndex ) { m_nGroupIndex = nGroupIndex; }
		virtual KDshort					getGroupIndex ( ) const { return m_nGroupIndex; }

	protected :

		KDushort						m_uCategoryBits;
		KDushort						m_uMaskBits;
		KDshort							m_nGroupIndex;

	public :

		ColliderFilter ( KDuintptr uCollisionType, KDuintptr uGroup );	// 0, 0

		KDvoid							updateCPShape ( cpShape* pShape );

		virtual KDvoid					setCollisionType ( KDuintptr uCollisionType ) { m_uCollisionType = uCollisionType; }
		virtual KDuintptr				getCollisionType ( KDvoid ) const { return m_uCollisionType; }

		virtual KDvoid					setGroup ( KDuintptr uGroup ) { m_uGroup = uGroup; }
		virtual KDuintptr				getGroup ( KDvoid ) const { return m_uGroup; }

	protected :

		KDuintptr						m_uCollisionType;
		KDuintptr						m_uGroup;
};

class ColliderBody : public cocos2d::Object
{
	public :

		 ColliderBody ( ContourData* pContourData );
		~ColliderBody ( KDvoid );

		inline ContourData*				getContourData ( KDvoid ) { return m_pContourData; }

		KDvoid							setColliderFilter ( ColliderFilter* pFilter );
		ColliderFilter*					getColliderFilter ( KDvoid );

		virtual KDvoid					setB2Fixture ( b2Fixture* pFixture ) { m_pFixture = pFixture; }
		virtual b2Fixture*				getB2Fixture ( KDvoid ) const { return m_pFixture; }

		virtual KDvoid					setShape ( cpShape* pShape ) { m_pShape = pShape; }
		virtual cpShape*				getShape ( KDvoid ) const { return m_pShape; }
	

	#if ENABLE_PHYSICS_SAVE_CALCULATED_VERTEX
		virtual const cocos2d::Array*	getCalculatedVertexList ( KDvoid ) const { return m_pCalculatedVertexList; }
	#endif
	private :

		b2Fixture*						m_pFixture;
		cpShape*						m_pShape;

		ContourData*					m_pContourData;
		ColliderFilter*					m_pFilter;

		cocos2d::Array*					m_pCalculatedVertexList;
};

/**
 *  @brief  ContourSprite used to draw the contour of the display
 */
class ColliderDetector : public cocos2d::Object
{
	public :

		static ColliderDetector*		create ( KDvoid );
		static ColliderDetector*		create ( Bone* pBone );

	public :

		/**
		 *	@js ctor
		 */
		ColliderDetector ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~ColliderDetector ( KDvoid );

		virtual KDbool					init ( KDvoid );
		virtual KDbool					init ( Bone* pBone );

		KDvoid							addContourData ( ContourData* pContourData );
		KDvoid							addContourDataList ( cocos2d::Array* pContourDataList );

		KDvoid							removeContourData ( ContourData* pContourData );
		KDvoid							removeAll ( KDvoid );

		KDvoid							updateTransform ( cocos2d::AffineTransform& t );

		KDvoid							setActive ( KDbool bActive );
		KDbool							getActive ( KDvoid );

		cocos2d::Array*					getColliderBodyList ( KDvoid );

		virtual KDvoid					setColliderFilter ( ColliderFilter* pFilter );
		virtual ColliderFilter*			getColliderFilter ( KDvoid );

		virtual KDvoid					setBone ( Bone* pBone ) { m_pBone = pBone; }
		virtual Bone*					getBone ( KDvoid ) const { return m_pBone; }

		virtual KDvoid					setB2Body ( b2Body* pBody );
		virtual b2Body*					getB2Body ( KDvoid ) const;

		virtual KDvoid					setCPBody ( cpBody* pBody );
		virtual cpBody*					getCPBody ( KDvoid ) const;

	 protected :

		cocos2d::Array*					m_pColliderBodyList;
		ColliderFilter*					m_pFilter;

		Bone*							m_pBone;

		b2Body*							m_pB2Body;
		cpBody*							m_pCPBody;

	protected :

		KDbool							m_bActive;
};

}

#endif	// __CCColliderDetector_h__
