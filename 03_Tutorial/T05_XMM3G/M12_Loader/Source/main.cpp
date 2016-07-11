/* --------------------------------------------------------------------------
 *
 *      File            main.cpp
 *      Description     M3G Sample - 12_Loader
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
static KDfloat                          l_fFovy;
static KDfloat                          l_fNear;
static KDfloat                          l_fFar;

static KDvoid xmEventCreate ( KDvoid )
{
    l_vObjs = M3GLoader::load 
    (
//		"/res/m3g/simple.m3g" 
//		"/res/m3g/test.m3g"
//		"/res/m3g/kitune-miko.m3g"
		"/res/m3g/abc09.m3g"			
    );

    for ( KDuint  i = 0; i < l_vObjs.size ( ); i++ ) 
    {
        l_pWorld = dynamic_cast<M3GWorld*> ( l_vObjs [ i ] );
        if ( l_pWorld ) 
    	{
            break;
        }
    }

    KDfloat     fParams [ 4 ];
    M3GCamera*  pCamera = l_pWorld->getActiveCamera ( );
    pCamera->getProjection ( fParams );
    l_fFovy = fParams [ 0 ];
    l_fNear = fParams [ 2 ];
    l_fFar  = fParams [ 3 ];

    M3GBackground*  pBackground = l_pWorld->getBackground ( );

    if ( pBackground == KD_NULL ) 
    {
        pBackground = new M3GBackground ( );
        l_pWorld->setBackground ( pBackground );
    }

    pBackground->setColor ( 0xff3f7fff );
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
    l_pWorld->animate ( msec );
}

static KDvoid xmEventResize ( KDint width, KDint height )
{
    M3GGraphics3D*  pG3D = M3GGraphics3D::getInstance ( );
    pG3D->setViewport ( 0, 0, width, height );

    M3GCamera*  pCamera = l_pWorld->getActiveCamera ( );
    pCamera->setPerspective ( 45, width / (KDfloat) height, 0.1f, 1000 );
}

static KDvoid xmEventTouchBegan ( KDTouch* touches, KDuint count )
{
/*
    M3GGraphics3D*  pG3D = M3GGraphics3D::getInstance ( );
    KDfloat  fWidth  = (KDfloat) pG3D->getViewportWidth  ( );
    KDfloat  fHeight = (KDfloat) pG3D->getViewportHeight ( );
    M3GCamera*  pCamera = l_pWorld->getActiveCamera ( );

    M3GRayIntersection  tRay;
    KDbool  bHit = l_pWorld->pick ( -1, touches [ 0 ].x / fWidth, touches [ 0 ].y / fHeight, pCamera, &tRay );
	
    kdLogMessagefKHR ( "%s", bHit ? "Hit!" : "Miss." );
*/   
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
