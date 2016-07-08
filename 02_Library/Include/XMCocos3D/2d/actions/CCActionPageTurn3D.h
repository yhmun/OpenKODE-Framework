/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionPageTurn3D.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      Sindesso Pty Ltd 
 *
 *         http://www.cocos2d-x.org      
 *         http://www.sindesso.com  
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

#ifndef __CCActionPageTurn3D_h__
#define __CCActionPageTurn3D_h__

#include "CCActionGrid3D.h"

NS_CC_BEGIN

/**
 *	@addtogroup actions
 *	@{
 */

/**
 *	@brief This action simulates a page turn from the bottom right hand corner of the screen.
 *	It's not much use by itself but is used by the PageTurnTransition.
 *
 *	Based on an original paper by L Hong et al.
 *	http://www.parc.com/publication/1638/turning-pages-of-3d-electronic-books.html
 * 
 *	@since v0.8.2
 */
class CC_DLL PageTurn3D : public Grid3DAction
{
	public :

		/** create the action */
		static PageTurn3D*		create ( KDfloat fDuration, const Size& tGridSize );

		// Overrides
		virtual PageTurn3D*		clone ( KDvoid ) const override;
		virtual KDvoid			update ( KDfloat fTime ) override;
};

//	end of actions group
/// @}

NS_CC_END

#endif	// __CCActionPageTurn3D_h__
