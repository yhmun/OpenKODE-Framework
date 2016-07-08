/* -----------------------------------------------------------------------------------
 *
 *      File            DictionaryHelper.h
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

#ifndef __DictionaryHelper_h__
#define __DictionaryHelper_h__

#include "CSContentJsonDictionary.h"
#include "../../../support/component/CCComponent.h"

#define DICTOOL DictionaryHelper::shareHelper()

namespace cocostudio {

class DictionaryHelper
{
	public :

		DictionaryHelper ( KDvoid );
		~DictionaryHelper ( KDvoid );

		static DictionaryHelper*	shareHelper ( KDvoid );
		static KDvoid				purgeDictionaryHelper ( KDvoid );

		cocos2d::Dictionary*		getSubDictionary				( cocos2d::Dictionary* pRoot, const KDchar* pKey );
		KDint						getIntValue						( cocos2d::Dictionary* pRoot, const KDchar* pKey );
		KDfloat						getFloatValue					( cocos2d::Dictionary* pRoot, const KDchar* pKey );
		const KDchar*				getStringValue					( cocos2d::Dictionary* pRoot, const KDchar* pKey );
		KDbool						getBooleanValue					( cocos2d::Dictionary* pRoot, const KDchar* pKey );
		cocos2d::Array*				getArrayValue					( cocos2d::Dictionary* pRoot, const KDchar* pKey );
		cocos2d::Object*			checkObjectExist				( cocos2d::Dictionary* pRoot, const KDchar* pKey );

		KDint						objectToIntValue				( cocos2d::Object* pObj );
		KDfloat						objectToFloatValue				( cocos2d::Object* pObj );
		const KDchar*				objectToStringValue				( cocos2d::Object* pObj );
		KDbool						objectToBooleanValue			( cocos2d::Object* pObj );
		cocos2d::Array*				objectToCCArray					( cocos2d::Object* pObj );
    
		JsonDictionary*				getSubDictionary_json			( JsonDictionary* pRoot, const KDchar* pKey );
		KDint						getIntValue_json				( JsonDictionary* pRoot, const KDchar* pKey );
		KDfloat						getFloatValue_json				( JsonDictionary* pRoot, const KDchar* pKey );
		const KDchar*				getStringValue_json				( JsonDictionary* pRoot, const KDchar* pKey );
		KDbool						getBooleanValue_json			( JsonDictionary* pRoot, const KDchar* pKey );
		KDint						getArrayCount_json				( JsonDictionary* pRoot, const KDchar* pKey );
		KDint						getIntValueFromArray_json		( JsonDictionary* pRoot, const KDchar* pArrayKey, KDint nIdx );
		KDfloat						getFloatValueFromArray_json		( JsonDictionary* pRoot, const KDchar* pArrayKey, KDint nIdx );
		KDbool						getBoolValueFromArray_json		( JsonDictionary* pRoot, const KDchar* pArrayKey, KDint nIdx );
		const KDchar*				getStringValueFromArray_json	( JsonDictionary* pRoot, const KDchar* pArrayKey, KDint nIdx );
		JsonDictionary*				getDictionaryFromArray_json		( JsonDictionary* pRoot, const KDchar* pArrayKey, KDint nIdx );
		KDbool						checkObjectExist_json			( JsonDictionary* pRoot, const KDchar* pKey );
};

}

#endif	// __DictionaryHelper_h__
