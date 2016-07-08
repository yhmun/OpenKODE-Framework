/* -----------------------------------------------------------------------------------
 *
 *      File            CCDataVisitor.cpp
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
#include "cocoa/CCObject.h"
#include "cocoa/CCBool.h"
#include "cocoa/CCInteger.h"
#include "cocoa/CCFloat.h"
#include "cocoa/CCDouble.h"
#include "cocoa/CCString.h"
#include "cocoa/CCArray.h"
#include "cocoa/CCDictionary.h"
#include "cocoa/CCSet.h"

NS_CC_BEGIN

KDvoid CCDataVisitor::visit ( const CCBool* pValue )
{
    visitObject ( pValue );
}

KDvoid CCDataVisitor::visit ( const CCInteger* pValue )
{
    visitObject ( pValue );
}

KDvoid CCDataVisitor::visit ( const CCFloat* pValue )
{
    visitObject ( pValue );
}

KDvoid CCDataVisitor::visit ( const CCDouble* pValue )
{
    visitObject ( pValue );
}

KDvoid CCDataVisitor::visit ( const CCString* pValue )
{
    visitObject ( pValue );
}

KDvoid CCDataVisitor::visit ( const CCArray* pValue )
{
    visitObject ( pValue );
}

KDvoid CCDataVisitor::visit ( const CCDictionary* pValue )
{
    visitObject ( pValue );
}

KDvoid CCDataVisitor::visit ( const CCSet* pValue )
{
    visitObject ( pValue );
}

// CCPrettyPrinter
CCPrettyPrinter::CCPrettyPrinter ( KDint nIndentLevel )
{
    setIndentLevel ( nIndentLevel );
}

KDvoid CCPrettyPrinter::clear ( KDvoid )
{
    m_sResult.clear ( );
}

std::string CCPrettyPrinter::getResult ( KDvoid )
{
    return m_sResult;
}

KDvoid CCPrettyPrinter::visitObject ( const CCObject* p )
{
    KDchar		aBuf [ 50 ] = { 0 };
    kdSprintf ( aBuf, "%p", p );
    m_sResult += aBuf;
}

KDvoid CCPrettyPrinter::visit ( const CCBool* p )
{
    KDchar		aBuf [ 50 ] = { 0 };
    kdSprintf ( aBuf, "%s", p->getValue ( ) ? "true" : "false" );
	m_sResult += aBuf;
}

KDvoid CCPrettyPrinter::visit ( const CCInteger* p )
{
    KDchar		aBuf [ 50 ] = { 0 };
    kdSprintf ( aBuf, "%d", p->getValue ( ) );
    m_sResult += aBuf;
}

KDvoid CCPrettyPrinter::visit ( const CCFloat* p )
{
    KDchar		aBuf [ 50 ] = { 0 };
    kdSprintf ( aBuf, "%f", p->getValue ( ) );
    m_sResult += aBuf;
}

KDvoid CCPrettyPrinter::visit ( const CCDouble* p )
{
    KDchar		aBuf [ 50 ] = { 0 };
    kdSprintf ( aBuf, "%lf", p->getValue ( ) );
    m_sResult += aBuf;
}

KDvoid CCPrettyPrinter::visit ( const CCString* p )
{
    m_sResult += p->getCString ( );
}

KDvoid CCPrettyPrinter::visit ( const CCArray* p )
{
    m_sResult += "\n";
    m_sResult += m_sIndentStr;
    m_sResult += "<array>\n";

    setIndentLevel ( m_nIndentLevel + 1 );

    CCObject*	pObj;
    KDint		i = 0;
    KDchar		aBuf [ 50 ] = { 0 };
    CCARRAY_FOREACH ( p, pObj )
    {
        if ( i > 0 )
		{
            m_sResult += "\n";
        }
        kdSprintf ( aBuf, "%s%02d: ", m_sIndentStr.c_str ( ), i );
        m_sResult += aBuf;

        CCPrettyPrinter		v ( m_nIndentLevel );
        pObj->acceptVisitor ( v );
        m_sResult += v.getResult ( );
        i++;
    }
    setIndentLevel ( m_nIndentLevel - 1 );
    
    m_sResult += "\n";
    m_sResult += m_sIndentStr;
    m_sResult += "</array>";
}

KDvoid CCPrettyPrinter::visit ( const CCDictionary* p )
{
    m_sResult += "\n";
    m_sResult += m_sIndentStr;
    m_sResult += "<dict>\n";
    
    setIndentLevel ( m_nIndentLevel + 1 );
    CCDictElement*		pElement;
    KDbool	bFirstElement = KD_TRUE;
    KDchar	aBuf [ 1000 ] = { 0 };
    CCDICT_FOREACH ( p, pElement )
    {
        if ( !bFirstElement )
		{
            m_sResult += "\n";
        }
        kdSprintf ( aBuf, "%s%s: ", m_sIndentStr.c_str ( ), pElement->getStrKey ( ) );
        m_sResult += aBuf;

        CCPrettyPrinter		v ( m_nIndentLevel );
        pElement->getObject ( )->acceptVisitor ( v );
        m_sResult += v.getResult ( );
        bFirstElement = KD_FALSE;
    }
    setIndentLevel ( m_nIndentLevel - 1 );
    
    m_sResult += "\n";
    m_sResult += m_sIndentStr;
    m_sResult += "</dict>";
}

KDvoid CCPrettyPrinter::visit ( const CCSet* p )
{
    m_sResult += "\n";
    m_sResult += m_sIndentStr;
    m_sResult += "<set>\n";
    
    setIndentLevel ( m_nIndentLevel + 1 );

    KDint			i = 0;
    CCSet*			tmp = const_cast<CCSet*> ( p );
    CCSetIterator	it = tmp->begin ( );

    for ( ; it != tmp->end(); ++it, ++i )
	{
        if ( i > 0 )
		{
            m_sResult += "\n";
        }
        m_sResult += m_sIndentStr.c_str ( );

        CCPrettyPrinter		v ( m_nIndentLevel );
        ( *it )->acceptVisitor ( v );
        m_sResult += v.getResult ( );
    }
    setIndentLevel ( m_nIndentLevel - 1 );
    
    m_sResult += "\n";
    m_sResult += m_sIndentStr;
    m_sResult += "</set>\n";
}

KDvoid CCPrettyPrinter::setIndentLevel ( KDint nIndentLevel )
{
    m_nIndentLevel = nIndentLevel;
    m_sIndentStr.clear ( );
    for ( KDint i = 0; i < m_nIndentLevel; ++i )
	{
        m_sIndentStr += "\t";
    }
}

NS_CC_END
