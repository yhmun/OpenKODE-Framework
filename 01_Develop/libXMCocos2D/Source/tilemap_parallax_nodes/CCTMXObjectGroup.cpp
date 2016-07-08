/* -----------------------------------------------------------------------------------
 *
 *      File            CCTMXObjectGroup.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Neophit
 *      Copyright (c) 2010      Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
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
#include "tilemap_parallax_nodes/CCTMXObjectGroup.h"

NS_CC_BEGIN
    
//
// implementation CCTMXObjectGroup
//
CCTMXObjectGroup::CCTMXObjectGroup ( KDvoid )
{
    m_tPositionOffset = CCPointZero;
    m_sGroupName      = "";

	m_pObjects = CCArray::create ( );
    m_pObjects->retain ( );

	m_pProperties = new CCDictionary ( );
}

CCTMXObjectGroup::~CCTMXObjectGroup ( KDvoid )
{
	CCLOGINFO ( "XMCocos2D : deallocing CCTMXObjectGroup = %p", this );

	CC_SAFE_RELEASE ( m_pObjects );
	CC_SAFE_RELEASE ( m_pProperties );
}

CCDictionary* CCTMXObjectGroup::objectNamed ( const KDchar* szObjectName )
{
	if ( m_pObjects && m_pObjects->count ( ) > 0 )
	{
		CCObject*  pObject = KD_NULL;
        CCARRAY_FOREACH ( m_pObjects, pObject )
        {
            CCDictionary*  pDict = (CCDictionary*) pObject;
            CCString*      pName = (CCString*) pDict->objectForKey ( "name" );
            if ( pName && pName->m_sString == szObjectName )
            {
                return pDict;
            }
        }
	}

	// object not found
	return KD_NULL;	
}

CCString* CCTMXObjectGroup::propertyNamed ( const KDchar* szPropertyName )
{
	return (CCString*) m_pProperties->objectForKey ( szPropertyName );
}

CCDictionary* CCTMXObjectGroup::getProperties ( KDvoid )
{ 
	return m_pProperties;
}

KDvoid CCTMXObjectGroup::setProperties ( CCDictionary* pProperties )
{
	CC_SAFE_RETAIN ( pProperties );
	CC_SAFE_RELEASE ( m_pProperties );
	m_pProperties = pProperties;
}

CCArray* CCTMXObjectGroup::getObjects ( KDvoid )
{
	return m_pObjects;
}

KDvoid CCTMXObjectGroup::setObjects ( CCArray* pObjects )
{
	CC_SAFE_RETAIN ( pObjects );
	CC_SAFE_RELEASE ( m_pObjects );
	m_pObjects = pObjects;
}


NS_CC_END