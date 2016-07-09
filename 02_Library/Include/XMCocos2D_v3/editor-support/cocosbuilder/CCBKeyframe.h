/* -----------------------------------------------------------------------------------
 *
 *      File            CCBKeyframe.h
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

#ifndef __CCBKeyframe_h__
#define __CCBKeyframe_h__

#include "../../base/CCObject.h"

namespace cocosbuilder {

class CCBKeyframe : public cocos2d::Object
{
	public :

		enum class EasingType
		{
			INSTANT,
        
			LINEAR,
        
			CUBIC_IN,
			CUBIC_OUT,
			CUBIC_INOUT,
        
			ELASTIC_IN,
			ELASTIC_OUT,
			ELASTIC_INOUT,
        
			BOUNCE_IN,
			BOUNCE_OUT,
			BOUNCE_INOUT,
        
			BACK_IN,
			BACK_OUT,
			BACK_INOUT,
		};

		/**
		 *	@js ctor
		 */
		CCBKeyframe ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~CCBKeyframe ( KDvoid );
    
		cocos2d::Object*		getValue ( KDvoid );
		KDvoid					setValue ( cocos2d::Object* pValue );	// retain
    
		KDfloat					getTime ( KDvoid );
		KDvoid					setTime ( KDfloat fTime );
    
		EasingType				getEasingType ( KDvoid );
		KDvoid					setEasingType ( EasingType eEasingType );
    
		KDfloat					getEasingOpt ( KDvoid );
		KDvoid					setEasingOpt ( KDfloat fEasingOpt );
    
	private :

		cocos2d::Object*		m_pValue;
		KDfloat					m_fTime;
		EasingType				m_eEasingType;
		KDfloat					m_fEasingOpt;
};

}

#endif // __CCBKeyframe_h__
