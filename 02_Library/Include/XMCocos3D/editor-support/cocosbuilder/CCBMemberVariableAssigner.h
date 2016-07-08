/* -----------------------------------------------------------------------------------
 *
 *      File            CCBMemberVariableAssigner.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#ifndef __CCBMemberVariableAssigner_h__
#define __CCBMemberVariableAssigner_h__

#include "CCBValue.h"

namespace cocosbuilder {

#define CCB_MEMBERVARIABLEASSIGNER_GLUE(TARGET, MEMBERVARIABLENAME, MEMBERVARIABLETYPE, MEMBERVARIABLE)			\
if ( pTarget == TARGET && 0 == kdStrcmp ( pMemberVariableName, ( MEMBERVARIABLENAME ) ) ) {						\
    MEMBERVARIABLETYPE pOldVar = MEMBERVARIABLE;																\
    MEMBERVARIABLE = dynamic_cast<MEMBERVARIABLETYPE> ( pNode );												\
    CC_ASSERT ( MEMBERVARIABLE );																				\
    if ( pOldVar != MEMBERVARIABLE ) {																			\
        CC_SAFE_RELEASE ( pOldVar );																			\
        MEMBERVARIABLE->retain ( );																				\
    }																											\
    return true;																								\
}

#define CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(TARGET, MEMBERVARIABLENAME, MEMBERVARIABLETYPE, MEMBERVARIABLE)	\
if ( pTarget == TARGET && 0 == kdStrcmp ( pMemberVariableName, MEMBERVARIABLENAME ) ) {							\
    MEMBERVARIABLE = dynamic_cast<MEMBERVARIABLETYPE> ( pNode );												\
    CC_ASSERT ( MEMBERVARIABLE );																				\
    return true;																								\
}

class CCBMemberVariableAssigner 
{
    public :

        /**
         *	@js NA
         *	@lua NA
         */
        virtual ~CCBMemberVariableAssigner ( KDvoid ) { }

        /**
         *  The callback function of assigning member variable.
         *  @note The member variable must be Node or its subclass.
         *  @param target               The custom class.
         *  @param memberVariableName   The name of the member variable.
         *  @param node                 The member variable.
         *  @return Whether the assignment was successful.
         */
        virtual KDbool		onAssignCCBMemberVariable ( cocos2d::Object* pTarget, const KDchar* pMemberVariableName, cocos2d::Node* pNode ) = 0;

        /**
         *  The callback function of assigning custom properties.
         *  @note The member variable must be Integer, Float, Boolean or String.
         *  @param target               The custom class.
         *  @param memberVariableName   The name of the member variable.
         *  @param value                The value of the property.
         *  @return Whether the assignment was successful.
         */
        virtual KDbool		onAssignCCBCustomProperty ( cocos2d::Object* pTarget, const KDchar* pMemberVariableName, CCBValue* pValue ) { return false; }
};

}

#endif	// __CCBMemberVariableAssigner_h__
