/* --------------------------------------------------------------------------
 *
 *      File            TestRenderTexture.h
 *      Author          Young-Hwan Mun
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
 *      version 2 of the License, or (at your option) any later version.
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

#ifndef __TestRenderTexture_h__
#define __TestRenderTexture_h__

#include "../TestBasic.h"

class TestRenderTexture : public TestBasic
{
	public :

		static  TestRenderTexture*		create ( KDvoid );

	protected :		

		virtual KDuint			count ( KDvoid );
};

class RenderTextureSave : public TestRenderTexture
{
	public :

		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit	 ( KDvoid );

		virtual KDvoid			ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );		

		KDvoid					clearImage ( CCObject* pSender );
		KDvoid					saveImage  ( CCObject* pSender );

	protected :

		CCRenderTexture*		m_pTarget;
		CCSprite*				m_pBrush;
};

class RenderTextureZbuffer : public TestRenderTexture
{
	public :
		
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

		virtual KDvoid			onEnter  ( KDvoid );

		virtual KDvoid			ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );
		
		KDvoid					renderScreenShot ( KDvoid );

	private :
		
		CCSpriteBatchNode*		m_pMgr;
		CCSprite*				m_aSp[9];
};

class RenderTextureTestDepthStencil : public TestRenderTexture
{
	public :
		
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

		virtual KDvoid			onEnter  ( KDvoid );
};

class RenderTextureTargetNode : public TestRenderTexture
{
	public :
		
		virtual const KDchar*	title	 ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

		virtual KDvoid			onEnter  ( KDvoid );

		virtual KDvoid			update   ( KDfloat fDelta );

		KDvoid					touched  ( CCObject* pSender );

	private :

		CCSprite*				m_pSprite1;
		CCSprite*				m_pSprite2;
		CCRenderTexture*		m_pRenderTexture;
};

#endif	// __TestRenderTexture_h__
