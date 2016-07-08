/* --------------------------------------------------------------------------
 *
 *      File            CCTexture2DMutable.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Lam Hoang Pham
 *      Improved by     Manuel Martinez-Almeida
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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

#ifndef __CCTexture2DMutable_h__
#define __CCTexture2DMutable_h__

class CCTexture2DMutable : public CCTexture2D
{
	public :

		static CCTexture2DMutable*	createWithImage ( CCImage* pImage );

		static CCTexture2DMutable*	createWithString ( const KDchar* szText, const KDchar* szFontPath, KDfloat fFontSize );

	public :		

		virtual KDbool				initWithData ( const KDvoid* pData, CCTexture2DPixelFormat ePixelFormat, const CCSize& tContentSize, KDuint uMipmaps = 1 );

		virtual ccColor4B			pixelAt ( const CCPoint& tPos );

		/**
		 *	@param pt is a point to get a pixel (0,0) is top-left to (width,height) bottom-right
		 *	@param c is a ccColor4B which is a colour.
		 *	@returns if a pixel was set
		 *	Remember to call apply to actually update the texture canvas.
		 */
		virtual KDbool				setPixelAt (const CCPoint& tPos, const ccColor4B& tColor );

		/**
		 *	Fill with specified colour
		 */
		virtual KDvoid				fill ( const ccColor4B& tColor );

		/**
		 *	@param textureToCopy is the texture image to copy over
		 *	@param offset also offset's the texture image
		 */
		virtual CCTexture2D*		copyMutable ( KDbool bIsMutable );

		virtual CCTexture2DMutable*	copy ( KDvoid );

		virtual KDvoid				copy ( CCTexture2DMutable* pTextureToCopy, const CCPoint& tOffset );

		/**
		 *	apply actually updates the texture with any new data we added.
		 */
		virtual KDvoid				apply ( KDvoid );

		virtual KDvoid*				getTexData ( KDvoid );

		virtual KDvoid				setTexData ( KDvoid* pTexData );

	protected :

		KDvoid*						m_pData;
		KDuint						m_uBytesPerPixel;
		KDbool						m_bDirty;
};

#endif	// __CCTexture2DMutable_h__
