/* --------------------------------------------------------------------------
 *
 *      File            main.cpp
 *      Description     M3G Sample - 07_Sprite
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
    M3GCamera*  pCamera = new M3GCamera ( );
    pCamera->lookAt 
    (
    	0, 0, 5,
    	0, 0, 0,
    	0, 1, 0
    );

    pCamera->translate ( 0, 0, 1 );

    M3GBackground*  pBackground = new M3GBackground ( );
    pBackground->setColor ( 0xff1f1f7f );

    M3GImage2D*  pImage = dynamic_cast<M3GImage2D*> ( M3GLoader::load ( "/res/image/mizugi-small.png" ) [ 0 ] );
    KDint  nWidth  = pImage->getWidth  ( );
    KDint  nHeight = pImage->getHeight ( );

    M3GCompositingMode*  pCompositing = new M3GCompositingMode ( );
    pCompositing->setDepthTestEnable ( KD_TRUE );

    M3GAppearance*  pAppearance1 = new M3GAppearance ( );
    pAppearance1->setCompositingMode ( pCompositing );

    M3GSprite3D*  pSprite1 = new M3GSprite3D ( KD_TRUE, pImage, pAppearance1 );
    pSprite1->translate ( 1, 0, 0 );
    pSprite1->setCrop ( 0, 0, -nWidth, nHeight );

    M3GAppearance*  pAppearance2 = new M3GAppearance ( );
    pAppearance2->setCompositingMode ( pCompositing );

    M3GSprite3D*  pSprite2 = new M3GSprite3D ( KD_FALSE, pImage, pAppearance2 );
    pSprite2->translate ( -1, 0, 0 );
    pSprite2->setCrop ( 0, 0, nWidth, nHeight );

    M3GWorld*  pWorld = new M3GWorld ( );
    pWorld->setActiveCamera ( pCamera );
    pWorld->setBackground ( pBackground );
    pWorld->addChild ( pCamera );
    pWorld->addChild ( pSprite1 );
    pWorld->addChild ( pSprite2 );

    l_pWorld = pWorld;

    l_vObjs.push_back ( pCamera );
    l_vObjs.push_back ( pBackground );
    l_vObjs.push_back ( pImage );
    l_vObjs.push_back ( pCompositing );
    l_vObjs.push_back ( pAppearance1 );
    l_vObjs.push_back ( pAppearance2 );
    l_vObjs.push_back ( pSprite1 );
    l_vObjs.push_back ( pSprite2 );
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
    pG3D->setViewport ( 0, 0, width, height );

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
