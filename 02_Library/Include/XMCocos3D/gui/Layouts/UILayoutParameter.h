/* -----------------------------------------------------------------------------------
 *
 *      File            UILayoutParameter.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2013      cocos2d-x.org
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

#ifndef __UILayoutParameter_h__
#define __UILayoutParameter_h__

#include "UILayoutDefine.h"
#include "../../base/CCObject.h"
#include <string>

namespace gui {

typedef enum
{
    LAYOUT_PARAMETER_NONE,
    LAYOUT_PARAMETER_LINEAR,
    LAYOUT_PARAMETER_RELATIVE
} LayoutParameterType;

/**
 *   @js NA
 *   @lua NA
 */
class UILayoutParameter : public cocos2d::Object
{
	public :

		/**
		 * Default constructor
		 */
		UILayoutParameter ( KDvoid ) :
			m_tMargin ( UIMargin ( ) )
		{
			m_eLayoutParameterType = LAYOUT_PARAMETER_NONE;
		}
    
		/**
		 * Default destructor
		 */
		virtual ~UILayoutParameter ( KDvoid ) { }
    
		/**
		 *	Allocates and initializes.
		 *	@return A initialized LayoutParameter which is marked as "autorelease".
		 */	
		static UILayoutParameter*			create ( KDvoid );
    
		/**
		 *	Sets Margin parameter for LayoutParameter.
		 * 
		 *	@see UIMargin
		 *
		 *	@param margin
		 */
		KDvoid								setMargin ( const UIMargin& tMargin );
    
		/**
		 *	Gets Margin parameter of LayoutParameter.
		 *
		 *	@see UIMargin
		 *
		 *	@return const UIMargin&
		 */
		const UIMargin&						getMargin ( KDvoid ) const;
    
		/**
		 *	Gets LayoutParameterType of LayoutParameter.
		 *
		 *	@see LayoutParameterType
		 *
		 *	@return LayoutParameterType
		 */
		LayoutParameterType					getLayoutType ( KDvoid ) const;

	protected :

		UIMargin							m_tMargin;
		LayoutParameterType					m_eLayoutParameterType;
};

/**
 *   @js NA
 *   @lua NA
 */
class UILinearLayoutParameter : public UILayoutParameter
{
	public :

		/**
		 *	Default constructor
		 */
		UILinearLayoutParameter ( KDvoid ) :
			m_eLinearGravity ( LINEAR_GRAVITY_NONE )
		{
			m_eLayoutParameterType = LAYOUT_PARAMETER_LINEAR;
		}
    
		/**
		 *	Default destructor
		 */
		virtual ~UILinearLayoutParameter ( KDvoid ) { }
    
		/**
		 *	Allocates and initializes.
		 *	@return A initialized LayoutParameter which is marked as "autorelease".
		 */
		static UILinearLayoutParameter*		create ( KDvoid );
    
		/**
		 *	Sets UILinearGravity parameter for LayoutParameter.
		 *
		 *	@see UILinearGravity
		 *
		 *	@param UILinearGravity
		 */
		KDvoid								setGravity ( UILinearGravity eGravity );
    
		/**
		 *	Gets UILinearGravity parameter for LayoutParameter.
		 *
		 *	@see UILinearGravity
		 *
		 *	@return UILinearGravity
		 */
		UILinearGravity						getGravity ( KDvoid ) const;

	protected :

		UILinearGravity						m_eLinearGravity;
};

/**
 *   @js NA
 *   @lua NA
 */

class UILayout;

class UIRelativeLayoutParameter : public UILayoutParameter
{
	public :

		/**
		 * Default constructor
		 */
		UIRelativeLayoutParameter ( KDvoid ) :
			m_eRelativeAlign ( RELATIVE_ALIGN_NONE ),
			m_sRelativeWidgetName ( "" ), 
			m_sRelativeLayoutName ( "" ),
			m_bPut ( false )
		{
			m_eLayoutParameterType = LAYOUT_PARAMETER_RELATIVE; 
		}
    
		/**
		 * Default destructor
		 */
		virtual ~UIRelativeLayoutParameter ( KDvoid ) { };
    
		/**
		 *	Allocates and initializes.
		 *	@return A initialized LayoutParameter which is marked as "autorelease".
		 */
		static UIRelativeLayoutParameter*	create ( KDvoid );
    
		/**
		 *	Sets UIRelativeAlign parameter for LayoutParameter.
		 *
		 *	@see UIRelativeAlign
		 *
		 *	@param UIRelativeAlign
		 */
		KDvoid								setAlign ( UIRelativeAlign eAlign );
    
		/**
		 *	Gets UIRelativeAlign parameter for LayoutParameter.
		 *
		 *	@see UIRelativeAlign
		 *
		 *	@return UIRelativeAlign
		 */
		UIRelativeAlign						getAlign ( KDvoid ) const;
    
		/**
		 *	Sets a key for LayoutParameter. Witch widget named this is relative to.
		 *
		 *	@param name
		 */
		KDvoid								setRelativeToWidgetName ( const KDchar* pName );
    
		/**
		 *	Gets the key of LayoutParameter. Witch widget named this is relative to.
		 *
		 *	@return name
		 */
		const KDchar*						getRelativeToWidgetName ( KDvoid ) const;
    
		/**
		 *	Sets a name in Relative Layout for LayoutParameter.
		 *
		 *	@param name
		 */
		KDvoid								setRelativeName ( const KDchar* pName );
    
		/**
		 *	Gets a name in Relative Layout of LayoutParameter.
		 *
		 *	@return name
		 */
		const KDchar*						getRelativeName ( KDvoid ) const;

	protected :

		UIRelativeAlign						m_eRelativeAlign;
		std::string							m_sRelativeWidgetName;
		std::string							m_sRelativeLayoutName;
		KDbool								m_bPut;

		friend class UILayout;
};

}

#endif	// __UILayoutParameter_h__
