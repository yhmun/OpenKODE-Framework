/* --------------------------------------------------------------------------
 *
 *      File            APSNumber.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com  
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
 *      version 2 of the License, or ( at your option ) any later version.
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
#include "extensions/CCArtPig/APSNumber.h"

NS_APS_BEGIN

const APSNumberData  APSZeroNumberData = { 0 };

#define RETURN_DATA_TYPE( _RTYPE )													\
	switch ( this->m_nType )														\
	{																				\
        case kAPSNumberType_boolean		: return (_RTYPE) this->m_tData.boolean;	\
        case kAPSNumberType_integer		: return (_RTYPE) this->m_tData.integer;	\
        case kAPSNumberType_uInteger	: return (_RTYPE) this->m_tData.uInteger;	\
        case kAPSNumberType_lInteger	: return (_RTYPE) this->m_tData.lInteger;	\
        case kAPSNumberType_ulInteger	: return (_RTYPE) this->m_tData.ulInteger;	\
        case kAPSNumberType_real		: return (_RTYPE) this->m_tData.real;		\
        case kAPSNumberType_lReal		: return (_RTYPE) this->m_tData.lReal;		\
        default							: return (_RTYPE) 0;						\
    }

APSNumber::APSNumber ( KDint nType, APSNumberData tData ) 
{
	m_nType = nType;
	m_tData = tData;
}

APSNumber* APSNumber::numberWithBoolean ( KDbool boolean )
{
    APSNumberData  tData;
    tData.boolean = boolean;
    return (APSNumber*) ( new APSNumber ( kAPSNumberType_boolean, tData ) )->autorelease ( );
}

APSNumber* APSNumber::numberWithInteger ( KDint integer )
{
    APSNumberData  tData;
    tData.integer = integer;
    return (APSNumber*) ( new APSNumber ( kAPSNumberType_integer, tData ) )->autorelease ( );
}

APSNumber* APSNumber::numberWithUnsignedInteger ( KDuint uInteger )
{
    APSNumberData  tData;
    tData.uInteger = uInteger;
    return (APSNumber*) ( new APSNumber ( kAPSNumberType_uInteger, tData ) )->autorelease ( );
}

APSNumber* APSNumber::numberWithLong ( KDint32 lInteger )
{
    APSNumberData  tData;
    tData.lInteger = lInteger;
    return (APSNumber*) ( new APSNumber ( kAPSNumberType_lInteger, tData ) )->autorelease ( );
}

APSNumber* APSNumber::numberWithUnsignedLong ( KDuint32 ulInteger )
{
    APSNumberData  tData;
    tData.ulInteger = ulInteger;
    return (APSNumber*) ( new APSNumber ( kAPSNumberType_ulInteger, tData ) )->autorelease ( );
}

APSNumber* APSNumber::numberWithFloat ( KDfloat real )
{
    APSNumberData  tData;
    tData.real = real;
    return (APSNumber*) ( new APSNumber ( kAPSNumberType_real, tData ) )->autorelease ( );
}

APSNumber* APSNumber::numberWithDouble ( KDdouble lReal )
{
    APSNumberData  tData;
    tData.lReal = lReal;
    return (APSNumber*) ( new APSNumber ( kAPSNumberType_lReal, tData ) )->autorelease ( );
}


APSNumber* APSNumber::numberWithBooleanString ( const std::string& sStr )
{
	static const std::string  sTrueString [ ] = { "true", "TRUE", "yes", "YES" };
    
    KDbool bFoundTRUE = KD_FALSE;
    
    for ( KDint i = 0; i < 4; i++ )
	{
        if ( sTrueString [ i ].compare ( sStr ) == 0 )
		{
            bFoundTRUE = KD_TRUE;
            break;
        }
    }
    
    return APSNumber::numberWithBoolean ( bFoundTRUE );
}


// APSIndex has 0 as the minimum value, and APS_NIL_INDEX is UINT_MAX value. When an index is stored in property list form, 1 becomes the minimum index value, and 0 represents nil index. 
APSNumber* APSNumber::numberWithIndex ( APSIndex uIndex ) 
{ 
    return APSNumber::numberWithUnsignedInteger ( APS_NIL_INDEX == uIndex ? 0 : uIndex + 1 );
}

APSIndex APSNumber::indexWithNumber ( APSNumber* pNumber )
{
    APSIndex  uIndex =  pNumber->getUnsignedInteger ( );
    return uIndex == 0 ? APS_NIL_INDEX : uIndex - 1;
}

KDuint APSNumber::integerFromIndex ( APSIndex uIndex )
{
    return APS_NIL_INDEX == uIndex? 0 : uIndex + 1;
}

APSIndex APSNumber::indexFromInteger ( KDuint uVal )
{
    return uVal == 0 ? APS_NIL_INDEX : uVal - 1;
}

KDbool APSNumber::isKindOfBoolean ( KDvoid ) const 
{
    return this->m_nType == kAPSNumberType_boolean;
}

KDbool APSNumber::isKindOfInteger ( KDvoid ) const 
{
    switch ( this->m_nType )
	{
        case kAPSNumberType_integer   :
        case kAPSNumberType_uInteger  :
        case kAPSNumberType_lInteger  :
        case kAPSNumberType_ulInteger :
            return KD_TRUE;
    }
    return KD_FALSE;
}

KDbool APSNumber::isKindOfReal ( KDvoid ) const
{
    switch ( this->m_nType ) 
	{
        case kAPSNumberType_real  :
        case kAPSNumberType_lReal :
            return KD_TRUE;
    }
    return KD_FALSE;
    
}

KDbool APSNumber::getBoolean ( KDvoid ) const 
{
	return (KDbool) this->m_tData.boolean;
}

KDint APSNumber::getInteger ( KDvoid ) const
{
	return (KDint) this->m_tData.integer;
}

KDuint APSNumber::getUnsignedInteger ( KDvoid ) const
{
	return (KDuint) this->m_tData.uInteger;
}

KDint32 APSNumber::getLong ( KDvoid ) const
{
	return (KDint32) this->m_tData.lInteger;
}

KDuint32 APSNumber::getUnsignedLong ( KDvoid ) const
{
	return (KDuint32) this->m_tData.ulInteger;
}

KDfloat APSNumber::getFloat ( KDvoid ) const
{
	return (KDfloat) this->m_tData.real;
}

KDdouble APSNumber::getDouble ( KDvoid ) const
{
	return (KDdouble) this->m_tData.lReal;
}

std::string APSNumber::getString ( KDvoid ) const
{
	KDchar  szRet[256] = "";

    switch ( this->m_nType )
	{
        case kAPSNumberType_boolean :
			kdSprintf ( szRet, "%s", this->m_tData.boolean ? "true" : "false" );
            break;

        case kAPSNumberType_integer :
			kdSprintf ( szRet, "%d", this->m_tData.integer );		
            break;

        case kAPSNumberType_uInteger :
			kdSprintf ( szRet, "%u", this->m_tData.uInteger );
            break;

        case kAPSNumberType_lInteger :
			kdSprintf ( szRet, "%d", this->m_tData.lInteger );
            break;

        case kAPSNumberType_ulInteger :
			kdSprintf ( szRet, "%u", this->m_tData.ulInteger );
            break;

        case kAPSNumberType_real :
			kdSprintf ( szRet, "%f", this->m_tData.real );
            break;

        case kAPSNumberType_lReal :
			kdSprintf ( szRet, "%f", this->m_tData.lReal );
            break;

        default :
            break;
    }
    
    return std::string ( szRet );
}

NS_APS_END





