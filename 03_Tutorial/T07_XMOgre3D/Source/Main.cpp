/* ------------------------------------------------------------------------------------
 *
 *      File            Main.cpp
 *      Description     This source file is part of OGRE
 *                      (Object-oriented Graphics Rendering Engine)
 *      Author          Y.H Mun
 *
 * ------------------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 *
 *      Contact Email: xmsoft77@gmail.com
 *
 * ------------------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2012 Torus Knot Software Ltd.
 *
 *      For the latest info, see http://www.ogre3d.org/
 *
 * ------------------------------------------------------------------------------------
 *
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ------------------------------------------------------------------------------------ */

#if defined ( Linux )
#include "kode.h"
#include "platform.h"
#endif

#include "Precompiled.h"
#include "platform.h"
#include "SampleBrowser.h"

XM_APP_MAIN_BEGIN
	XM_APP_WND_REALIZE ( XM_SYS_QUIT | XM_SYS_LANDSCAPE )
XM_APP_MAIN_END

static SampleBrowser*    l_pSample = KD_NULL;
     
KD_API KDvoid KD_APIENTRY xmEventProc ( const KDEvent* event )
{
	switch ( event->type )
	{
		case KD_EVENT_NATIVE :  
			{
//				#if !defined ( SHP ) && defined ( _WIN32 ) 
//				KDEventNativeWin32*  proc = (KDEventNativeWin32*) event->data.native.p;
//				#endif
			}
			break;
		
		case KD_EVENT_CREATE :
			{
				// event->data.size.width;
				// event->data.size.height;

                l_pSample = new SampleBrowser ( );
                l_pSample->initApp ( );
			}
			break;

		case KD_EVENT_DESTROY :      
			{
                l_pSample->closeApp ( );
			}
			break;

		case KD_EVENT_RESIZE :       

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
                l_pSample->render ( );
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
                
                l_pSample->touchPressed ( event->data.touch );
			}
			break;

		case KD_EVENT_TOUCH_MOVED :            
			{
                l_pSample->touchMoved ( event->data.touch );
			}
			break;

		case KD_EVENT_TOUCH_ENDED :            
			{
                l_pSample->touchReleased ( event->data.touch );
			}
			break;

		case KD_EVENT_TOUCH_CANCELLED :          
			{
                l_pSample->touchReleased ( event->data.touch );
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
