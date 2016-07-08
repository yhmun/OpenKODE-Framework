/* --------------------------------------------------------------------------
 *
 *      File            TestTexture2D.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org. All rights reserved.
 *
 *         http://www.cocos2d-x.org      
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

#ifndef __TestTexture2D_h__
#define __TestTexture2D_h__

#include "../TestBasic.h"

class TestTexture2D : public TestBasic
{
	public :

		static  TestTexture2D*	create ( KDvoid );

	protected :		

		virtual KDuint			count  ( KDvoid );
};

class TextureTIFF : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePNG : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TextureJPEG : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TextureWEBP : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TextureMipMap : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*   subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVR : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVR2BPP : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVR4BPP : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRRGBA8888 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRBGRA8888 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRRGBA4444 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRRGBA4444GZ : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*   subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRRGBA4444CCZ : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*   subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRRGBA5551 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRRGB565 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRRGB888 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRA8 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRI8 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRAI88 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVR2BPPv3 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*   subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRII2BPPv3 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*   subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVR4BPPv3 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*   subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRII4BPPv3 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*   subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRRGBA8888v3 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*   subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRBGRA8888v3 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*   subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRRGBA4444v3 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*   subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRRGBA5551v3 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*   subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRRGB565v3 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*   subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRRGB888v3 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*   subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRA8v3 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*   subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRI8v3 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*   subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRAI88v3 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*   subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRBadEncoding : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRMipMap : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRMipMap2 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRNonSquare : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRNPOT4444 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePVRNPOT8888 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TextureAlias : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TexturePixelFormat : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TextureBlend : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TextureAsync : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );

	protected :

		KDvoid		loadImages  ( KDfloat fDelta );

		KDvoid		imageLoaded ( CCObject* pObject );

	private :

		KDint		m_nImageOffset;
};

class TextureGlRepeat : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class TextureGlClamp : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
};

class TextureSizeTest : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TextureCache1 : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class FileUtilsTest : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TextureDrawAtPoint : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
		virtual KDvoid			draw     ( KDvoid );

	private :

		CCTexture2D*			m_pTex1;
		CCTexture2D*			m_pTex2;
};

class TextureDrawInRect : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );
		virtual KDvoid			draw     ( KDvoid );

	private :

		CCTexture2D*			m_pTex1;
		CCTexture2D*			m_pTex2;
};

class TextureMemoryAlloc : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		KDvoid					updateImage ( CCObject* pSender );
		KDvoid					changeBackgroundVisible ( CCObject* pSender );
		
	private :

		CCSprite*				m_pBackground;
};

class TexturePVRv3Premult : public TestTexture2D
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

	private :

		KDvoid					transformSprite ( CCSprite* pSprite );
};

#endif // __TestTexture2D_h__
