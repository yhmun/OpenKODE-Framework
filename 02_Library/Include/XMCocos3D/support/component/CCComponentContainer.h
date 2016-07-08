/* -----------------------------------------------------------------------------------
 *
 *      File            CCComponentContainer.h
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

#ifndef __CCComponentContainer_h__
#define __CCComponentContainer_h__

#include "../../base/CCDictionary.h"

NS_CC_BEGIN

class Component;
class Node;

class CC_DLL ComponentContainer
{
	protected :

		/**
		 *	@js ctor
		 */
		ComponentContainer ( Node* pNode );
    
	public :

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ComponentContainer ( KDvoid );

		virtual Component*		get ( const KDchar* pName ) const;

		virtual KDbool			add ( Component* pCom );

		virtual KDbool			remove ( const KDchar* pName );
		virtual KDvoid			removeAll ( KDvoid );
		virtual KDvoid			visit ( KDfloat fDelta );

	public :

		KDbool					isEmpty ( KDvoid ) const;
    
	private :

		KDvoid					alloc ( KDvoid );
    
	private :

		Dictionary*				m_pComponents;        ///< Dictionary of components
		Node*					m_pOwner;
    
		friend class Node;
};

NS_CC_END

#endif  // __CCComponentContainer_h__
