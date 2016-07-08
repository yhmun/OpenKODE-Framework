/* --------------------------------------------------------------------------
 *
 *      File            tizen_window.cpp
 *      Description     Window functions.
 *
 *      Author          Young-Hwan Mun
 *                      Hyeon-Ho Jeong
 *
 *      Contact         xmsoft77@gmail.com
 *                      skdldnjs@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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

#include <FBase.h>
#include <FApp.h>
#include <FUi.h>
#include <Fsystem.h>
#include <FGraphics.h>
#include <FMedia.h>

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Base::Runtime;
using namespace Tizen::System;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Graphics;
using namespace Tizen::Media;

class XMApplication;
class XMFrame;

static XMApplication*		  l_wnd  = KD_NULL;
static KDmode                 l_mode = KD_NULL;
static EGLNativeWindowType    l_form = 0;
static KDTouch                l_touches [ KD_TOUCH_MAX ];

typedef void ( *EditTextCallback ) ( const char* pText, void* ctx );

class XMForm
    : public Tizen::Ui::Controls::Form
    , public Tizen::Ui::ITouchEventListener
    , public Tizen::Ui::ITextEventListener
{
	public :

    	XMForm ( void )
        : m_pKeypad ( null )
        , m_pEditTextCallback ( null )
        , m_pCtx ( null )
    	{
    		l_form = this;
    	}

		virtual ~XMForm ( void )
		{
			l_form = null;
		}

		virtual result OnInitializing ( void )
		{
		    AddTouchEventListener ( *this );
		    SetMultipointTouchEnabled ( true );

		    return E_SUCCESS;
		}

		virtual result OnTerminating ( void )
		{
		    if ( m_pKeypad )
		    {
		        m_pKeypad->Destroy ( );
		    }

		    return E_SUCCESS;
		}

		virtual void  OnTouchDoublePressed ( const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo )
		{

		}

		virtual void  OnTouchFocusIn ( const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo )
		{

		}

		virtual void  OnTouchFocusOut ( const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo )
		{

		}

		virtual void  OnTouchLongPressed ( const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo )
		{

		}

		virtual void  OnTouchMoved ( const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo )
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

		virtual void  OnTouchPressed ( const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo )
		{
			l_touches [ 0 ].id = (KDvoid *) ( touchInfo.GetPointId( ) + 0xff000000 );
			l_touches [ 0 ].x  = currentPosition.x;
			l_touches [ 0 ].y  = currentPosition.y;

			_xmEventTouchBegan ( l_touches, 1 );
		}

		virtual void  OnTouchReleased ( const Tizen::Ui::Control& source, const Tizen::Graphics::Point& currentPosition, const Tizen::Ui::TouchEventInfo& touchInfo )
		{
			l_touches [ 0 ].id = (KDvoid *) ( touchInfo.GetPointId( ) + 0xff000000 );
			l_touches [ 0 ].x  = currentPosition.x;
			l_touches [ 0 ].y  = currentPosition.y;

			_xmEventTouchEnded ( l_touches, 1 );
		}

		virtual void OnTextValueChanged ( const Tizen::Ui::Control& source )
		{
			/*
		    Tizen::Base::String 	text   = m_pKeypad->GetText ( );
		    Utf8Encoding 			utf8;
		    ByteBuffer* 			buffer = utf8.GetBytesN ( text );
		    const char* 			pText  = "";

		    if ( buffer )
		    {
		        pText = (const char*) buffer->GetPointer ( );
		    }

		    if ( m_pEditTextCallback )
		    {
		        m_pEditTextCallback ( pText, m_pCtx );
		    }
		    else
		    {
		        const char*  pContentText = IMEDispatcher::sharedDispatcher()->getContentText();

		        for ( unsigned int i = strlen ( pContentText ); i > 0; i-- )
		        {
		            //IMEDispatcher::sharedDispatcher ( )->dispatchDeleteBackward ( );
		        }

		        std::string text ( "" );
		        if ( pText != null )
		        {
		            text = pText;
		            if ( text.compare ( "" ) == 0 )
		            {
		                text = "\n";
		            }

		            if ( text.at ( text.length ( ) - 1 ) != '\n' )
		            {
		                text += '\n';
		            }
		        }

		        IMEDispatcher::sharedDispatcher()->dispatchInsertText ( text.c_str ( ), text.length ( ) );
		    }

		    if ( buffer )
		    {
		        delete buffer;
		    }
		    */
		}

		virtual void OnTextValueChangeCanceled ( const Tizen::Ui::Control& source )
		{
			/*
		    if ( m_pEditTextCallback )
		    {
		        m_pEditTextCallback ( "", m_pCtx );
		    }
		    else
		    {
		        const char*  pContentText = IMEDispatcher::sharedDispatcher ( )->getContentText ( );

		        for ( unsigned int i = strlen ( pContentText ); i > 0; i-- )
		        {
		            IMEDispatcher::sharedDispatcher ( )->dispatchDeleteBackward ( );
		        }
		    }
		    */
		}


		void ShowKeypad ( const char* pMessage, Tizen::Ui::Controls::KeypadStyle keypadStyle, Tizen::Ui::Controls::KeypadInputModeCategory keypadCategory, bool bSingleLineEnabled, bool bTextPrediction, int nMaxLength, EditTextCallback pfEditTextCallback, void* pCtx )
		{
			/*
			m_pEditTextCallback = pfEditTextCallback;
			m_pCtx = pCtx;

			if (__pKeypad)
			{
				m_pKeypad->RemoveTextEventListener ( *this );
				m_pKeypad->Destroy ( );
				m_pKeypad = null;
			}

			if ( nMaxLength > 100 )
			{
				nMaxLength = 100;
			}
			else if ( nMaxLength == -1 )
			{
				nMaxLength = 100;
			}

			m_pKeypad = new Keypad ( );
			m_pKeypad->Construct ( keypadStyle, keypadCategory, nMaxLength );
			m_pKeypad->AddTextEventListener ( *this );

			m_pKeypad->SetTextPredictionEnabled ( bTextPrediction );
			m_pKeypad->SetSingleLineEnabled ( bSingleLineEnabled );
			m_pKeypad->SetText ( Tizen::Base::String ( pMessage ) );
			m_pKeypad->SetShowState ( true );
			m_pKeypad->Show ( );
			*/
		}

		void ShowKeypad ( )
		{
			/*
		   ShowKeypad
		   (
				IMEDispatcher::sharedDispatcher ( )->getContentText ( ),
				KEYPAD_STYLE_NORMAL,
				KEYPAD_MODE_ALPHA,
				true,
				true,
				100,
				null,
				null
			);
		   */
		}

		void CloseKeypad ( void )
		{
		    m_pKeypad->SetShowState ( false );
		    Invalidate ( true );
		}

		Tizen::Ui::Controls::Keypad*		m_pKeypad;
		EditTextCallback 					m_pEditTextCallback;
		void* 								m_pCtx;
};

class XMApplication : public Tizen::App::Application, public Tizen::Base::Runtime::ITimerEventListener, public Tizen::Ui::IKeyEventListener
{
	public :

		static Tizen::App::Application* 	CreateInstance ( void )
		{
			return new (std::nothrow) XMApplication ( );
		}

	public :

		XMApplication ( void )
		: m_pFrame ( null ), m_pForm ( null ), m_pTimer ( null )
		{
			l_wnd = this;
		}

		virtual ~XMApplication ( void )
		{
			l_wnd = KD_NULL;

			if ( m_pTimer )
			{
				delete m_pTimer;
				m_pTimer = null;
			}
		}

	public :

		// Called when the UiApp is initializing.
		virtual bool OnAppInitializing ( Tizen::App::AppRegistry& appRegistry )
		{
			result				 r = E_SUCCESS;
			IAppFrame*   pAppFrame = null;
			Frame*          pFrame = null;

		    m_pFrame = new (std::nothrow) Frame ( );
		    TryReturn ( m_pFrame != null, E_FAILURE, "[XMApp] Generating a frame failed." );

		    r = m_pFrame->Construct ( );
		    TryReturn ( !IsFailed ( r ), E_FAILURE, "[XMApp] pAppFrame->Construct() failed." );

		    this->AddFrame ( *m_pFrame );

		    m_pForm = new (std::nothrow) XMForm;
		    TryCatch ( m_pForm != null, , "[XMApp] Allocation of OspForm failed." );

		    r = m_pForm->Construct ( FORM_STYLE_NORMAL );
		    TryCatch ( !IsFailed ( r ), , "[XMApp] m_pForm->Construct(FORM_STYLE_NORMAL) failed." );

		    pAppFrame = GetAppFrame ( );
		    pFrame = pAppFrame->GetFrame ( );
		    r = pFrame->AddControl ( *m_pForm );
		    TryCatch ( !IsFailed ( r ), , "[XMApp] GetAppFrame()->GetFrame()->AddControl(*m_pForm) failed." );

			r = pAppFrame->AddKeyEventListener ( *this );
			TryCatch ( !IsFailed ( r ), , "[XMApp] GetAppFrame()->AddKeyEventListener(*this) failed." );

			pFrame->SetOrientation ( l_mode & XM_SYS_LANDSCAPE ? ORIENTATION_LANDSCAPE : ORIENTATION_PORTRAIT );

		    _xmEventCreate ( (XMWindow*) pFrame );
			_xmEventResize ( pFrame->GetWidth( ), pFrame->GetHeight( ) );

			m_pTimer = new Timer ( );
			m_pTimer->Construct ( *this );

		    return true;

		CATCH :

			if ( m_pForm )
			{
				delete m_pForm;
				m_pForm = null;
			}

		    return false;
		}

		// Called when the UiApp initializing is finished.
		virtual bool OnAppInitialized ( void )
		{
			return true;
		}

		// Called when the UiApp is requested to terminate.
		virtual bool OnAppTerminating ( Tizen::App::AppRegistry& appRegistry, bool forcedTermination = false )
		{
			_xmEventDestroy ( );

			return true;
		}

		// Called when the UiApp's frame moves to the top of the screen.
		virtual void OnForeground ( void )
		{
			if ( m_pTimer )
			{
				m_pTimer->Start ( g_interval == 0 ? 1 : g_interval );
			}

			_xmEventFocus ( 1 );
		}

		// Called when this UiApp's frame is moved from top of the screen to the background.
		virtual void OnBackground ( void )
		{
			if ( m_pTimer )
			{
				m_pTimer->Cancel ( );
			}

			_xmEventFocus ( 0 );
		}

		// Called when the system memory is not sufficient to run the UiApp any further.
		virtual void OnLowMemory ( void )
		{
			_xmEventMemoryWarning ( );
		}

		// Called when the battery level changes.
		virtual void OnBatteryLevelChanged ( Tizen::System::BatteryLevel batteryLevel )
		{

		}

		// Called when the screen turns on.
		virtual void OnScreenOn ( void )
		{
			_xmEventVisible ( 1 );
		}

		// Called when the screen turns off.
		virtual void OnScreenOff ( void )
		{
			_xmEventVisible ( 0 );
		}

		virtual void OnTimerExpired ( Tizen::Base::Runtime::Timer& timer )
		{
			static KDuint old_msec = 0;
				   KDuint new_msec = 0;

			if ( !m_pTimer )
			{
				return;
			}
			m_pTimer->Start ( g_interval == 0 ? 1 : g_interval );

			new_msec = KD_GET_UST2MSEC;
			if ( g_interval == 0 || ( new_msec - old_msec ) > ( g_interval - 5 ) || new_msec < old_msec )
			{
				old_msec = new_msec;

				_xmEventRedraw ( );
				_xmEventUpdate ( );
			}
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

	private :

	    Tizen::Ui::Controls::Frame* 		m_pFrame;
	    Tizen::Ui::Controls::Form* 			m_pForm;
	    Tizen::Base::Runtime::Timer*        m_pTimer;
};

// xmPlatformGetKeyIndex : Get XMKode key code from platform key code
KDint xmPlatformGetKeyCode ( KDint win_code )
{
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
	l_mode = mode;

	KDint     	nArgc = xmGetArgc ( );
	KDchar**  	pArgv = xmGetArgv ( );

	result    	tResult;
	KDint     	i;
	ArrayList	aArgs ( SingleObjectDeleter );
	aArgs.Construct ( );

	AppLogTag ( "XMKdoe","Application started." );

	for ( i = 0; i < nArgc; i++ )
	{
		aArgs.Add ( new ( std::nothrow ) String ( pArgv [ i ] ) );
	}

	tResult = Tizen::App::UiApp::Execute ( XMApplication::CreateInstance, &aArgs );

	if ( IsFailed ( tResult ) )
	{
	  kdLogMessagefKHR ( "Application execution failed-[%s].", GetErrorMessage( tResult ) );
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
			xmSoundCompleted ( this );
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

KDint xmPlatformSoundSetVolume ( XMSound* sound, KDfloat volume )
{
	return -1;
}

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
	if ( l_wnd )
	{
		l_wnd->Terminate ( );
	}
}
