/* --------------------------------------------------------------------------
 *
 *      File            KWScene.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 11/03/05 
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. 
 *      Copyright (c) 2011-2013 Kawaz. All rights reserved.                             
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

#ifndef __KWScene_h__
#define __KWScene_h__

class KWScene : public CCLayer 
{
	public :

		KWScene	( KDvoid );

	public :

		virtual KDbool		init	( KDvoid );

		virtual KDvoid		update	( KDfloat fDelta );

	protected :

		ccColor4B			m_tBackgroundColor;	
		CCSize				m_tLyrSize;
};

#endif	// __KWScene_h__
