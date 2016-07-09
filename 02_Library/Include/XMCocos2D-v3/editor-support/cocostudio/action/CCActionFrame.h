/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionFrame.h
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

#ifndef __CCActionFrame_h__
#define __CCActionFrame_h__

#include "../../../ccTypes.h"
#include "../../../2d/actions/CCAction.h"

namespace cocostudio {

enum FrameType
{
	kKeyframeMove = 0,
	kKeyframeScale,
	kKeyframeRotate,
	kKeyframeTint,
	kKeyframeFade,
	kKeyframeMax
};

class ActionFrame : public cocos2d::Object
{
	public :
    
		/**
		 *	Default constructor
		 */
		ActionFrame ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~ActionFrame ( KDvoid );

		/**
		 *	Changes the index of action frame
		 *
		 *	@param index   the index of action frame
		 */
		KDvoid						setFrameIndex ( KDint nIndex );
    
		/**
		 *	Gets the index of action frame
		 *
		 *	@return the index of action frame
		 */
		KDint						getFrameIndex ( KDvoid );

		/**
		 *	Changes the time of action frame
		 *
		 *	@param fTime  the time of action frame
		 */
		KDvoid						setFrameTime ( KDfloat fTime );
    
		/**
		 *	Gets the time of action frame
		 *
		 *	@return fTime  the time of action frame
		 */
		KDfloat						getFrameTime ( KDvoid );

		/**
		 *	Changes the type of action frame
		 *
		 *	@param frameType   the type of action frame
		 */
		KDvoid						setFrameType ( KDint nFrameType );
    
		/**
		 *	Gets the type of action frame
		 *
		 *	@return the type of action frame
		 */
		KDint						getFrameType ( KDvoid );

		/**
		 *	Changes the easing type.
		 *
		 *	@param easingType the easing type.
		 */
		KDvoid						setEasingType ( KDint nEasingType );
    
		/**
		 *	Gets the easing type.
		 *
		 *	@return the easing type.
		 */
		KDint						getEasingType ( KDvoid );

		/**
		 *	Gets the CCAction of ActionFrame.
		 *
		 *	@parame fDuration   the duration time of ActionFrame
		 *
		 *	@return CCAction
		 */
		virtual cocos2d::Action*	getAction ( KDfloat fDuration );

	protected :

		KDint						m_nFrameType;
		KDint						m_nEasingType;
		KDint						m_nFrameIndex;
		KDfloat						m_fTime;
};

class ActionMoveFrame:public ActionFrame
{
	public :
    
		/**
		 *	Default constructor
		 */
		ActionMoveFrame ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~ActionMoveFrame ( KDvoid );
    
		/**
		 *	Changes the move action position.
		 *
		 *	@param the move action position.
		 */
		KDvoid						setPosition ( cocos2d::Point tPos );
    
		/**
		 *	Gets the move action position.
		 *
		 *	@return the move action position.
		 */
		cocos2d::Point				getPosition ( KDvoid );

		/**
		 *	Gets the CCAction of ActionFrame.
		 *
		 *	@parame fDuration   the duration time of ActionFrame
		 *
		 *	@return CCAction
		 */
		virtual cocos2d::Action*	getAction ( KDfloat fDuration );

	protected :

		cocos2d::Point				m_tPosition;
};

class ActionScaleFrame:public ActionFrame
{
	public :
    
		/**
		 *	Default constructor
		 */
		ActionScaleFrame ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~ActionScaleFrame ( KDvoid );
    
		/**
		 *	Changes the scale action scaleX.
		 *
		 *	@param the scale action scaleX.
		 */
		KDvoid						setScaleX ( KDfloat fScaleX );
    
		/**
		 *	Gets the scale action scaleX.
		 *
		 *	@return the scale action scaleX.
		 */
		KDfloat						getScaleX ( KDvoid );

		/**
		 *	Changes the scale action scaleY.
		 *
		 *	@param rotation the scale action scaleY.
		 */
		KDvoid						setScaleY ( KDfloat fScaleY );
    
		/**
		 *	Gets the scale action scaleY.
		 *
		 *	@return the the scale action scaleY.
		 */
		KDfloat						getScaleY ( KDvoid );
    
		/**
		 *	Gets the CCAction of ActionFrame.
		 *
		 *	@parame fDuration   the duration time of ActionFrame
		 *
		 *	@return CCAction
		 */
		virtual cocos2d::Action*	getAction ( KDfloat fDuration );

	protected :

		KDfloat						m_fScaleX;
		KDfloat						m_fScaleY;
};

class ActionRotationFrame:public ActionFrame
{
	public :
    
		/**
		 *	Default constructor
		 */
		ActionRotationFrame ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~ActionRotationFrame ( KDvoid );
    
		/**
		 *	Changes rotate action rotation.
		 *
		 *	@param rotation rotate action rotation.
		 */
		KDvoid						setRotation ( KDfloat fRotation );
    
		/**
		 *	Gets the rotate action rotation.
		 *
		 *	@return the rotate action rotation.
		 */
		KDfloat						getRotation ( KDvoid );

		/**
		 *	Gets the CCAction of ActionFrame.
		 *
		 *	@parame fDuration   the duration time of ActionFrame
		 *
		 *	@return CCAction
		 */
		virtual cocos2d::Action*	getAction ( KDfloat fDuration );

	protected :

		KDfloat						m_fRotation;
};

class ActionFadeFrame:public ActionFrame
{
	public :
    
		/**
		 *	Default constructor
		 */
		ActionFadeFrame ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~ActionFadeFrame ( KDvoid );
    
		/**
		 *	Changes the fade action opacity.
		 *
		 *	@param opacity the fade action opacity
		 */
		KDvoid						setOpacity ( KDint nOpacity );
    
		/**
		 *	Gets the fade action opacity.
		 *
		 *	@return the fade action opacity.
		 */
		KDint						getOpacity ( KDvoid );
    
		/**
		 *	Gets the CCAction of ActionFrame.
		 *
		 *	@parame fDuration   the duration time of ActionFrame
		 *
		 *	@return CCAction
		 */
		virtual cocos2d::Action*	getAction ( KDfloat fDuration );

	protected :

		KDfloat						m_fOpacity;
};

class ActionTintFrame:public ActionFrame
{
	public :
    
		/**
		 *	Default constructor
		 */
		ActionTintFrame ( KDvoid );
    
		/**
		 *	Default destructor
		 */
		virtual ~ActionTintFrame ( KDvoid );
    
		/**
		 *	Changes the tint action color.
		 *
		 *	@param ccolor the tint action color
		 */
		KDvoid						setColor ( cocos2d::Color3B tColor );
    
		/**
		 *	Gets the tint action color.
		 *
		 *	@return the tint action color.
		 */
		cocos2d::Color3B			getColor ( KDvoid );
    
		/**
		 *	Gets the CCAction of ActionFrame.
		 *
		 *	@parame fDuration   the duration time of ActionFrame
		 *
		 *	@return CCAction
		 */
		virtual cocos2d::Action*	getAction ( KDfloat fDuration );

	protected :

		cocos2d::Color3B			m_tColor;
};

}

#endif	// __CCActionFrame_h__
