/* --------------------------------------------------------------------------
 *
 *      File            GameMenuWindow.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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

#ifndef __GameMenuWindow_h__
#define __GameMenuWindow_h__

class GameMenuWindow : public CCNode
{
	public :

		static   GameMenuWindow*	create ( const KDchar* szTitle, const CCSize& tSize );

	public :		

		virtual KDbool				initWithTitle ( const KDchar* szTitle, const CCSize& tSize );

		virtual KDvoid				ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid				ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid				ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDvoid				minimize ( CCObject* pSender );

		virtual CCRect				titleBarRect ( KDvoid );
		
		virtual CCRect				rect ( KDvoid );

		virtual KDvoid				bringToFront ( KDvoid );

		virtual KDbool				 isOpen ( KDvoid );
		virtual KDvoid				setOpen ( KDbool bIsOpen );

		virtual const CCSize&		getSize ( KDvoid );
		virtual KDvoid				setSize ( const CCSize& tSize );

		virtual const KDchar*		getTitle ( KDvoid );
		virtual KDvoid				setTitle ( const KDchar* szTitle );

		virtual CCSprite*			getContent ( KDvoid );
		virtual KDvoid				setContent ( CCSprite* pContent );

		virtual CCSprite*			getBg ( KDvoid );
		virtual KDvoid				setBg ( CCSprite* pBg );

		virtual CCSprite*			getTitleBar ( KDvoid );
		virtual KDvoid				setTitleBar ( CCSprite* pTitleBar );

		virtual KDint				getTouchHash ( KDvoid );
		virtual KDvoid				setTouchHash ( KDint nTouchHash );

		virtual KDbool				 isTouched ( KDvoid );
		virtual KDvoid				setTouched ( KDbool bIsTouched );

	private :

		KDbool						m_bIsOpen;
		CCSize						m_tSize;
		std::string					m_sTitle;
		CCSprite*					m_pContent;
		CCSprite*					m_pBg;
		CCSprite*					m_pTitleBar;		
		KDint						m_nTouchHash;
		KDbool						m_bIsTouched;
		CCPoint						m_tTouchedPoint;
};

#endif	// __GameMenuWindow_h__
