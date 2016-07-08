/* -----------------------------------------------------------------------------------
 *
 *      File            CCTransitionProgress.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCTransitionProgress_h__
#define __CCTransitionProgress_h__

#include "CCTransition.h"

NS_CC_BEGIN

class ProgressTimer;
class RenderTexture;

/**
 *	@addtogroup transition
 *	@{
 */
class CC_DLL TransitionProgress : public TransitionScene
{
	public :

		static TransitionProgress*				create ( KDfloat t, Scene* pScene );

		TransitionProgress ( KDvoid );

		//
		// Overrides
		//
		virtual KDvoid							onEnter ( KDvoid ) override;
		virtual KDvoid							onExit ( KDvoid ) override;

	protected :

		virtual KDvoid							sceneOrder ( KDvoid ) override;

	protected :

		virtual ProgressTimer*					progressTimerNodeWithRenderTexture ( RenderTexture* pTexture );
		virtual KDvoid							setupTransition ( KDvoid );

	protected :

		KDfloat									m_fTo;
		KDfloat									m_fFrom;
		Scene*									m_pSceneToBeModified;
};


/** 
 *	TransitionRadialCCW transition.
 *	A counter clock-wise radial transition to the next scene
 */
class CC_DLL TransitionProgressRadialCCW : public TransitionProgress
{
	public :

		static TransitionProgressRadialCCW*		create ( KDfloat t, Scene* pScene );

	protected :

		//
		// Overrides
		//
		virtual ProgressTimer*					progressTimerNodeWithRenderTexture ( RenderTexture* pTexture ) override;
  
};


/**
 *	TransitionRadialCW transition.
 *	A counter clock-wise radial transition to the next scene
 */
class CC_DLL TransitionProgressRadialCW : public TransitionProgress
{
	public :

		static TransitionProgressRadialCW*		create ( KDfloat t, Scene* pScene );

	protected :

		//
		// Overrides
		//
		virtual ProgressTimer*					progressTimerNodeWithRenderTexture ( RenderTexture* pTexture ) override;

};

/** 
 *	TransitionProgressHorizontal transition.
 *	A  clock-wise radial transition to the next scene
 */
class CC_DLL TransitionProgressHorizontal : public TransitionProgress
{
	public :

		static TransitionProgressHorizontal*	create ( KDfloat t, Scene* pScene );

	protected :

		//
		// Overrides
		//
		virtual ProgressTimer*					progressTimerNodeWithRenderTexture ( RenderTexture* pTexture ) override;
};

class CC_DLL TransitionProgressVertical : public TransitionProgress
{
	public :

		static TransitionProgressVertical*		create ( KDfloat t, Scene* pScene );

	protected :

		//
		// Overrides
		//
		virtual ProgressTimer*					progressTimerNodeWithRenderTexture  ( RenderTexture* pTexture ) override;
};

class CC_DLL TransitionProgressInOut : public TransitionProgress
{
	public :

		static TransitionProgressInOut*			create ( KDfloat t, Scene* pScene );

	protected :

		//
		// Overrides
		//
		virtual ProgressTimer*					progressTimerNodeWithRenderTexture ( RenderTexture* pTexture ) override;
		virtual KDvoid							sceneOrder ( KDvoid ) override;
		virtual KDvoid							setupTransition ( KDvoid ) override;
};

class CC_DLL TransitionProgressOutIn : public TransitionProgress
{
	public :

		static TransitionProgressOutIn*			create ( KDfloat t, Scene* pScene );

	protected :

		//
		// Overrides
		//
		virtual ProgressTimer*					progressTimerNodeWithRenderTexture ( RenderTexture* pTexture ) override;

};

// end of transition group
/// @}

NS_CC_END

#endif	// __CCTransitionProgress_h__

