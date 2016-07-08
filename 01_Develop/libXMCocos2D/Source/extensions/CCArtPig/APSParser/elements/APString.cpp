/* --------------------------------------------------------------------------
 *
 *      File            APSString.cpp
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
#include "extensions/CCArtPig/APSString.h"
#include "extensions/CCArtPig/APSNumber.h"
#include "extensions/CCArtPig/APSArray.h"
#include "extensions/CCArtPig/APSResourceArray.h"

NS_APS_BEGIN

APSString::APSString ( KDvoid )
{

}

APSString::APSString ( const APSString* pStr )
{
    this->m_sString = pStr->getString ( );
}

APSString::APSString ( const std::string& sStr )
{
    this->m_sString = sStr;
}

APSString::APSString ( const APSNumber* pNumber )
{
    this->m_sString = pNumber->getString ( );
}

APSString::APSString ( const ccColor4B& tColor )
{
    KDubyte  aFields [ ] = { tColor.r, tColor.g, tColor.b, tColor.a };
    KDchar   szBuff  [ 4 ];

    this->m_sString = "";

    for ( KDint i = 0; i < 4; i++ )
	{
        if ( aFields [ i ] < 10 )
		{
            kdSprintf ( szBuff, "00%d", aFields [ i ] );
        }
		else if ( aFields [ i ] < 100 )
		{
            kdSprintf ( szBuff, "0%d", aFields [ i ] );
        }
		else 
		{
            kdSprintf ( szBuff, "%d", aFields [ i ] );
        }

        this->m_sString += szBuff;
    }
}

APSString::APSString ( const CCPoint& tPoint )
{
	KDchar  szRet [256] = "";
	kdSprintf ( szRet, "%f,%f", tPoint.x, tPoint.y );
    this->m_sString = szRet;
}

APSString::APSString ( const CCSize& tSize )
{
	KDchar  szRet [256] = "";
	kdSprintf ( szRet, "%f,%f", tSize.cx, tSize.cy );
    this->m_sString = szRet;
}

APSString::APSString ( const CCRect& tRect )
{
	KDchar  szRet [256] = "";
	kdSprintf ( szRet, "%f,%f,%f,%f", tRect.origin.x, tRect.origin.y, tRect.size.cx, tRect.size.cy );
    this->m_sString = szRet;
}

APSString* APSString::stringWithString ( const APSString* pStr )
{
    return (APSString*) ( new APSString ( pStr ) )->autorelease ( );
}

APSString* APSString::stringWithString ( const std::string& sStr )
{
    return (APSString*) ( new APSString ( sStr ) )->autorelease ( );
}

APSString* APSString::stringWithNumber ( const APSNumber* pNumber )
{
    return (APSString*) ( new APSString ( pNumber ) )->autorelease ( );
}

APSString* APSString::stringWithColor ( const ccColor4B& tColor )
{
    return (APSString*) ( new APSString ( tColor ) )->autorelease ( );
}

APSString* APSString::stringWithPoint ( const CCPoint& tPoint ) 
{
    return (APSString*) ( new APSString ( tPoint ) )->autorelease ( );
}

APSString* APSString::stringWithSize ( const CCSize& tSize )
{
    return (APSString*) ( new APSString ( tSize ) )->autorelease ( );
}

APSString* APSString::stringWithRect ( const CCRect& tRect )
{
    return (APSString*) ( new APSString ( tRect ) )->autorelease ( );
}

// Sets APSPathFields of file's location ,filename, and extension.
KDvoid APSString::getPathFields ( const std::string& sPath, APSPathFields* pPathFields )
{
    if ( pPathFields )
	{
		std::string  sFileName;
        
        // separate path with location and filename
        KDsize  uFound = sPath.rfind ( "/" );
        if ( uFound < sPath.size ( ) - 1 )
		{
            if ( uFound > 0 )
			{
                pPathFields->sLocation = sPath.substr ( 0, uFound );
            }
            sFileName = sPath.substr ( uFound + 1 );
        }
		else 
		{
            // entire string is a filename
            sFileName = sPath;
        }
        
        // Separate filename with name and extension
        uFound = sFileName.find_last_of ( '.' );
        if ( uFound > 0 && uFound < sFileName.size ( ) - 1 )
		{
            pPathFields->sName = sFileName.substr ( 0, uFound );
            pPathFields->sExtension = sFileName.substr ( uFound + 1 );
        }
		else 
		{
            pPathFields->sName = sFileName;
        }
    }
}

KDbool APSString::hasSuffix ( const KDchar* szStr, const KDchar* szSuffix )
{

    KDsize  uStrLen  = kdStrlen ( szStr );
    KDsize  uSuffLen = kdStrlen ( szSuffix );
    
    if ( uStrLen < uSuffLen )
	{
        return KD_FALSE;
    }
    
    return !kdStrcmp ( szStr + uStrLen - uSuffLen, szSuffix );
}

const KDchar* APSString::getIntsFromString ( const KDchar* szStr, KDchar cDel, KDint* pValues, KDint nMaxNValues, KDint* nValues )
{
    KDchar  aBuff[16];
    KDint   nIndex = 0;

    const KDchar*  pPtr = szStr;
    while ( *pPtr && nIndex < nMaxNValues ) 
	{
        const KDchar* pTv = pPtr;
        KDint nSize = 0;
        while ( *pTv && *pTv != cDel ) 
		{
            aBuff [ nSize++ ] = *pTv;;
            pTv++;
        }
        aBuff [ nSize ] = 0;
        pValues [ nIndex++ ] = kdAtoi ( aBuff );
        
        if ( !*pTv )
		{
            pPtr = pTv;
            break;
        }
		else 
		{
            pPtr = pTv + 1;
        }
    }

    *nValues = nIndex;
    return pPtr;
}

const KDchar* APSString::getFloatsFromString ( const KDchar* szStr, KDchar cDel, KDfloat* pValues, KDint nMaxNValues, KDint* nValues )
{
    KDchar  aBuff[16];
    KDint   nIndex = 0;

    const KDchar* pPtr = szStr;
    while ( *pPtr && nIndex < nMaxNValues )
	{
        const KDchar*  pTv = pPtr;
        KDint  nSize = 0;
        while ( *pTv && *pTv != cDel )
		{
            aBuff [ nSize++ ] = *pTv;
            pTv++;
        }
        aBuff [ nSize ] = 0;
        pValues [ nIndex++ ] = kdAtof ( aBuff );
        
        if ( !*pTv )
		{
            pPtr = pTv;
            break;
        }
		else 
		{
            pPtr = pTv + 1;
        }
    }

    *nValues = nIndex;
    return pPtr;
}

KDbool APSString::isEqualToString ( APSString* pStr ) const
{
    if ( kdStrcmp ( this->m_sString.c_str ( ), pStr->c_str ( ) ) == 0 )
	{
        return KD_TRUE;
    }
    return KD_FALSE;
}

const std::string& APSString::getString ( KDvoid ) const
{
    return this->m_sString;
}

const KDchar* APSString::c_str ( KDvoid ) const 
{
    return this->m_sString.c_str ( );
}

KDvoid APSString::getInts ( std::vector<KDint>* pList ) const 
{
    KDint   aValues[12];
    KDint   nSize;

    const KDchar*  pPtr = m_sString.c_str ( );
    
    while ( *pPtr )
	{
        pPtr = getIntsFromString ( pPtr, ',', aValues, 12, &nSize );

        pList->reserve ( pList->size ( ) + nSize );

        for ( KDint i = 0; i < nSize; i++ )
		{
            pList->push_back ( aValues [ i ] );
        }
    }
}

KDvoid APSString::getFloats(std::vector<KDfloat>* pList) const 
{
    KDfloat  aValues[12];
    KDint    nSize;

    const KDchar*  pPtr = m_sString.c_str ( );
    
    while ( *pPtr )
	{
        pPtr = getFloatsFromString ( pPtr, ',', aValues, 12, &nSize );
        
        pList->reserve ( pList->size ( ) + nSize );
        
        for ( KDint i = 0; i < nSize; i++ )
		{
            pList->push_back ( aValues [ i ] );
        }
    }
}

KDvoid APSString::getIndexes ( std::vector<APSIndex>* pList ) const
{
	this->getInts ( (std::vector<KDint>*) pList );
}

KDvoid APSString::getResourceArray ( APSResourceArray* pList ) const
{
    KDint  aValues[12];
    KDint  nSize;

    const KDchar*  pPtr = m_sString.c_str ( );    
    while ( *pPtr ) 
	{
        pPtr = getIntsFromString ( pPtr, ',', aValues, 12, &nSize );
        
        pList->reserve ( pList->size ( ) + nSize );
        
        for ( KDint i = 0; i < nSize; i++ )
		{
            pList->pushBack ( APSNumber::indexFromInteger ( aValues [ i ] ) );
        }
    }
}

ccColor4B APSString::getColorValue ( KDvoid ) const 
{
    const KDchar*  szStr = m_sString.c_str ( );
    KDint   nStrLength   = kdStrlen ( szStr );
    KDint   aFields [ ]  = { 0, 0, 0, 255 };
    KDchar  aBuff   [ ]  = { 0, 0, 0, 0 };
    KDint   nFieldLength = sizeof ( KDchar ) * 3;
    
    for ( KDint i = 0; i < 4; i++ )
	{
        if ( i * 3 + 3 <= nStrLength )
		{
            kdMemcpy ( aBuff, szStr + i * 3, nFieldLength );
            aFields [ i ] = kdAtoi ( aBuff );
        }
    }
    
    return ccc4 ( aFields [0], aFields [1], aFields [2], aFields [3] );
}

CCPoint APSString::getPointValue ( KDvoid ) const 
{
    KDfloat  aVal [2];
    KDint    nSize;
    getFloatsFromString ( m_sString.c_str ( ), ',', aVal, 2, &nSize );

    if ( nSize < 2 )
	{
        return CCPointZero;
    }
    return CCPointMake ( aVal [0], aVal [1] );
}

CCSize APSString::getSizeValue ( KDvoid ) const
{
    CCPoint  tP = this->getPointValue ( );
    return CCSizeMake ( tP.x, tP.y );
}

CCRect APSString::getRectValue ( KDvoid ) const
{
    KDfloat  aVal[4];
    KDint    nSize;
    getFloatsFromString ( m_sString.c_str(), ',', aVal, 4, &nSize );
    
    if ( nSize < 4 )
	{
        return CCRectZero;
    }
	else 
	{
        return CCRect ( aVal [0], aVal [1], aVal [2], aVal [3] );
    }
}

NS_APS_END





