/* -----------------------------------------------------------------------------------
 *
 *      File            CCScriptSupport.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCScriptSupport_h__
#define __CCScriptSupport_h__

#include "../event_dispatcher/CCTouch.h"
#include "../event_dispatcher/CCEventTouch.h"
#include "../event_dispatcher/CCEventKeyboard.h"
#include "../../base/CCSet.h"
#include <map>
#include <string>
#include <list>

typedef struct lua_State	lua_State;

NS_CC_BEGIN

class Timer;
class Layer;
class MenuItem;
class NotificationCenter;
class CallFunc;
class Acceleration;

enum ccScriptType 
{
    kScriptTypeNone = 0,
    kScriptTypeLua,
    kScriptTypeJavascript
};

class ScriptHandlerEntry : public Object
{
	public :

		static ScriptHandlerEntry*		create ( KDint nHandler );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~ScriptHandlerEntry ( KDvoid );
    
		KDint				getHandler ( KDvoid )
		{
			return m_nHandler;
		}
    
		KDint				getEntryId ( KDvoid )
		{
			return m_nEntryId;
		}
    
	protected :

		ScriptHandlerEntry ( KDint nHandler )
		: m_nHandler ( nHandler )
		{
			static KDint	nNewEntryId = 0;
			nNewEntryId++;
			m_nEntryId = nNewEntryId;
		}
    
		KDint				m_nHandler;
		KDint				m_nEntryId;
};

/**
 *	@addtogroup script_support
 *	@{
 */

class SchedulerScriptHandlerEntry : public ScriptHandlerEntry
{
	public :

		// nHandler return by tolua_ref_function(), called from LuaCocos2d.cpp
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		static SchedulerScriptHandlerEntry*		create ( KDint nHandler, KDfloat fInterval, KDbool bPaused );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		~SchedulerScriptHandlerEntry ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		Timer*		getTimer ( KDvoid )
		{
			return m_pTimer;
		}
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDbool		isPaused ( KDvoid )
		{
			return m_bPaused;
		}
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid		markedForDeletion ( KDvoid )
		{
			m_bMarkedForDeletion = true;
		}
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDbool		isMarkedForDeletion ( KDvoid )
		{
			return m_bMarkedForDeletion;
		}
    
	private :

		SchedulerScriptHandlerEntry ( KDint nHandler )
		: ScriptHandlerEntry ( nHandler )
		, m_pTimer ( KD_NULL )
		, m_bPaused ( false )
		, m_bMarkedForDeletion ( false )
		{

		}

		KDbool		init ( KDfloat fInterval, KDbool bPaused );
    
		Timer*		m_pTimer;
		KDbool		m_bPaused;
		KDbool		m_bMarkedForDeletion;
};

class TouchScriptHandlerEntry : public ScriptHandlerEntry
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		static TouchScriptHandlerEntry*		create ( KDint nHandler, KDbool bIsMultiTouches, KDint nPriority, KDbool bSwallowsTouches );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~TouchScriptHandlerEntry ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDbool		isMultiTouches ( KDvoid )
		{
			return m_bIsMultiTouches;
		}

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDint		getPriority ( KDvoid )
		{
			return m_nPriority;
		}

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDbool		getSwallowsTouches ( KDvoid )
		{
			return m_bSwallowsTouches;
		}
    
	private :

		TouchScriptHandlerEntry ( KDint nHandler )
		: ScriptHandlerEntry ( nHandler )
		, m_bIsMultiTouches ( false )
		, m_nPriority ( 0 )
		, m_bSwallowsTouches ( false )
		{

		}

		KDbool		init ( KDbool bIsMultiTouches, KDint nPriority, KDbool bSwallowsTouches );
    
		KDbool		m_bIsMultiTouches;
		KDint		m_nPriority;
		KDbool		m_bSwallowsTouches;
};

enum ScriptEventType
{
    kNodeEvent = 0,
    kMenuClickedEvent,
    kNotificationEvent,
    kCallFuncEvent,
    kScheduleEvent,
    kTouchEvent,
    kTouchesEvent,
    kKeypadEvent,
    kAccelerometerEvent,
    kControlEvent,
    kCommonEvent,
    kTableViewEvent,			// Now it's only used in LuaBinding
    kAssetsManagerEvent,		// Now it's only used in Lua Binding
    kCocoStudioEventListener,	// Now it's only used in Lua Binding
    kArmatureWrapper,			// Now it's only used in Lua Binding
};

struct BasicScriptData
{
    // nativeobject:to get handler for lua or to get jsobject for js
    KDvoid*		pNativeObject;
    // value: a pointer to a object that already defined
    KDvoid*		pValue;
    
    // Constructor
    /**
     * @js NA
     * @lua NA
     */
    BasicScriptData ( KDvoid* pInObject, KDvoid* pInValue = KD_NULL )
    : pNativeObject ( pInObject ), pValue ( pInValue )
    {

    }
};

struct SchedulerScriptData
{
    // lua use
    KDint		nHandler;
    KDfloat		fElapse;
    // js use
    KDvoid*		pNode;
    
    // Constructor

    /**
     *	@js NA
     *	@lua NA
     */
    SchedulerScriptData ( KDint nInHandler, KDfloat fInElapse, KDvoid* pInNode = KD_NULL )
    : nHandler ( nInHandler ),
      fElapse ( fInElapse ),
      pNode ( pInNode )
    {

    }
};

struct TouchesScriptData
{
    EventTouch::EventCode		eActionType;
    KDvoid*						pNativeObject;
    const std::vector<Touch*>&	aTouches;
    
    // Constructor
    /**
     *	@js NA
     *	@lua NA
     */
    TouchesScriptData ( EventTouch::EventCode eInActionType, KDvoid* pInNativeObject, const std::vector<Touch*>& aInTouches )
    : eActionType ( eInActionType ),
      pNativeObject ( pInNativeObject ),
      aTouches ( aInTouches )
    {

    }
};

struct TouchScriptData
{
    EventTouch::EventCode		eActionType;
    KDvoid*						pNativeObject;
    Touch*						pTouch;
    
    // Constructor
    /**
     *	@js NA
     *	@lua NA
     */
    TouchScriptData ( EventTouch::EventCode eInActionType, KDvoid* pInNativeObject, Touch* pInTouch )
    : eActionType ( eInActionType ),
      pNativeObject ( pInNativeObject ),
      pTouch ( pInTouch )
    {

    }
};

struct KeypadScriptData
{
    EventKeyboard::KeyCode		eActionType;
    KDvoid*						pNativeObject;
    
    // Constructor
    /**
     *	@js NA
     *	@lua NA
     */
    KeypadScriptData ( EventKeyboard::KeyCode eInActionType, KDvoid* pInNativeObject )
    : eActionType ( eInActionType ), pNativeObject ( pInNativeObject )
    {

    }
};

struct CommonScriptData
{
    // Now this struct is only used in LuaBinding.
    KDint						nHandler;
    KDchar						szEventName [ 64 ];
    Object*						pEventSource;
    KDchar						szEventSourceClassName [ 64 ];
    
    // Constructor
    /**
     *	@js NA
     *	@lua NA
     */
    CommonScriptData ( KDint nInHandler, const KDchar* pInName, Object* pInSource = KD_NULL, const KDchar* pInClassName = KD_NULL )
    : nHandler ( nInHandler ),
      pEventSource ( pInSource )
    {
        kdStrncpy ( szEventName, pInName, 64 );
        
        if ( KD_NULL == pInClassName )
        {
            kdMemset ( szEventSourceClassName, 0, 64 * sizeof ( KDchar ) );
        }
        else
        {
            kdStrncpy ( szEventSourceClassName, pInClassName, 64 );
        }
    }
};

struct ScriptEvent
{
    ScriptEventType				eType;
    KDvoid*						pData;
    
    // Constructor
    /**
     *	@js NA
     *	@lua NA
     */
    ScriptEvent ( ScriptEventType eInType, KDvoid* pInData )
    : eType ( eInType ),
      pData ( pInData )
    {

    }
};

// Don't make ScriptEngineProtocol inherits from Object since setScriptEngine is invoked only once in AppDelegate.cpp,
// It will affect the lifecycle of ScriptCore instance, the autorelease pool will be destroyed before destructing ScriptCore.
// So a crash will appear on Win32 if you click the close button.
class CC_DLL ScriptEngineProtocol
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ScriptEngineProtocol ( KDvoid ) { }
    
		/**
		 *	Get script type 
		 *	@js NA
		 *	@lua NA
		 */
		virtual ccScriptType	getScriptType ( KDvoid ) { return kScriptTypeNone; }

		/** 
		 *	Remove script object. 
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			removeScriptObjectByObject ( Object* pObj ) = 0;
    
		/**
		 *	Remove script function handler, only LuaEngine class need to implement this function. 
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid			removeScriptHandler ( KDint nHandler ) { }
    
		/**
		 *	Reallocate script function handler, only LuaEngine class need to implement this function. 
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDint			reallocateScriptHandler ( KDint nHandler ) { return 0; }
    
		/**
		 *	@brief Execute script code contained in the given string.
		 *	@param codes holding the valid script code that should be executed.
		 *	@return 0 if the string is executed correctly.
		 *	@return other if the string is executed wrongly.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDint			executeString ( const KDchar* pCodes ) = 0;
    
		/**
		 *	@brief Execute a script file.
		 *	@param filename String object holding the filename of the script file that is to be executed
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDint			executeScriptFile ( const KDchar* pFilename ) = 0;
    
		/**
		 *	@brief Execute a scripted global function.
		 *	@brief The function should not take any parameters and should return an integer.
		 *	@param functionName String object holding the name of the function, in the global script environment, that is to be executed.
		 *	@return The integer value returned from the script function.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDint			executeGlobalFunction ( const KDchar* pFunctionName ) = 0;
    
		/**
		 *	when trigger a script event ,call this func,add params needed into ScriptEvent object.nativeObject is object triggering the event, can be NULL in lua
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDint			sendEvent ( ScriptEvent* pEvt ) = 0;
    
		/** 
		 *	called by CCAssert to allow scripting engine to handle failed assertions
		 *	@return true if the assert was handled by the script engine, false otherwise.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDbool			handleAssert ( const KDchar* pMsg ) = 0;
};

/**
 *	ScriptEngineManager is a singleton which holds an object instance of ScriptEngineProtocl
 *	It helps cocos2d-x and the user code to find back LuaEngine object
 *	@since v0.99.5-x-0.8.5
 */
class CC_DLL ScriptEngineManager
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~ScriptEngineManager ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		ScriptEngineProtocol*			getScriptEngine ( KDvoid )
		{
			return m_pScriptEngine;
		}
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid							setScriptEngine ( ScriptEngineProtocol* pScriptEngine );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid							removeScriptEngine ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		static ScriptEngineManager*		getInstance ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		static KDvoid					destroyInstance ( KDvoid );
    
	private :

		ScriptEngineManager ( KDvoid )
		: m_pScriptEngine ( KD_NULL )
		{

		}
    
		ScriptEngineProtocol*			m_pScriptEngine;
};

//	end of script_support group
/// @}

NS_CC_END

#endif	// __CCScriptSupport_h__
