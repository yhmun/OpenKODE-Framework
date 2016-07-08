/* -----------------------------------------------------------------------------------
 *
 *      File            CCNS.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "cocoa/CCNS.h"
#include <vector>
#include <string>

NS_CC_BEGIN
    
typedef std::vector<std::string>  strArray;

// string toolkit
static KDvoid split ( std::string sString, const KDchar* szToken, strArray& vStrings )
{
	KDint  nEnd   = 0;
	KDint  nBegin = 0;

	while ( nEnd != -1 )
	{
		nEnd = sString.find ( szToken, nBegin );

		if ( nEnd == -1 )
		{
			vStrings.push_back ( sString.substr ( nBegin, sString.length ( ) - nBegin ) );
		}
		else
		{
			vStrings.push_back ( sString.substr ( nBegin, nEnd - nBegin ) );
		}

		nBegin = nEnd + kdStrlen ( szToken );
	}
}

// first, judge whether the form of the string like this: {x,y}
// if the form is right,the string will be split into the parameter strs;
// or the parameter strs will be empty.
// if the form is right return true,else return false.
static KDbool splitWithForm ( const KDchar* szString, strArray& vStrings )
{
	KDbool  bRet = KD_FALSE;

    do 
    {
        CC_BREAK_IF ( !szString );

        // string is empty
        std::string   sContent = szString;
        CC_BREAK_IF ( sContent.length ( ) == 0 );

        KDuint  nPosLeft  = sContent.find ( '{' );
        KDuint  nPosRight = sContent.find ( '}' );

        // don't have '{' and '}'
        CC_BREAK_IF ( nPosLeft == std::string::npos || nPosRight == std::string::npos );
        // '}' is before '{'
        CC_BREAK_IF ( nPosLeft > nPosRight );

        std::string   sPointStr = sContent.substr ( nPosLeft + 1, nPosRight - nPosLeft - 1 );
        // nothing between '{' and '}'
        CC_BREAK_IF ( sPointStr.length ( ) == 0 );

        KDuint  nPos1 = sPointStr.find ( '{' );
        KDuint  nPos2 = sPointStr.find ( '}' );
        // contain '{' or '}' 
        CC_BREAK_IF ( nPos1 != std::string::npos || nPos2 != std::string::npos );

        split ( sPointStr, ",", vStrings );
        if ( vStrings.size ( ) != 2 || vStrings [ 0 ].length ( ) == 0 || vStrings [ 1 ].length ( ) == 0 )
        {
            vStrings.clear ( );
            break;
        }

        bRet = KD_TRUE;

    } while ( 0 );

    return bRet;
}

// implement the functions
CCRect CCRectFromString ( const KDchar* szContent )
{
	CCRect  tRet = CCRectZero;

	do 
	{
		CC_BREAK_IF ( !szContent );
		std::string  sContent = szContent;
		
		// find the first '{' and the third '}'
		KDuint  nPosLeft  = sContent.find ( '{' );
		KDuint  nPosRight = sContent.find ( '}' );

		for ( KDint i = 1; i < 3; ++i )
		{
			if ( nPosRight == std::string::npos )
			{
				break;
			}

			nPosRight = sContent.find ( '}', nPosRight + 1 );
		}
		
		CC_BREAK_IF ( nPosLeft == std::string::npos || nPosRight == std::string::npos );
		
		sContent = sContent.substr ( nPosLeft + 1, nPosRight - nPosLeft - 1 );
		
		KDuint  nPointEnd = sContent.find ( '}' );
		CC_BREAK_IF ( nPointEnd == std::string::npos );
		nPointEnd = sContent.find ( ',', nPointEnd );
		CC_BREAK_IF ( nPointEnd == std::string::npos );
		
		// get the point string and size string
		std::string  sPointStr = sContent.substr ( 0, nPointEnd );
		std::string  sSizeStr  = sContent.substr ( nPointEnd + 1, sContent.length ( ) - nPointEnd );
		
		// split the string with ','
		strArray  vPointInfo;
		CC_BREAK_IF ( !splitWithForm ( sPointStr.c_str ( ), vPointInfo ) );

		strArray  vSizeInfo;
		CC_BREAK_IF ( !splitWithForm ( sSizeStr.c_str ( ), vSizeInfo ) );

		tRet = CCRectMake ( kdStrtodKHR ( vPointInfo [ 0 ].c_str ( ), KD_NULL ),
                            kdStrtodKHR ( vPointInfo [ 1 ].c_str ( ), KD_NULL ), 
                            kdStrtodKHR ( vSizeInfo  [ 0 ].c_str ( ), KD_NULL ), 
                            kdStrtodKHR ( vSizeInfo  [ 1 ].c_str ( ), KD_NULL ));

	} while ( 0 );
	
	return tRet;
}

CCPoint CCPointFromString ( const KDchar* szContent )
{
    strArray  vStrings;
    if ( splitWithForm ( szContent, vStrings ) )
    {
        return CCPointMake ( kdStrtodKHR ( vStrings [ 0 ].c_str ( ), KD_NULL ) ,
                             kdStrtodKHR ( vStrings [ 1 ].c_str ( ), KD_NULL ) );
    }
    else
    {
        return CCPointZero;
    }
}

CCSize CCSizeFromString ( const KDchar* szContent )
{
    strArray  vStrings;
    if ( splitWithForm ( szContent, vStrings ) )
    {
        return CCSizeMake ( kdStrtodKHR ( vStrings [ 0 ].c_str ( ), KD_NULL ) ,
                            kdStrtodKHR ( vStrings [ 1 ].c_str ( ), KD_NULL ) );
    }
    else
    {
        return CCSizeZero;
    }
}

NS_CC_END