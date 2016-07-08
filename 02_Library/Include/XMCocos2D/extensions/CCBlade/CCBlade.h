/* -----------------------------------------------------------------------------------
 *
 *      File            CCBlade.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2011      Ngo Duc Hiep. All rights reserved.
 *
 *         http://precognitiveresearch.com      
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 * 
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 * 
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCBlade_h__
#define __CCBlade_h__

#include "../../base_nodes/CCNode.h"


NS_CC_BEGIN


class CCBlade : public CCNode 
{
	public :
				 CCBlade ( KDvoid );
		virtual ~CCBlade ( KDvoid );

		static	 CCBlade*	create ( KDint nLimit );

	public :

		virtual	KDbool		initWithMaximumPoint ( KDint nLimit );

		virtual	KDvoid		update ( KDfloat fDelta );

		virtual KDvoid		draw ( KDvoid );

		virtual KDvoid		setTexture ( CCTexture2D* pTexture );

		virtual KDbool		 isAutoDim ( KDvoid );
		virtual KDvoid		setAutoDim ( KDbool bAutoDim );

		virtual CCArray*	getPath ( KDvoid ) const;

		KDvoid				push (CCPoint v );
		
		KDvoid				pop ( KDint n );
		
		KDvoid				clear ( KDvoid );

		KDvoid				reset ( KDvoid );
		
		KDvoid				dim ( KDbool bDim );

		KDvoid				finish ( KDvoid );
		
	private :

		KDvoid				populateVertices ( KDvoid );

	protected :

		KDint				m_nCount;
		CCPoint*			m_pVertices;
		CCPoint*			m_pCoordinates;
		KDbool				m_bReset;
		KDbool				m_bFinish;
		KDbool				m_bWillPop;	    
		KDfloat				m_fTimeSinceLastPop;
		KDfloat				m_fPopTimeInterval;
		KDuint				m_uPointLimit;
		CCTexture2D*		m_pTexture;
		KDfloat				m_fWidth;
		KDbool				m_bAutoDim;
		CCArray*			m_pPath;
};


NS_CC_END


#endif	// __CCBlade_h__