/* -----------------------------------------------------------------------------------
 *
 *      File            CCNodeLoaderLibrary.h
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

#ifndef __CCNodeLoaderLibrary_h__
#define __CCNodeLoaderLibrary_h__

#include "CCBReader.h"

namespace cocosbuilder {

class NodeLoader;

typedef std::map<std::string, NodeLoader*>	NodeLoaderMap;
typedef std::pair<std::string, NodeLoader*> NodeLoaderMapEntry;

class NodeLoaderLibrary : public cocos2d::Object
{
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( NodeLoaderLibrary, library );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		NodeLoaderLibrary ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~NodeLoaderLibrary ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid						registerDefaultNodeLoaders ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid						registerNodeLoader ( const KDchar* pClassName, NodeLoader* pNodeLoader );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid						unregisterNodeLoader ( const KDchar* pClassName );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		NodeLoader*					getNodeLoader ( const KDchar* pClassName );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid						purge ( KDbool bDelete );
    
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		static NodeLoaderLibrary*	getInstance ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		static KDvoid				destroyInstance ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		static NodeLoaderLibrary*	newDefaultNodeLoaderLibrary ( KDvoid );
  
	private :

		NodeLoaderMap				m_tNodeLoaders;
};

}

#endif	// __CCNodeLoaderLibrary_h__
