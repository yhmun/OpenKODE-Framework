/* -----------------------------------------------------------------------------------
 *
 *      File            CCComponent.cpp
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
#include "support/component/CCComponent.h"
#include "support/CCDirector.h"


NS_CC_BEGIN

CCComponent::CCComponent ( KDvoid )
{
	m_pOwner = KD_NULL;
	m_bEnabled = KD_TRUE;
}

CCComponent::~CCComponent ( KDvoid )
{

}

KDbool CCComponent::init ( KDvoid )
{
    return KD_TRUE;
}

KDvoid CCComponent::onEnter ( KDvoid )
{

}

KDvoid CCComponent::onExit ( KDvoid )
{

}

KDvoid CCComponent::update ( KDfloat fDelta )
{

}

KDvoid CCComponent::serialize ( KDvoid* ar )
{

}

CCComponent* CCComponent::create ( KDvoid )
{
    CCComponent*	pRet = new CCComponent ( );

    if ( pRet != KD_NULL && pRet->init ( ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

const KDchar* CCComponent::getName ( KDvoid ) const
{
    return m_sName.c_str ( );
}

KDvoid CCComponent::setName ( const KDchar* szName )
{
	m_sName.assign ( szName );
}

CCNode* CCComponent::getOwner ( KDvoid ) const
{
    return m_pOwner;
}

KDvoid CCComponent::setOwner ( CCNode* pOwner )
{
    m_pOwner = pOwner;
}

KDbool CCComponent::isEnabled ( KDvoid ) const
{
    return m_bEnabled;
}

KDvoid CCComponent::setEnabled ( KDbool bEnable )
{
    m_bEnabled = bEnable;
}

CCNode* CCComponent::getNode ( KDvoid )
{
	return KD_NULL;
}

KDvoid CCComponent::setNode ( CCNode* pNode )
{
	CC_UNUSED_PARAM ( pNode );
}

NS_CC_END
