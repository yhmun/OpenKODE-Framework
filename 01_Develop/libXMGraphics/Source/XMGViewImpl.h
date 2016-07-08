/*
 *
 *      File            XMGViewImpl.h
 *      Description     View implemation
 *      Version         0.20.0801, 2011-08-01
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2011 XMSoft. All rights reserved.
 * 
 *      Contact Email: chris@xmsoft.co.kr 
 *                     xmsoft77@gmail.com	
 * 
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
 */

#ifndef __XMGViewImpl_h__
#define __XMGViewImpl_h__

class XMGViewImpl
{
	friend class XMGView;		

	private :

		 XMGViewImpl ( void );
		~XMGViewImpl ( void );	

		void	UpdateLookF ( void );
		void	UpdateLookX ( void );

		XMGRect			m_viewport;

		XMGType			m_type_proj;
		XMGType			m_type_look;

		XMGMatrix4		m_proj;
		XMGMatrix4		m_look;

		XMGVector3		m_eye;
		XMGVector3		m_at;
		XMGVector3		m_up;
		XMGVector3		m_side;
};

#endif 
