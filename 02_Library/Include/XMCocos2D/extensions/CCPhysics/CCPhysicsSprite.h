/* --------------------------------------------------------------------------
 *
 *      File            CCPhysicsSprite.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2013 cocos2d-x.org
 *      Copyright (c) 2012 Scott Lembcke and Howling Moon Software
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

#ifndef __CCPhysicsSprite_h__
#define __CCPhysicsSprite_h__

#include "../../sprite_nodes/CCSprite.h"
#include "XMChipmunk/chipmunk.h"
#include "XMBox2D/Box2D.h"

NS_CC_BEGIN

/**
 *	A CCSprite subclass that is bound to a physics body.
 *
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
class CCPhysicsSprite : public CCSprite
{
	public :
				 CCPhysicsSprite ( KDvoid );
		virtual ~CCPhysicsSprite ( KDvoid );

		static	 CCPhysicsSprite*		create ( KDvoid );

		/**
		 *	Creates an sprite with a texture.
		 *	The rect used will be the size of the texture.
		 *	The offset will be (0,0).
		 */
		static	 CCPhysicsSprite*		createWithTexture ( CCTexture2D* pTexture );

		/**
		 *	Creates an sprite with a texture and a rect.
		 *	The offset will be (0,0).
		 */
		static	 CCPhysicsSprite*		createWithTexture ( CCTexture2D* pTexture, const CCRect& tRect );

		/** Creates an sprite with an sprite frame. */
		static	 CCPhysicsSprite*		createWithSpriteFrame ( CCSpriteFrame* pSpriteFrame );

		/**
		 *	Creates an sprite with an sprite frame name.
		 *	An CCSpriteFrame will be fetched from the CCSpriteFrameCache by name.
		 *	If the CCSpriteFrame doesn't exist it will raise an exception.
		 *	@since v0.9
		 */
		static	 CCPhysicsSprite*		createWithSpriteFrameName ( const KDchar* szSpriteFrameName );

		/** 
		 *	Creates an sprite with an image filename.
		 *	The rect used will be the size of the image.
		 *	The offset will be (0,0).
		 */
		static	 CCPhysicsSprite*		create ( const KDchar* szFileName );

		/**
		 *	Creates an sprite with an image filename and a rect.
		 *	The offset will be (0,0).
		 */
		static	 CCPhysicsSprite*		create ( const KDchar* szFileName, const CCRect& tRect );

	public :

		virtual KDbool				isDirty ( KDvoid );
	    
		/** Keep the sprite's rotation separate from the body. */
		KDbool						 isIgnoreBodyRotation ( KDvoid ) const;
		KDvoid						setIgnoreBodyRotation ( KDbool bIgnoreBodyRotation );
	    	    
		virtual const CCPoint&		getPosition  ( KDvoid );
		virtual KDvoid				getPosition  ( KDfloat* x, KDfloat* y );
		virtual KDfloat				getPositionX ( KDvoid );
		virtual KDfloat				getPositionY ( KDvoid );
		virtual KDvoid				setPosition  ( const CCPoint &tPosition );

		virtual KDfloat				getRotation ( KDvoid );
		virtual KDvoid				setRotation ( KDfloat fRotation );

		virtual CCAffineTransform	nodeToParentTransform ( KDvoid );
	    
		/** Body accessor when using regular Chipmunk */
		cpBody*						getCPBody ( KDvoid ) const;
		KDvoid						setCPBody ( cpBody* pBody );	   
	    
		/** Body accessor when using box2d */
		b2Body*						getB2Body ( KDvoid ) const;
		KDvoid						setB2Body ( b2Body* pBody );

		KDfloat						getPTMRatio ( KDvoid ) const;
		KDvoid						setPTMRatio ( KDfloat fPTMRatio );

	protected :

		KDvoid						updatePosFromPhysics ( KDvoid );

	protected :

		KDbool						m_bIgnoreBodyRotation;
		cpBody*						m_pCPBody;
		b2Body*						m_pB2Body;		
		KDfloat						m_fPTMRatio;	// Pixels to Meters ratio
};

NS_CC_END

#endif	// __CCPhysicsSprite_h__
