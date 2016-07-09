/* -----------------------------------------------------------------------------------
 *
 *      File            CCCamera.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Ricardo Quesada
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

#include "2d/CCCamera.h"
#include "base/CCString.h"
#include "2d/draw_nodes/CCDrawingPrimitives.h"
#include "2d/CCDirector.h"
#include "XMKazmath/GL/matrix.h"

using namespace std;

NS_CC_BEGIN

Camera::Camera(void)
{
    init();
}

Camera::~Camera(void)
{
}

const char* Camera::description(void) const
{
    return String::createWithFormat("<Camera | center = (%.2f,%.2f,%.2f)>", m_fCenterX, m_fCenterY, m_fCenterZ)->getCString();
}

void Camera::init(void)
{
    restore();
}

void Camera::restore(void)
{
    m_fEyeX = m_fEyeY = 0.0f;
    m_fEyeZ = getZEye();

    m_fCenterX = m_fCenterY = m_fCenterZ = 0.0f;

    m_fUpX = 0.0f;
    m_fUpY = 1.0f;
    m_fUpZ = 0.0f;

    kmMat4Identity(&m_tLookupMatrix);

    m_bDirty = false;
}

void Camera::locate(void)
{
    if (m_bDirty)
    {
        kmVec3 eye, center, up;

        kmVec3Fill(&eye, m_fEyeX, m_fEyeY , m_fEyeZ);
        kmVec3Fill(&center, m_fCenterX, m_fCenterY, m_fCenterZ);

        kmVec3Fill(&up, m_fUpX, m_fUpY, m_fUpZ);
        kmMat4LookAt(&m_tLookupMatrix, &eye, &center, &up);

        m_bDirty = false;
    }
    kmGLMultMatrix(&m_tLookupMatrix);
}

float Camera::getZEye(void)
{
    return FLT_EPSILON;
}

void Camera::setEye(float eyeX, float eyeY, float eyeZ)
{
    m_fEyeX = eyeX;
    m_fEyeY = eyeY;
    m_fEyeZ = eyeZ;

    m_bDirty = true;
}

void Camera::setCenter(float centerX, float centerY, float centerZ)
{
    m_fCenterX = centerX;
    m_fCenterY = centerY;
    m_fCenterZ = centerZ;

    m_bDirty = true;
}

void Camera::setUp(float upX, float upY, float upZ)
{
    m_fUpX = upX;
    m_fUpY = upY;
    m_fUpZ = upZ;

    m_bDirty = true;
}

void Camera::getEye(float *eyeX, float *eyeY, float *eyeZ) const
{
    *eyeX = m_fEyeX;
    *eyeY = m_fEyeY;
    *eyeZ = m_fEyeZ;
}

void Camera::getCenter(float *centerX, float *centerY, float *centerZ) const
{
    *centerX = m_fCenterX;
    *centerY = m_fCenterY;
    *centerZ = m_fCenterZ;
}

void Camera::getUp(float *upX, float *upY, float *upZ) const
{
    *upX = m_fUpX;
    *upY = m_fUpY;
    *upZ = m_fUpZ;
}

NS_CC_END

