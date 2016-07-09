/* --------------------------------------------------------------------------
 *
 *      File            ScrGame.h
 *      Description     Game Scene
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

#ifndef __ScrGame_h__
#define __ScrGame_h__

class CWorld;
class CWeapon;
class CBullet;
class CTarget;
class CEnemies;

class CScrGame : public CCLayer
{
	public :

		CREATE_FUNC ( CScrGame );

	protected :

		KDbool  init ( KDvoid );

		KDvoid  onEnterTransitionDidFinish ( KDvoid );
        KDvoid  onExit ( KDvoid );

		KDvoid  draw ( KDvoid );
    
        KDbool  ccTouchBegan ( CCTouch* pTouch, CCEvent* pEvent );
        KDvoid  ccTouchMoved ( CCTouch* pTouch, CCEvent* pEvent );
        KDvoid  ccTouchEnded ( CCTouch* pTouch, CCEvent* pEvent );
    
        KDvoid  onEvent ( const KDEvent* pEvent );
    
		KDvoid  onNextMain ( CCObject* pSender );
    
        KDvoid  onPause  ( CCObject* pSender );
        KDvoid  onResume ( CCObject* pSender );
    
        KDvoid  onMode   ( CCObject* pSender );
    
        KDvoid  onReload ( CCObject* pSender );
        KDvoid  onChange ( CCObject* pSender );
        
    public :
    
        KDvoid  onPitch   ( KDfloat fPitch );
        KDvoid  onBearing ( KDfloat fBearing );
                           
        KDvoid  onRotate ( KDfloat fBearing, KDfloat fPitch );
    
        KDvoid  onSoundEffect ( CCNode* pTarget, KDvoid* pData );
                      
        KDvoid  setVibrate   ( KDbool bOn );
        KDvoid  setDamaged   ( KDbool bOn );
        KDvoid  setEmergency ( KDbool bOn );
               
        KDvoid  refreshBulletLabel ( KDuint uIndex );    
        KDvoid  refreshScore  ( KDvoid );    
        KDvoid  refreshEnergy ( KDvoid );
    
    protected :
    
        KDvoid  onRemove ( CCNode* pSender ); 
        KDvoid  onStart  ( CCNode* pSender );
    
        KDvoid  onFire  ( CCNode* pSender );
        KDvoid  onFired ( CCNode* pSender );
    
        KDvoid  onBullet ( KDuint uID, KDint uParam );
    
        
    
	private :

        CWorld*                         m_pWorld;
        CWeapon*                        m_pWeapon;
        CTarget*                        m_pTarget;
        CBullet*                        m_pBullet;
        CEnemies*                       m_pEnemies;

        KDuint                          m_uTime;        
        KDuint                          m_uBgm;

		CCLayer*                        m_uiLayer;    
        CCSprite*                       m_uiDamaged;
        CCSprite*                       m_uiEmergency;
    
        CCProgressTimer*                m_uiEnergy;
        CCLabelBMFont*                  m_uiScore;
        CCLabelBMFont*                  m_uiMode;
        CCSprite*                       m_uiRader;        
        CCSprite*                       m_uiWeapon;
        CCLabelBMFont*                  m_uiBulletNum[2];
        CCSprite*                       m_uiPad[2][3];
        CCSprite*                       m_uiTarget[2][2];
    
        CCTouch                         m_tWorldTouch;
        core::vector3df                 m_tWorldDelta;
        core::vector3df                 m_tCameraRotate;
    
        KDbool                          m_bTargetCtl;
        KDuint                          m_uWeaponIndex;    
        KDuint                          m_uBulletNum[2][2];
                
        KDbool                          m_bUseSensor;
    
        KDfloat                         m_fOffsetPitch;
        KDfloat                         m_fOffsetBearing;
        KDfloat                         m_fPrevPitch;     
        KDfloat                         m_fPrevBearing;
};

#endif
