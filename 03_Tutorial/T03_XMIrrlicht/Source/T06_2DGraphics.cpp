/* --------------------------------------------------------------------------
 *
 *      File            T06_2DGraphics.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
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

#include "Precompiled.h"
#include "T06_2DGraphics.h"

CT06_2DGraphics::CT06_2DGraphics ( KDvoid )
{
	/*
		All 2d graphics in this example are put together into one texture,
		2ddemo.png. Because we want to draw colorkey based sprites, we need to
		load this texture and tell the engine, which part of it should be
		transparent based on a colorkey.

		In this example, we don't tell it the color directly, we just say "Hey
		Irrlicht Engine, you'll find the color I want at position (0,0) on the
		texture.". Instead, it would be also possible to call
		driver->makeColorKeyTexture(images, video::SColor(0,0,0,0)), to make
		e.g. all black pixels transparent. Please note that
		makeColorKeyTexture just creates an alpha channel based on the color.
	*/

	m_pImage = s_pDriver->getTexture ( "/res/media/2ddemo.png" );
	s_pDriver->makeColorKeyTexture ( m_pImage, core::position2d<s32> ( 0, 0 ) );

	/*
		To be able to draw some text with two different fonts, we first load
		them. Ok, we load just one. As the first font we just use the default
		font which is built into the engine. Also, we define two rectangles
		which specify the position of the images of the red imps (little flying
		creatures) in the texture.
	*/
	m_pFont  = m_pGuiEnv->getBuiltInFont ( );
	m_pFont2 = m_pGuiEnv->getFont ( "/res/media/fonthaettenschweiler.bmp" );

	/*
		Prepare a nicely filtering 2d render mode for special cases.
	*/
	s_pDriver->getMaterial2D ( ).TextureLayer[0].BilinearFilter = true;
	s_pDriver->getMaterial2D ( ).AntiAliasing = video::EAAM_FULL_BASIC;
}

CT06_2DGraphics::~CT06_2DGraphics ( KDvoid )
{
	
}

KDvoid CT06_2DGraphics::Draw ( KDvoid )
{
	/*
		Everything is prepared, now we can draw everything in the draw loop,
		between the begin scene and end scene calls. In this example, we are
		just doing 2d graphics, but it would be no problem to mix them with 3d
		graphics. Just try it out, and draw some 3d vertices or set up a scene
		with the scene manager and draw it.
	*/

	u32  uTime = s_pDevice->getTimer ( )->getTime ( );

	core::rect<s32>  tRect1 ( 349, 15, 385, 78 );
	core::rect<s32>  tRect2 ( 387, 15, 423, 78 );

	/*
		First, we draw 3 sprites, using the alpha channel we
		created with makeColorKeyTexture. The last parameter
		specifies that the drawing method should use this alpha
		channel. The last-but-one parameter specifies a
		color, with which the sprite should be colored.
		(255,255,255,255) is full white, so the sprite will
		look like the original. The third sprite is drawn
		with the red channel modulated based on the time.
	*/

	// draw fire & dragons background world
	s_pDriver->draw2DImage 
	(
		m_pImage,
		core::position2d<s32> ( 50, 50 ),
		core::rect<s32> ( 0, 0, 342, 224 ),
		0,
		video::SColor ( 255, 255, 255, 255 ),
		true
	);

	// draw flying imp
	s_pDriver->draw2DImage 
	(
		m_pImage, 
		core::position2d<s32> ( 164, 125 ),
		( uTime / 500 % 2 ) ? tRect1 : tRect2,
		0,
		video::SColor ( 255, 255, 255, 255 ),
		true
	);

	// draw second flying imp with colorcylce
	s_pDriver->draw2DImage 
	(
		m_pImage, 
		core::position2d<s32> ( 270, 105 ),
		( uTime / 500 % 2 ) ? tRect1 : tRect2,
		0,
		video::SColor ( 255, ( uTime ) % 255, 255, 255 ),
		true
	);

	/*
		Drawing text is really simple. The code should be self
		explanatory.
	*/

	// draw some text
	m_pFont->draw
	(
		L"This demo shows that Irrlicht is also capable of drawing 2D graphics.",
		core::rect<s32> ( 130, 10, 300, 50 ),
		video::SColor ( 255, 255, 255, 255 )
	);

	// draw some other text
	m_pFont2->draw
	(
		L"Also mixing with 3d graphics is possible.",
		core::rect<s32> ( 130, 20, 300, 60 ),
		video::SColor ( 255, uTime % 255, uTime % 255, 255 )
	);

	/*
		Next, we draw the Irrlicht Engine logo (without
		using a color or an alpha channel). Since we slightly scale
		the image we use the prepared filter mode.
	*/
	s_pDriver->enableMaterial2D ( );
	s_pDriver->draw2DImage 
	(
		m_pImage,
		core::rect<s32> (  10, 10, 108,  48 ),
		core::rect<s32> ( 354, 87, 442, 118 ) 
	);
	s_pDriver->enableMaterial2D ( false );

	/*
		Finally draw a half-transparent rect under the mouse cursor.
	*/
	core::position2d<s32>  tPos = s_pDevice->getCursorControl ( )->getPosition ( );
	s_pDriver->draw2DRectangle 
	(
		video::SColor ( 100, 255, 255, 255 ),
		core::rect<s32> ( tPos.X - 20, tPos.Y - 20, tPos.X + 20, tPos.Y + 20 )
	);

	CTutorial::Draw ( );
}

video::SColor CT06_2DGraphics::getClear ( KDvoid )
{
	return video::SColor ( 255, 120, 102, 136 );
}

const wchar_t* CT06_2DGraphics::getTitle ( KDvoid )
{
	return L"06. 2DGraphics";
}