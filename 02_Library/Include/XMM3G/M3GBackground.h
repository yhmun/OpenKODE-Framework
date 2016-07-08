/* --------------------------------------------------------------------------
 *
 *      File            M3GBackground.h
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

#ifndef __M3GBackground_h__
#define __M3GBackground_h__

#include "M3GObject3D.h"

class M3GImage2D;

/**
 * @~English	Defines whether and how to clear the viewport.
 */
class M3GBackground : public M3GObject3D
{
	public :

		/**
		 * @~English	Specifies that the imaginary pixels outside of the source image boundaries
		 *				in X or Y direction are considered to have the background color.
		 */
		static const KDint	BORDER = 32;

		/**
		 * @~English	Specifies that the imaginary pixels outside of the source image boundaries
		 *				in X or Y direction are considered to have the same color as the pixel 
		 *				in the corresponding position in the source image.
		 */
		static const KDint	REPEAT = 33;

	public :

		/**
		 * @~English	Constructs a new Background with default values.
		 */
		M3GBackground ( KDvoid );

		/**
		 * @~English	Destructs this object.
		 */
		virtual ~M3GBackground ( KDvoid );

	public :

		virtual KDvoid  addAnimationTrack ( M3GAnimationTrack* animationTrack );

		virtual KDint  animate ( KDint time );

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		virtual KDint  getReferences ( M3GObject3D** references ) const;

		/**
		 * @~English	Retrieves the current background color.
		 */
		KDint	getColor ( KDvoid ) const;

		/**
		 * @~English	Gets the current cropping rectangle height within the source image.
		 */
		KDint	getCropHeight ( KDvoid ) const;

		/**
		 * @~English	Gets the current cropping rectangle width within the source image.
		 */
		KDint	getCropWidth ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current cropping rectangle X offset relative to the source image top left corner.
		 */
		KDint	getCropX ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current cropping rectangle Y offset relative to the source image top left corner.
		 */
		KDint	getCropY ( KDvoid ) const;

		/**
		 * @~English	Gets the current background image.
		 */
		M3GImage2D*  getImage ( KDvoid ) const;

		/**
		 * @~English	Gets the current background image repeat mode for the X dimension.
		 */
		KDint	getImageModeX ( KDvoid ) const;

		/**
		 * @~English	Gets the current background image repeat mode for the Y dimension.
		 */
		KDint	getImageModeY ( KDvoid ) const;

		/**
		 * @~English	Queries whether color buffer clearing is enabled.
		 */
		KDbool	isColorClearEnabled ( KDvoid ) const;

		/**
		 * @~English	Queries whether depth buffer clearing is enabled.
		 */
		KDbool	isDepthClearEnabled ( KDvoid ) const;

		/**
		 * @~English	Sets the background color.
		 */
		KDvoid	setColor ( KDint ARGB );

		/**
		 * @~English	Enables or disables color buffer clearing.
		 */
		KDvoid	setColorClearEnable ( KDbool enable );

		/**
		 * @~English	Sets a cropping rectangle within the background image.
		 */
		KDvoid	setCrop ( KDint cropX, KDint cropY, KDint cropWidth, KDint cropHeight );

		/**
		 * @~English	Enables or disables depth buffer clearing.
		 */
		KDvoid	setDepthClearEnable ( KDbool enable );

		/**
		 * @~English	Sets the background image, or switches from background image mode to background color mode.
		 */
		KDvoid	setImage ( M3GImage2D* image );

		/**
		 * @~English	Sets the background image repeat mode for the X and Y directions.
		 */
		KDvoid	setImageMode ( KDint modeX, KDint modeY );

	public :

		static  KDvoid  render ( KDvoid );
		virtual KDvoid  render ( M3GRenderState& state ) const; 

	protected :

		KDvoid  _duplicate ( M3GBackground* pObj ) const;

	private :

		KDuint			m_uIndicesID;
		KDuint			m_uPositionID;
		KDuint			m_uTexCoordID;
		KDuint			m_uTextureID;
        KDbool			m_bColorClearEnable;
        KDbool			m_bDepthClearEnable;
        KDint			m_nColor;
        M3GImage2D*		m_pImage;
        KDint			m_nModeX;
		KDint			m_nModeY;
		KDfloat			m_fCropX;
		KDfloat			m_fCropY;
		KDfloat			m_fCropWidth;
		KDfloat			m_fCropHeight;
};

#endif
