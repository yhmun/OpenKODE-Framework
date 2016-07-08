/* -----------------------------------------------------------------------------------
 *
 *      File            CSContentJsonDictionary.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
 *      Copyright (c) 2012      Chukong Technologies, Inc.
 *
 *         http://www.cocos2d-x.org      
 *         http://www.cocostudio.com
 *         http://tools.cocoachina.com
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

#ifndef __CSContentJsonDictionary_h__
#define __CSContentJsonDictionary_h__

#include "XMJson/json.h"
#include "../../../ccMacros.h"
#include <vector>
#include <string>

namespace cocostudio {

typedef enum _DicItemType
{
    EDIC_TYPENULL = 0,
    EDIC_TYPEINT,
    EDIC_TYPEUINT,
    EDIC_TYPEFLOAT,
    EDIC_TYPESTRING,
    EDIC_TYPEBOOLEN,
    EDIC_TYPEARRAY,
    EDIC_TYPEOBJECT
} DicItemType;

class JsonDictionary
{
    public :

        JsonDictionary ( KDvoid );
        virtual ~JsonDictionary ( KDvoid );

    public :

        KDvoid						initWithDescription		( const KDchar* pszDescription );

        KDvoid						insertItem				( const KDchar* pszKey, KDint nValue );
        KDvoid						insertItem				( const KDchar* pszKey, KDdouble fValue );
        KDvoid						insertItem				( const KDchar* pszKey, const KDchar* pszValue );
        KDvoid						insertItem				( const KDchar* pszKey, JsonDictionary* pSubDictionary );
        KDvoid						insertItem				( const KDchar* pszKey, KDbool bValue);
        KDbool						deleteItem				( const KDchar* pszKey );
        
		KDvoid						cleanUp					( KDvoid );

        KDbool						isKeyValidate			( const KDchar* pszKey);

        KDint						getItemIntValue			( const KDchar* pszKey, KDint    nDefaultValue );
        KDdouble					getItemFloatValue		( const KDchar* pszKey, KDdouble fDefaultValue );
        const KDchar*				getItemStringValue		( const KDchar* pszKey );
        KDbool						getItemBoolvalue		( const KDchar* pszKey, KDbool   bDefaultValue );
        JsonDictionary*				getSubDictionary		( const KDchar* pszKey );

        std::string					getDescription();

        KDbool						insertItemToArray		( const KDchar* pszArrayKey, KDint nValue );
        KDbool						insertItemToArray		( const KDchar* pszArrayKey, KDdouble fValue );
        KDbool						insertItemToArray		( const KDchar* pszArrayKey, const KDchar* pszValue );
        KDbool						insertItemToArray		( const KDchar* pszArrayKey, JsonDictionary* pSubDictionary );

        KDint						getArrayItemCount		( const KDchar* pszArrayKey );
        KDint						getIntValueFromArray	( const KDchar* pszArrayKey, KDint nIndex, KDint    nDefaultValue );
        KDdouble					getFloatValueFromArray	( const KDchar* pszArrayKey, KDint nIndex, KDdouble fDefaultValue );
        KDbool						getBoolValueFromArray	( const KDchar* pszArrayKey, KDint nIndex, KDbool   bDefaultValue );
        const KDchar*				getStringValueFromArray ( const KDchar* pszArrayKey, KDint nIndex );
        JsonDictionary*				getSubItemFromArray		( const KDchar* pszArrayKey, KDint nIndex );
        DicItemType					getItemTypeFromArray	( const KDchar* pszArrayKey, KDint nIndex );

        KDint						getItemCount			( KDvoid );
        DicItemType					getItemType				( KDint nIndex );
        DicItemType					getItemType				( const KDchar* pszKey );
        std::vector<std::string>	getAllMemberNames		( KDvoid );

    protected :

        Json::Value					m_cValue;

    private :

        KDvoid						initWithValue			( Json::Value& tValue );
        inline KDbool				isKeyValidate			( const KDchar* pszKey, Json::Value& tRoot );
        inline Json::Value*			validateArrayItem		( const KDchar* pszArrayKey, KDint nIndex );
};

}

#endif	// __CSContentJsonDictionary_h__
