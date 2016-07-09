/* ------------------------------------------------------------------------------------
 *
 *      File            Lighting.h
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

#ifndef __Lighting_h__
#define __Lighting_h__

#include "../SdkSample.h"

class Sample_Lighting : public SdkSample, public RenderObjectListener
{
    public :
    
        Sample_Lighting ( KDvoid );
    
        bool    frameRenderingQueued ( const FrameEvent& tEvent );
    
    protected :
    
        KDvoid  testCapabilities ( const RenderSystemCapabilities* pCaps );
    
        KDvoid  setupContent ( KDvoid );
    
        KDvoid  setupLights ( KDvoid );
    
        // Event raised when render single object started.
        virtual KDvoid  notifyRenderSingleObject ( Renderable* pRend, const Pass* pPass, const AutoParamDataSource* pSource,
                                                   const LightList* pLightList, bool bSuppressRenderStateChanges );
    
        KDvoid  cleanupContent ( KDvoid );
    
    protected :
    
        AnimationState*             m_pGreenLightAnimState;
        AnimationState*             m_pYellowLightAnimState;
    
        RibbonTrail*                m_pTrail;
    
        Billboard*                  m_pLight1BBFlare;
        BillboardSet*               m_pLight1BBQueryArea;
        BillboardSet*               m_pLight1BBQueryVisible;
        Billboard*                  m_pLight2BBFlare;
        BillboardSet*               m_pLight2BBQueryArea;
        BillboardSet*               m_pLight2BBQueryVisible;
    
        HardwareOcclusionQuery*     m_pLight1QueryArea;
        HardwareOcclusionQuery*     m_pLight1QueryVisible;
        HardwareOcclusionQuery*     m_pLight2QueryArea;
        HardwareOcclusionQuery*     m_pLight2QueryVisible;
        HardwareOcclusionQuery*     m_pActiveQuery;
    
        bool                        m_bUseOcclusionQuery;
        bool                        m_bDoOcclusionQuery;
};

#endif // __Lighting_h__
