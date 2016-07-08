/* --------------------------------------------------------------------------
 *
 *      File            TestChipmunk2.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (C) 2010-2012 XMSoft.
 *      Copyright (C) 2010-2013 cocos2d-x.org. All rights reserved.
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
 
#include "Precompiled.h"
#include "TestChipmunk2.h"

extern chipmunkDemo LogoSmash;
extern chipmunkDemo Simple;
extern chipmunkDemo PyramidStack;
extern chipmunkDemo Plink;
extern chipmunkDemo Tumble;
extern chipmunkDemo PyramidTopple;
extern chipmunkDemo Bounce;
extern chipmunkDemo Planet;
extern chipmunkDemo Springies;
extern chipmunkDemo Pump;
extern chipmunkDemo TheoJansen;
extern chipmunkDemo MagnetsElectric;
extern chipmunkDemo UnsafeOps;
extern chipmunkDemo Query;
extern chipmunkDemo OneWay;
extern chipmunkDemo Player;
extern chipmunkDemo Sensors;
extern chipmunkDemo Joints;

static chipmunkDemo*	l_pDemos [ ] =
{
	&LogoSmash			,
	&Simple				,
	&PyramidStack		,
	&Plink				,
	&Tumble				,
	&PyramidTopple		,
	&Bounce				,
	&Planet				,
	&Springies			,
	&Pump				,
	&TheoJansen			,
	&MagnetsElectric	,
	&UnsafeOps			,
	&Query				,
	&OneWay				,
	&Player				,
	&Sensors			,
	&Joints				,
};

static drawSpaceOptions	l_tOptions = 
{
	0,
	0,
	1,
	4.0f,
	0.0f,
	1.5f,
};

cpVect		arrowDirection;
KDchar		messageString[1024] = "";

KDuint TestChipmunk2::count ( KDvoid )
{
	return sizeof ( l_pDemos ) / sizeof ( l_pDemos [ 0 ] );
}

const KDchar* TestChipmunk2::subtitle ( KDvoid )
{
	return l_pDemos [ s_nSubTestIndex ]->name;
}

KDvoid TestChipmunk2::onEnter ( KDvoid )
{
	TestBasic::onEnter ( );

	cpInitChipmunk ( );	
		
	m_pCurDemo = l_pDemos [ s_nSubTestIndex ];	
	m_pSpace   = m_pCurDemo->initFunc ( );	

	this->schedule ( schedule_selector ( TestChipmunk2::step ), 0.01f ); 
}

KDvoid TestChipmunk2::onExit ( KDvoid )
{
	cpSpaceFree ( m_pSpace );

	TestBasic::onExit ( );
}

KDvoid TestChipmunk2::draw ( KDvoid )
{
	TestBasic::draw ( );

	const CCSize&  s = this->getContentSize ( );

	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position );

	kmGLPushMatrix ( );

	kmGLTranslatef ( s.cx / 2 , s.cy / 2, 0 );
	kmGLScalef ( 0.6f, 0.6f, 1.0f );
	kmGLScalef ( s.cx / 640.f, s.cy / 480.f, 1.0f );
		
	drawSpace ( m_pSpace, m_pCurDemo->drawOptions ? m_pCurDemo->drawOptions : &l_tOptions );	

	kmGLPopMatrix  ( );
}

KDvoid TestChipmunk2::step ( KDfloat fDelta )
{
	m_pCurDemo->updateFunc ( 1 );
}
