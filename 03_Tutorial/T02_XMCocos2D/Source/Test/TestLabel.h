/* --------------------------------------------------------------------------
 *
 *      File            TestLabel.h
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

#ifndef __TestLabel_h__
#define __TestLabel_h__

#include "../TestBasic.h"

class TestLabel : public TestBasic
{
	public :

		static  TestLabel*		create ( KDvoid );

	protected :		

		virtual KDuint			count  ( KDvoid );
};

class Atlas1 : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );

		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit   ( KDvoid );

		virtual KDvoid			draw     ( KDvoid );

	protected :

		CCTextureAtlas*			m_pTextureAtlas;
};

class LabelAtlasTest : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		virtual KDvoid			step	 ( KDfloat fDelta );

	protected :

		KDfloat					m_fTime;
};

class LabelAtlasColorTest : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		virtual KDvoid			step	 ( KDfloat fDelta );

		KDvoid					actionFinishCallback ( KDvoid );

	protected :

		KDfloat					m_fTime;
};

class LabelTTFAlignment : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class Atlas3 : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		virtual KDvoid			step	 ( KDfloat fDelta );

	protected :

		KDfloat					m_fTime;
};

class Atlas4 : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			draw	 ( KDvoid );

		virtual KDvoid			step	 ( KDfloat fDelta );

	protected :

		KDfloat					m_fTime;
};

class Atlas5 : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class Atlas6 : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class AtlasBitmapColor : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class AtlasFastBitmap : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class BitmapFontMultiLine : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class LabelsEmpty : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

	    KDvoid					updateStrings ( KDfloat fDelta );

	private :

		KDbool					m_bSetEmpty;
};

class LabelBMFontHD : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class LabelAtlasHD : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class LabelGlyphDesigner : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class LabelTTFTest : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );

		KDvoid					setAlignmentLeft   ( CCObject* pSender );
		KDvoid					setAlignmentCenter ( CCObject* pSender );
		KDvoid					setAlignmentRight  ( CCObject* pSender );
		KDvoid					setAlignmentTop    ( CCObject* pSender );
		KDvoid					setAlignmentMiddle ( CCObject* pSender );
		KDvoid					setAlignmentBottom ( CCObject* pSender );

		KDvoid					updateAlignment		( KDvoid );
		const KDchar*			getCurrentAlignment ( KDvoid );

	private :

		CCLayerColor*				m_pBG;
		CCTextAlignment				m_eHorizAlign;
		CCVerticalTextAlignment		m_eVertAlign;
};

class LabelTTFMultiline : public TestLabel
{
	public:
		
		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class LabelTTFChinese : public TestLabel
{
	public :
		
		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class LabelBMFontChinese : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class BitmapFontMultiLineAlignment : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			onExit	 ( KDvoid );

		virtual KDvoid			ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid			ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );

		KDvoid					snapArrowsToEdge ( KDvoid );
		KDvoid					stringChanged	 ( CCObject* pSender );
		KDvoid					alignmentChanged ( CCObject* pSender );

	public :

		CCLabelBMFont*			m_pLabelShouldRetain;
		CCSprite*				m_pArrowsBarShouldRetain;
		CCSprite*				m_pArrowsShouldRetain;
		CCMenuItemFont*			m_pLastSentenceItem;
		CCMenuItemFont*			m_pLastAlignmentItem;
		KDbool					m_bDrag;
};

class LabelTTFA8Test : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class BMFontOneAtlas : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class BMFontUnicode : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class BMFontInit : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class TTFFontInit : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
};

class LabelBMFontBounds : public TestLabel
{
	public :

		virtual const KDchar*	title    ( KDvoid );
		virtual const KDchar*	subtitle ( KDvoid );
		virtual KDvoid			onEnter  ( KDvoid );
		virtual KDvoid			draw     ( KDvoid );

	protected :

		CCLabelBMFont*			m_pLabel1;
};

#endif // __TestLabel_h__
