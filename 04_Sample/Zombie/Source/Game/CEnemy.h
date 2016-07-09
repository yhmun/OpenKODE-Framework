/* --------------------------------------------------------------------------
 *
 *      File            CEnemy.h
 *      Description     Game Enemy
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

#ifndef __CEnemy_h__
#define __CEnemy_h__

class CEnemy : public scene::IAnimationEndCallBack
{    
    public :
    
        enum EAction
        {
            eActSpawn       ,
            eActHold        ,
            eActWalk        ,
            eActDead        ,
            eActAttack      ,
            eActDamage      ,
            eActDisappear   ,
        };
    
    public :
    
        CEnemy ( KDvoid );
        virtual ~CEnemy ( KDvoid );
        
        KDbool  load ( KDuint uType, KDfloat fDir, KDfloat fDist, CCSprite* pRaderBG );
/*    
        KDvoid           setPosition ( core::vector3df tPoint );
        core::vector3df  getPosition ( KDvoid );
    
        KDvoid           setRotate ( core::vector3df tRotate );
        core::vector3df  getRotate ( KDvoid );
    
        KDvoid           setScale ( core::vector3df tScale );
        core::vector3df  getScale ( KDvoid );
*/                
        KDfloat  getCos ( KDvoid );
        KDfloat  getSin ( KDvoid );
    
        KDfloat  getDistance  ( KDvoid );
    
        KDfloat  getDirection ( KDvoid );
    
        KDvoid  setDamage ( KDuint uDamage );
    
        KDbool  onUpdate ( KDuint uTime );
    
    protected :
    
        KDvoid  setAction ( EAction eAction );
    
        virtual KDvoid  OnAnimationEnd ( scene::IAnimatedMeshSceneNode* pNode );
    
    private :
    
        KDvoid  loadSpawn ( KDvoid );

    private :
    
        scene::ISceneNodeAnimator*          m_pAniMove;
    
        scene::IAnimatedMeshSceneNode*      m_pNode;
        KDuint                              m_uType;
    
        KDfloat                             m_fDir;
        KDfloat                             m_fDist;
        KDfloat                             m_fCos;
        KDfloat                             m_fSin;
    
        EAction                             m_eAction;
        EAction                             m_eActReady;
        KDuint                              m_uAniBegin;
        KDuint                              m_uAniEnd;   
    
        KDuint                              m_uFrames;
        KDuint                              m_uTime;
        KDuint                              m_uEnergy;
    
        CCSprite*                           m_pRaderBG;
        CCSprite*                           m_pRader;
};

class CEnemies
{
    public :
    
         CEnemies ( KDvoid );
        ~CEnemies ( KDvoid );
    
    public :
        
        KDbool  init ( const core::vector3df& tWorldPos, KDfloat fSpawnDist, CCSprite* pRaderBG );
    
        KDvoid  onUpdate ( KDuint uTime );
    
    public :
    
        KDfloat                 m_fSpawnDist;
        KDuint                  m_uSpawnTime;
        KDuint                  m_uSpawnDirMax;
        core::array<KDfloat>    m_fSpawnDirs;
        core::array<CEnemy*>    m_aEnemies;       
        CCSprite*               m_pRaderBG;
};

#endif
