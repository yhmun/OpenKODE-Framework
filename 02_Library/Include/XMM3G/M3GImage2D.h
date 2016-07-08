/* --------------------------------------------------------------------------
 *
 *      File            M3GImage2D.h
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

#ifndef __M3GImage2D_h__
#define __M3GImage2D_h__

#include "M3GObject3D.h"

/**
 * @~English	A two-dimensional image that can be used as a texture, backgoround or sprite image.
 */
class M3GImage2D : public M3GObject3D
{
	public :

		/**
		 * @~English	A constructor parameter specifying that this Image2D has an alpha component only.
		 */
		static const KDint	ALPHA           = 96;

		/**
		 * @~English	A constructor parameter specifying that this Image2D has a luminance component only.
		 */
		static const KDint	LUMINANCE       = 97;

		/**
		 * @~English	A constructor parameter specifying that this Image2D has luminance and alpha components.
		 */
		static const KDint	LUMINANCE_ALPHA = 98;

		/**
		 * @~English	A constructor parameter specifying that this Image2D has red, green and blue color components.
		 */
		static const KDint	RGB             = 99;

		/**
		 * @~English	A constructor parameter specifying that this Image2D has red, green, blue and alpha components.
		 */
		static const KDint	RGBA            = 100;

	protected :

		M3GImage2D ( KDvoid );

	public :

		/**
		 * @~English	Constructs an empty, mutable Image2D with the given dimensions.
		 * @param[in]	format
		 * @param[in]	width
		 * @param[in]	height
		 */
		M3GImage2D ( KDint format, KDint width, KDint height );

		/**
		 * @~English	Constructs an immutable Image2D by copying pixels from a byte array.
		 * @param[in]	format
		 * @param[in]	width
		 * @param[in]	height
		 * @param[in]	pixels
		 */
		M3GImage2D ( KDint format, KDint width, KDint height, KDubyte* pixels );

		/**
		 * @~English	Constructs an immutable Image2D by copying palette indices from a byte array,
		 *				and the palette entries from another byte array.
		 * @param[in]	format
		 * @param[in]	width
		 * @param[in]	height
		 * @param[in]	pixels
		 * @param[in]	palette
		 */
		M3GImage2D ( KDint format, KDint width, KDint height, KDubyte* pixels, KDubyte* palette );

		/**
		 * @~English	Destruct this object.
		 */
		virtual ~M3GImage2D ( KDvoid );

	public :

		virtual M3GObject3D*  duplicate ( KDvoid ) const;

		/**
		 * @~English	Gets the internal format of this Image2D.
		 */
		KDint	getFormat ( KDvoid ) const;

		/**
		 * @~English	Gets the height of this Image2D, in pixels.
		 */
		KDint	getHeight ( KDvoid ) const;

		/**
		 * @~English	Gets the width of this Image2D, in pixels.
		 */
		KDint	getWidth ( KDvoid ) const;

		/**
		 * @~English	Queries whether this Image2D is mutable.
		 */
		KDbool	isMutable ( KDvoid ) const;

		/**
		 * @~English	Updates a rectangular area of this Image2D by copying pixels from a byte array.
		 * @param[in]	x
		 * @param[in]	y
		 * @param[in]	width
		 * @param[in]	height
		 * @param[in]	image
		 */
		KDvoid	set ( KDint x, KDint y, KDint width, KDint height, const KDubyte* image );

	public :

		KDubyte*  getPixels ( KDvoid );

		KDint     getOpenGLFormat ( KDvoid );

	protected :

		KDvoid  init ( KDint format, KDint width, KDint height, KDubyte* pixels = KD_NULL, KDubyte* palette = KD_NULL );

		KDvoid  _duplicate ( M3GImage2D* pObj ) const;

	private :

        KDint			m_nFormat;
        KDint			m_nWidth;
        KDint			m_nHeight;
        KDubyte*		m_pImage;       
        KDbool			m_bImmutable;

		friend class M3GLoader;
};

static KDint  m3gGetBpp ( KDint format );

#endif
