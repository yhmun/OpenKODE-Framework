/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionCamera.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008-2010 Ricardo Quesada
 *      Copyright (c) 2011      Zynga Inc.
 *
 *         http://www.cocos2d-x.org      
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
 * --------------------------------------------------------------------------------- */ 

#include "2d/actions/CCActionCamera.h"
#include "2d/CCCamera.h"

NS_CC_BEGIN
//
// CameraAction
//
void ActionCamera::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    
    Camera *camera = target->getCamera();
    camera->getCenter(&m_fCenterXOrig, &m_fCenterYOrig, &m_fCenterZOrig);
    camera->getEye(&m_fEyeXOrig, &m_fEyeYOrig, &m_fEyeZOrig);
    camera->getUp(&m_fUpXOrig, &m_fUpYOrig, &m_fUpZOrig);
}

ActionCamera* ActionCamera::clone() const
{
	// no copy constructor
	auto a = new ActionCamera();
	a->autorelease();
	return a;
}

ActionCamera * ActionCamera::reverse() const
{
    // FIXME: This conversion isn't safe.
    return (ActionCamera*)ReverseTime::create(const_cast<ActionCamera*>(this));
}
//
// OrbitCamera
//

OrbitCamera * OrbitCamera::create(float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX)
{
    OrbitCamera * obitCamera = new OrbitCamera();
    if(obitCamera->initWithDuration(t, radius, deltaRadius, angleZ, deltaAngleZ, angleX, deltaAngleX))
    {
        obitCamera->autorelease();
        return obitCamera;
    }
    CC_SAFE_DELETE(obitCamera);
    return NULL;
}

OrbitCamera* OrbitCamera::clone() const
{
	// no copy constructor	
	auto a = new OrbitCamera();
	a->initWithDuration(m_fDuration, m_fRadius, m_fDeltaRadius, m_fAngleZ, m_fDeltaAngleZ, m_fAngleX, m_fDeltaAngleX);
	a->autorelease();
	return a;
}

bool OrbitCamera::initWithDuration(float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX)
{
    if ( ActionInterval::initWithDuration(t) )
    {
        m_fRadius = radius;
        m_fDeltaRadius = deltaRadius;
        m_fAngleZ = angleZ;
        m_fDeltaAngleZ = deltaAngleZ;
        m_fAngleX = angleX;
        m_fDeltaAngleX = deltaAngleX;

        m_fRadDeltaZ = (float)CC_DEGREES_TO_RADIANS(deltaAngleZ);
        m_fRadDeltaX = (float)CC_DEGREES_TO_RADIANS(deltaAngleX);
        return true;
    }
    return false;
}

void OrbitCamera::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    float r, zenith, azimuth;
    this->sphericalRadius(&r, &zenith, &azimuth);
    if( kdIsNan(m_fRadius) )
        m_fRadius = r;
    if( kdIsNan(m_fAngleZ) )
        m_fAngleZ = (float)CC_RADIANS_TO_DEGREES(zenith);
    if( kdIsNan(m_fAngleX) )
        m_fAngleX = (float)CC_RADIANS_TO_DEGREES(azimuth);

    m_fRadZ = (float)CC_DEGREES_TO_RADIANS(m_fAngleZ);
    m_fRadX = (float)CC_DEGREES_TO_RADIANS(m_fAngleX);
}

void OrbitCamera::update(float dt)
{
    float r = (m_fRadius + m_fDeltaRadius * dt) * Camera::getZEye();
    float za = m_fRadZ + m_fRadDeltaZ * dt;
    float xa = m_fRadX + m_fRadDeltaX * dt;

    float i = kdSinf(za) * kdCosf(xa) * r + m_fCenterXOrig;
    float j = kdSinf(za) * kdSinf(xa) * r + m_fCenterYOrig;
    float k = kdCosf(za) * r + m_fCenterZOrig;

    m_pTarget->getCamera()->setEye(i,j,k);
}

void OrbitCamera::sphericalRadius(float *newRadius, float *zenith, float *azimuth)
{
    float ex, ey, ez, cx, cy, cz, x, y, z;
    float r; // radius
    float s;

    Camera* camera = m_pTarget->getCamera();
    camera->getEye(&ex, &ey, &ez);
    camera->getCenter(&cx, &cy, &cz);

    x = ex-cx;
    y = ey-cy;
    z = ez-cz;

    r = kdSqrtf ( kdPowf ( x, 2 ) + kdPowf ( y, 2 ) + kdPowf(z,2));
    s = kdSqrtf ( kdPowf ( x, 2 ) + kdPowf ( y, 2 ) );
    if( s == 0.0f )
        s = FLT_EPSILON;
    if(r==0.0f)
        r = FLT_EPSILON;

    *zenith = kdAcosf( z/r);
    if( x < 0 )
        *azimuth= KD_PI_F - kdAsinf(y/s);
    else
        *azimuth = kdAsinf(y/s);

    *newRadius = r / Camera::getZEye();                
}

NS_CC_END
