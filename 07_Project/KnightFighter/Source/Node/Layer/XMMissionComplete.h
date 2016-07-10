/* --------------------------------------------------------------------------
 *
 *      File            XMMissionComplete.h
 *      Description     XMMissionComplete
 *      Author          Pill-Gwan Jeon
 *      Contact         jpg3087@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved.
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

#ifndef __XMMissionComplete_h__
#define __XMMissionComplete_h__


class XMMissionComplete : public LayerColor
{
	public :

							 XMMissionComplete ( );
		virtual				~XMMissionComplete ( );

							CREATE_FUNC ( XMMissionComplete );

	private :

		KDbool						init ( );

		KDvoid						setTouchEnable		( KDvoid );

		KDvoid						makeRestart			( KDvoid );
		KDvoid						makeConfirm			( KDvoid );
		KDvoid						makeGoNextStage		( KDvoid );
		
		KDbool						onTouchBegan		( Touch* pTouch, Event* pEvent );
		KDvoid						onTouchMoved		( Touch* pTouch, Event* pEvent );
		KDvoid						onTouchEnded		( Touch* pTouch, Event* pEvent );
		KDvoid						onTouchCancelled	( Touch* pTouch, Event* pEvent );

	private :
		
		EventListenerTouchOneByOne*			m_pEventListener;
		Sprite*								m_pUiBg;
		std::vector<Sprite*>				m_aUiSprite;

};

#endif  // __XMMissionComplete_h__