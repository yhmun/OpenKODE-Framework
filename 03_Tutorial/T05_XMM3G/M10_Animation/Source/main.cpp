/* --------------------------------------------------------------------------
 *
 *      File            main.cpp
 *      Description     M3G Sample - 10_Animation
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
    M3GAnimationController*  pControllerTranslate   = new M3GAnimationController ( );
    M3GAnimationController*  pControllerOrientation = new M3GAnimationController ( );

    pControllerTranslate  ->setSpeed ( 1, 0 );
    pControllerOrientation->setSpeed ( 2, 0 );

    KDfloat  fFrameMeshTranslate [3][3] = 
    {
    	{  0, 0, 0 },
    	{  4, 0, 0 },
    	{ -4, 0, 0 }
    };

    KDfloat  fFrameMeshOrientation [3][4] = 
    {
    	{ 0, 0, 0,  1 },
    	{ 0, 1, 0,  0 },
    	{ 0, 0, 0, -1 },
    };

    M3GKeyframeSequence*  pKeyframeMeshTranslate   = new M3GKeyframeSequence ( 4, 3, M3GKeyframeSequence::LINEAR );
    M3GKeyframeSequence*  pKeyframeMeshOrientation = new M3GKeyframeSequence ( 3, 4, M3GKeyframeSequence::SLERP );

    pKeyframeMeshTranslate->setKeyframe     ( 0,   0, fFrameMeshTranslate [0] );
    pKeyframeMeshTranslate->setKeyframe     ( 1, 200, fFrameMeshTranslate [1] );
    pKeyframeMeshTranslate->setKeyframe     ( 2, 600, fFrameMeshTranslate [2] );
    pKeyframeMeshTranslate->setKeyframe     ( 3, 800, fFrameMeshTranslate [0] );
    pKeyframeMeshTranslate->setRepeatMode   ( M3GKeyframeSequence::LOOP );
    pKeyframeMeshTranslate->setValidRange   ( 0, 3 );
    pKeyframeMeshTranslate->setDuration     ( 800 );

    pKeyframeMeshOrientation->setKeyframe   ( 0,    0, fFrameMeshOrientation [0] );
    pKeyframeMeshOrientation->setKeyframe   ( 1,  500, fFrameMeshOrientation [1] );
    pKeyframeMeshOrientation->setKeyframe   ( 2, 1000, fFrameMeshOrientation [2] );
    pKeyframeMeshOrientation->setRepeatMode ( M3GKeyframeSequence::LOOP );
    pKeyframeMeshOrientation->setValidRange ( 0, 2 );
    pKeyframeMeshOrientation->setDuration   ( 1000 );
	
    M3GAnimationTrack*  pAnimationMeshTranslate   = new M3GAnimationTrack ( pKeyframeMeshTranslate  , M3GAnimationTrack::TRANSLATION );
    M3GAnimationTrack*  pAnimationMeshOrientation = new M3GAnimationTrack ( pKeyframeMeshOrientation, M3GAnimationTrack::ORIENTATION );

    pAnimationMeshTranslate  ->setController ( pControllerTranslate );
    pAnimationMeshOrientation->setController ( pControllerOrientation );

    KDint16  nPositionValues [ ] =
    {
    	 1, -1,  0,
    	 1,  1,  0, 
    	-1, -1,  0, 
    	-1,  1,  0
    };
    M3GVertexArray*  pPositions = new M3GVertexArray ( 4, 3, 2 );
    pPositions->set ( 0, 4, nPositionValues );

    KDfloat  fScale    = 1;
    KDfloat  fBias [3] = { 0, 0, 0 };
    M3GVertexBuffer*  pVertices = new M3GVertexBuffer ( );
    pVertices->setPositions ( pPositions, fScale, fBias );
  
    KDint  nIndices [ ] = { 0, 1, 2, 3 };
    KDint  nStrips  [ ] = { 4 };
    M3GTriangleStripArray*  pTriangles = new M3GTriangleStripArray ( 4, nIndices, 1, nStrips );

    M3GMaterial*  pMaterial = new M3GMaterial ();
    pMaterial->setColor ( M3GMaterial::DIFFUSE, 0xff0000ff );

    M3GPolygonMode*  pPolygonMode = new M3GPolygonMode ( );
    pPolygonMode->setCulling ( M3GPolygonMode::CULL_NONE );

    M3GAppearance*  pAppearance = new M3GAppearance ( );
    pAppearance->setMaterial ( pMaterial );
    pAppearance->setPolygonMode ( pPolygonMode );

    M3GMesh*  pMesh = new M3GMesh ( pVertices, pTriangles, pAppearance );
    pMesh->addAnimationTrack ( pAnimationMeshTranslate );
    pMesh->addAnimationTrack ( pAnimationMeshOrientation );

    M3GBackground*  pBackground = new M3GBackground ( );
    pBackground->setColor ( 0xff7f7f7f );

    M3GCamera*  pCamera = new M3GCamera ( );
    pCamera->lookAt
    (
    	0, 0, 10,
    	0, 0,  0,
    	0, 1,  0 
    );

    M3GWorld*  pWorld = new M3GWorld ( );
    pWorld->setBackground ( pBackground );
    pWorld->setActiveCamera ( pCamera );
    pWorld->addChild ( pCamera );
    pWorld->addChild ( pMesh );

    l_pWorld = pWorld;

    l_vObjs.push_back ( pControllerTranslate );
    l_vObjs.push_back ( pControllerOrientation );
    l_vObjs.push_back ( pKeyframeMeshTranslate );
    l_vObjs.push_back ( pKeyframeMeshOrientation );
    l_vObjs.push_back ( pAnimationMeshTranslate );
    l_vObjs.push_back ( pAnimationMeshOrientation );
    l_vObjs.push_back ( pPositions );
    l_vObjs.push_back ( pVertices );
    l_vObjs.push_back ( pTriangles );
    l_vObjs.push_back ( pMaterial );
    l_vObjs.push_back ( pPolygonMode );
    l_vObjs.push_back ( pAppearance );
    l_vObjs.push_back ( pMesh );
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
    l_pWorld->animate ( msec );
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

				xmEventUpdate ( event->data.update.msec );
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