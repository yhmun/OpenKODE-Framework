/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsSprite.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2012      cocos2d-x.org
 *      Copyright (c) 2012      Scott Lembcke and Howling Moon Software
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

#ifndef __CCPhysicsSprite_h__
#define __CCPhysicsSprite_h__

#include "../ExtensionMacros.h"
#include "../../2d/sprite_nodes/CCSprite.h"
#include "XMChipmunk/chipmunk.h"
#include "XMBox2D/Box2D.h"

struct cpBody;
class b2Body;

NS_CC_EXT_BEGIN
/**
 *	A Sprite subclass that is bound to a physics body.
 *	It works with:
 *		- Chipmunk: Preprocessor macro CC_ENABLE_CHIPMUNK_INTEGRATION should be defined
 *		- Objective-Chipmunk: Preprocessor macro CC_ENABLE_CHIPMUNK_INTEGRATION should be defined
 *		- Box2d: Preprocessor macro CC_ENABLE_BOX2D_INTEGRATION should be defined
 *
 *	Features and Limitations:
 *		- Scale and Skew properties are ignored.
 *		- Position and rotation are going to updated from the physics body
 *		- If you update the rotation or position manually, the physics body will be updated
 *		- You can't enble both Chipmunk support and Box2d support at the same time. Only one can be enabled at compile time
 */
class PhysicsSprite : public Sprite
{
	public :

		static PhysicsSprite*			create ( KDvoid );

		/** 
		 *	Creates an sprite with a texture.
		 *	The rect used will be the size of the texture.
		 *	The offset will be (0,0).
		 */
		static PhysicsSprite*			createWithTexture ( Texture2D* pTexture );

		/** 
		 *	Creates an sprite with a texture and a rect.
		 *	The offset will be (0,0).
		 */
		static PhysicsSprite*			createWithTexture ( Texture2D* pTexture, const Rect& tRect );

		/** Creates an sprite with an sprite frame. */
		static PhysicsSprite*			createWithSpriteFrame ( SpriteFrame* pSpriteFrame );

		/**
		 *	Creates an sprite with an sprite frame name.
		 *	An SpriteFrame will be fetched from the SpriteFrameCache by name.
		 *	If the SpriteFrame doesn't exist it will raise an exception.
		 *	@since v0.9
		 */
		static PhysicsSprite*			createWithSpriteFrameName ( const KDchar* pszSpriteFrameName );

		/** 
		 *	Creates an sprite with an image filename.
		 *	The rect used will be the size of the image.
		 *	The offset will be (0,0).
		 */
		static PhysicsSprite*			create ( const KDchar* pszFileName );

		/**
		 *	Creates an sprite with an image filename and a rect.
		 *	The offset will be (0,0).
		 */
		static PhysicsSprite*			create ( const KDchar* pszFileName, const Rect& tRect );

		PhysicsSprite ( KDvoid );

		virtual KDbool					isDirty ( KDvoid ) const;

		/** Keep the sprite's rotation separate from the body. */
		KDbool							 isIgnoreBodyRotation ( KDvoid ) const;
		KDvoid							setIgnoreBodyRotation ( KDbool bIgnoreBodyRotation );

		//
		// Chipmunk specific
		//
		/** Body accessor when using regular Chipmunk */
		cpBody*							getCPBody ( KDvoid ) const;
		KDvoid							setCPBody ( cpBody* pBody );

		//
		// Box2d specific
		//

		/** Body accessor when using box2d */
		b2Body*							getB2Body ( KDvoid ) const;
		KDvoid							setB2Body ( b2Body* pBody );

		KDfloat							getPTMRatio ( KDvoid ) const;
		KDvoid							setPTMRatio ( KDfloat fPTMRatio );

		// overrides
		virtual const Point&			getPosition ( KDvoid ) const override;
		virtual KDvoid					getPosition ( KDfloat* x, KDfloat* y ) const override;
		virtual KDfloat					getPositionX ( KDvoid ) const override;
		virtual KDfloat					getPositionY ( KDvoid ) const override;
		virtual KDvoid					setPosition ( const Point& tPosition ) override;
		virtual KDfloat					getRotation ( KDvoid ) const override;
		virtual KDvoid					setRotation ( KDfloat fRotation ) override;
		virtual const AffineTransform&	getNodeToParentTransform ( KDvoid ) const override;

	protected :

		const Point&					getPosFromPhysics ( KDvoid ) const;

	protected :

		KDbool							m_bIgnoreBodyRotation;

		// chipmunk specific
		cpBody*							m_pCPBody;

		// box2d specific
		b2Body*							m_pB2Body;
		KDfloat							m_fPTMRatio;
};

NS_CC_EXT_END

#endif // __CCPhysicsSprite_h__
