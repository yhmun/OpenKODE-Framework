/* --------------------------------------------------------------------------
 *
 *      File            bada_window.c
 *      Description     Window functions.
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
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

#include "../kd_library.h"

KDvoid xmPlatformEventAttach ( KDenum mode )
{

}

KDvoid xmPlatformEventDetach ( KDenum mode )
{

}

KDint xmPlatformSoundOpen ( XMSound* sound )
{
	/*
	SoundPlayer*  player = new SoundPlayer ( sound->fpath );
	if ( !player )
	{
		return 0;
	}

	if ( !player->m_player )
	{
		delete player;
		return -1;
	}

	sound->device = (KDvoid*) player;
	*/
	return 0;
}

KDint xmPlatformSoundClose ( XMSound* sound )
{
	/*
	SoundPlayer*  player = (SoundPlayer*) sound->device;

	delete player;
	sound->device = 0;
	*/
	return 0;
}

KDint xmPlatformSoundPlay ( XMSound* sound )
{
	/*
	SoundPlayer*  player = (SoundPlayer*) sound->device;
	result        r = E_SUCCESS;

	player->m_playing = KD_TRUE;
	r = player->m_player->Play ( );

	return IsFailed ( r ) ? -1 : 0;
	*/
	return 0;
}

KDint xmPlatformSoundStop ( XMSound* sound )
{
	/*
	SoundPlayer*  player = (SoundPlayer*) sound->device;
	result        r = E_SUCCESS;

	if ( player->m_playing == KD_FALSE )
	{
		return 0;
	}

	r = player->m_player->Stop ( );

	return IsFailed ( r ) ? -1 : 0;
	*/
	return 0;
}

KDint xmPlatformSoundPause ( XMSound* sound )
{
	/*
	SoundPlayer*  player = (SoundPlayer*) sound->device;
	result        r = E_SUCCESS;

	r = player->m_player->Pause ( );

	return IsFailed ( r ) ? -1 : 0;
	*/
	return 0;
}

KDint xmPlatformSoundResume ( XMSound* sound )
{
	/*
	SoundPlayer*  player = (SoundPlayer*) sound->device;
	result        r = E_SUCCESS;

	r = player->m_player->Play ( );

	return IsFailed ( r ) ? -1 : 0;
	*/
	return 0;
}


//KDint   xmPlatformSoundSetVolume ( XMSound* sound, KDfloat volume );
//KDfloat xmPlatformSoundGetVolume ( XMSound* sound );

KDint xmPlatformSoundSetVolume ( XMSound* sound, KDfloat percent )
{
	return -1;
}

KDfloat xmPlatformSoundGetVolume ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundGetDuration ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundSetPosition ( XMSound* sound, KDint percent )
{
	return -1;
}

KDint xmPlatformSoundGetPosition ( XMSound* sound )
{
	return -1;
}

/*
class  XMApplication;

static XMApplication*         l_app  = 0;
static KDmode                 l_mode = 0;
static EGLNativeWindowType    l_form = 0;
static KDTouch                l_touches [ KD_TOUCH_MAX ];

class XMForm : public Form, public ITouchEventListener
{
public :
	
	XMForm ( void )
	{

	}

	~XMForm ( void )
	{

	}

	virtual result OnDraw ( void )
	{
		return E_SUCCESS;
	}

	virtual result OnInitializing ( void )
	{
		Touch     touch;

		AddTouchEventListener ( *this );
		touch.SetMultipointEnabled ( *this, true );

		if ( l_mode & XM_SYS_LANDSCAPE )
		{
			SetOrientation ( ORIENTATION_LANDSCAPE );
		}
		else
		{
			SetOrientation ( ORIENTATION_PORTRAIT );
		}

		return E_SUCCESS;
	}

	virtual result OnTerminating ( void )
	{
		return E_SUCCESS;
	}

	virtual void OnTouchDoublePressed ( const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo )
	{

	}

	virtual void OnTouchFocusIn ( const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo )
	{

	}

	virtual void OnTouchFocusOut ( const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo )
	{

	}

	virtual void OnTouchLongPressed ( const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo )
	{

	}

	virtual void OnTouchMoved ( const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo )
	{
		Touch      touch;
		TouchInfo* info;
		IList*     list;
		KDint      i;
		KDint      cnt;

		list = touch.GetTouchInfoListN ( source );
		if ( list )
		{
			cnt = list->GetCount ( );
			for ( i = 0; i < cnt; i++ )
			{
				info = (TouchInfo*) list->GetAt ( i );

				l_touches [ i ].id = (KDvoid *) ( info->id + 0xff000000 );
				l_touches [ i ].x  = info->position.x;
				l_touches [ i ].y  = info->position.y;
			}

			if ( cnt )
			{
				_xmEventTouchMoved ( l_touches, cnt );
			}

			list->RemoveAll ( false );
			delete list;
		}
	}

	virtual void OnTouchPressed ( const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo )
	{
		l_touches [ 0 ].id = (KDvoid *) ( touchInfo.GetPointId( ) + 0xff000000 );
		l_touches [ 0 ].x  = currentPosition.x;
		l_touches [ 0 ].y  = currentPosition.y;

		_xmEventTouchBegan ( l_touches, 1 );
	}

	virtual void OnTouchReleased ( const Control &source, const Point &currentPosition, const TouchEventInfo &touchInfo )
	{
		l_touches [ 0 ].id = (KDvoid *) ( touchInfo.GetPointId( ) + 0xff000000 );
		l_touches [ 0 ].x  = currentPosition.x;
		l_touches [ 0 ].y  = currentPosition.y;

		_xmEventTouchEnded ( l_touches, 1 );
	}
};

class XMApplication : public Application, public IScreenEventListener, public IKeyEventListener, public ITimerEventListener
{
public :

	XMApplication ( void ) : m_timer ( 0 ), m_form ( 0 )
	{

	}

	~XMApplication ( void )
	{
		if ( m_timer )
		{
			delete m_timer;
			m_timer = 0;
		}
	}

	bool OnAppInitializing ( AppRegistry& appRegistry )
	{
		result       r;
		IAppFrame*   appframe = GetAppFrame ( );
		Frame*       frame = appframe->GetFrame ( );

		m_form = new XMForm ( );
		if ( !m_form )
		{
			kdLogMessage ( "Create form failed." );
			return false;
		}

		m_form->Construct ( FORM_STYLE_NORMAL );
		r = frame->AddControl ( *m_form );
		if ( IsFailed ( r ) )
		{
			kdLogMessage ( "Add control failed. ( form )" );
			return false;
		}

		l_form = (EGLNativeWindowType) m_form;

		r = appframe->AddKeyEventListener ( *this );
		if ( IsFailed ( r ) )
		{
			return false;
		}

		_xmEventCreate ( (XMWindow*) appframe );

		if ( l_mode & XM_SYS_LANDSCAPE )
		{
			_xmEventResize ( frame->GetHeight( ), frame->GetWidth( ) );
		}
		else
		{
			_xmEventResize ( frame->GetWidth( ), frame->GetHeight( ) );
		}

		m_timer = new Timer ( );
		m_timer->Construct ( *this );

		return true;
	}

	bool OnAppTerminating ( AppRegistry& appRegistry, bool forcedTermination = false )
	{
		if ( l_app )
		{
			l_app = KD_NULL;

			if ( m_timer )
			{
				delete m_timer;
				m_timer = 0;
			}

			_xmEventDestroy ( );

			for ( KDint i = 0; i < 10; i++ )
			{
				kdLogMessage ( "" );
			}
		}

		return true;
	}

	void OnForeground ( void )
	{
		if ( m_timer )
		{
			m_timer->Start ( g_interval == 0 ? 1 : g_interval );
		}

		_xmEventFocus ( 1 );
	}

	void OnBackground ( void )
	{
		if ( m_timer )
		{
			m_timer->Cancel ( );
		}

		_xmEventFocus ( 0 );
	}

	void OnLowMemory ( void )
	{

	}

	void OnBatteryLevelChanged ( BatteryLevel batteryLevel )
	{

	}

	void OnScreenOn ( void )
	{
		_xmEventVisible ( 1 );
	}

	void OnScreenOff ( void )
	{
		_xmEventVisible ( 0 );
	}

	void OnKeyPressed ( const Control& source, KeyCode keyCode )
	{
		_xmEventKeyPressed ( keyCode );
	}

	void OnKeyReleased ( const Control& source, KeyCode keyCode )
	{
		_xmEventKeyReleased ( keyCode );
	}

	void OnKeyLongPressed ( const Control& source, KeyCode keyCode )
	{

	}

	void OnTimerExpired ( Timer& timer )
	{
		static KDuint old_msec = 0;
		       KDuint new_msec = 0;

		if ( !m_timer )
		{
			return;
		}
		m_timer->Start ( g_interval == 0 ? 1 : g_interval );

		new_msec = KD_GET_UST2MSEC;
		if ( g_interval == 0 || ( new_msec - old_msec ) > ( g_interval - 5 ) || new_msec < old_msec )
		{
			old_msec = new_msec;

			_xmEventRedraw ( );
			_xmEventUpdate ( );
		}
	}

	static Application*	CreateInstance ( void )
	{
		return l_app = new XMApplication ( );
	}

	Timer*    m_timer;
	XMForm*   m_form;
};

// xmPlatformGetKeyIndex : Get XMKode key code from platform key code
KDint xmPlatformGetKeyCode ( KDint win_code )
{
	static struct XMKeyCode  codes[ ] =
	{
		{ KD_INPUT_XMSOFT_ESCAPE, KEY_INVALID },
		{ 0                     , 0           },
	};

	KDint      idx;

	for ( idx = 0; codes[ idx ].kd_code != 0; idx++ )
	{
		if( codes[ idx ].win_code == win_code )
		{
			return codes[ idx ].kd_code;
		}
	}

	return 0;
}

// xmPlatformGetEGLNative : Get EGL native window and display type.
KDvoid xmPlatformGetEGLNative ( EGLNativeWindowType* native_window, EGLNativeDisplayType* native_display )
{
	*native_window  = (EGLNativeWindowType) l_form;
	*native_display = 0;
}

// xmPlatformRealizeWindow : Realize window and ready event loop in platform specific.
KDvoid xmPlatformRealizeWindow ( KDmode mode )
{
	ArrayList*  pArgs = new ArrayList ( );

	result    r;
	KDint     i;

	l_mode = mode;

	pArgs->Construct ( );
	for ( i = 0; i < g_argc; i++ )
	{
		pArgs->Add ( *( new String( g_argv[ i ] ) ) );
	}

	r = Application::Execute ( XMApplication::CreateInstance, pArgs );

	pArgs->RemoveAll ( true );
	delete pArgs;

	if ( IsFailed( r ) )
	{
		kdLogMessagefKHR ( "Application execution failed-[%s].", GetErrorMessage( r ) );
		kdExit ( -3 );
	}
}

// xmPlatformFreeWindow : Release window resources in platform specific.
KDvoid xmPlatformFreeWindow ( KDvoid )
{

}

KDvoid xmPlatformEventAttach ( KDenum mode )
{
  
}

KDvoid xmPlatformEventDetach ( KDenum mode )
{
    
}

class SoundPlayer : public IPlayerEventListener
{
	public :

		Player*     m_player;
		KDbool      m_playing;

		SoundPlayer ( const KDchar* path )
		{
			result r = E_SUCCESS;

			m_player = new Player ( );
			r = m_player->Construct ( *this, 0 );
			if ( IsFailed ( r ) )
			{
				delete m_player;
				m_player = KD_NULL;
			}

			r = m_player->OpenFile ( path );
			if ( IsFailed ( r ) )
			{
				delete m_player;
				m_player = KD_NULL;
			}
		}

		~SoundPlayer ( void )
		{
			if ( m_player )
			{
				m_player->Close ( );
				delete m_player;
				m_player = KD_NULL;
			}
		}

		void OnPlayerOpened ( result r )
		{

		}

		void OnPlayerEndOfClip ( void )
		{
			m_playing = KD_FALSE;
			xmSoundDone ( this );
		}

		void OnPlayerBuffering ( int percent )
		{

		}

		void OnPlayerErrorOccurred ( PlayerErrorReason r )
		{

		}

		void OnPlayerInterrupted ( void )
		{

		}

		void OnPlayerReleased ( void )
		{

		}

		void OnPlayerSeekCompleted ( result r )
		{

		}
};

KDint xmPlatformSoundOpen ( XMSound* sound )
{
	SoundPlayer*  player = new SoundPlayer ( sound->fpath );
	if ( !player )
	{
		return 0;
	}

	if ( !player->m_player )
	{
		delete player;
		return -1;
	}

	sound->device = (KDvoid*) player;
	return 0;
}

KDint xmPlatformSoundClose ( XMSound* sound )
{
	SoundPlayer*  player = (SoundPlayer*) sound->device;

	delete player;
	sound->device = 0;

	return 0;
}

KDint xmPlatformSoundPlay ( XMSound* sound )
{
	SoundPlayer*  player = (SoundPlayer*) sound->device;
	result        r = E_SUCCESS;

	player->m_playing = KD_TRUE;
	r = player->m_player->Play ( );

	return IsFailed ( r ) ? -1 : 0;
}

KDint xmPlatformSoundStop ( XMSound* sound )
{
	SoundPlayer*  player = (SoundPlayer*) sound->device;
	result        r = E_SUCCESS;

	if ( player->m_playing == KD_FALSE )
	{
		return 0;
	}

	r = player->m_player->Stop ( );

	return IsFailed ( r ) ? -1 : 0;
}

KDint xmPlatformSoundPause ( XMSound* sound )
{
	SoundPlayer*  player = (SoundPlayer*) sound->device;
	result        r = E_SUCCESS;

	r = player->m_player->Pause ( );

	return IsFailed ( r ) ? -1 : 0;
}

KDint xmPlatformSoundResume ( XMSound* sound )
{
	SoundPlayer*  player = (SoundPlayer*) sound->device;
	result        r = E_SUCCESS;

	r = player->m_player->Play ( );

	return IsFailed ( r ) ? -1 : 0;
}


KDint xmPlatformSoundSetVolume ( XMSound* sound, KDint percent )
{
	return -1;
}

KDint xmPlatformSoundGetVolume ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundGetDuration ( XMSound* sound )
{
	return -1;
}

KDint xmPlatformSoundSetPosition ( XMSound* sound, KDint percent )
{
	return -1;
}

KDint xmPlatformSoundGetPosition ( XMSound* sound )
{
	return -1;
}

KDvoid xmPlatformExit ( KDint status )
{
	if ( l_app )
	{
		l_app->Terminate ( );
	}
}

*/
