/* -----------------------------------------------------------------------------------
 *
 *      File            CCTransitionProgress.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      Lam Pham
 *      Copyright (c) 2012      Ricardo Quesada
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCTransitionProgress_h__
#define __CCTransitionProgress_h__

#include "CCTransition.h"

NS_CC_BEGIN

/**
 * @addtogroup transition
 * @{
 */

class CCProgressTimer;
class CCRenderTexture;

class CCTransitionProgress : public CCTransitionScene
{
	public :

		CCTransitionProgress ( KDvoid );

		static CCTransitionProgress*	create ( KDfloat fDuration, CCScene* pScene );

	protected :
    
		virtual KDvoid				onEnter ( KDvoid );
		virtual KDvoid				onExit  ( KDvoid );

	    virtual CCProgressTimer*	progressTimerNodeWithRenderTexture ( CCRenderTexture* pTexture );

		virtual KDvoid				setupTransition ( KDvoid );
		virtual KDvoid				sceneOrder ( KDvoid );

	protected :

		KDfloat						m_fTo;
		KDfloat						m_fFrom;
		CCScene*					m_pSceneToBeModified;
};


/** 
 *	CCTransitionRadialCCW transition.
 *	A counter clock-wise radial transition to the next scene
 */
class CCTransitionProgressRadialCCW : public CCTransitionProgress
{
	public :

		static CCTransitionProgressRadialCCW*	create ( KDfloat fDuration, CCScene* pScene );

	protected :

		virtual CCProgressTimer*	progressTimerNodeWithRenderTexture ( CCRenderTexture* pTexture );
};


/** 
 *	CCTransitionRadialCW transition.
 *	A counter clock-wise radial transition to the next scene
 */
class CCTransitionProgressRadialCW : public CCTransitionProgress
{
	public :

		static CCTransitionProgressRadialCW*	create ( KDfloat fDuration, CCScene* pScene );

	protected :

		virtual CCProgressTimer*	progressTimerNodeWithRenderTexture ( CCRenderTexture* pTexture );
};

/** 
 *	CCTransitionProgressHorizontal transition.
 *	A clock-wise radial transition to the next scene
 */
class CCTransitionProgressHorizontal : public CCTransitionProgress
{
	public :

		static CCTransitionProgressHorizontal*	create ( KDfloat fDuration, CCScene* pScene );

	protected :

		virtual CCProgressTimer*	progressTimerNodeWithRenderTexture ( CCRenderTexture* pTexture );
};

class CCTransitionProgressVertical : public CCTransitionProgress
{
	public :

		static CCTransitionProgressVertical*	create ( KDfloat fDuration, CCScene* pScene );

	protected :

		virtual CCProgressTimer*	progressTimerNodeWithRenderTexture ( CCRenderTexture* pTexture );
};

class CCTransitionProgressInOut : public CCTransitionProgress
{
	public :

		static CCTransitionProgressInOut*	create ( KDfloat fDuration, CCScene* pScene );

	protected :

		virtual CCProgressTimer*	progressTimerNodeWithRenderTexture ( CCRenderTexture* pTexture );

		virtual KDvoid				sceneOrder ( KDvoid );
		virtual KDvoid				setupTransition ( KDvoid );
};

class CCTransitionProgressOutIn : public CCTransitionProgress
{
	public :

		static CCTransitionProgressOutIn*	create ( KDfloat fDuration, CCScene* pScene );

	protected :

		virtual CCProgressTimer*	progressTimerNodeWithRenderTexture ( CCRenderTexture* pTexture );
};

// end of transition group
/// @}

NS_CC_END

#endif // __CCTransitionProgress_h__

