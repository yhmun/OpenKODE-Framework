/* -----------------------------------------------------------------------------------
 *
 *      File            CCObject.h
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

#ifndef __CCObject_h__
#define __CCObject_h__

#include "CCDataVisitor.h"

NS_CC_BEGIN

/**
 * @addtogroup base_nodes
 * @{
 */

class CCZone;
class CCObject;
class CCNode;
class CCEvent;

class CCCopying
{
	public :

		virtual CCObject*	copyWithZone ( CCZone* pZone );
};

class CCObject : public CCCopying
{
	friend class CCAutoReleasePool;

	public :
		         CCObject ( KDvoid );
		virtual ~CCObject ( KDvoid );

	public :

		virtual	KDbool		init ( KDvoid );

		KDvoid				release ( KDvoid );
	
		KDvoid				retain  ( KDvoid );

		CCObject*			autorelease ( KDvoid );

		CCObject*			copy ( KDvoid );

		KDbool				isSingleReference ( KDvoid ) const;

		KDuint				retainCount ( KDvoid ) const;

		virtual KDbool		isEqual ( const CCObject* pObject );

		virtual KDvoid		acceptVisitor ( CCDataVisitor& tVisitor );

		virtual KDvoid		update ( KDfloat fDelta ) { CC_UNUSED_PARAM ( fDelta ); };

	protected :
		
		// object id, CCScriptSupport need public m_uID
		KDuint				m_uID;             

		// Lua reference id
		KDint				m_nLuaID;

		// count of references	
		KDuint				m_uReference;      	

		// count of autorelease
		KDuint				m_uAutoReleaseCount;    
};

typedef KDvoid ( CCObject::*SEL_SCHEDULE	 ) ( KDfloat   );
typedef KDvoid ( CCObject::*SEL_CallFunc	 ) ( KDvoid    );
typedef KDvoid ( CCObject::*SEL_CallFuncN	 ) ( CCNode*   );
typedef KDvoid ( CCObject::*SEL_CallFuncND	 ) ( CCNode*, KDvoid* );
typedef KDvoid ( CCObject::*SEL_CallFuncO	 ) ( CCObject* );
typedef KDvoid ( CCObject::*SEL_MenuHandler	 ) ( CCObject* );
typedef KDvoid ( CCObject::*SEL_EventHandler ) ( CCEvent*  );
typedef KDint  ( CCObject::*SEL_Compare      ) ( CCObject* );
typedef KDvoid ( CCObject::*SEL_SOUND        ) ( KDuint uID, KDint uParam );

#define schedule_selector(_SELECTOR)	( SEL_SCHEDULE	   ) ( &_SELECTOR )
#define callfunc_selector(_SELECTOR)	( SEL_CallFunc	   ) ( &_SELECTOR )
#define callfuncN_selector(_SELECTOR)	( SEL_CallFuncN    ) ( &_SELECTOR )
#define callfuncND_selector(_SELECTOR)	( SEL_CallFuncND   ) ( &_SELECTOR )
#define callfuncO_selector(_SELECTOR)	( SEL_CallFuncO    ) ( &_SELECTOR ) 
#define menu_selector(_SELECTOR)		( SEL_MenuHandler  ) ( &_SELECTOR )
#define event_selector(_SELECTOR)		( SEL_EventHandler ) ( &_SELECTOR )
#define compare_selector(_SELECTOR)     ( SEL_Compare      ) ( &_SELECTOR )
#define sound_selector(_SELECTOR)		( SEL_SOUND        ) ( &_SELECTOR )

// end of base_nodes group
/// @}

NS_CC_END

#endif // __CCObject_h__
