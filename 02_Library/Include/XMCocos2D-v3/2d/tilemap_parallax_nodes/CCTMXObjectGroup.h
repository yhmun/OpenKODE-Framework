/* -----------------------------------------------------------------------------------
 *
 *      File            CCTMXObjectGroup.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Neophit
 *      Copyright (c) 2010      Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
 *
 *         http://www.cocos2d-x.org      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *     
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 *
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCTMXObjectGroup_h__
#define __CCTMXObjectGroup_h__

#include "../../base/CCGeometry.h"
#include "../../base/CCString.h"
#include "../../base/CCArray.h"
#include "../../base/CCDictionary.h"

NS_CC_BEGIN

/**
 *	@addtogroup tilemap_parallax_nodes
 *	@{
 */

/** 
 *	@brief TMXObjectGroup represents the TMX object group.
 *	@since v0.99.0
 */
class CC_DLL TMXObjectGroup : public Object
{
	public :

		/**
		 *	@js ctor
		 */
		TMXObjectGroup ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~TMXObjectGroup ( KDvoid );

		inline const KDchar*	getGroupName ( KDvoid ) { return m_sGroupName.c_str(); }
		inline KDvoid			setGroupName ( const KDchar* pGroupName ) { m_sGroupName = pGroupName; }

		/** return the value for the specific property name */
		String*					getProperty ( const KDchar* pPropertyName ) const;
    
		/** 
		 *	return the dictionary for the specific object name.
		 *	It will return the 1st object found on the array for the given name.
		 */
		Dictionary*				getObject ( const KDchar* pObjectName ) const;
    
		/** Gets the offset position of child objects */
		inline const Point&		getPositionOffset ( KDvoid ) const { return m_tPositionOffset; };
    
		/** Sets the offset position of child objects */
		inline KDvoid			setPositionOffset ( const Point& tOffset ) { m_tPositionOffset = tOffset; }
    
		/** Gets the list of properties stored in a dictionary */
		inline Dictionary*		getProperties ( KDvoid ) const { return m_pProperties; }
    
		/** Sets the list of properties */
		inline KDvoid			setProperties ( Dictionary* pProperties )
		{
			CC_SAFE_RETAIN ( pProperties );
			CC_SAFE_RELEASE ( m_pProperties );
			m_pProperties = pProperties;
		}
    
		/** Gets the array of the objects */
		inline Array*			getObjects ( KDvoid ) const { return m_pObjects; }
    
		/** Sets the array of the objects */
		inline KDvoid			setObjects ( Array* pObjects )
		{
			CC_SAFE_RETAIN ( pObjects );
			CC_SAFE_RELEASE ( m_pObjects );
			m_pObjects = pObjects;
		}
    
	protected :

		/** name of the group */
		std::string				m_sGroupName;
		/** offset position of child objects */
		Point					m_tPositionOffset;
		/** list of properties stored in a dictionary */
		Dictionary*				m_pProperties;
		/** array of the objects */
		Array*					m_pObjects;
};

//	end of tilemap_parallax_nodes group
/// @}

NS_CC_END

#endif	// __CCTMXObjectGroup_h__