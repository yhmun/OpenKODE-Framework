/* -----------------------------------------------------------------------------------
 *
 *      File            TextureManage.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
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
#include "TextureManage.h"

std::vector<cTextureManage::cTexture> cTextureManage::m_aTextures;

cTextureManage::cTextureManage ( KDvoid )
{

}

cTextureManage::~cTextureManage ( KDvoid )
{

}

KDint cTextureManage::texture_alloc ( const KDchar* pFileName )
{
	// init
	KDint		nAllocUid = m_aTextures.size ( );

	// same check & create uid
	for ( KDint i = (KDint) m_aTextures.size ( )  - 1; i >= 0; i-- )
	{
		// same check
		if(  !m_aTextures [ i ].sFileName.compare ( pFileName ) )
		{
			++m_aTextures [ i ].nRefCount;
			return i;
		}

		// get create uid
		if ( m_aTextures [ i ].pSprite == nullptr )
		{
			nAllocUid = i;
		}
	}

	// init texture class
	if ( nAllocUid < (KDint) m_aTextures.size ( ) )
	{
		m_aTextures [ nAllocUid ].nRefCount = 1;
		m_aTextures [ nAllocUid ].sFileName = pFileName;
		m_aTextures [ nAllocUid ].pSprite   = Sprite::create ( pFileName );
		CC_SAFE_RETAIN ( m_aTextures [ nAllocUid ].pSprite );
	}
	else
	{
		cTexture	tTex;
		tTex.nRefCount	= 1;
		tTex.sFileName	= pFileName;
		tTex.pSprite	= Sprite::create ( pFileName );
		CC_SAFE_RETAIN ( tTex.pSprite );
		m_aTextures.push_back ( tTex );
	}

	return nAllocUid;
}

KDvoid cTextureManage::texture_release ( KDint uid )
{
	CCAssert ( uid >= 0 && uid < (KDint) m_aTextures.size ( ) && m_aTextures [ uid ].pSprite != nullptr, "" );

	// release
	--m_aTextures [ uid ].nRefCount;

	// reference count
	if ( m_aTextures [ uid ].nRefCount == 0 )
	{
		// release
		CC_SAFE_RELEASE ( m_aTextures [ uid ].pSprite );

		m_aTextures [ uid ].sFileName.clear ( ); 
	}
}

Sprite* cTextureManage::getTexture ( KDint uid )
{
	CCAssert ( uid >= 0 && uid < (KDint) m_aTextures.size ( ), "" );

	return m_aTextures [ uid ].pSprite;
}