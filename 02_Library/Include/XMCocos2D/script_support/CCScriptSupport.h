/* -----------------------------------------------------------------------------------
 *
 *      File            CCScriptSupport.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCScriptSupport_h__
#define __CCScriptSupport_h__

#include "../cocoa/CCSet.h"
#include "../touch_dispatcher/CCTouch.h"
#include <map>
#include <string>
#include <list>

NS_CC_BEGIN
    
/**
 * @addtogroup script_support
 * @{
 */

class CCTimer;
class CCLayer;
class CCMenuItem;
class CCNotificationCenter;
class CCCallFunc;
class CCAcceleration;

enum ccScriptType 
{
    kScriptTypeNone			= 0	,
    kScriptTypeLua				,
    kScriptTypeJavascript		,
};

class CCScriptHandlerEntry : public CCObject
{
	protected :

		CCScriptHandlerEntry ( KDint nHandler );

	public :

		virtual ~CCScriptHandlerEntry ( KDvoid );

		static CCScriptHandlerEntry*	create ( KDint nHandler );
		
	public :
	    
		inline KDint			getHandler ( KDvoid ) { return m_nHandler; }	    
		inline KDint			getEntryId ( KDvoid ) { return m_nEntryId; }
	    
	protected :

		KDint					m_nHandler;
		KDint					m_nEntryId;
};

class CCSchedulerScriptHandlerEntry : public CCScriptHandlerEntry
{
	private :

		CCSchedulerScriptHandlerEntry ( KDint nHandler );

	public :

		virtual ~CCSchedulerScriptHandlerEntry ( KDvoid );

		// nHandler return by tolua_ref_function(), called from LuaCocos2d.cpp
		static   CCSchedulerScriptHandlerEntry*		create ( KDint nHandler, KDfloat fInterval, KDbool bPaused );

	private :

		virtual KDbool			init ( KDfloat fInterval, KDbool bPaused );

	public :

		inline  CCTimer*		getTimer ( KDvoid ) { return m_pTimer; }
	    
		inline  KDbool			isPaused ( KDvoid ) { return m_bPaused; }
	    
		inline  KDvoid			  markedForDeletion ( KDvoid ) { m_bMarkedForDeletion = KD_TRUE; }	    
		inline  KDbool			isMarkedForDeletion ( KDvoid ) { return m_bMarkedForDeletion; }
	  
	private :

		CCTimer*				m_pTimer;
		KDbool					m_bPaused;
		KDbool					m_bMarkedForDeletion;
};

class CCTouchScriptHandlerEntry : public CCScriptHandlerEntry
{
	private :

		CCTouchScriptHandlerEntry ( KDint nHandler );

	public :

		virtual ~CCTouchScriptHandlerEntry ( KDvoid );

		static   CCTouchScriptHandlerEntry*		create ( KDint nHandler, KDbool bIsMultiTouches, KDint nPriority, KDbool bSwallowsTouches );
		
	private :

		virtual  KDbool			init ( KDbool bIsMultiTouches, KDint nPriority, KDbool bSwallowsTouches );
	    
	public :
	    
		inline KDbool			isMultiTouches ( KDvoid ) { return m_bIsMultiTouches; }
	    
		inline KDint			getPriority ( KDvoid ) { return m_nPriority; }
	    
		inline KDbool			getSwallowsTouches ( KDvoid ) { return m_bSwallowsTouches; }
	    
	private :

		KDbool					m_bIsMultiTouches;
		KDint					m_nPriority;
		KDbool					m_bSwallowsTouches;
};

// Don't make CCScriptEngineProtocol inherits from CCObject since setScriptEngine is invoked only once in AppDelegate.cpp,
// It will affect the lifecycle of ScriptCore instance, the autorelease pool will be destroyed before destructing ScriptCore.
// So a crash will appear on Win32 if you click the close button.
class CCScriptEngineProtocol
{
	public :

        virtual ~CCScriptEngineProtocol ( KDvoid ) { };
    
		/** Get script type */
		virtual ccScriptType	getScriptType ( KDvoid ) { return kScriptTypeNone; };

		/** Remove script object. */
		virtual KDvoid			removeScriptObjectByCCObject ( CCObject* pObject ) = 0;

		/** Remove script function handler, only CCLuaEngine class need to implement this function. */
		virtual KDvoid			removeScriptHandler ( KDint nHandler ) { };
	    
		/** Reallocate script function handler, only CCLuaEngine class need to implement this function. */
		virtual KDint			reallocateScriptHandler ( KDint nHandler ) { return -1; }

		/**
		 *	@brief Execute script code contained in the given string.
		 *	@param codes holding the valid script code that should be executed.
		 *	@return 0 if the string is executed correctly.
		 *	@return other if the string is executed wrongly.
		 */
		virtual KDint			executeString ( const KDchar* codes ) = 0;
	    
		/**
		 *	@brief Execute a script file.
		 *	@param filename String object holding the filename of the script file that is to be executed
		 */
		virtual KDint			executeScriptFile ( const KDchar* filename ) = 0;
	    
		/**
		 *	@brief Execute a scripted global function.
		 *	@brief The function should not take any parameters and should return an integer.
		 *	@param functionName String object holding the name of the function, in the global script environment, that is to be executed.
		 *	@return The integer value returned from the script function.
		 */
		virtual KDint			executeGlobalFunction ( const KDchar* functionName ) = 0;
	    
		/**
		 *	@brief  Execute a node event function
		 *	@param  pNode which node produce this event
		 *	@param  nAction kCCNodeOnEnter,kCCNodeOnExit,kCCMenuItemActivated,kCCNodeOnEnterTransitionDidFinish,kCCNodeOnExitTransitionDidStart
		 *	@return The integer value returned from the script function.
		 */
		virtual KDint			executeNodeEvent ( CCNode* pNode, KDint nAction ) = 0;
	    
		virtual KDint			executeMenuItemEvent ( CCMenuItem* pMenuItem ) = 0;
		/** Execute a notification event function */
		virtual KDint			executeNotificationEvent ( CCNotificationCenter* pNotificationCenter, const KDchar* szName ) = 0;
	    
		/** execute a callfunc event */
		virtual KDint			executeCallFuncActionEvent ( CCCallFunc* pAction, CCObject* pTarget = KD_NULL ) = 0;
		/** execute a schedule function */
		virtual KDint			executeSchedule ( KDint nHandler, KDfloat dt, CCNode* pNode = KD_NULL ) = 0;
	    
		/** functions for executing touch event */
		virtual KDint			executeLayerTouchesEvent  ( CCLayer* pLayer, KDint nEventType, CCSet* pTouches ) = 0;
		virtual KDint			executeLayerTouchEvent    ( CCLayer* pLayer, KDint nEventType, CCTouch* pTouch ) = 0;

		/** functions for keypad event */
		virtual KDint			executeLayerKeypadEvent   ( CCLayer* pLayer, KDint nEventType ) = 0;

		/** execute a accelerometer event */
		virtual KDint			executeAccelerometerEvent ( CCLayer* pLayer, CCAcceleration* pAccelerationValue ) = 0;

		/** function for common event */
		virtual KDint			executeEvent ( KDint nHandler, const KDchar* szEventName, CCObject* pEventSource = KD_NULL, const KDchar* pEventSourceClassName = KD_NULL ) = 0;

		/** function for c++ call back lua function */
		virtual KDint			executeEventWithArgs ( KDint nHandler, CCArray* pArgs ) { return 0; }

		/** called by CCAssert to allow scripting engine to handle failed assertions
		 * @return true if the assert was handled by the script engine, false otherwise.
		 */
		virtual KDbool			handleAssert ( const KDchar* szMsg ) = 0;
};

/**
 *	CCScriptEngineManager is a singleton which holds an object instance of CCScriptEngineProtocl
 *	It helps cocos2d-x and the user code to find back LuaEngine object
 *	@since	v0.99.5-x-0.8.5
 */
class CCScriptEngineManager
{
	protected :

		         CCScriptEngineManager ( KDvoid );
		virtual ~CCScriptEngineManager ( KDvoid );

	public :

		static CCScriptEngineManager*	sharedManager ( KDvoid );
		static KDvoid                    purgeManager ( KDvoid );
	    
	public :

		KDvoid						setScriptEngine ( CCScriptEngineProtocol* pScriptEngine );
		CCScriptEngineProtocol*		getScriptEngine ( KDvoid );

		KDvoid						removeScriptEngine ( KDvoid );
	    
	private :
	    
		CCScriptEngineProtocol*     m_pScriptEngine;
};

// end of script_support group
/// @}

NS_CC_END

#endif // __CCScriptSupport_h__