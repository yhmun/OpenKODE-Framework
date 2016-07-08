/* --------------------------------------------------------------------------
 *
 *      File            CCBValue.h
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

#ifndef __CCBValue_h__
#define __CCBValue_h__

#include "../../ccTypes.h"
#include "../../cocoa/CCObject.h"
#include "../../cocoa/CCArray.h"
#include <string>

NS_CC_BEGIN

/*
 *	These classes are wrapper of basic types, such as ccColor3B
 */
class ccColor3BWapper : public CCObject
{    
	public :

		static ccColor3BWapper*		create ( const ccColor3B& tColor );
	    
	public :

		const ccColor3B&			getColor ( KDvoid ) const;

	private :

		ccColor3B					m_tColor;
};

enum
{
    kCCBIntValue			,
    kCCBFloatValue			,
    kCCBBoolValue			,
    kCCBUnsignedCharValue	,
	kCCBStringValue			,
	kCCBArrayValue			,
};

class CCBValue : public CCObject
{	    
	public :

		static CCBValue*	create ( KDint   nValue );
		static CCBValue*	create ( KDfloat fValue );
		static CCBValue*	create ( KDbool  bValue );
		static CCBValue*	create ( KDubyte cValue );
		static CCBValue*	create ( const KDchar* szValue );
		static CCBValue*	create ( CCArray* pValue );
	    
	public :

		KDint				getIntValue    ( KDvoid );
		KDfloat				getFloatValue  ( KDvoid );
		KDbool				getBoolValue   ( KDvoid );
		KDubyte				getByteValue   ( KDvoid );
		const KDchar*		getStringValue ( KDvoid );
		CCArray*			getArrayValue  ( KDvoid );

		KDint				getType ( KDvoid );

	private :

		union 
		{
			KDint			nValue;
			KDfloat			fValue;
		}	m_tValue;
	    
		std::string			m_sValue;
		CCArray*			m_aValue;
		KDint				m_nType;
};

NS_CC_END

#endif	// __CCBValue_h__
