/* --------------------------------------------------------------------------
 *
 *      File            CCBKeyframe.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org   
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#ifndef __CCBKeyframe_h__
#define __CCBKeyframe_h__

#include "../../cocoa/CCObject.h"

NS_CC_BEGIN

class CCBKeyframe : public CCObject
{	    
	public :
				 CCBKeyframe ( KDvoid );
		virtual ~CCBKeyframe ( KDvoid );
	    
	public :

		CCObject*		getValue ( KDvoid );
		KDvoid			setValue ( CCObject* pValue );			// retain
	    
		KDfloat			getTime ( KDvoid );
		KDvoid			setTime ( KDfloat fTime );
	    
		KDint			getEasingType ( KDvoid );
		KDvoid			setEasingType ( KDint nEasingType );
	    
		KDfloat			getEasingOpt ( KDvoid );
		KDvoid			setEasingOpt ( KDfloat fEasingOpt );

	private :

		CCObject*		m_pValue;
		KDfloat			m_fTime;
		KDint			m_nEasingType;
		KDfloat			m_fEasingOpt;
};

NS_CC_END

#endif	// __CCBKeyframe_h__
