/* -----------------------------------------------------------------------------------
 *
 *      File            MobCasinoStatus.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft  
 *      Copyright (c) 2011      FOWCOM. All rights reserved.
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

#ifndef __MobCasinoStatus_h__
#define __MobCasinoStatus_h__

#include "Object/Sprite/Sprite.h"
#include "Object/Sprite/SprAni.h"
#include "Object/Sprite/SprAnis.h"
#include "Object/Label/Labels.h"
#include "Object/Label/LabelAtlas.h"
#include "Object/Label/LabelAtlasCustom.h"
#include "Unit/Unit/Unit.h"
//#include <queue>

typedef struct _sLabelInt
{
	KDint				nSub;
	LabelTTF*			pLabel;
} sLabelInt;

class cStatusIcons : public Object
{
	public :

		cStatusIcons ( KDvoid )
		{

		}

		virtual ~cStatusIcons ( KDvoid )
		{
			for ( auto pNode : aIcons )
			{
				CC_SAFE_RELEASE ( pNode );
			}
			aIcons.clear ( );
		}

		std::vector<cSprAni*>		aIcons;
};

class cMobCasinoStatus : public Object
{
	public :
		// int & destroy
		cMobCasinoStatus ( Node* pNode, const std::vector<cUnit*>& rMobs, eModeType eMode, KDint nBetSeatIndex );
		virtual ~cMobCasinoStatus ( KDvoid );

		KDvoid							update ( const std::vector<cUnit*>& rMobs, const std::vector<KDfloat>& rActiveGages );

		KDvoid							setParaSub ( cUnit* pMob );
		KDvoid							setModeStatus ( KDvoid );
		KDvoid							setModeMagnification ( KDvoid );


	private :

		// scene
		Node*							m_pNode;

		//active gage
		std::vector<cSprite*>			m_aActiveBars;
		std::vector<cSprite*>			m_aActiveBarEnds;

		// labels
		cLabels*						m_pLabels;

		// parameter labels
		std::vector<sLabelInt>			m_aLabelHPs;
		std::vector<sLabelInt>			m_aLabelATKs;
		std::vector<sLabelInt>			m_aLabelDEFs;
		std::vector<sLabelInt>			m_aLabelAGLs;
		std::vector<sLabelInt>			m_aLabelCRTs;

		// status icons
		std::vector<cStatusIcons*>		m_aStatusIcons;

		// status cover color layer
		std::vector<LayerColor*>		m_aColorStatusCovers;

		// portrait
		std::vector<cSprAnis*>			m_aSprPortraits;

		//label magnifications
		std::vector<cLabelAtlas*>		m_pUIMobBarMagnifications;
		std::vector<cLabelAtlas*>		m_pUIMobBarMagnifications2;
		std::vector<cSprite*>			m_pUIMobBarMagnifications3;
		std::vector<cSprite*>			m_pUIMobBarMagnifications4;

		// survival kill count
		cSprite*						m_pUIKillFont;
		cLabelAtlasCustom*				m_pUIKillNum;
	
		// mode type
		eModeType						m_eModeType;
};


#endif	// __MobCasinoStatus_h__
