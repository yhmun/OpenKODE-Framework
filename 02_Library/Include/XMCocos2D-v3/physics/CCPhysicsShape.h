/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsShape.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2013      cocos2d-x.org
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

#ifndef __CCPhysicsShape_h__
#define __CCPhysicsShape_h__

#include "CCPhysicsSetting.h"
#ifdef CC_USE_PHYSICS

#include "../base/CCObject.h"
#include "../base/CCGeometry.h"

NS_CC_BEGIN

class PhysicsShapeInfo;
class PhysicsBody;
class PhysicsBodyInfo;


typedef struct PhysicsMaterial
{
    KDfloat			density;
    KDfloat			restitution;
    KDfloat			friction;
    
    PhysicsMaterial ( KDvoid )
    : density ( 0.0f )
    , restitution ( 0.0f )
    , friction ( 0.0f )
    {

	}
    
    PhysicsMaterial ( KDfloat fDensity, KDfloat fRestitution, KDfloat fFriction )
    : density ( fDensity )
    , restitution ( fRestitution )
    , friction ( fFriction )
    {

	}
} PhysicsMaterial;

const PhysicsMaterial PHYSICSSHAPE_MATERIAL_DEFAULT ( 0.0f, 0.5f, 0.5f );

/**
 *	@brief A shape for body. You do not create PhysicsWorld objects directly, instead, you can view PhysicsBody to see how to create it.
 */
class PhysicsShape : public Object
{
	public :

		enum class Type
		{
			UNKNOWN,
			CIRCLE,
			BOX,
			POLYGEN,
			EDGESEGMENT,
			EDGEBOX,
			EDGEPOLYGEN,
			EDGECHAIN,
		};
    
	public :

		inline PhysicsBody*				getBody ( KDvoid ) const { return m_pBody; }
		inline Type						getType ( KDvoid ) const { return m_eType; }
		inline KDfloat					getArea ( KDvoid ) const { return m_fArea; }
		inline KDfloat					getMoment ( KDvoid ) const { return m_fMoment; }
		KDvoid							setMoment ( KDfloat fMoment );
		inline KDvoid					setTag ( KDint nTag ) { m_nTag = nTag; }
		inline KDint					getTag ( KDvoid ) const { return m_nTag; }
    
		inline KDfloat					getMass ( KDvoid ) const { return m_fMass; }
		KDvoid							setMass ( KDfloat fMass );
		inline KDfloat					getDensity ( KDvoid ) const { return m_eMaterial.density; }
		KDvoid							setDensity ( KDfloat fDensity );
		KDvoid							setRestitution ( KDfloat fRestitution );
		KDvoid							setFriction ( KDfloat fFriction );
		KDvoid							setMaterial ( const PhysicsMaterial& tMaterial );
    
		virtual KDfloat					calculateDefaultMoment ( KDvoid ) { return 0; }
		virtual KDfloat					calculateDefaultArea ( KDvoid ) { return 0; }
		virtual Point					getOffset ( KDvoid ) { return Point::ZERO; }
		virtual Point					getCenter ( KDvoid ) { return getOffset ( ); }
		KDbool							containsPoint ( const Point& tPoint ) const;
    
		static Point*					recenterPoints ( Point* pPoints, KDint nCount, const Point& tCenter = Point::ZERO );
		static Point					getPolyonCenter ( const Point* pPoints, KDint nCount );
    
		inline KDvoid					setCategoryBitmask		( KDint nBitmask ) { m_nCategoryBitmask = nBitmask; }
		inline KDint					getCategoryBitmask		( KDvoid ) const { return m_nCategoryBitmask; }
		inline KDvoid					setContactTestBitmask	( KDint nBitmask ) { m_nContactTestBitmask = nBitmask; }
		inline KDint					getContactTestBitmask	( KDvoid ) const { return m_nContactTestBitmask; }
		inline KDvoid					setCollisionBitmask		( KDint nBitmask ) { m_nCollisionBitmask = nBitmask; }
		inline KDint					getCollisionBitmask		( KDvoid ) const { return m_nCollisionBitmask; }
    
		KDvoid							setGroup ( KDint nGroup );
		inline KDint					getGroup ( KDvoid ) { return m_nGroup; }
    
	protected :

		KDbool							init ( Type eType );
    
		/**
		 *	@brief PhysicsShape is PhysicsBody's friend class, but all the subclasses isn't. so this method is use for subclasses to catch the bodyInfo from PhysicsBody.
		 */
		PhysicsBodyInfo*				bodyInfo ( KDvoid ) const;
    
		KDvoid							setBody ( PhysicsBody* pBody );
    
	protected :

				 PhysicsShape ( KDvoid );
		virtual ~PhysicsShape ( KDvoid ) = 0;
    
	protected :

		PhysicsBody*					m_pBody;
		PhysicsShapeInfo*				m_pInfo;
		Type							m_eType;
		KDfloat							m_fArea;
		KDfloat							m_fMass;
		KDfloat							m_fMoment;
		PhysicsMaterial					m_eMaterial;
		KDint							m_nTag;
		KDint							m_nCategoryBitmask;
		KDint							m_nCollisionBitmask;
		KDint							m_nContactTestBitmask;
		KDint							m_nGroup;
    
		friend class PhysicsWorld;
		friend class PhysicsBody;
		friend class PhysicsJoint;
		friend class PhysicsDebugDraw;
};

/** A circle shape */
class PhysicsShapeCircle : public PhysicsShape
{
	public :

		static PhysicsShapeCircle*		create ( KDfloat fRadius, const PhysicsMaterial& tMaterial = PHYSICSSHAPE_MATERIAL_DEFAULT, const Point& tOffset = Point ( 0, 0 ) );
		static KDfloat					calculateArea ( KDfloat fRadius );
		static KDfloat					calculateMoment ( KDfloat fMass, KDfloat fRadius, const Point& tOffset = Point::ZERO );
    
		virtual KDfloat					calculateDefaultArea ( KDvoid ) override;
		virtual KDfloat					calculateDefaultMoment ( KDvoid ) override;
    
		KDfloat							getRadius ( KDvoid ) const;
		virtual Point					getOffset ( KDvoid ) override;

	protected :

		KDbool							init ( KDfloat fRadius, const PhysicsMaterial& tMaterial = PHYSICSSHAPE_MATERIAL_DEFAULT, const Point& tOffset = Point::ZERO );
    
	protected :

				 PhysicsShapeCircle ( KDvoid );
		virtual ~PhysicsShapeCircle ( KDvoid );
};

/** A box shape */
class PhysicsShapeBox : public PhysicsShape
{
	public :

		static PhysicsShapeBox*			create ( const Size& tSize, const PhysicsMaterial& tMaterial = PHYSICSSHAPE_MATERIAL_DEFAULT, const Point& tOffset = Point::ZERO );
		static KDfloat					calculateArea ( const Size& tSize );
		static KDfloat					calculateMoment ( KDfloat fMass, const Size& tSize, const Point& tOffset = Point::ZERO );
    
		virtual KDfloat					calculateDefaultArea ( KDvoid ) override;
		virtual KDfloat					calculateDefaultMoment ( KDvoid ) override;
    
		KDvoid							getPoints ( Point* pOutPoints ) const;
		Size							getSize ( KDvoid ) const;
		virtual Point					getOffset ( KDvoid ) override { return m_tOffset; }
    
	protected :

		KDbool							init ( const Size& tSize, const PhysicsMaterial& tMaterial = PHYSICSSHAPE_MATERIAL_DEFAULT, const Point& tOffset = Point::ZERO );
    
	protected :

				 PhysicsShapeBox ( KDvoid );
		virtual ~PhysicsShapeBox ( KDvoid );
    
	protected :

		Point							m_tOffset;
};

/** A polygon shape */
class PhysicsShapePolygon : public PhysicsShape
{
	public :

		static PhysicsShapePolygon*		create ( const Point* pPoints, KDint nCount, const PhysicsMaterial& tMaterial = PHYSICSSHAPE_MATERIAL_DEFAULT, const Point& tOffset = Point::ZERO );
		static KDfloat					calculateArea ( const Point* pPoints, KDint nCount );
		static KDfloat					calculateMoment ( KDfloat fMass, const Point* pPoints, KDint nCount, const Point& tOffset = Point::ZERO );
		
		KDfloat							calculateDefaultArea ( KDvoid ) override;
		KDfloat							calculateDefaultMoment ( KDvoid ) override;
    
		Point							getPoint ( KDint i ) const;
		KDvoid							getPoints ( Point* pOutPoints ) const;
		KDint32							getPointsCount ( KDvoid ) const;
		virtual Point					getCenter ( KDvoid ) override;

	protected :

		KDbool							init ( const Point* pPoints, KDint nCount, const PhysicsMaterial& tMaterial = PHYSICSSHAPE_MATERIAL_DEFAULT, const Point& tOffset = Point::ZERO );
    
	protected :

				 PhysicsShapePolygon ( KDvoid );
		virtual ~PhysicsShapePolygon ( KDvoid );
    
	protected :

		Point							m_tCenter;
};

/** A segment shape */
class PhysicsShapeEdgeSegment : public PhysicsShape
{
	public :

		static PhysicsShapeEdgeSegment*	create ( const Point& a, const Point& b, const PhysicsMaterial& tMaterial = PHYSICSSHAPE_MATERIAL_DEFAULT, KDfloat fBorder = 1 );
    
		Point							getPointA ( KDvoid ) const;
		Point							getPointB ( KDvoid ) const;
		virtual Point					getCenter ( KDvoid ) override;
    
	protected :

		KDbool							init ( const Point& a, const Point& b, const PhysicsMaterial& tMaterial = PHYSICSSHAPE_MATERIAL_DEFAULT, KDfloat fBorder = 1 );
    
	protected :

				 PhysicsShapeEdgeSegment ( KDvoid );
		virtual ~PhysicsShapeEdgeSegment ( KDvoid );
    
	protected :

		Point							m_tCenter;
    
		friend class PhysicsBody;
};

/** An edge box shape */
class PhysicsShapeEdgeBox : public PhysicsShape
{
	public :

		static PhysicsShapeEdgeBox*		create ( const Size& tSize, const PhysicsMaterial& tMaterial = PHYSICSSHAPE_MATERIAL_DEFAULT, KDfloat fBorder = 0, const Point& tOffset = Point::ZERO );

		virtual Point					getOffset ( KDvoid ) override { return m_tOffset; }
		KDvoid							getPoints ( const Point* pOutPoints ) const;
		KDint32							getPointsCount ( KDvoid ) const;
    
	protected :

		KDbool							init ( const Size& tSize, const PhysicsMaterial& material = PHYSICSSHAPE_MATERIAL_DEFAULT, KDfloat fBorder = 1, const Point& tOffset = Point::ZERO );
    
	protected :

				 PhysicsShapeEdgeBox ( KDvoid );
		virtual ~PhysicsShapeEdgeBox ( KDvoid );
    
	protected :

		Point							m_tOffset;
    
		friend class PhysicsBody;
};

/** An edge polygon shape */
class PhysicsShapeEdgePolygon : public PhysicsShape
{
	public :

		static PhysicsShapeEdgePolygon* create ( const Point* pPoints, KDint nCount, const PhysicsMaterial& tMaterial = PHYSICSSHAPE_MATERIAL_DEFAULT, KDfloat fBorder = 1 );

		virtual Point					getCenter ( KDvoid ) override;
		KDvoid							getPoints ( Point* pOutPoints ) const;
		KDint32							getPointsCount ( KDvoid ) const;
    
	protected :

		KDbool							init ( const Point* pPoints, KDint nCount, const PhysicsMaterial& tMaterial = PHYSICSSHAPE_MATERIAL_DEFAULT, KDfloat fBorder = 1 );
    
	protected :

				 PhysicsShapeEdgePolygon ( KDvoid );
		virtual ~PhysicsShapeEdgePolygon ( KDvoid );
    
		friend class PhysicsBody;
    
	protected :

		Point							m_tCenter;
};

/** a chain shape */
class PhysicsShapeEdgeChain : public PhysicsShape
{
	public :

		static PhysicsShapeEdgeChain*	create ( const Point* pPoints, KDint nCount, const PhysicsMaterial& tMaterial = PHYSICSSHAPE_MATERIAL_DEFAULT, KDfloat fBorder = 1 );

		virtual Point					getCenter ( KDvoid ) override;
		KDvoid							getPoints ( Point* pOutPoints ) const;
		KDint32							getPointsCount ( KDvoid ) const;
    
	protected :

		KDbool							init ( const Point* pPoints, KDint nCount, const PhysicsMaterial& tMaterial = PHYSICSSHAPE_MATERIAL_DEFAULT, KDfloat fBorder = 1 );
    
	protected :

				 PhysicsShapeEdgeChain ( KDvoid );
		virtual ~PhysicsShapeEdgeChain ( KDvoid );
    
	protected :

		Point							m_tCenter;
    
		friend class PhysicsBody;
};

NS_CC_END

#endif	// CC_USE_PHYSICS
#endif	// __CCPhysicsShape_h__
