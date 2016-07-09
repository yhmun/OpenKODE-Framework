/* -----------------------------------------------------------------------------------
 *
 *      File            CCDecorativeDisplay.h
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

#ifndef __CCDecorativeDisplay_h__
#define __CCDecorativeDisplay_h__

#include "../utils/CCArmatureDefine.h"
#include "CCDisplayFactory.h"
#include "../datas/CCDatas.h"
#include "../../../../2d/base_nodes/CCNode.h"

#if ENABLE_PHYSICS_BOX2D_DETECT || ENABLE_PHYSICS_CHIPMUNK_DETECT
#include "../physics/CCColliderDetector.h"
#endif

namespace cocostudio {

class  DecorativeDisplay : public cocos2d::Object
{
	public :

		static DecorativeDisplay*	create ( KDvoid );

	public :

		/**
		 *	@js ctor
		 */
		DecorativeDisplay ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~DecorativeDisplay ( KDvoid );

		virtual KDbool				init ( KDvoid );

		virtual KDvoid				setDisplay ( cocos2d::Node* pDisplay ) 
		{ 
			if ( m_pDisplay != pDisplay )
			{
				CC_SAFE_RETAIN ( pDisplay );
				CC_SAFE_RELEASE ( m_pDisplay );
				m_pDisplay = pDisplay; 
			}
		}

		virtual cocos2d::Node*		getDisplay ( KDvoid ) const { return m_pDisplay; }

		virtual KDvoid				setDisplayData ( DisplayData *pData )
		{
			if ( m_pDisplayData != pData )
			{
				CC_SAFE_RETAIN ( pData );
				CC_SAFE_RELEASE ( m_pDisplayData );
				m_pDisplayData = pData; 
			}
		}

		virtual DisplayData*		getDisplayData ( KDvoid ) const { return m_pDisplayData; }

	#if ENABLE_PHYSICS_BOX2D_DETECT || ENABLE_PHYSICS_CHIPMUNK_DETECT
		virtual KDvoid				setColliderDetector ( ColliderDetector* pDetector )
		{
			if ( m_pColliderDetector != pDetector )
			{
				CC_SAFE_RETAIN ( pDetector);
				CC_SAFE_RELEASE ( m_pColliderDetector );
				m_pColliderDetector = pDetector; 
			}
		}
		virtual ColliderDetector*	getColliderDetector ( KDvoid ) const { return m_pColliderDetector; }
	#endif

	protected :

		cocos2d::Node*				m_pDisplay;
		DisplayData*				m_pDisplayData;

	#if ENABLE_PHYSICS_BOX2D_DETECT || ENABLE_PHYSICS_CHIPMUNK_DETECT
		ColliderDetector*			m_pColliderDetector;
	#endif
};

}

#endif	// __CCDecorativeDisplay_h__
