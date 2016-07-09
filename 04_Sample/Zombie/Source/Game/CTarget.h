/* --------------------------------------------------------------------------
 *
 *      File            CTarget.h
 *      Description     Game Target
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

#ifndef __CTarget_h__
#define __CTarget_h__

class CTarget
{
    public :
    
         CTarget ( KDvoid );
        ~CTarget ( KDvoid );
    
    public :
    
        KDbool  init ( scene::IAnimatedMeshSceneNode* pWorld );
    
        KDvoid  onUpdate ( KDuint uTime, const core::position2di& tPoint1, const core::position2di& tPoint2 );
    
        scene::ISceneNode*  getTarget ( KDuint uIndex );
    
        const core::line3df&  getRayLine ( KDuint uIndex );
        
        const core::triangle3df&  getTriangle ( KDuint uIndex );
    
    private :
    
        scene::ITriangleSelector*       m_pSelector;
        scene::ISceneNode*              m_pArms[2];
        scene::IBillboardSceneNode*     m_pNodes[2];        
        scene::ISceneNode*              m_pTagets[2];
        core::line3df                   m_tRays[2];
        core::triangle3df               m_tTris[2];
};

#endif
