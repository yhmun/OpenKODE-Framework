/* --------------------------------------------------------------------------
 *
 *      File            main.cpp
 *      Description     M3G Sample - 09_MorphingMesh
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
    KDfloat  fValues [4][2] = 
    {
    	{ 0, 0 },
    	{ 1, 0 },
    	{ 0, 1 },
    	{ 0, 0 }
    };

    M3GKeyframeSequence*  pKeyframeSequence = new M3GKeyframeSequence ( 4, 2, M3GKeyframeSequence::LINEAR );
    pKeyframeSequence->setKeyframe   ( 0,    0, fValues [ 0 ] );
    pKeyframeSequence->setKeyframe   ( 1,  400, fValues [ 1 ] );
    pKeyframeSequence->setKeyframe   ( 2,  800, fValues [ 2 ] );
    pKeyframeSequence->setKeyframe   ( 3, 1200, fValues [ 3 ] );
    pKeyframeSequence->setRepeatMode ( M3GKeyframeSequence::LOOP );
    pKeyframeSequence->setValidRange ( 0, 3 );
    pKeyframeSequence->setDuration   ( 1600 );

    M3GAnimationController*  pController = new M3GAnimationController ( );
    M3GAnimationTrack*  pAnimationTrack = new M3GAnimationTrack ( pKeyframeSequence, M3GAnimationTrack::MORPH_WEIGHTS );
    pAnimationTrack->setController ( pController );

    KDfloat  fScale    = 1;
    KDfloat  fBias [3] = { 0, 0, 0 };
    M3GVertexArray*  pBasePositions = new M3GVertexArray ( 4, 3, 4 );
    KDfloat  fBasePositionsValues [12] = { 1,-1,0, 1,1,0, -1,-1,0, -1,1,0 };
    pBasePositions->set ( 0, 4, fBasePositionsValues );

    M3GVertexArray*  pTargetPositions1 = new M3GVertexArray ( 4, 3, 4 );
    KDfloat  fTargetPositionsValues1 [12] = { 1,-1,0, 0,0,0, -1,-1,0, 0,0,0 };
    pTargetPositions1->set ( 0, 4, fTargetPositionsValues1 );

    M3GVertexArray*  pTargetPositions2 = new M3GVertexArray ( 4, 3, 4 );
    KDfloat  fTargetPositionsValues2 [12] = { 2,-1,0, 2,-0.5,0, -2,-1,0, -2,-0.5,0 };
    pTargetPositions2->set ( 0, 4, fTargetPositionsValues2 );

    M3GVertexBuffer*  pBaseVertices = new M3GVertexBuffer ( );
    pBaseVertices->setPositions ( pBasePositions, fScale, fBias );

    M3GVertexBuffer*  pTargetVertices [ ] = { 0, 0 };

    pTargetVertices [0] = new M3GVertexBuffer ( );
    pTargetVertices [0]->setPositions ( pTargetPositions1, fScale, fBias );

    pTargetVertices [1] = new M3GVertexBuffer ( );
    pTargetVertices [1]->setPositions ( pTargetPositions2, fScale, fBias );
  
    KDint  nStrips  [ ] = { 4 };
    M3GTriangleStripArray*  pTriangles = new M3GTriangleStripArray ( 0, 1, nStrips );

    M3GAppearance*  pAppearance = new M3GAppearance ( );

    M3GMorphingMesh*  pMesh = new M3GMorphingMesh ( pBaseVertices, 2, pTargetVertices, pTriangles, pAppearance );

    KDfloat  fWeights [2] = { 0, 0 };
    pMesh->setWeights ( 2, fWeights );
    pMesh->addAnimationTrack ( pAnimationTrack );

    M3GCamera*  pCamera = new M3GCamera ( );
    pCamera->translate ( 0, 0, 10 );

    M3GWorld*  pWorld = new M3GWorld ( );
    pWorld->setActiveCamera ( pCamera );
    pWorld->addChild ( pCamera );
    pWorld->addChild ( pMesh );

    l_pWorld = pWorld;

    l_vObjs.push_back ( pKeyframeSequence );
    l_vObjs.push_back ( pController );
    l_vObjs.push_back ( pAnimationTrack );
    l_vObjs.push_back ( pBasePositions );
    l_vObjs.push_back ( pTargetPositions1 );
    l_vObjs.push_back ( pTargetPositions2 );
    l_vObjs.push_back ( pBaseVertices );
    l_vObjs.push_back ( pTargetVertices [ 0 ] );
    l_vObjs.push_back ( pTargetVertices [ 1 ] );
    l_vObjs.push_back ( pTriangles );
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
