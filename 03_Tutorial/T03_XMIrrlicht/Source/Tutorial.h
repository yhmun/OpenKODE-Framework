/* --------------------------------------------------------------------------
 *
 *      File            Tutorial.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
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

#ifndef __Tutorial_h__
#define __Tutorial_h__

class CTutorial : public IEventReceiver
{
	public :

		CTutorial ( KDvoid );
		virtual ~CTutorial ( KDvoid );

	public :

		virtual KDvoid  Init ( KDvoid );

		virtual bool  OnEvent ( const SEvent& event );

		virtual KDvoid  Draw ( KDvoid );

		virtual video::SColor  getClear ( KDvoid );

		virtual const wchar_t*  getTitle ( KDvoid );

		virtual KDbool  getVirPad ( KDvoid );

		static  KDvoid  Redraw ( KDvoid );
		static  KDvoid  SetTutorial ( KDuint uIndex );

	protected :

		core::rect<s32>  _rect ( s32 x, s32 y, s32 w, s32 h );

	protected :
		
		scene::ISceneManager*			m_pScrMgr;
		gui::IGUIEnvironment*			m_pGuiEnv;

	public :

		static  irr::IrrlichtDevice*	s_pDevice;
		static	video::IVideoDriver*	s_pDriver;
		static  CTutorial*				s_pTutorial;
		static  KDuint					s_uIndex;		
};

#endif