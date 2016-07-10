/* --------------------------------------------------------------------------
 *
 *      File            XMMissionFailed.h
 *      Description     XMMissionFailed
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

#ifndef __XMMissionFailed_h__
#define __XMMissionFailed_h__


class XMMissionFailed : public LayerColor
{
	public :

							 XMMissionFailed ( );
		virtual				~XMMissionFailed ( );

							CREATE_FUNC ( XMMissionFailed );

	private :

		KDbool						init ( );

		KDvoid						setTouchEnable		( KDvoid );

		KDvoid						makeRestart			( KDvoid );
		KDvoid						makeConfirm			( KDvoid );
		KDvoid						makeGoPrivStage		( KDvoid );
		
		KDbool						onTouchBegan		( Touch* pTouch, Event* pEvent );
		KDvoid						onTouchMoved		( Touch* pTouch, Event* pEvent );
		KDvoid						onTouchEnded		( Touch* pTouch, Event* pEvent );
		KDvoid						onTouchCancelled	( Touch* pTouch, Event* pEvent );

	private :

		EventListenerTouchOneByOne*			m_pEventListener;
		Sprite*								m_pUiBg;
		std::vector<Sprite*>				m_aUiSprite;

};

#endif  // __XMMissionFailed_h__