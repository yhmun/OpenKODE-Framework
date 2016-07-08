/* --------------------------------------------------------------------------
 *
 *      File            APSSprite.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com  
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

#ifndef __APSSprite_h__
#define __APSSprite_h__

#include "APSGraphic.h"
#include "../../sprite_nodes/CCSprite.h"

NS_APS_BEGIN

class APSImageHolder;
class APSSpriteSheetHolder;

/** 
 *	@brief APSSprite
 *	APSSprite create a CCSprite object. To support multiple screen sizes with 
 *	different width and height ratio, APSSprite creates CCNode object and attaches
 *	CCSprite object to it as a child. scaleX and scaleY properties of the CCSprite
 *	object are adjusted to fit loading image to APSSprite object's width and height
 *	properties.
 */
class APSSprite : public APSGraphic 
{
	public :

		APSSprite ( APSDictionary* pProperties = KD_NULL );

		virtual ~APSSprite ( KDvoid );

		APS_CLASS_NAME ( APSSprite );

	public :
	    
		virtual KDbool  initWithProperties ( APSDictionary* pProperties );
	        
		/**
		 *	Initializes CCNode objects in APSGraphic. initNode ( ) can be called 
		 *	multiple times to reset CCNode objects' states back to original.
		 */
		virtual KDbool  initNode ( KDvoid );
	    
		/** 
		 *	Updates accOpacity, accOpacity value propagates to the all descendant
		 *	graphics. Opacities are multiplied from parent to children thoughout 
		 *	graphics hierarchy. For performance reason, updateAccOpacity ( ) should
		 *	be called as minimum times as you can.
		 */
		virtual KDvoid  updateAccOpacity ( KDvoid );
	    
		/** Returns an APSImageHolder object in APSResourceManager. imageHolderCode
		 must be set to use this method.
		 */
		APSImageHolder*  getImageHolder ( KDvoid );

		APSSpriteSheetHolder*  getSpriteSheetHolder ( KDvoid );
	    
		/** Returns position of this node in parent symbol's coordinations. */
		CCPoint  getPositionInSymbol ( KDvoid );

		/** 
		 *	Assigns properties to a given cocos2d sprite for none ArtPig 
		 *	environment. Properties relative to the symbol's coordinations are 
		 *	assigned to the sprite instead of parent graphic group's coordinations.
		 *	The scale parameter is multiplied to points and sizes to adjust scale of 
		 *	coordinations.
		 */
		virtual KDvoid  assignSpriteProperties ( CCSprite* pSprite, KDfloat fScale = 1.0f );

		virtual KDvoid  updateSpriteFrame ( KDvoid );
	    
	protected :

		virtual CCNode*  createNode ( KDvoid );

		APS_SYNTHESIZE_WITH_KEY ( KDbool, m_bIsAnimatable, IsAnimatable );
	    
		APS_SYNTHESIZE_WITH_KEY ( KDbool, m_bFlipX, FlipX );
		APS_SYNTHESIZE_WITH_KEY ( KDbool, m_bFlipY, FlipY );
	    
		/**
		 *	A code value referencing to APSImageHolder in APSResourceManager.
		 *	APSImageHolder provides the most appropriate image for the current device.
		 */
		APS_SYNTHESIZE_STRING_WITH_KEY ( m_sImageHolderCode, ImageHolderCode );
	    
	protected :

		CCSprite*			m_pSpriteNode;
		APSImageHolder*		m_pImageHolder;
};

NS_APS_END

#endif	// __APSSprite_h__
