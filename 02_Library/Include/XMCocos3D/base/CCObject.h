/* -----------------------------------------------------------------------------------
 *
 *      File            CCObject.h
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

#ifndef __CCObject_h__
#define __CCObject_h__

#include "CCDataVisitor.h"
#include "../ccMacros.h"

NS_CC_BEGIN

/**
 *	@addtogroup base_nodes
 *	@{
 */

class Object;
class Node;

/** Interface that defines how to clone an object */
class CC_DLL Clonable
{
	public :

		/** returns a copy of the object */
		virtual Clonable*	clone ( KDvoid ) const = 0;

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~Clonable ( KDvoid ) { };
};

class CC_DLL Object
{
	public :

		/// object id, ScriptSupport need public _ID
		KDuint				m_uID;
		/// Lua reference id
		KDint				m_nLuaID;

	protected :

		/// count of references
		KDuint				m_uReference;
		/// count of autorelease
		KDuint				m_uAutoReleaseCount;

	public :

		/**
		 *	Constructor
		 *
		 *	The object's reference count is 1 after construction.
		 *	@js NA
		 */
		Object ( KDvoid );
    
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~Object ( KDvoid );
    
		/**
		 *	Release the ownership immediately.
		 *
		 *	This decrements the object's reference count.
		 *
		 *	If the reference count reaches 0 after the descrement, this object is
		 *	destructed.
		 *
		 *	@see retain, autorelease
		 *	@js NA
		 */
		inline KDvoid		release ( KDvoid )
		{
			CCASSERT ( m_uReference > 0, "reference count should greater than 0" );
			--m_uReference;

			if ( m_uReference == 0 )
				delete this;
		}

		/**
		 *	Retains the ownership.
		 *
		 *	This increases the object's reference count.
		 *
		 *	@see release, autorelease
		 *	@js NA
		 */
		inline KDvoid		retain ( KDvoid )
		{
			CCASSERT ( m_uReference > 0, "reference count should greater than 0" );
			++m_uReference;
		}

		/**
		 *	Release the ownership sometime soon automatically.
		 *
		 *	This descrements the object's reference count at the end of current
		 *	autorelease pool block.
		 *
		 *	If the reference count reaches 0 after the descrement, this object is
		 *	destructed.
		 *
		 *	@returns The object itself.
		 *
		 *	@see AutoreleasePool, retain, release
		 *	@js NA
		 *	@lua NA
		 */
		Object*				autorelease ( KDvoid );

		/**
		 *	Returns a boolean value that indicates whether there is only one
		 *	reference to the object. That is, whether the reference count is 1.
		 *
		 *	@returns Whether the object's reference count is 1.
		 *	@js NA
		 */
		KDbool				isSingleReference ( KDvoid ) const;

		/**
		 *	Returns the object's current reference count.
		 *
		 *	@returns The object's reference count.
		 *	@js NA
		 */
		KDuint				retainCount ( KDvoid ) const;

		/**
		 *	Returns a boolean value that indicates whether this object and a given
		 *	object are equal.
		 *
		 *	@param object    The object to be compared to this object.
		 *
		 *	@returns True if this object and @p object are equal, otherwise false.
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDbool		isEqual ( const Object* pObject );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid		acceptVisitor ( DataVisitor& tVisitor );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual KDvoid		update ( KDfloat dt ) { CC_UNUSED_PARAM ( dt ); }
    
		friend class AutoreleasePool;
};


typedef KDvoid ( Object::*SEL_SCHEDULE    ) ( KDfloat );
typedef KDvoid ( Object::*SEL_CallFunc    ) ( KDvoid );
typedef KDvoid ( Object::*SEL_CallFuncN   ) ( Node*);
typedef KDvoid ( Object::*SEL_CallFuncND  ) ( Node*, KDvoid* );
typedef KDvoid ( Object::*SEL_CallFuncO   ) ( Object* );
typedef KDvoid ( Object::*SEL_MenuHandler ) ( Object* );
typedef KDint  ( Object::*SEL_Compare     ) ( Object* );

#define schedule_selector(_SELECTOR)						static_cast<cocos2d::SEL_SCHEDULE>		( &_SELECTOR )
#define callfunc_selector(_SELECTOR)						static_cast<cocos2d::SEL_CallFunc>		( &_SELECTOR )
#define callfuncN_selector(_SELECTOR)						static_cast<cocos2d::SEL_CallFuncN>		( &_SELECTOR )
#define callfuncND_selector(_SELECTOR)						static_cast<cocos2d::SEL_CallFuncND>	( &_SELECTOR )
#define callfuncO_selector(_SELECTOR)						static_cast<cocos2d::SEL_CallFuncO>		( &_SELECTOR )
#define menu_selector(_SELECTOR)							static_cast<cocos2d::SEL_MenuHandler>	( &_SELECTOR )
#define event_selector(_SELECTOR)							static_cast<cocos2d::SEL_EventHandler>	( &_SELECTOR )
#define compare_selector(_SELECTOR)							static_cast<cocos2d::SEL_Compare>		( &_SELECTOR )

// new callbacks based on C++11
#define CC_CALLBACK_0( __selector__, __target__, ... )		std::bind ( &__selector__, __target__, ##__VA_ARGS__ )
#define CC_CALLBACK_1( __selector__, __target__, ... )		std::bind ( &__selector__, __target__, std::placeholders::_1, ##__VA_ARGS__ )
#define CC_CALLBACK_2( __selector__, __target__, ... )		std::bind ( &__selector__, __target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__ )
#define CC_CALLBACK_3( __selector__, __target__, ... )		std::bind ( &__selector__, __target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ##__VA_ARGS__ )

// end of base_nodes group
/// @}

NS_CC_END

#endif	// __CCObject_h__
