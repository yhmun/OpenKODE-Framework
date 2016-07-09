/* --------------------------------------------------------------------------
 *
 *      File            StringLabel.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2010      Ricardo Ruiz López, 2010. All rights reserved.
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

#ifndef __StringLabel_h__
#define __StringLabel_h__

#include "Label.h"

class StringLabel : public Label 
{
	public :

		static StringLabel*		create ( KDvoid );

		static StringLabel*		create ( KDfloat fFontSize, KDint nOffset, const KDchar* szFontName, const ccColor3B& tForegroundColor, const ccColor3B& tBackgroundColor );

	public :

		virtual KDbool			init ( KDvoid );

		virtual KDbool			initWithFontSize ( KDfloat fFontSize, KDint nOffset, const KDchar* szFontName, const ccColor3B& tForegroundColor, const ccColor3B& tBackgroundColor );
		
		virtual KDvoid			setString ( const KDchar* szText );

		virtual const KDchar*	getString ( KDvoid );

	public :

		std::string				m_sString;
};

#endif	// __StringLabel_h__
