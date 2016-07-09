/* --------------------------------------------------------------------------
 *
 *      File            SimpleCharacter.h
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

#ifndef __SimpleCharacter_h__
#define __SimpleCharacter_h__

class SimpleCharacter : public CCObject
{
	public :

		static SimpleCharacter*		create ( KDvoid );

	public :		

		virtual KDbool			init ( KDvoid );

		virtual KDvoid			addCharacterToNode ( CCNode* pNode, const CCPoint& tPosition );

		virtual const KDchar*	getCharColor ( KDvoid );
		virtual KDvoid			setCharColor ( const KDchar* szText );

		virtual const KDchar*	getCharClass ( KDvoid );
		virtual KDvoid			setCharClass ( const KDchar* szText );

		virtual KDint			getStrength ( KDvoid );
		virtual KDvoid			setStrength ( KDint nValue );

		virtual KDint			getDexterity ( KDvoid );
		virtual KDvoid			setDexterity ( KDint nValue );

		virtual KDint			getConstitution ( KDvoid );
		virtual KDvoid			setConstitution ( KDint nValue );

		virtual KDint			getIntelligence ( KDvoid );
		virtual KDvoid			setIntelligence ( KDint nValue );

		virtual KDint			getWisdom ( KDvoid );
		virtual KDvoid			setWisdom ( KDint nValue );

		virtual KDint			getCharisma ( KDvoid );
		virtual KDvoid			setCharisma ( KDint nValue );

	protected :

		std::string				m_sCharColor;
		std::string				m_sCharClass;
		KDint					m_nStrength;
		KDint					m_nDexterity;
		KDint					m_nConstitution;
		KDint					m_nIntelligence;
		KDint					m_nWisdom;
		KDint					m_nCharisma;
};

#endif	// __SimpleCharacter_h__
