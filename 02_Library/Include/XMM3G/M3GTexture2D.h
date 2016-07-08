/* --------------------------------------------------------------------------
 *
 *      File            M3GTexture2D.h
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

#ifndef __M3GTexture2D_h__
#define __M3GTexture2D_h__

#include "M3GTransformable.h"

class M3GImage2D;

/**
 * @~English	Constructs a new texture object with the given image.
 */
class M3GTexture2D : public M3GTransformable
{
	public :

		/**
		 * @~English	A level filtering parameter to setFiltering that selects the base level image, even if mipmap levels exist.
		 */
		static const KDint	FILTER_BASE_LEVEL = 208;

		/**
		 * @~English	A parameter to setFiltering that selects linear filtering.
		 */
		static const KDint	FILTER_LINEAR     = 209;

		/**
		 * @~English	A parameter to setFiltering that selects nearest neighbor filtering.
		 */
		static const KDint	FILTER_NEAREST    = 210;

		/**
		 * @~English	A parameter to setBlending, specifying that the texel color is to be added to the fragment color.
		 */
		static const KDint	FUNC_ADD          = 224;

		/**
		 * @~English	A parameter to setBlending, specifying that the texture blend color is to be blended
		 *				into the fragment color in proportion to the texel RGB values.
		 */
		static const KDint	FUNC_BLEND        = 225;

		/**
		 * @~English	A parameter to setBlending, specifying that the texel color is to be blended 
		 *				into the fragment color in proportion to the texel alpha.
		 */
		static const KDint	FUNC_DECAL        = 226;

		/**
		 * @~English	A parameter to setBlending, specifying that the texel color and/or alpha are 
		 *				to be multiplied with the fragment color and alpha.
		 */
		static const KDint	FUNC_MODULATE     = 227;

		/**
		 * @~English	A parameter to setBlending, specifying that the texel color and/or alpha are 
		 *				to replace the fragment color and alpha.
		 */
		static const KDint	FUNC_REPLACE      = 228;

		/**
		 * @~English	A parameter to setWrapping, specifying that the texture image is to be repeated only once.
		 */
		static const KDint	WRAP_CLAMP        = 240;

		/**
		 * @~English	A parameter to setWrapping, specifying that the texture image is to be repeated indefinitely.
		 */
		static const KDint	WRAP_REPEAT       = 241;

	protected :

		M3GTexture2D ( KDvoid );

	public :

		/**
		 * @~English	Constructs a new texture object with the given image, setting the texture attributes to their default values.
		 */
		M3GTexture2D ( M3GImage2D* image );

		/**
		 * @~English	Destructs this object.
		 */
		virtual ~M3GTexture2D ( KDvoid );

	public :

		virtual KDvoid  addAnimationTrack ( M3GAnimationTrack* animationTrack );

		virtual KDint  animate ( KDint time );

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		virtual KDint  getReferences ( M3GObject3D** references ) const;

		/**
		 * @~English	Returns the current texture blend color for this Texture2D.
		 * @return
		 */
		KDint  getBlendColor ( KDvoid ) const;

		/**
		 * @~English	Returns the current texture blend mode for this Texture2D.
		 * @return
		 */
		KDint  getBlending ( KDvoid ) const;

		/**
		 * @~English	Retrieves the current base level (full size) texture image.
		 * @return
		 */
		M3GImage2D*  getImage ( KDvoid ) const;

		/**
		 * @~English	Returns the current texture image filter.
		 * @return
		 */
		KDint  getImageFilter ( KDvoid ) const;

		/**
		 * @~English	Returns the current texture level filter.
		 * @return
		 */
		KDint  getLevelFilter ( KDvoid ) const;

		/**
		 * @~English	Returns the current texture wrapping mode for the S texture coordinate.
		 * @return
		 */
		KDint	getWrappingS ( KDvoid ) const;

		/**
		 * @~English	Returns the current texture wrapping mode for the T texture coordinate.
		 * @return
		 */
		KDint	getWrappingT ( KDvoid ) const;

		/**
		 * @~English	Sets the texture blend color for this Texture2D.
		 * @param[in]	RGB 
		 */
		KDvoid	setBlendColor ( KDint RGB );

		/**
		 * @~English	Selects the texture blend mode, or blend function, for this Texture2D.
		 * @param[in]	func 
		 */
		KDvoid	setBlending ( KDint func );

		/**
		 * @~English	Selects the filtering mode for this Texture2D.
		 * @param[in]	levelFilter 
		 * @param[in]	imageFilter 
		 */
		KDvoid	setFiltering ( KDint levelFilter, KDint imageFilter );

		/**
		 * @~English	Sets the given Image2D as the texture image of this Texture2D.
		 * @param[in]	image 
		 */
		KDvoid	setImage ( M3GImage2D* image );

		/**
		 * @~English	Sets the wrapping mode for the S and T texture coordinates.
		 * @param[in]	wrapS 
		 * @param[in]	wrapT 
		 */
		KDvoid	setWrapping ( KDint wrapS, KDint wrapT );

	public :

		static  KDvoid  render ( KDvoid );
		virtual KDvoid  render ( M3GRenderState& state ) const; 

	protected :

		KDvoid  init ( M3GImage2D* image );
		KDvoid  _duplicate ( M3GTexture2D* pObj ) const;

	private :

		KDuint				m_uID;
		M3GImage2D*			m_pImage;
		KDint				m_nBlendColor;
		KDint				m_nBlending;
		KDint				m_nLevelFilter;
		KDint				m_nImageFilter;
		KDint				m_nWrappingS;
		KDint				m_nWrappingT;

		friend class M3GLoader;
};


#endif
