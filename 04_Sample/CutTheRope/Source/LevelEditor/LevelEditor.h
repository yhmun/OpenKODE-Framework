/* --------------------------------------------------------------------------
 *
 *      File            LevelEditor.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2013      guanghui on 5/11/13. All rights reserved.
 *
 *         https://github.com/andyque/CutTheRope-x.git
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

#ifndef __LevelEditor_h__
#define __LevelEditor_h__

#include "PopupMenu.h"
#include "RopeSprite.h"

USING_NS_CC;

enum EEditorMode
{
    kEditMode,
    kRopeAnchorPineappleMode,
    kRopeAnchorAnyMode
};

class LevelFileHelper;
class PineappleModel;
class RopeModel;

class LevelEditor : public CCLayer, public PopupMenuDelegate
{
	private :
				 LevelEditor ( KDvoid );
	public :
				 LevelEditor ( LevelFileHelper* pLevelHelper );
		virtual ~LevelEditor ( KDvoid );

		static CCScene*		createWithLevel ( LevelFileHelper* pLevelHelper );
	    
	public :

		virtual KDbool		init ( KDvoid );
	    
		virtual KDvoid		ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent );
		virtual KDvoid		ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent );

		virtual KDvoid		createPineappleAt ( CCPoint tPosition );
		virtual KDvoid		createRopeAt	  ( CCPoint tPosition );
	    
		KDvoid				createMenu ( KDvoid );
		KDvoid				resetLevel ( CCObject* pSender );
		KDvoid				saveLevel  ( CCObject* pSender );
		KDvoid				playLevel  ( CCObject* pSender );

		KDvoid				drawLoadedLevel ( KDvoid );

		KDvoid				createPineappleSpriteFromModel ( PineappleModel* pModel );
		KDvoid				createRopeSpriteFromModel	   ( RopeModel*	pModel );
	    	    
		KDvoid				togglePopupMenu ( CCPoint tTouchLocation );
	    
		KDvoid				setMode ( EEditorMode eMode );
		
		CCSprite*			pineappleAtPosition ( CCPoint tPos );

		KDvoid				selectFirstAnchorPoint  ( CCPoint touchLocation );
		KDvoid				selectSecondAnchorPoint ( CCPoint touchLocation );
	    
	    
		RopeSprite*			ropeAtPosition ( CCPoint tPosition );
		KDvoid				removeRopeAtPosition ( CCPoint tPosition );
		KDvoid				removePineappleAtPosition ( CCPoint tPosition );

		CCArray*			getAllRopesConnectedToPineappleWithID ( KDint nPineappleID );
		CCArray*			getAllRopesConnectedToPineappleWithID ( KDint nPineappleID, KDbool bSetSelectedAnchor );
	    
		KDvoid				selectRopeAnchor ( CCPoint tTouchLocation );
		KDvoid				selectPineapple  ( CCPoint tTouchLocation );
	    
		KDvoid				longPress ( CCPoint tPos );

	private :
	    
		LevelFileHelper*		m_pFileHandler;
		CCSprite*				m_pBackground;
		CCSpriteBatchNode*		m_pPineapplesSpriteSheet;
		CCSpriteBatchNode*		m_pRopeSpriteSheet;
	    
		CCArray*				m_pRopeSpritesArray;
		PopupMenu*				m_pPopupMenu;
	    
		EEditorMode             m_eMode;
		RopeModel*              m_pNewRope;
	    
		CCObject*               m_pSelectedObject;
		CCPoint                 m_tOriginalRopeAnchor;
		CCArray*				m_pConnectedRopes;
};

#endif	// __LevelEditor_h__
