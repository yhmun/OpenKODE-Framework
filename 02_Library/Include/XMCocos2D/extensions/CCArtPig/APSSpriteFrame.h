/* --------------------------------------------------------------------------
 *
 *      File            APSSpriteFrame.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Created by Kim Kiyoung on 5/7/12.
 *      Copyright (c) 2012 ArtPig Software LLC
 *
 *         http://www.artpigsoft.com  
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
 *      version 2 of the License, or ( at your option ) any later version.
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

#ifndef __APSSpriteFrame_h__
#define __APSSpriteFrame_h__

#include "APSElements.h"
#include "../../sprite_nodes/CCSpriteFrame.h"

NS_APS_BEGIN

class APSSpriteSheetHolder;

class APSSpriteFrame
{	    
	public :

		APSSpriteFrame ( KDvoid );
	    
		APSSpriteFrame ( APSDictionary *properties, APSSpriteSheetHolder *sprietSheetHolder );	   
	        
	public :

		inline CCSpriteFrame*  getCCSpriteFrame ( KDvoid ) const { return m_pSpriteFrame; }
		inline KDvoid          setCCSpriteFrame ( CCSpriteFrame* pSpriteFrame ) { m_pSpriteFrame = pSpriteFrame; }
	    
	protected :

		APS_SYNTHESIZE_WITH_KEY ( KDuint, m_uFrameIndex, FrameIndex );
	    
		APS_SYNTHESIZE_STRING_WITH_KEY ( m_sName, Name );
	    
		APS_SYNTHESIZE_STRING_WITH_KEY ( m_sInternalName, InternalName );
	    
		APS_SYNTHESIZE ( APSSpriteSheetHolder*, m_pSpriteSheetHolder, SpriteSheetHolder )

	protected :

		CCSpriteFrame*		m_pSpriteFrame;
};

NS_APS_END

#endif	// __APSSpriteFrame_h__
