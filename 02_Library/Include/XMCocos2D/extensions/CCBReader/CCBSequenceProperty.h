/* --------------------------------------------------------------------------
 *
 *      File            CCBSequenceProperty.h
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

#ifndef __CCBSequenceProperty_h__
#define __CCBSequenceProperty_h__

#include "CCBKeyframe.h"
#include "../../cocoa/CCArray.h"
#include <string>

NS_CC_BEGIN

class CCBSequenceProperty : public CCObject
{	    
	public :
				 CCBSequenceProperty ( KDvoid );
		virtual ~CCBSequenceProperty ( KDvoid );
	    
	public :

		virtual KDbool		init ( KDvoid );
	    
		const KDchar*		getName ( KDvoid );
		KDvoid				setName ( const KDchar* szName );
	    
		KDint				getType ( KDvoid );
		KDvoid				setType ( KDint nType );
	    
		CCArray*			getKeyframes ( KDvoid );

	private :

		std::string			m_sName;
		KDint				m_nType;
		CCArray*			m_pKeyframes;
};

NS_CC_END

#endif	// __CCBSequenceProperty_h__
