/* --------------------------------------------------------------------------
 *
 *      File            main.cpp
 *      Description     M3G Sample - 02_Texture
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

static KDvoid xmEventCreate ( KDvoid )
{
    KDint16  nPositions [ ] =
    {
    	 1, -1,  0,
    	 1,  1,  0, 
    	-1, -1,  0,
    	-1,  1,  0
    };
    M3GVertexArray*  pPositions = new M3GVertexArray ( 4, 3, 2 );
    pPositions->set ( 0, 4, nPositions );

    KDint16  nTexCoords [ ] =
    {
    	 1,  0,  
    	 1,  1, 
    	 0,  0, 
    	 0,  1, 
    };
    M3GVertexArray*  pTexCoords = new M3GVertexArray ( 4, 2, 2 );
    pTexCoords->set ( 0, 4, nTexCoords );

    KDfloat  fScale = 1;
    KDfloat  fBias [ 3 ] = { 0, 0, 0 };
    M3GVertexBuffer*  pVertices = new M3GVertexBuffer ( );
    pVertices->setPositions ( pPositions, fScale, fBias );
    pVertices->setTexCoords ( 0, pTexCoords, fScale, fBias );
    pVertices->setTexCoords ( 1, pTexCoords, fScale, fBias );
  
    KDint  nIndices [ ] = { 0, 1, 2, 2, 1, 3 };
    KDint  nStrips  [ ] = { 3, 3 };
    M3GTriangleStripArray*  pTraiangles = new M3GTriangleStripArray ( 6, nIndices, 2, nStrips );

    M3GImage2D*    pImage0   = dynamic_cast<M3GImage2D*> ( M3GLoader::load ( "/res/image/yumemi.png" ) [ 0 ] );
    M3GTexture2D*  pTexture0 = new M3GTexture2D ( pImage0 );

    M3GImage2D*    pImage1   = dynamic_cast<M3GImage2D*> ( M3GLoader::load ( "/res/image/onnanoko.png" ) [ 0 ] );
    M3GTexture2D*  pTexture1 = new M3GTexture2D ( pImage1 );
    pTexture1->setBlending ( M3GTexture2D::FUNC_DECAL );  

    M3GAppearance*  pAppearance = new M3GAppearance ( );
    pAppearance->setTexture ( 0, pTexture0 );
    pAppearance->setTexture ( 1, pTexture1 );

    M3GMesh*  pMesh = new M3GMesh ( pVertices, pTraiangles, pAppearance );
    pMesh->translate ( 0, 0, 2 );

    M3GCamera*  pCamera = new M3GCamera ( );
    pCamera->translate ( 0, 0, 5 );
	
    M3GWorld*  pWorld = new M3GWorld ( );
    pWorld->addChild ( pCamera );
    pWorld->setActiveCamera ( pCamera );
    pWorld->addChild ( pMesh );

    l_pWorld = pWorld;
    l_vObjs.push_back ( pPositions );
    l_vObjs.push_back ( pTexCoords );
    l_vObjs.push_back ( pVertices );
    l_vObjs.push_back ( pTraiangles );
    l_vObjs.push_back ( pImage0 );
    l_vObjs.push_back ( pImage1 );
    l_vObjs.push_back ( pTexture0 );
    l_vObjs.push_back ( pTexture1 );
    l_vObjs.push_back ( pAppearance );
    l_vObjs.push_back ( pMesh );
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

static KDvoid xmEventResize ( KDint width, KDint height )
{
    M3GGraphics3D*  pG3D = M3GGraphics3D::getInstance ( );
    pG3D->setViewport ( 0, 0, width,height );

    M3GCamera*  pCamera = l_pWorld->getActiveCamera ( );
    pCamera->setPerspective ( 45, width / (KDfloat) height, 0.1f, 100 );
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
			}
			break;

		case KD_EVENT_TOUCH_BEGAN :     
			{
				// event->data.touch.touches;
				// event->data.touch.count;
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