/* --------------------------------------------------------------------------
 *
 *      File            M3GSprite3D.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010 UEDA.Takashi
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

#ifndef __M3GSprite3D_h__
#define __M3GSprite3D_h__

#include "M3GNode.h"

class M3GAppearance;
class M3GImage2D;
class M3GVector;
class M3GCamera;
class M3GRayIntersection;

/**
 * @~English	A scene graph node that represents a 2-dimensional image with a 3D position.
 */
class M3GSprite3D : public M3GNode
{
	protected :

		M3GSprite3D ( KDvoid );

	public :

		/**
		 * @~English	Constructs a new Sprite3D with the given scaling mode, image and appearance.
		 * @param[in]	scaled
		 * @param[in]	image
		 * @param[in]	appearance
		 */
		M3GSprite3D ( KDbool scaled, M3GImage2D* image, M3GAppearance* appearance );

		/**
		 * @~English	Destructs this object.
		 */
		virtual ~M3GSprite3D ( KDvoid );

	public :

		virtual KDvoid  addAnimationTrack ( M3GAnimationTrack* animationTrack );

		virtual KDint  animate ( KDint time );

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		virtual KDint  getReferences ( M3GObject3D** references ) const;

		/**
		 * @~English	Gets the current Appearance of this Sprite3D.
		 * @return
		 */
		M3GAppearance*  getAppearance ( KDvoid ) const;

		/**
		 * @~English	Gets the current cropping rectangle height within the source image.
		 * @return
		 */
		KDint	getCropHeight ( KDvoid ) const;

		/**
		 * @~English	Gets the current cropping rectangle width within the source image.
		 * @return
		 */
		KDint	getCropWidth ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current cropping rectangle X offset relative to the source image top left corner.
		 * @return
		 */
		KDint	getCropX ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current cropping rectangle Y offset relative to the source image top left corner.
		 * @return
		 */
		KDint	getCropY ( KDvoid ) const;

		/**
		 * @~English	Gets the current Sprite3D image.
		 * @return
		 */
		M3GImage2D*  getImage ( KDvoid ) const;

		/**
		 * @~English	Returns the automatic scaling status of this Sprite3D.
		 * @return
		 */
		KDbool	isScaled ( KDvoid ) const;

		/**
		 * @~English	Sets the Appearance of this Sprite3D.
		 * @param[in]	appearance
		 */
		KDvoid	setAppearance ( M3GAppearance* appearance );

		/**
		 * @~English	Sets a cropping rectangle within the source image.
		 * @param[in]	cropX
		 * @param[in]	cropY
		 * @param[in]	cropWidth
		 * @param[in]	cropHeight
		 */
		KDvoid	setCrop ( KDint cropX, KDint cropY, KDint cropWidth, KDint cropHeight );

		/**
		 * @~English	Sets the sprite image to display.
		 * @param[in]	image
		 */
		KDvoid	setImage ( M3GImage2D* image );

	public :

		virtual KDvoid  render ( M3GRenderState& state ) const; 

		KDbool  intersect ( const M3GVector& org, const M3GVector& dir, const M3GCamera* camera, M3GRayIntersection* ri ) const;

	protected :

		KDvoid  init ( KDbool scaled, M3GImage2D* image, M3GAppearance* appearance );
		KDvoid  _duplicate ( M3GSprite3D* pObj ) const;

	private :

		M3GVector	getCenterPoint  ( const M3GCamera* camera ) const;

		KDfloat		getScaledWidth  ( const M3GCamera* camera ) const;
		KDfloat		getScaledHeight ( const M3GCamera* camera ) const;

		KDfloat		getUnscaledWidth  ( KDvoid ) const;
		KDfloat		getUnscaledHeight ( KDvoid ) const;

		M3GVector	getTexCoord0 ( KDvoid ) const;
		M3GVector	getTexCoord1 ( KDvoid ) const;

	private :

		KDuint					m_uIndicesID;
		KDuint					m_uPositionID;
		KDuint					m_uTexCoordID;
		KDuint					m_uTextureID;
		KDbool					m_bScaled;
		M3GImage2D*				m_pImage;
		M3GAppearance*			m_pAppearance;
		KDfloat					m_fCropX;
		KDfloat					m_fCropY;
		KDfloat					m_fCropWidth;
		KDfloat					m_fCropHeight;

		friend class M3GLoader;
};

#endif
