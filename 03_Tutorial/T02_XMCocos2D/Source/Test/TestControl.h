/* --------------------------------------------------------------------------
 *
 *      File            TestControl.h
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

#ifndef __TestControl_h__
#define __TestControl_h__

#include "../TestBasic.h"

class TestControl : public TestBasic
{
	public :

		static  TestControl*	create ( KDvoid );	

		virtual KDuint			count ( KDvoid );
};

class ControlSlider : public TestControl
{
	public :

		virtual KDbool			init ( KDvoid );

		virtual const KDchar*	subtitle ( KDvoid );

	protected :

		KDvoid					onValueChanged ( CCObject* pSender, CCControlEvent uControlEvent );

	private :

		CCLabelTTF*				m_pLabel;
};

class ControlSwitch : public TestControl
{
	public :

		virtual KDbool			init ( KDvoid );

		virtual const KDchar*	subtitle ( KDvoid );

		KDvoid					onSwitch ( CCObject* pSender, CCControlEvent uControlEvent );

	private :

		CCLabelTTF*				m_pLabel;
};

class ControlColourPicker : public TestControl
{
	public :

		virtual KDbool			init ( KDvoid );

		virtual const KDchar*	subtitle ( KDvoid );

	protected :

		KDvoid					onColourValueChanged ( CCObject* pSender, CCControlEvent uControlEvent );

	private :

		CCLabelTTF*				m_pLabel;
};

class ControlButton : public TestControl
{
	public :

		virtual KDbool			init ( KDvoid );

		virtual const KDchar*	subtitle ( KDvoid );

	protected :

		CCControlButton*		standardButtonWithTitle ( const KDchar* szTitle );
};

class ControlButtonEvent : public TestControl
{
	public :

		virtual KDbool			init ( KDvoid );

		virtual const KDchar*	subtitle ( KDvoid );

	protected :

		KDvoid					touchDownAction			( CCObject* pSender, CCControlEvent uControlEvent );
		KDvoid					touchDragInsideAction	( CCObject* pSender, CCControlEvent uControlEvent );
		KDvoid					touchDragOutsideAction	( CCObject* pSender, CCControlEvent uControlEvent );
		KDvoid					touchDragEnterAction	( CCObject* pSender, CCControlEvent uControlEvent );
		KDvoid					touchDragExitAction		( CCObject* pSender, CCControlEvent uControlEvent );
		KDvoid					touchUpInsideAction		( CCObject* pSender, CCControlEvent uControlEvent );
		KDvoid					touchUpOutsideAction	( CCObject* pSender, CCControlEvent uControlEvent );

	private :

		CCLabelTTF*				m_pLabel;
};

class ControlButtonStyling : public TestControl
{
	public :

		virtual KDbool			init ( KDvoid );
		
		virtual const KDchar*	subtitle ( KDvoid );

	protected :

		CCControlButton*		standardButtonWithTitle ( const KDchar* szTitle );
};

class ControlPotentiometer : public TestControl
{
	public :

		virtual KDbool			init ( KDvoid );

		virtual const KDchar*	subtitle ( KDvoid );

	protected :

		KDvoid					onValueChanged ( CCObject* pSender, CCControlEvent uControlEvent );

	private :

		CCLabelTTF*				m_pLabel;
};

class ControlStepper : public TestControl
{
	public :

		virtual KDbool			init ( KDvoid );

		virtual const KDchar*	subtitle ( KDvoid );

	protected :

		KDvoid					onValueChanged ( CCObject* pSender, CCControlEvent uControlEvent );

	private :

		CCLabelTTF*				m_pLabel;
};

#endif // TestControl
