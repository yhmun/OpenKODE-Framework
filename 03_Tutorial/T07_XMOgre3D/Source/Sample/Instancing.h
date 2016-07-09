/* ------------------------------------------------------------------------------------
 *
 *      File            Instancing.h
 *      Description     This source file is part of OGRE
 *                      (Object-oriented Graphics Rendering Engine)
 *      Author          Y.H Mun
 *
 * ------------------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
 *
 *      Contact Email: xmsoft77@gmail.com
 *
 * ------------------------------------------------------------------------------------
 *
 *      Copyright (c) 2010-2012 Torus Knot Software Ltd. 
 *
 *      For the latest info, see http://www.ogre3d.org/
 *
 * ------------------------------------------------------------------------------------
 *
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ------------------------------------------------------------------------------------ */

#ifndef __Instancing_h__
#define __Instancing_h__

#include "../SdkSample.h"

class Sample_Instancing : public SdkSample
{
    public :
    
        enum CurrentGeomOpt
        {
            INSTANCE_OPT    ,
            STATIC_OPT      ,
            ENTITY_OPT      ,
        };
    
        Sample_Instancing ( KDvoid );
    
    protected :
    
        KDvoid  setupContent ( KDvoid );
    
    protected :
/*
        KDdouble                            m_dAvgFrameTime;
        KDsize                              m_uSelectedMesh;
        KDsize                              m_uNumMeshes;
        KDsize                              m_uObjectCount;
        String                              m_sDebugText;
        CurrentGeomOpt                      m_eCurrentGeomOpt;
    
        KDsize                              m_uNumRendered;
    
        Timer*                              m_pTimer;
        KDdouble                            m_dLastTime;
        KDdouble                            m_dBurnAmount;
    
        vector<InstancedGeometry*>::type    m_aRenderInstance;
        vector<StaticGeometry*>::type       m_aRenderStatic;
        vector<Entity*>::type               m_aRenderEntity;
        vector<SceneNode*>::type            m_aNodes;
        vector<Vector3*>::type              m_aPosMatrices;
*/ 
};

#endif // __Instancing_h__
