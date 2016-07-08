/* --------------------------------------------------------------------------
 *
 *      File            TestEditBox.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2012      James Chen
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

#ifndef __TestEditBox_h__
#define __TestEditBox_h__

#include "../TestBasic.h"

class TestEditBox : public TestBasic, public CCEditBoxDelegate
{
	public :

		CREATE_FUNC ( TestEditBox );

	protected :		

		virtual KDbool		init ( KDvoid );

		virtual KDvoid		editBoxEditingBegan ( CCEditBox* pEditBox );
		virtual KDvoid		editBoxEditingEnded ( CCEditBox* pEditBox );
		virtual KDvoid		editBoxTextChanged	( CCEditBox* pEditBox, const KDchar* szText );
		virtual KDvoid		editBoxReturn		( CCEditBox* pEditBox );

	private :

		CCEditBox*			m_pEditName;
		CCEditBox*			m_pEditPassword;
		CCEditBox*			m_pEditEmail;
};

#endif	// __TestEditBox_h__
