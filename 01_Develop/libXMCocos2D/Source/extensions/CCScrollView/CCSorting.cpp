/* --------------------------------------------------------------------------
 *
 *      File            CCSorting.cpp
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Sangwoo Im
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

#include "Precompiled.h"
#include "extensions/CCScrollView/CCSorting.h"
#include "cocoa/CCString.h"

NS_CC_BEGIN

class CCSortedObject : public CCObject, public CCSortableObject
{
	public :

		CCSortedObject ( KDvoid )
		{
			m_uObjectID = 0;
		}

		virtual KDvoid  setObjectID ( KDuint uObjectID )
		{
			m_uObjectID = uObjectID;
		}

		virtual KDuint  getObjectID ( KDvoid )
		{
			return m_uObjectID;
		}

	private :

		KDuint		m_uObjectID;
};

CCArrayForObjectSorting::CCArrayForObjectSorting ( KDvoid ) : CCArray ( )
{

}

KDvoid CCArrayForObjectSorting::insertSortedObject ( CCSortableObject* pObject )
{
    KDuint     uIdx;
    CCObject*  pObj = dynamic_cast<CCObject*> ( pObject );
    CCAssert ( pObj, "Invalid parameter." );
    uIdx = this->indexOfSortedObject ( pObject );
    
    this->insertObject ( pObj, uIdx );
}

KDvoid CCArrayForObjectSorting::removeSortedObject ( CCSortableObject* pObject )
{
    if ( this->count ( ) == 0 )
	{
        return;
    }

    KDuint				uIdx;
    CCSortableObject*	pFoundObj;

    uIdx = this->indexOfSortedObject ( pObject );
    
    if ( uIdx < this->count ( ) && uIdx != CC_INVALID_INDEX )
	{
        pFoundObj = dynamic_cast<CCSortableObject*> ( this->objectAtIndex ( uIdx ) );
        
        if ( pFoundObj->getObjectID ( ) == pObject->getObjectID ( ) )
		{
            this->removeObjectAtIndex ( uIdx );
        }
    }
}

KDvoid CCArrayForObjectSorting::setObjectID_ofSortedObject ( KDuint uTag, CCSortableObject* pObject )
{
    CCSortableObject*  pFoundObj;
    KDuint			   uIdx;
    
    uIdx = this->indexOfSortedObject ( pObject );
    if ( uIdx < this->count ( ) && uIdx != CC_INVALID_INDEX )
    {
        pFoundObj = dynamic_cast<CCSortableObject*> ( this->objectAtIndex ( uIdx ) );
        CCObject*  pObj = dynamic_cast<CCObject*> ( pFoundObj );
        pObj->retain ( );
        
        if ( pFoundObj->getObjectID ( ) == pObject->getObjectID ( ) )
		{
            this->removeObjectAtIndex ( uIdx );
            pFoundObj->setObjectID ( uTag );
            this->insertSortedObject ( pFoundObj );
            pObj->release ( );
        }
		else 
		{
            pObj->release ( );
        }
    }
}

CCSortableObject* CCArrayForObjectSorting::objectWithObjectID ( KDuint uTag )
{
    if ( this->count ( ) == 0 )
	{
        return KD_NULL;
    }
    
    KDuint			   uIdx;
    CCSortableObject*  pFoundObj;
    
    pFoundObj = new CCSortedObject ( );
    pFoundObj->setObjectID ( uTag );
    
    uIdx = this->indexOfSortedObject ( pFoundObj );
    
    ( (CCSortedObject*) pFoundObj )->release ( );
    pFoundObj = KD_NULL;
    
    if ( uIdx < this->count ( ) && uIdx != CC_INVALID_INDEX )
    {
        pFoundObj = dynamic_cast<CCSortableObject*> ( this->objectAtIndex ( uIdx ) );
        if ( pFoundObj->getObjectID ( ) != uTag )
		{
            pFoundObj = KD_NULL;
        }
    }
    
    return pFoundObj;
}

KDuint CCArrayForObjectSorting::indexOfSortedObject ( CCSortableObject* pObject )
{
    KDuint  uIdx = 0;

    if ( pObject )
    {
        // FIXME: need to use binary search to improve performance
        CCObject*  pObj = KD_NULL;
        KDuint  uPrevObjectID = 0;
        KDuint  uOfSortObjectID = pObject->getObjectID ( );

        CCARRAY_FOREACH ( this, pObj )
        {
            CCSortableObject*  pSortableObj = dynamic_cast<CCSortableObject*> ( pObj );
            KDuint  uCurObjectID = pSortableObj->getObjectID ( );
            if ( ( uOfSortObjectID == uCurObjectID ) || ( uOfSortObjectID >= uPrevObjectID && uOfSortObjectID < uCurObjectID ) )
            {
                break;
            }
            
            uPrevObjectID = uCurObjectID;
            uIdx++;
        }
    }
    else
    {
        uIdx = CC_INVALID_INDEX;
    }

    return uIdx;
}

NS_CC_END
