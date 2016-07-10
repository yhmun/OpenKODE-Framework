/* --------------------------------------------------------------------------
 *
 *      File            UIControl.h
 *      Author          Y.H Mun
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

#ifndef __UIControl_h__
#define __UIControl_h__

class Sinbad;

class UIControl : public CCLayer, public FrameListener
{
	public :		

		CREATE_FUNC ( UIControl );

	protected :
		
		virtual KDbool  init ( KDvoid ); 

		virtual KDvoid  draw ( KDvoid );

	public :

		virtual KDvoid  keyReleased ( KDint nID );
		virtual KDvoid  keyPressed  ( KDint nID );

		KDvoid  onClose ( CCObject* pSender );

	protected :

		KDbool  initOgre3D ( KDvoid );

        virtual bool  frameRenderingQueued ( const FrameEvent& tEvent );
    
	protected :

		Root*               m_pRoot;                    
		RenderWindow*       m_pWindow;
		Sinbad*				m_pSinbad;
};

class RollerOver : public CCMenuItemImage
{
	public :

		static   RollerOver*  create ( const KDchar* szNormalImage, const KDchar* szSelectedImage, KDint nTag );

	protected :

		virtual KDvoid  selected   ( KDvoid );
		virtual KDvoid  unselected ( KDvoid );
};

#endif // __UIControl_h__
