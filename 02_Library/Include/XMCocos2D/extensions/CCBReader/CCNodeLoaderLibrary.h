/* --------------------------------------------------------------------------
 *
 *      File            CCNodeLoaderLibrary.h
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org   
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

#ifndef __CCNodeLoaderLibrary_h__
#define __CCNodeLoaderLibrary_h__

#include "CCBReader.h"
#include <map>

NS_CC_BEGIN

class CCNodeLoader;

typedef std::map<std::string, CCNodeLoader*>	CCNodeLoaderMap;
typedef std::pair<std::string, CCNodeLoader*>	CCNodeLoaderMapEntry;

class CCNodeLoaderLibrary : public CCObject 
{
    public :

        virtual ~CCNodeLoaderLibrary ( KDvoid );

		CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( CCNodeLoaderLibrary, library );

        static CCNodeLoaderLibrary*		     sharedCCNodeLoaderLibrary ( KDvoid );
        static KDvoid					purgeSharedCCNodeLoaderLibrary ( KDvoid );
        static CCNodeLoaderLibrary*		 newDefaultCCNodeLoaderLibrary ( KDvoid );

	public :

        KDvoid				registerDefaultCCNodeLoaders ( KDvoid );

        KDvoid				registerCCNodeLoader   ( const KDchar* szClassName, CCNodeLoader* pCCNodeLoader );

        KDvoid				unregisterCCNodeLoader ( const KDchar* szClassName );

        CCNodeLoader*		getCCNodeLoader ( const KDchar* szClassName );
 
        KDvoid				purge ( KDbool pDelete );

    private :

        CCNodeLoaderMap		m_aCCNodeLoaders;
};

NS_CC_END

#endif	// __CCNodeLoaderLibrary_h__
