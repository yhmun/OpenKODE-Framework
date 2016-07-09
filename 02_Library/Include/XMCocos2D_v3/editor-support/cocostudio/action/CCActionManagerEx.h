/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionManagerEx.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
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

#ifndef __CCActionManagerEx_h__
#define __CCActionManagerEx_h__

#include "CCActionObject.h"
#include "../json/CSContentJsonDictionary.h"

namespace cocostudio {

class ActionManagerEx : public cocos2d::Object
{
	public :
    
		/**
		 *	Default constructor
		 */
		ActionManagerEx ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~ActionManagerEx ( KDvoid );
    
		/**
		 *	Gets the static instance of ActionManager.
		 */
		static ActionManagerEx*		shareManager ( KDvoid );

		/**
		 *	Purges ActionManager point.
		 */
		static KDvoid				purgeActionManager ( KDvoid );

		/**
		 *	Gets an ActionObject with a name.
		 *
		 *	@param jsonName  UI file name
		 *
		 *	@param actionName  action name in the UI file.
		 *
		 *	@return  ActionObject which named as the param name
		 */
		ActionObject*				getActionByName ( const KDchar* pJsonName, const KDchar* pActionName );

		/**
		 *	Play an Action with a name.
		 *
		 *	@param jsonName  UI file name
		 *
		 *	@param actionName  action name in teh UIfile.
		 *
		 *	@return  ActionObject which named as the param name
		 */
		ActionObject*				playActionByName ( const KDchar* pJsonName, const KDchar* pActionName );
    
		/** init properties with json dictionay */
		KDvoid						initWithDictionary ( const KDchar* pJsonName, JsonDictionary* pDic, cocos2d::Object* pRoot );

		/**
		 *	Release all actions.
		 *
		 */
		KDvoid						releaseActions ( KDvoid );

	protected :

		cocos2d::Dictionary*		m_pActionDic;
};

}

#endif	// __CCActionManagerEx_h__
