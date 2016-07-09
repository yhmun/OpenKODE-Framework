/* -----------------------------------------------------------------------------------
 *
 *      File            EquipScene.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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

#ifndef __EquipScene_h__
#define __EquipScene_h__

#define EQUIP_LAYOUT_TAG_ROOT                            1000
#define EQUIP_LAYOUT_TAG_UP                              1001
#define EQUIP_LAYOUT_TAG_CLOTHES                         1002
#define EQUIP_LAYOUT_TAG_WEAPONS                         1003
#define EQUIP_LAYOUT_TAG_PETS                            1004

#define EQUIP_SWITCH_LAYOUT_BUTTON_TAG_CLOTHES           1005
#define EQUIP_SWITCH_LAYOUT_BUTTON_TAG_WEAPONS           1006
#define EQUIP_SWITCH_LAYOUT_BUTTON_TAG_PETS              1007

class EquipScene : public Scene
{
	public :

		 EquipScene ( KDvoid );
		~EquipScene ( KDvoid );
    
		virtual KDvoid			onEnter ( KDvoid );
		virtual KDvoid			onExit  ( KDvoid );   
    
	protected :

		KDvoid					menuCloseCallback ( Object* pSender );
    
		// equip																original function name
		KDvoid					EquipInit ( KDvoid );							// EquipInit
		KDvoid					create ( KDvoid );								// EquipCreate
		KDvoid					createClothes ( KDvoid );						// EquipCreateClothes
		KDvoid					createWeapons ( KDvoid );						// EquipCreateWeapons
		KDvoid					createPets ( KDvoid );							// EquipCreatePets
    
		KDvoid					switchBtnCallBack ( Object* pSender );			// EquipSwitchBtnCallBack
    
		KDvoid					touch ( Object* pSender );                      // EquipTouch
		KDvoid					move ( Object* pSender );                       // EquipMove
		KDvoid					drop ( Object* pSender );                       // EquipDrop
    
		KDvoid					backOver ( Object* pObject );                   // EquipBackOver
    
		KDvoid					close ( Object* pObject );                      // EquipClose
    
	protected :

		enum EQUIP_TYPE
		{
			EQUIP_TYPE_NONE,
			EQUIP_TYPE_CLOTHES,
			EQUIP_TYPE_WEAPONS,
			EQUIP_TYPE_PETS,
		};
    
	protected :

		UILayer*				m_pUILayer;
    
		// equip
		EQUIP_TYPE				m_eEquipType;
    
		Dictionary*				m_pDicBeUsedSlot;
		Dictionary*				m_pDicClothesSlot;
		Dictionary*				m_pDicWeaponsSlot;
		Dictionary*				m_pDicPetsSlot;
    
		Dictionary*				m_pDicClothes;
		Dictionary*				m_pDicWeapons;
		Dictionary*				m_pDicPets;
    
		KDint					m_nContainer1Zorder;
		KDint					m_nContainer2Zorder;
		KDint					m_nContainer3Zorder;
    
		Point					m_tContainer1Position;
		Point					m_tContainer2Position;
		Point					m_tContainer3Position;
    
		Point					m_tMovePoint;
		Point					m_tLastPoint;
    
		Point					m_tWidgetLastWorldSpace;
		Point					m_tWidgetLastNodeSpace;
		UIWidget*				m_pLastWidgetParent;
};

#endif	// __EquipScene_h__
