/* --------------------------------------------------------------------------
 *
 *      File            main.cpp
 *      Description     M3G Sample - 11_Align
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010 UEDA.Takashi
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

#include <XMM3G/m3g.h>
#include "platform.h"

XM_APP_MAIN_BEGIN
	XM_APP_WND_REALIZE ( XM_SYS_LANDSCAPE | XM_SYS_QUIT )
XM_APP_MAIN_END

static std::vector<M3GObject3D*>        l_vObjs;
static M3GWorld*                        l_pWorld;
static KDuint                           l_uIndex = 0;

static KDvoid xmEventCreate ( KDvoid )
{
    KDfloat  fPositions [ ] =
    {
    	 1, -1,  0,
    	 1,	 1,  0,
    	-1, -1,  0,
    	-1,  1,  0 
    };
    M3GVertexArray*  pPositions = new M3GVertexArray ( 4, 3, 4 );
    pPositions->set ( 0, 4, fPositions );

    KDubyte  nColors [ ] =
    {
    	 255, 255, 255, 255,
    	 255,   0,   0, 255, 
    	 255, 255, 255, 255,
    	   0,   0, 255, 255
    };
    M3GVertexArray*  pColors = new M3GVertexArray ( 4, 4, 1 );
    pColors->set ( 0, 4, nColors );

    KDfloat  fScale    = 1;
    KDfloat  fBias [3] = { 0, 0, 0 };
    M3GVertexBuffer*  pVertices = new M3GVertexBuffer ( );
    pVertices->setPositions ( pPositions, fScale, fBias );
    pVertices->setColors    ( pColors );

    KDint  nIndices [ ] = { 0, 1, 2, 3 };
    KDint  nStrips  [ ] = { 4 };
    M3GTriangleStripArray*  pTraiangles = new M3GTriangleStripArray ( 4, nIndices, 1, nStrips );

    M3GPolygonMode*  pPolygon = new M3GPolygonMode ( );
    pPolygon->setCulling ( M3GPolygonMode::CULL_NONE );

    M3GMaterial*  pMaterial = new M3GMaterial ( );
    pMaterial->setVertexColorTrackingEnable ( KD_TRUE );

    M3GAppearance*  pAppearance = new M3GAppearance ( );
    pAppearance->setMaterial ( pMaterial );
    pAppearance->setPolygonMode ( pPolygon );

    M3GMesh*  pMesh1 = new M3GMesh ( pVertices, pTraiangles, pAppearance );

    M3GMesh*  pMesh2 = new M3GMesh ( pVertices, pTraiangles, pAppearance );
    pMesh2->translate ( 3, 0, 0 );

    M3GMesh*  pMesh3 = new M3GMesh ( pVertices, pTraiangles, pAppearance );
    pMesh3->translate ( -3, 0, 0);

    M3GBackground*  pBackground = new M3GBackground ( );
    pBackground->setColor ( 0xff3f3fff );

    M3GCamera*  pCamera = new M3GCamera ( );
    pCamera->lookAt 
    (
    	0, 0, 15,
    	0, 0,  0,
    	0, 1,  0
    );

    M3GWorld*  pWorld = new M3GWorld ( );
    pWorld->setBackground ( pBackground );
    pWorld->setActiveCamera ( pCamera );
    pWorld->addChild ( pCamera );
    pWorld->addChild ( pMesh1 );
    pWorld->addChild ( pMesh2 );
    pWorld->addChild ( pMesh3 );

    pMesh1->setAlignment ( pCamera, M3GNode::NONE  , pWorld , M3GNode::NONE   );
    pMesh2->setAlignment ( pCamera, M3GNode::ORIGIN, pWorld , M3GNode::ORIGIN );
    pMesh3->setAlignment ( pCamera, M3GNode::ORIGIN, pCamera, M3GNode::ORIGIN );

    l_pWorld = pWorld;

    l_vObjs.push_back ( pPositions );
    l_vObjs.push_back ( pColors );
    l_vObjs.push_back ( pVertices );
    l_vObjs.push_back ( pTraiangles );
    l_vObjs.push_back ( pMaterial );
    l_vObjs.push_back ( pPolygon );
    l_vObjs.push_back ( pAppearance );
    l_vObjs.push_back ( pMesh1 );
    l_vObjs.push_back ( pMesh2 );
    l_vObjs.push_back ( pMesh3 );
    l_vObjs.push_back ( pBackground );
    l_vObjs.push_back ( pCamera );
    l_vObjs.push_back ( pWorld );
}

static KDvoid xmEventDestroy ( KDvoid )
{	
    M3GGraphics3D*  pG3D = M3GGraphics3D::getInstance ( );

    while ( !l_vObjs.empty ( ) )
    {
    	M3GObject3D*  pObj = l_vObjs.back ( );

    	delete pObj;

    	l_vObjs.pop_back ( );
    }

    delete pG3D;
}

static KDvoid xmEventRedraw ( KDvoid )
{
    M3GGraphics3D*  pG3D = M3GGraphics3D::getInstance ( );
    pG3D->render ( l_pWorld );
}

static KDvoid xmEventUpdate ( KDuint msec )
{
    l_pWorld->animate ( msec / 10 );
}

static KDvoid xmEventResize ( KDint width, KDint height )
{
    M3GGraphics3D*  pG3D = M3GGraphics3D::getInstance ( );
    pG3D->setViewport ( 0, 0, width, height );

    M3GCamera*  pCamera = l_pWorld->getActiveCamera ( );
    pCamera->setPerspective ( 45, width / (KDfloat) height, 0.1f, 100 );
}

static KDvoid xmEventTouchBegan ( KDTouch* touches, KDuint count )
{
    l_uIndex = l_uIndex + 1 == 4 ? 0 : l_uIndex + 1;

    M3GCamera*  pCamera = l_pWorld->getActiveCamera ( );

    switch ( l_uIndex )
    {
    	case 0 : pCamera->postRotate (  50, 0, 1, 0 ); break;
    	case 1 : pCamera->postRotate ( -50, 0, 1, 0 ); break;
    	case 2 : pCamera->postRotate (  50, 1, 0, 0 ); break;
    	case 3 : pCamera->postRotate ( -50, 1, 0, 0 ); break;
    }
    
    l_pWorld->align ( KD_NULL );
}

KD_API KDvoid KD_APIENTRY xmEventProc ( const KDEvent* event )
{
	switch ( event->type )
	{
		case KD_EVENT_NATIVE :  
			{
//				#if !defined ( SHP ) && defined ( _WIN32 ) 
//				KDEventNativeWin32*  proc = (KDEventNativeWin32*) event->data.native.p;
//				KDEventNativeLinux*  proc = (KDEventNativeLinux*) event->data.native.p;
//				#endif
			}
			break;
		
		case KD_EVENT_CREATE :
			{
				// event->data.size.width;
				// event->data.size.height;

				xmEventCreate ( );
				xmEventResize ( event->data.size.width, event->data.size.height );
			}
			break;

		case KD_EVENT_DESTROY :  
			{
				xmEventDestroy ( );
			}
			break;

		case KD_EVENT_RESIZE :       
			{
				// event->data.size.width;
				// event->data.size.height;	
			}
			break;

		case KD_EVENT_FOCUS :        
			{
				// event->data.value.i;
				// 1 : focus
			}
			break;

		case KD_EVENT_VISIBLE :    
			{
				// event->data.value.i;
				// 1 : visible
			}
			break;

		case KD_EVENT_REDRAW :    
			{
				xmEventRedraw ( );
			}
			break;

		case KD_EVENT_UPDATE :      
			{
				// event->data.update.msec;

				xmEventUpdate ( event->data.update.msec );
			}
			break;

		case KD_EVENT_TOUCH_BEGAN :     
			{
				// event->data.touch.touches;
				// event->data.touch.count;

				xmEventTouchBegan ( event->data.touch.touches, event->data.touch.count );
			}
			break;

		case KD_EVENT_TOUCH_MOVED :            
			{

			}
			break;

		case KD_EVENT_TOUCH_ENDED :            
			{

			}
			break;

		case KD_EVENT_TOUCH_CANCELLED :          
			{

			}
			break;

		case KD_EVENT_KEY_RELEASED :    
			{
				// event->data.keypad.code;		
			}
			break;

		case KD_EVENT_KEY_PRESSED :    
			{

			}
			break;

		case KD_EVENT_ACCELEROMETER :   
			{
				// event->data.accelerometer.x;
				// event->data.accelerometer.y;
				// event->data.accelerometer.z;
			}
			break;

		case KD_EVENT_LOCATION :                 
			{
				// event->data.value.i;
				// KD_NMEA_UPDATED_GPS, KD_NMEA_UPDATED_USER
			}
			break;

		case KD_EVENT_INSERT_TEXT :               
			{
				// event->data.insert.utf8;
			}
			break;

		case KD_EVENT_SERIALIZE :                
			{
				// event->data.serialize.type;
				// event->data.serialize.data;
				// event->data.serialize.size;
			}
			break;
	}
}
