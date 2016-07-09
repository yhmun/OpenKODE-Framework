/* --------------------------------------------------------------------------
 *
 *      File            Controller.cpp
 *      Description     Application Entrance.
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

#ifndef __Controller_h__
#define __Controller_h__

typedef KDvoid ( *PFNExample ) ( KDint nIdx ); 
typedef KDvoid ( *PFNDraw ) ( KDint nMsec );
typedef KDvoid ( *PFNExit ) ( KDvoid );

class Controller : public CCLayer
{
	public :

		CREATE_FUNC ( Controller );

	protected :

		virtual KDbool  init ( KDvoid );

		virtual KDvoid  onExit ( KDvoid );
		virtual KDvoid  draw ( KDvoid );

		KDvoid  onRestart ( CCObject* pSender );
		KDvoid  onPrev    ( CCObject* pSender );
		KDvoid  onNext    ( CCObject* pSender );	

		KDvoid  setExample ( KDint nIndex );

	public :

		KDvoid  setFrame ( KDint nLocation, Mat& tImage );

		KDvoid  setMessage ( const KDchar* szMsg );

		KDvoid  setDrawFunc ( PFNDraw func );
		KDvoid  setExitFunc ( PFNExit func );

	private :

		CCLabelTTF*		m_pTitle;
		CCLabelTTF*		m_pSubTitle;
		CCLabelTTF*		m_pMessage;
		KDint			m_nIndex;
		KDuint          m_uTexture;
		PFNDraw			m_pDrawFunc;
		PFNExit			m_pExitFunc;
};

extern Controller*		g_pController;

#endif