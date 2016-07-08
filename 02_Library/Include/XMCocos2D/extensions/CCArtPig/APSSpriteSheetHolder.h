/* --------------------------------------------------------------------------
 *
 *      File            APSSpriteSheetHolder.h
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

#ifndef __APSSpriteSheetHolder_h__
#define __APSSpriteSheetHolder_h__

#include "APSSpriteFrame.h"
#include "APSImageHolder.h"

NS_APS_BEGIN

class APSSpriteSheetHolder : public APSImageHolder
{
	public :

		APSSpriteSheetHolder ( const KDchar* szCode = "", APSResourceManager* pManager = KD_NULL );
	    
		APSSpriteSheetHolder ( APSDictionary* pProperties );

		virtual ~APSSpriteSheetHolder ( KDvoid );

		APS_CLASS_NAME ( APSSpriteSheetHolder );
	    
		static KDvoid  getPlFilenameWithImageFilename ( std::string*  pPlFileName, const KDchar* szImageFile );
	    
		static const KDchar*  getPlFileExtension ( KDvoid ) { return "plist"; }

	public :
	    
		virtual KDbool  initWithProperties ( APSDictionary* pProperties );
	        
		/** preloads image before it is used. */
		virtual KDvoid  preloadData ( KDvoid );
	    
		/** Minimizes memory usage. */
		virtual KDvoid  clearCachedData ( KDvoid );
	    
		CCSpriteFrame*  getCCSpriteFrameAtIndex ( KDuint uIndex );
	    	    
		const KDchar*   getRuntimePlPath ( KDvoid );
	    
		/** if bLoadCache == KD_TRUE, sprite frames are cached from file. */
		std::vector<APSSpriteFrame*>*  getFrames ( KDbool bLoadCache = KD_TRUE );
	    
		inline KDuint  getCount ( KDvoid ) { return m_aFrames.size ( ); }
	    
	protected :

		APS_PROPERTY_KEY ( Frames );
	    
		APS_SYNTHESIZE ( KDbool, m_bIsCached, IsCached );

	protected :

		std::vector<APSSpriteFrame*>					m_aFrames;
		std::map<const std::string, APSSpriteFrame*>	m_aFramesMap;
		std::string*									m_pRuntimePlPath;
};

NS_APS_END


#endif	// __APSSpriteSheetHolder_h__
