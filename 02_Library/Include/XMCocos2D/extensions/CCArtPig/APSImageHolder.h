/* --------------------------------------------------------------------------
 *
 *      File            APSImageHolder.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
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

#ifndef __APSImageHolder_h__
#define __APSImageHolder_h__

#include "APSMedium.h"
#include "../../textures/CCTexture2D.h"

NS_APS_BEGIN

class APSResourceManager;
class APSDevice;

class APSImageHolder : public APSMedium 
{		    
	public :

		APSImageHolder ( const KDchar* szCode = "", APSResourceManager* pManager = KD_NULL );
	    
		APSImageHolder ( APSDictionary* pProperties );
	    
		virtual ~APSImageHolder ( KDvoid );
	    
		APS_CLASS_NAME ( APSImageHolder );

	public :

		virtual KDbool  initWithProperties ( APSDictionary* pProperties );
	        
		/** 
		 *	Returns image file name that determined by applied device in runtime.
		 *	Runtime file name is created when getRuntimeFilename ( ) is called for the
		 *	first time, and it returns the same instance for later calls.
		 */
		const std::string&  getRuntimeFilename ( KDvoid );

		/** Returns image file name for a given device. */
		virtual std::string  getFilenameForDevice ( APSDevice* pDevice );
	    
		/** Returns image's full path that determined by applied device in runtime. */
		const std::string*  getRuntimeFullPath ( KDvoid );
	    
		/** preloads image before it is used. */
		virtual KDvoid  preloadData ( KDvoid );

		virtual CCTexture2D*  getTexture ( KDvoid );
	    
	protected :

		APS_SYNTHESIZE_WITH_KEY ( CCSize, m_tModelTextureSize, ModelTextureSize );

	protected :

		std::string*			m_pRuntimeFullPath;
		const std::string*		m_pRuntimeFileName;
};

NS_APS_END

#endif	// __APSImageHolder_h__
