/* --------------------------------------------------------------------------
 *
 *      File            CCBValue.cpp
 *      Author          Y.H Mun
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
 *      version 2 of the License, or (at your otPositionion) any later version.
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

#include "Precompiled.h"
#include "extensions/CCBReader/CCBValue.h"
#include "cocoa/CCString.h"

NS_CC_BEGIN

//
// Implementation of ccColor3BWapper
//

ccColor3BWapper* ccColor3BWapper::create ( const ccColor3B& tColor )
{
    ccColor3BWapper*  pRet = new ccColor3BWapper ( );

    if ( pRet )
    {
        pRet->m_tColor = tColor;
        
        pRet->autorelease ( );
    }
    
    return pRet;
}

const ccColor3B& ccColor3BWapper::getColor ( KDvoid ) const
{
    return m_tColor;
}

//
// Implementation of CCBValue
//

CCBValue* CCBValue::create ( KDint nValue )
{
    CCBValue*  pRet = new CCBValue ( );

    if ( pRet )
    {
        pRet->m_tValue.nValue = nValue;
        pRet->m_nType = kCCBIntValue;
        pRet->autorelease ( );
    }
    
    return pRet;
}

CCBValue* CCBValue::create ( KDfloat fValue )
{
    CCBValue*  pRet = new CCBValue ( );

    if ( pRet )
    {
        pRet->m_tValue.fValue = fValue;
        pRet->m_nType = kCCBFloatValue;
        pRet->autorelease ( );
    }
    
    return pRet;
}

CCBValue* CCBValue::create ( KDbool bValue )
{
    CCBValue*  pRet = new CCBValue ( );

    if ( pRet )
    {
        pRet->m_tValue.nValue = bValue ? 1 : 0;
        pRet->m_nType = kCCBBoolValue;
        pRet->autorelease ( );
    }
    
    return pRet;
}

CCBValue* CCBValue::create ( KDubyte cValue )
{
    CCBValue*  pRet = new CCBValue ( );

    if ( pRet )
    {
        pRet->m_tValue.nValue = cValue;
        pRet->m_nType = kCCBUnsignedCharValue;
        pRet->autorelease ( );
    }
    
    return pRet;
}

CCBValue* CCBValue::create ( const KDchar* szValue )
{
    CCBValue*  pRet = new CCBValue ( );

    if ( pRet )
    {
        pRet->m_sValue = szValue;
        pRet->m_nType = kCCBStringValue;
        pRet->autorelease();
    }
    
    return pRet;
}

CCBValue* CCBValue::create ( CCArray* pValue )
{
    CCBValue*  pRet = new CCBValue ( );

    if ( pRet )
    {
        pRet->m_aValue = pValue;
        pRet->m_nType = kCCBArrayValue;
        pRet->autorelease();
    }
    
    return pRet;
}

KDint CCBValue::getIntValue ( KDvoid )
{
    CCAssert ( m_nType == kCCBIntValue, "" );
    
    return m_tValue.nValue;
}

KDfloat CCBValue::getFloatValue ( KDvoid )
{
    CCAssert ( m_nType == kCCBFloatValue, "" );
    
    return m_tValue.fValue;
}

KDbool CCBValue::getBoolValue ( KDvoid )
{
    CCAssert ( m_nType == kCCBBoolValue, "" );
    
    return m_tValue.nValue == 1 ? KD_TRUE : KD_FALSE;
}

KDubyte CCBValue::getByteValue ( KDvoid )
{
    CCAssert ( m_nType = kCCBUnsignedCharValue, "" );
    
    return (KDubyte) ( m_tValue.nValue );
}

CCArray* CCBValue::getArrayValue ( KDvoid )
{
    CCAssert ( m_nType == kCCBArrayValue, "" );
    
    return m_aValue;
}

const KDchar* CCBValue::getStringValue ( KDvoid )
{
    CCAssert ( m_nType == kCCBStringValue, "" );
    
    return m_sValue.c_str ( );
}

KDint CCBValue::getType ( KDvoid )
{
    return m_nType;
}

NS_CC_END
