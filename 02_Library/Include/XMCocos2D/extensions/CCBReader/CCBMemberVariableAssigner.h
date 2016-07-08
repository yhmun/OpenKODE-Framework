/* --------------------------------------------------------------------------
 *
 *      File            CCBMemberVariableAssigner.h
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

#ifndef __CCBMemberVariableAssigner_h__
#define __CCBMemberVariableAssigner_h__

#include "CCBValue.h"

NS_CC_BEGIN

#define CCB_MEMBERVARIABLEASSIGNER_GLUE( _TARGET, _MEMBERVARIABLENAME, _MEMBERVARIABLETYPE, _MEMBERVARIABLE )		\
    if ( pTarget == _TARGET && kdStrcmp ( pMemberVariableName, _MEMBERVARIABLENAME ) == 0 )							\
	{																												\
        _MEMBERVARIABLETYPE pOldVar = _MEMBERVARIABLE;																\
        _MEMBERVARIABLE = dynamic_cast<_MEMBERVARIABLETYPE> ( pNode );												\
        CC_ASSERT ( _MEMBERVARIABLE );																				\
        if ( pOldVar != _MEMBERVARIABLE )																			\
		{																											\
            CC_SAFE_RELEASE ( pOldVar );																			\
            _MEMBERVARIABLE->retain ( );																			\
        }																											\
        return KD_TRUE;																								\
    }

#define CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK( _TARGET, _MEMBERVARIABLENAME, _MEMBERVARIABLETYPE, _MEMBERVARIABLE )	\
    if ( pTarget == _TARGET && 0 == kdStrcmp ( pMemberVariableName, _MEMBERVARIABLENAME ) )							\
	{																												\
        _MEMBERVARIABLE = dynamic_cast<_MEMBERVARIABLETYPE> ( pNode );												\
        CC_ASSERT ( _MEMBERVARIABLE );																				\
        return KD_TRUE;																								\
    }

class CCBMemberVariableAssigner
{
    public :

		virtual ~CCBMemberVariableAssigner ( KDvoid ) { };

        /**
         *  The callback function of assigning member variable.
         *  @note The member variable must be CCNode or its subclass.
         *  @param pTarget The custom class.
         *  @param pMemberVariableName The name of the member variable.
         *  @param pNode The member variable.
         *  @return Whether the assignment was successful.
         */

        virtual KDbool		onAssignCCBMemberVariable ( CCObject* pTarget, const KDchar* pMemberVariableName, CCNode* pNode ) = 0;

        /**
         *  The callback function of assigning custom properties.
         *  @note The member variable must be Integer, Float, Boolean or String.
         *  @param pTarget The custom class.
         *  @param pMemberVariableName The name of the member variable.
         *  @param pValue The value of the property.
         *  @return Whether the assignment was successful.
         */
        virtual KDbool		onAssignCCBCustomProperty ( CCObject* pTarget, const KDchar* pMemberVariableName, CCBValue* pCCBValue ) { return KD_FALSE; };

};

NS_CC_END

#endif // __CCBMemberVariableAssigner_h__
