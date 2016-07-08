/* --------------------------------------------------------------------------
 *
 *      File            TestBasic.h
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

#ifndef __TestBasic_h__
#define __TestBasic_h__

class TestBasic : public CCLayer
{
	public :
		
		CREATE_FUNC ( TestBasic );

	protected :
		
		virtual KDbool				init ( KDvoid );

		virtual const KDchar*		title ( KDvoid );

		virtual const KDchar*		subtitle ( KDvoid );

		virtual KDuint				count ( KDvoid );

		virtual KDvoid				onPrev    ( CCObject* pSender );
		virtual KDvoid				onNext    ( CCObject* pSender );		
		virtual KDvoid				onRestart ( CCObject* pSender );

	private :

		KDvoid						onTestList ( CCObject* pSender );		

	protected :

		CCLayer*					m_pBG;
		CCLabelTTF*					m_pTitle;
		CCLabelTTF*					m_pSubTitle;
		
	public :

		static  KDuint				s_nSubTestIndex;  
};

#endif // __TestBasic_h__
