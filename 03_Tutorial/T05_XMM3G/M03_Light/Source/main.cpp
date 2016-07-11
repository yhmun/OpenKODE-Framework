/* --------------------------------------------------------------------------
 *
 *      File            main.cpp
 *      Description     M3G Sample - 03_Light
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
#include "data.h"

XM_APP_MAIN_BEGIN
	XM_APP_WND_REALIZE ( XM_SYS_LANDSCAPE | XM_SYS_QUIT )
XM_APP_MAIN_END

static std::vector<M3GObject3D*>        l_vObjs;
static M3GWorld*                        l_pWorld;
static M3GLight*                        l_pLight;

static KDvoid xmEventCreate ( KDvoid )
{
    M3GAnimationController*  pController = new M3GAnimationController ( );

    M3GKeyframeSequence*  pKeyframeSequence = new M3GKeyframeSequence ( 3, 3, M3GKeyframeSequence::LINEAR );
    
    KDfloat  fLightColor [3][3] = 
    {
    	{ 1.0f, 1.0f, 1.0f },
    	{ 0.5f, 0.5f, 0.5f },
    	{ 1.0f, 1.0f, 1.0f }
    };

    pKeyframeSequence->setKeyframe   ( 0,   0, fLightColor [ 0 ] );
    pKeyframeSequence->setKeyframe   ( 1, 200, fLightColor [ 1 ] );
    pKeyframeSequence->setKeyframe   ( 2, 400, fLightColor [ 2 ] );
    pKeyframeSequence->setRepeatMode ( M3GKeyframeSequence::LOOP );
    pKeyframeSequence->setValidRange ( 0, 2 );
    pKeyframeSequence->setDuration   ( 400 );

    M3GAnimationTrack*  pAnimationTrack = new M3GAnimationTrack ( pKeyframeSequence, M3GAnimationTrack::COLOR );
    pAnimationTrack->setController ( pController );

    M3GVertexArray*  pPositions = new M3GVertexArray ( 840, 3, 2 );
    M3GVertexArray*  pNormals   = new M3GVertexArray ( 840, 3, 1 );
    M3GVertexArray*  pTexcoords = new M3GVertexArray ( 840, 2, 4 );

    pPositions->set ( 0, 840, g_Positions );
    pNormals  ->set ( 0, 840, g_Normals );
    pTexcoords->set ( 0, 840, g_Texcoords );

    KDfloat  fScale = 1;
    KDfloat  fBias [ 3 ] = { 0, 0, 0 };
    M3GVertexBuffer*  pVertices = new M3GVertexBuffer ( );

    pVertices->setPositions ( pPositions, fScale, fBias );
    pVertices->setNormals   ( pNormals );
    pVertices->setTexCoords ( 0, pTexcoords, fScale, fBias );
  
    KDint  nStrips  [ ] = { 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42 };
    M3GTriangleStripArray*  pTraiangles = new M3GTriangleStripArray ( 0, 20, nStrips );

    M3GMaterial*  pMaterial = new M3GMaterial ( );
    pMaterial->setColor ( M3GMaterial::DIFFUSE, 0xffffffff );

    M3GImage2D*    pImage   = dynamic_cast<M3GImage2D*> ( M3GLoader::load ( "/res/image/kingyo.png" ) [ 0 ] );
    M3GTexture2D*  pTexture = new M3GTexture2D ( pImage );

    M3GAppearance*  pAppearance = new M3GAppearance ( );
    pAppearance->setMaterial ( pMaterial );
    pAppearance->setTexture ( 0, pTexture );

    M3GMesh*  pMesh = new M3GMesh ( pVertices, pTraiangles, pAppearance );

    M3GCamera*  pCamera = new M3GCamera ( );
    pCamera->lookAt 
    (
    	0,  20, 20,
    	0,  0,  0,
    	0,  1,  0
    );

    M3GLight*  pLight = new M3GLight ( );
    pLight->setColor     ( 0x00ffffff );   
    pLight->setIntensity ( 2 );            
    pLight->setMode      ( M3GLight::OMNI );
    pLight->translate    ( 0, 2, 0 );
    pLight->postRotate   ( -90, 1,0,0 );
    pLight->setSpotAngle ( 60 );
    pLight->addAnimationTrack ( pAnimationTrack );

    M3GWorld*  pWorld = new M3GWorld ( );
    pWorld->addChild ( pCamera );
    pWorld->setActiveCamera ( pCamera );
    pWorld->addChild ( pLight );
    pWorld->addChild ( pMesh );

    l_pWorld = pWorld;
    l_pLight = pLight;

    l_vObjs.push_back ( pController );
    l_vObjs.push_back ( pKeyframeSequence );
    l_vObjs.push_back ( pAnimationTrack );
    l_vObjs.push_back ( pPositions );
    l_vObjs.push_back ( pNormals );
    l_vObjs.push_back ( pTexcoords );
    l_vObjs.push_back ( pVertices );
    l_vObjs.push_back ( pTraiangles );
    l_vObjs.push_back ( pMaterial );
    l_vObjs.push_back ( pTexture );
    l_vObjs.push_back ( pImage );
    l_vObjs.push_back ( pAppearance );
    l_vObjs.push_back ( pMesh );
    l_vObjs.push_back ( pCamera );
    l_vObjs.push_back ( pLight );
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
    l_pWorld->animate ( msec );
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
    switch ( l_pLight->getMode ( ) )
    {
    	case M3GLight::DIRECTIONAL :

    	    l_pLight->setMode ( M3GLight::OMNI );
    	    kdLogMessagefKHR ( "Switch to OMNI" );

    	    break;

    	case M3GLight::OMNI :

    	    l_pLight->setMode ( M3GLight::SPOT );
    	    kdLogMessagefKHR ( "Switch to SPOT" );

    	    break;

    	case M3GLight::SPOT :

    	    l_pLight->setMode ( M3GLight::DIRECTIONAL );
    	    kdLogMessagefKHR ( "Switch to DIRECTIONAL" );

    }
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