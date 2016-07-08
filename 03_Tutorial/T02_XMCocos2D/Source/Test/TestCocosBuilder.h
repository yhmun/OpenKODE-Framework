/* --------------------------------------------------------------------------
 *
 *      File            TestCocosBuilder.h
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

#ifndef __TestCocosBuilder_h__
#define __TestCocosBuilder_h__

#include "../TestBasic.h"

class TestCocosBuilder : public CCLayer
{
	public :

		CREATE_FUNC ( TestCocosBuilder );

	protected :		

		virtual KDbool					init ( KDvoid );
};

class HelloCocosBuilderLayer : public CCLayer, public CCBSelectorResolver, public CCBMemberVariableAssigner, public CCNodeLoaderListener
{
    public :
				 HelloCocosBuilderLayer ( KDvoid );
        virtual ~HelloCocosBuilderLayer ( KDvoid );

        CREATE_FUNC ( HelloCocosBuilderLayer );

    public :

        KDvoid							openTest ( const KDchar* szCCBFileName, const KDchar* szCCNodeName = KD_NULL, CCNodeLoader* pCCNodeLoader = KD_NULL );

        virtual SEL_MenuHandler			onResolveCCBCCMenuItemSelector ( CCObject* pTarget, const KDchar* pSelectorName );
        virtual SEL_CCControlHandler	onResolveCCBCCControlSelector  ( CCObject* pTarget, const KDchar* pSelectorName );

        virtual KDbool					onAssignCCBMemberVariable ( CCObject* pTarget, const KDchar* pMemberVariableName, CCNode* pNode );
        virtual KDbool					onAssignCCBCustomProperty ( CCObject* pTarget, const KDchar* pMemberVariableName, CCBValue* pCCBValue );

        virtual KDvoid					onNodeLoaded ( CCNode* pNode, CCNodeLoader* pNodeLoader);

        KDvoid							onMenuTestClicked				( CCObject* pSender, CCControlEvent uCCControlEvent );
        KDvoid							onSpriteTestClicked				( CCObject* pSender, CCControlEvent uCCControlEvent );
        KDvoid							onButtonTestClicked				( CCObject* pSender, CCControlEvent uCCControlEvent );
        KDvoid							onAnimationsTestClicked			( CCObject* pSender, CCControlEvent uCCControlEvent );
        KDvoid							onParticleSystemTestClicked		( CCObject* pSender, CCControlEvent uCCControlEvent );
        KDvoid							onScrollViewTestClicked			( CCObject* pSender, CCControlEvent uCCControlEvent );
        KDvoid							onTimelineCallbackSoundClicked	( CCObject* pSender, CCControlEvent uCCControlEvent );

    private :

        CCSprite*						m_pBurstSprite;
		CCLabelTTF*						m_pTestTitleLabelTTF;

        KDint							m_nCustomPropertyInt;
        KDfloat							m_fCustomPropertyFloat;
        KDbool							m_bCustomPropertyBoolean;
        std::string						m_sCustomPropertyString;
};

class HelloCocosBuilderLayerLoader : public CCLayerLoader
{
    public :

        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( HelloCocosBuilderLayerLoader, loader );

    protected :

        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( HelloCocosBuilderLayer );
};

class TestHeaderLayer : public CCLayer, public CCBSelectorResolver
{
    public :

        CREATE_FUNC ( TestHeaderLayer );

    public :

        virtual SEL_MenuHandler			onResolveCCBCCMenuItemSelector ( CCObject* pTarget, const KDchar* pSelectorName );
        virtual SEL_CCControlHandler	onResolveCCBCCControlSelector  ( CCObject* pTarget, const KDchar* pSelectorName );

        KDvoid  onBackClicked ( CCObject* pSender );
};

class TestHeaderLayerLoader : public CCLayerLoader
{
    public :

        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( TestHeaderLayerLoader, loader );

    protected :

        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( TestHeaderLayer );
};

class MenuTestLayer : public CCLayer, public CCBSelectorResolver, public CCBMemberVariableAssigner 
{
    public :
				 MenuTestLayer ( KDvoid );
        virtual ~MenuTestLayer ( KDvoid );

		CREATE_FUNC ( MenuTestLayer );

    public :

        virtual SEL_MenuHandler			onResolveCCBCCMenuItemSelector ( CCObject* pTarget, const KDchar* pSelectorName );
        virtual SEL_CCControlHandler	onResolveCCBCCControlSelector  ( CCObject* pTarget, const KDchar* pSelectorName );
        virtual KDbool					onAssignCCBMemberVariable      ( CCObject* pTarget, const KDchar* pMemberVariableName, CCNode* pNode );

        KDvoid							onMenuItemAClicked ( CCObject* pSender );
        KDvoid							onMenuItemBClicked ( CCObject* pSender );
        KDvoid							onMenuItemCClicked ( CCObject* pSender );

    private :

        CCLabelBMFont*					m_pMenuItemStatusLabelBMFont;
};

class MenuTestLayerLoader : public CCLayerLoader
{
    public :

        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( MenuTestLayerLoader, loader );

    protected :

        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( MenuTestLayer );
};

class SpriteTestLayer : public CCLayer 
{
    public :

        CREATE_FUNC ( SpriteTestLayer );
};

class SpriteTestLayerLoader : public CCLayerLoader
{
    public :

        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( SpriteTestLayerLoader, loader );

    protected :

        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( SpriteTestLayer );
};

class ButtonTestLayer : public CCLayer, public CCBMemberVariableAssigner, public CCBSelectorResolver
{
	public :
				 ButtonTestLayer ( KDvoid );
		virtual ~ButtonTestLayer ( KDvoid );

		CREATE_FUNC ( ButtonTestLayer );

	public :

        virtual SEL_MenuHandler			onResolveCCBCCMenuItemSelector ( CCObject* pTarget, const KDchar* pSelectorName );
        virtual SEL_CCControlHandler	onResolveCCBCCControlSelector  ( CCObject* pTarget, const KDchar* pSelectorName );
        virtual KDbool					onAssignCCBMemberVariable      ( CCObject* pTarget, const KDchar* pMemberVariableName, CCNode* pNode );

		KDvoid							onCCControlButtonClicked ( CCObject* pSender, CCControlEvent uCCControlEvent );

	private :

		CCLabelBMFont*					m_pCCControlEventLabel;
};

class ButtonTestLayerLoader : public CCLayerLoader
{
    public :

        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( ButtonTestLayerLoader, loader );

    protected :

        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( ButtonTestLayer );
};

class ParticleSystemTestLayer : public CCLayer 
{
    public :

        CREATE_FUNC ( ParticleSystemTestLayer );
};

class ParticleSystemTestLayerLoader : public CCLayerLoader
{
    public :

        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( ParticleSystemTestLayerLoader, loader );

    protected :

        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( ParticleSystemTestLayer );
};

class ScrollViewTestLayer : public CCLayer 
{
    public :

        CREATE_FUNC ( ScrollViewTestLayer );
};

class ScrollViewTestLayerLoader : public CCLayerLoader
{
    public :

        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( ScrollViewTestLayerLoader, loader );

    protected :

        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( ScrollViewTestLayer );
};

class AnimationsTestLayer : public CCLayer, public CCBSelectorResolver, public CCBMemberVariableAssigner 
{
	public :
				 AnimationsTestLayer ( KDvoid );
		virtual ~AnimationsTestLayer ( KDvoid );

		CREATE_FUNC ( AnimationsTestLayer );
	    
	public :
	    
        virtual SEL_MenuHandler			onResolveCCBCCMenuItemSelector ( CCObject* pTarget, const KDchar* pSelectorName );
        virtual SEL_CCControlHandler	onResolveCCBCCControlSelector  ( CCObject* pTarget, const KDchar* pSelectorName );
        virtual KDbool					onAssignCCBMemberVariable      ( CCObject* pTarget, const KDchar* pMemberVariableName, CCNode* pNode );
 
		KDvoid							onCCControlButtonIdleClicked  ( CCObject* pSender, CCControlEvent uCCControlEvent );
		KDvoid							onCCControlButtonWaveClicked  ( CCObject* pSender, CCControlEvent uCCControlEvent );
		KDvoid							onCCControlButtonJumpClicked  ( CCObject* pSender, CCControlEvent uCCControlEvent );
		KDvoid							onCCControlButtonFunkyClicked ( CCObject* pSender, CCControlEvent uCCControlEvent );
	    
		KDvoid							setAnimationManager ( CCBAnimationManager* pAnimationManager );
	    
	private :

		CCBAnimationManager*			m_pAnimationManager;
};

class AnimationsTestLayerLoader : public CCLayerLoader
{
    public :

        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( AnimationsTestLayerLoader, loader );

    protected :

        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( AnimationsTestLayer );
};

class TimelineCallbackTestLayer : public CCLayer, public CCBSelectorResolver, public CCBMemberVariableAssigner 
{
    public :
				 TimelineCallbackTestLayer ( KDvoid );
        virtual ~TimelineCallbackTestLayer ( KDvoid );

        CREATE_FUNC ( TimelineCallbackTestLayer );

    public :

        virtual SEL_MenuHandler			onResolveCCBCCMenuItemSelector	( CCObject* pTarget, const KDchar* pSelectorName );
        virtual SEL_CCControlHandler	onResolveCCBCCControlSelector	( CCObject* pTarget, const KDchar* pSelectorName );
        virtual SEL_CallFuncN			onResolveCCBCCCallFuncSelector	( CCObject* pTarget, const KDchar* pSelectorName );
        virtual KDbool					onAssignCCBMemberVariable		( CCObject* pTarget, const KDchar* pMemberVariableName, CCNode* pNode );
    
        KDvoid							onCallback1 ( CCNode* pSender );
        KDvoid							onCallback2 ( CCNode* pSender );

	private :

		CCLabelTTF*						m_pHelloLabel;
};


class TimelineCallbackTestLayerLoader : public CCLayerLoader 
{
    public :

        CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD ( TimelineCallbackTestLayerLoader, loader );

    protected :

        CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD ( TimelineCallbackTestLayer );
};



#endif	// __TestCocosBuilder_h__
