/* -----------------------------------------------------------------------------------
 *
 *      File            CCBValue.h
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

#ifndef __CCBValue_h__
#define __CCBValue_h__

#include "../../ccTypes.h"
#include "../../base/CCArray.h"

/*
 These classes are wrapper of basic types, such as Color3B
 */

namespace cocosbuilder {

class Color3BWapper : public cocos2d::Object
{
	public :

		static Color3BWapper*		create ( const cocos2d::Color3B& tColor );
    
		const cocos2d::Color3B&		getColor ( KDvoid ) const;
    
	private :

		cocos2d::Color3B			m_tColor;
};



class CCBValue : public cocos2d::Object
{
	public :
    
		enum class Type
		{
			INT,
			FLOAT,
			BOOL,
			UNSIGNED_CHAR,
			STRING,
			ARRAY
		};
    
		static CCBValue*			create ( KDint nValue );
		static CCBValue*			create ( KDbool bValue );
		static CCBValue*			create ( KDfloat fValue );
		static CCBValue*			create ( KDubyte cByte );
		static CCBValue*			create ( const KDchar* pStr );
		static CCBValue*			create ( cocos2d::Array* pArr );

    
		KDint						getIntValue		( KDvoid );
		KDfloat						getFloatValue	( KDvoid );
		KDbool						getBoolValue	( KDvoid );
		KDubyte						getByteValue	( KDvoid );
		const KDchar*				getStringValue	( KDvoid );
		cocos2d::Array*				getArrayValue	( KDvoid );
    
		Type						getType			( KDvoid );
    
	private :

		union
		{
			KDint					intValue;
			KDfloat					floatValue;
		} m_tValue;
    
		std::string					m_sStrValue;
		cocos2d::Array*				m_pArrValue;
		Type						m_eType;
};

}

#endif	// __CCBValue_h__
