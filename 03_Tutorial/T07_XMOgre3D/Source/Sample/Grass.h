/* ------------------------------------------------------------------------------------
 *
 *      File            Grass.h
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

#ifndef __Grass_h__
#define __Grass_h__

#include "../SdkSample.h"

class Sample_Grass : public SdkSample
{
    struct GrassVertex
	{
        KDfloat  x, y, z;
        KDfloat  nx, ny, nz;
        KDfloat  u, v;
    };
    
    public :
    
        Sample_Grass ( KDvoid );
    
        bool    frameRenderingQueued ( const FrameEvent& tEvent );
    
    protected :
    
        KDvoid  setupContent ( KDvoid );
    
        KDvoid  createGrassMesh ( KDvoid );
    
        KDvoid  setupLighting ( KDvoid );
    
        KDvoid  waveGrass ( Real fTimeElapsed );
    
        KDvoid  cleanupContent ( KDvoid );
    
       protected :
    
        const Real          GRASS_WIDTH;
        const Real          GRASS_HEIGHT;
        StaticGeometry*     m_pField;
        AnimationState*     m_pLightAnimState;
        Controller<Real>*   m_pLightController;
};

/**
 *  This class will be used to pulsate the light and billboard.
 */
class LightPulse : public ControllerValue<Real>
{
    public :
    
        LightPulse ( Light* pLight, Billboard* pBillboard, const ColourValue& tMaxColour, Real fMaxSize );

        Real    getValue ( KDvoid ) const;    
        KDvoid  setValue ( Real fValue );
    
    protected :
    
        Light*          m_pLight;
        Billboard*      m_pBillboard;
        ColourValue     m_tMaxColour;
        Real            m_fMaxSize;
        Real            m_fIntensity;
};

#endif // __SkyPlane_h__
