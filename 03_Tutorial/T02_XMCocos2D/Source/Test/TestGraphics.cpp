/* --------------------------------------------------------------------------
 *
 *      File            TestGraphics.cpp
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#include "Precompiled.h"
#include "TestGraphics.h"

KDfloat TestGraphics::s_fLon = -38.0f;
KDfloat TestGraphics::s_fLat = -210.0f;

KDbool TestGraphics::init ( KDvoid )
{	
	if ( !TestBasic::init ( ) )
	{
		return KD_FALSE;
	}

#ifndef USE_OPEN_GL

	const CCRect& tViewRect = CCEGLView::sharedOpenGLView ( )->getViewPortRect ( );

	XMGMatrix4F  tMatrix;
	tMatrix.Perspective ( 45.0f, tViewRect.size.cx / tViewRect.size.cy, 0.1f, 100.0f );

	m_pView = new XMGView ( );	
	m_pView->SetProject ( tMatrix );
	m_pView->SetViewport ( XMGRectF ( tViewRect.origin.x, tViewRect.origin.y, 0, tViewRect.size.cx, tViewRect.size.cy ) );

	m_pTexture = new XMGTexture ( "/res/xm_supports/earth.jpg" );
	m_pSphere  = new XMGSphere ( );

	GLfloat  fRadius = 1.0f;
	m_pSphere->SetVertexArray ( &fRadius );	
	m_pSphere->SetPosition ( XMGVector3F ( 0.0f, 0.0f, -4.5f ) );
	m_pSphere->SetColor ( XMGColorF ( 0.4f, 0.8f, 0.0f, 1.0f ) );
	m_pSphere->SetTexture ( m_pTexture );
	
	switch ( s_nSubTestIndex )
	{
		case 0 : m_pSphere->SetDispMode ( XMG_DISP_TEXTURE );	break;
		case 1 : m_pSphere->SetDispMode ( XMG_DISP_POINT );		break;
		case 2 : m_pSphere->SetDispMode ( XMG_DISP_LINE );		break;
		case 3 : m_pSphere->SetDispMode ( XMG_DISP_FILL );		break;
	} 

	CCDirector::sharedDirector ( )->setGLDefaultValues ( );

	this->schedule ( schedule_selector ( TestGraphics::onRotate ), 0.02f ); 
	this->onRotate ( 0 );

#endif

	return KD_TRUE;
}

KDvoid TestGraphics::onExit ( KDvoid )
{
#ifndef USE_OPEN_GL

	CC_SAFE_DELETE ( m_pView );
	CC_SAFE_DELETE ( m_pTexture );
	CC_SAFE_DELETE ( m_pSphere );

#endif

	TestBasic::onExit ( );
}

KDvoid TestGraphics::draw ( KDvoid )
{
#ifndef USE_OPEN_GL

	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_None );

	m_pView  ->Update ( );
	m_pSphere->Render ( );

	CCDirector::sharedDirector ( )->setGLDefaultValues ( );

#endif
}

KDuint TestGraphics::count ( KDvoid )
{
	return 4;
}

const KDchar* TestGraphics::subtitle ( KDvoid )
{
	switch ( s_nSubTestIndex )
	{
		case 0  : return "DRAWING TEXTURE";
		case 1  : return "DRAWING POINTS";
		case 2  : return "DRAWING LINES";
		case 3  : return "DRAWING FILL";
		default : return "";
	}
}

KDvoid TestGraphics::onRotate ( KDfloat dt )
{
#ifndef USE_OPEN_GL
	XMGMatrix4F	 tMatrix;
	
	tMatrix.Rotate ( -s_fLat, 1.0f, 0.0f, 0.0f );	
	tMatrix.Rotate (  s_fLon, 0.0f, 1.0f, 0.0f );	

	m_pSphere->SetMatrix ( tMatrix );

	s_fLat += 1.2f;
	s_fLon += 0.8f;
#endif
}