/* --------------------------------------------------------------------------
 *
 *      File            Controller.cpp
 *      Description     Application Entrance.
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
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

#ifndef __Controller_h__
#define __Controller_h__

class Controller : public CCLayer
{
	public :

		CREATE_FUNC ( Controller );

	protected :

		virtual KDbool      init ( KDvoid );

		virtual KDvoid      onExit ( KDvoid );
		virtual KDvoid      draw ( KDvoid );

		virtual KDbool      ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid      ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
		virtual KDvoid      ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );

        virtual KDvoid      onEvent ( const KDEvent* pEvent );
    
		KDvoid      onRestart ( CCObject* pSender );
		KDvoid      onPrev    ( CCObject* pSender );
		KDvoid      onNext    ( CCObject* pSender );
    
        KDvoid      onUseMagnetometer ( CCObject* pSender );
    
		KDvoid      setPanorama ( KDint nIdx );
    
	private :

        CCLabelTTF*     m_pLog;
		CCSprite*		m_pJoypad;
		CCPoint			m_tPosition;
		CCSize			m_tIncrease;
		KDbool			m_bTouched;
		KDbool			m_bMagnetometer;

		XMGTexture*		m_pTexture;
		XMGSphere*		m_pSphere;
		XMGView*		m_pView;
		KDfloat			m_fLon;
		KDfloat			m_fLat;
		KDint		    m_nIdx;
};

#endif