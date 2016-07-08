/* --------------------------------------------------------------------------
 *
 *      File            ShadowLabel.h
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

#ifndef __ShadowLabel_h__
#define __ShadowLabel_h__

class ShadowLabel : public CCMenuItemLabel
{
	public :
				 ShadowLabel ( KDvoid );
		virtual ~ShadowLabel ( KDvoid );

		static   ShadowLabel*		create ( const KDchar* szText, CCObject* pListener, SEL_MenuHandler pSelector );

		static   ShadowLabel*		create ( const KDchar* szText );

	public :		

		virtual KDbool				initWithText ( const KDchar* szText, CCObject* pListener, SEL_MenuHandler pSelector );

		virtual KDvoid				setPosition ( const CCPoint& tPosition );

		virtual KDvoid				activate ( KDvoid );

		virtual KDvoid				selected ( KDvoid );

		virtual KDvoid				unselected ( KDvoid );

		virtual CCMenuItemLabel*	getShadow ( KDvoid );
		virtual KDvoid				setShadow ( CCMenuItemLabel* pShadow );

		virtual const ccColor3B&	getShadowColor ( KDvoid );
		virtual KDvoid				setShadowColor ( const ccColor3B& tShadowColor );

		virtual const ccColor3B&	getActiveColor ( KDvoid );
		virtual KDvoid				setActiveColor ( const ccColor3B& tActiveColor );

	private :

		CCMenuItemLabel*			m_pShadow;
		ccColor3B					m_tShadowColor;
		ccColor3B					m_tActiveColor;
};

#endif	// __ShadowLabel_h__
