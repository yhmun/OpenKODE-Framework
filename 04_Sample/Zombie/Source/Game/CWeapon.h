/* --------------------------------------------------------------------------
 *
 *      File            CWeapon.h
 *      Description     Game Weapon
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

#ifndef __CWeapon_h__
#define __CWeapon_h__

class CWeapon : public scene::IAnimationEndCallBack
{
    public :
    
        enum EAction
        {
            eActLoad    ,
            eActReload  ,
            eActHold    ,
            eActFire    ,  
            eActFired   ,
        };
    
    public :
    
        CWeapon ( KDvoid );
        virtual ~CWeapon ( KDvoid );
    
    public :
    
        KDbool  init ( KDvoid );
    
        KDbool  load ( KDuint uType = 0 );
    
        KDvoid  onUpdate ( KDuint uTime );
    
        EAction getAction ( KDuint uIndex );
        KDvoid  setAction ( EAction eAction, KDuint uIndex );
        
        KDvoid  onAnimate ( KDuint uTime );
    
        scene::IAnimatedMeshSceneNode*  getNode ( KDuint uIndex );
    
        KDuint  getFireDelay  ( KDvoid );
        KDuint  getFireRepeat ( KDvoid );        
    
        KDuint  getBulletNum  ( KDuint uType );      
    
    protected :
    
        virtual KDvoid OnAnimationEnd ( scene::IAnimatedMeshSceneNode* pNode );
    
        KDvoid  onSound ( KDuint uIndex );
    
        KDvoid  clear ( KDvoid );
    
    private :
    
        KDuint                          m_uType;
        KDuint                          m_uTime;  
        scene::IAnimatedMeshSceneNode*  m_pNodes[2]; 
        scene::ISceneNode*              m_pFireNodes[2];
        CSound*                         m_pSounds[2][2];
        EAction                         m_eAction[2];
        KDuint                          m_uActDone[2];    
        KDuint                          m_uSndTime[2];
        KDbool                          m_bSndFire[2];
};

#endif
