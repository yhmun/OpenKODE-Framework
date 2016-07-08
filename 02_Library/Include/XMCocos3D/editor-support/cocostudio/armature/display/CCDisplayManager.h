/* -----------------------------------------------------------------------------------
 *
 *      File            CCDisplayManager.h
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

#ifndef __CCDisplayManager_h__
#define __CCDisplayManager_h__

#include "../utils/CCArmatureDefine.h"
#include "CCDecorativeDisplay.h"
#include "../datas/CCDatas.h"

namespace cocostudio {

class Bone;

//! DisplayManager manages Bone's display
class  DisplayManager : public cocos2d::Object
{
	public :

		static DisplayManager*		create ( Bone* pBone );

	public :

		/**
		 *	@js ctor
		 */
		DisplayManager ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~DisplayManager ( KDvoid );

		KDbool						init ( Bone* pBone );

		/**
		 *	Use BoneData to init the display list.
		 *	If display is a sprite, and it have texture info in the TexutreData, then use TexutreData to init the display's anchor point
		 *	If the display is a Armature, then create a new Armature
		 */
		virtual KDvoid				initDisplayList ( BoneData* pBoneData );

		/**
		 *	Add display and use  _DisplayData init the display.
		 *	If index already have a display, then replace it.
		 *	If index is current display index, then also change display to _index
		 *
		 *	@param 	displayData it include the display information, like DisplayType.
		 *						If you want to create a sprite display, then create a SpriteDisplayData param
		 *
		 *	@param 	index the index of the display you want to replace or add to
		 *						-1 : append display from back
		 */
		KDvoid						addDisplay ( DisplayData* pDisplayData, KDint nIndex );

		KDvoid						addDisplay ( cocos2d::Node* pDisplay, KDint nIndex );

		KDvoid						removeDisplay ( KDint nIndex );

		cocos2d::Array*				getDecorativeDisplayList ( KDvoid ) const;

		/**
		 *	Change display by index. You can just use this method to change display in the display list.
		 *	The display list is just used for this bone, and it is the displays you may use in every frame.
		 *
		 *	Note : if index is the same with prev index, the method will not effect
		 *
		 *	@param index The index of the display you want to change
		 *	@param force If true, then force change display to specified display, or current display will set to  display index edit in the flash every key frame.
		 */
		KDvoid						changeDisplayByIndex ( KDint nIndex, KDbool bForce );

		cocos2d::Node*				getDisplayRenderNode ( KDvoid ) const;
		DisplayType					getDisplayRenderNodeType ( KDvoid ) const;

		KDint						getCurrentDisplayIndex ( KDvoid ) const;

		virtual KDvoid				setCurrentDecorativeDisplay ( DecorativeDisplay* pDecoDisplay );
		virtual DecorativeDisplay*	getCurrentDecorativeDisplay ( KDvoid ) const;
		virtual DecorativeDisplay*	getDecorativeDisplayByIndex ( KDint nIndex ) const;

		/**
		 *	Sets whether the display is visible
		 *	The default value is true, a node is default to visible
		 *
		 *	@param visible   true if the node is visible, false if the node is hidden.
		 */
		virtual KDvoid				setVisible ( KDbool bVisible );

		/**
		 *	Determines if the display is visible
		 *
		 *	@see setVisible(bool)
		 *	@return true if the node is visible, false if the node is hidden.
		 */
		virtual KDbool				isVisible ( KDvoid ) const;

		cocos2d::Size				getContentSize ( KDvoid ) const;
		cocos2d::Rect				getBoundingBox ( KDvoid ) const;

		cocos2d::Point				getAnchorPoint ( KDvoid ) const;
		cocos2d::Point				getAnchorPointInPoints ( KDvoid ) const;

		/**
		 *	Check if the position is inside the bone.
		 */
		virtual KDbool				containPoint ( cocos2d::Point& tPoint );

		/**
		 *	Check if the position is inside the bone.
		 */
		virtual KDbool				containPoint ( KDfloat x, KDfloat y );

		virtual KDvoid				setForceChangeDisplay ( KDbool bForce ) { m_bForceChangeDisplay = bForce; }
		virtual KDbool				 isForceChangeDisplay ( KDvoid ) const { return m_bForceChangeDisplay; }

	protected :

		cocos2d::Array*				m_pDecoDisplayList;
		//! Display render node.
		cocos2d::Node*				m_pDisplayRenderNode;
		//! Display render node type
		DisplayType					m_eDisplayType;
		//! Include current display information, like contour sprite, etc.
		DecorativeDisplay*			m_pCurrentDecoDisplay;
		//! Current display index
		KDint						m_nDisplayIndex;

		KDbool						m_bForceChangeDisplay;

		//! Whether of not the bone is visible. Default is true
		KDbool						m_bVisible;

		Bone*						m_pBone;
};

}

#endif	// __CCDisplayManager_h__
