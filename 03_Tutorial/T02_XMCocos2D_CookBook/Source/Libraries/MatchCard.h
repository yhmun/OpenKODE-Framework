/* --------------------------------------------------------------------------
 *
 *      File            MatchCard.h
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

#ifndef __MatchCard_h__
#define __MatchCard_h__

class MatchCard : public CCNode
{
	public :
				 MatchCard ( KDvoid );
		virtual ~MatchCard ( KDvoid );

		static   MatchCard*		createWithSpriteFrameName ( const KDchar* szSpriteFrameName );

	public :		

		virtual KDbool			initWithSpriteFrameName ( const KDchar* szSpriteFrameName );

		virtual CCRect			rect ( KDvoid );

		virtual KDvoid			setAllPositions ( const CCPoint& tPosition );

		virtual KDvoid			flipCard ( KDvoid );

		virtual KDvoid			flipCardNoAnim ( KDvoid );

		virtual KDvoid			processTouch ( const CCPoint& tPoint );

		virtual KDbool			 isCardUp ( KDvoid );
		virtual KDvoid			setCardUp ( KDbool bIsUp );

		virtual CCSprite*		getCardUp ( KDvoid );
		virtual KDvoid			setCardUp ( CCSprite* pCardUp );

		virtual CCSprite*		getCardDown ( KDvoid );
		virtual KDvoid			setCardDown ( CCSprite* pCardDown );

		virtual const KDchar*	getCardType ( KDvoid );
		virtual KDvoid			setCardType ( const KDchar* szCardType );

		virtual KDvoid			doneAnimating ( KDvoid );

	protected :

		CCSprite*				m_pCardUp;
		CCSprite*				m_pCardDown;
		KDbool					m_bCardIsUp;
		KDbool					m_bAnimating;
		KDbool					m_bHasBeenFlipped;
		std::string				m_sCardType;
};

#endif	// __MatchCard_h__
