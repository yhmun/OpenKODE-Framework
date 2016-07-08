/* --------------------------------------------------------------------------
 *
 *      File            CCGrabber.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *      Copyright (c) 2009      On-Core
 *
 *         http://www.cocos2d-x.org      
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
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

#ifndef __CCGrabber_h__
#define __CCGrabber_h__

#include "support/CCConfiguration.h"
#include "cocoa/CCObject.h"

NS_CC_BEGIN
    
class CCTexture2D;

/** FBO class that grabs the the contents of the screen */
class CCGrabber : public CCObject
{
	public :

		 CCGrabber ( KDvoid );
		~CCGrabber ( KDvoid );

	public :

		KDvoid  grab ( CCTexture2D* pTexture );
		KDvoid  beforeRender ( CCTexture2D* pTexture );
		KDvoid  afterRender ( CCTexture2D* pTexture );

	protected :

		GLuint      m_uFBO;
		GLint       m_nOldFBO;
};

NS_CC_END

#endif // __CCGrabber_h__
