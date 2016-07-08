/* -----------------------------------------------------------------------------------
 *
 *      File            CCCamera.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "support/CCCamera.h"
#include "support/CCDirector.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "cocoa/CCString.h"
#include "kazmath/GL/matrix.h"

NS_CC_BEGIN
    
CCCamera::CCCamera ( KDvoid )
{
	init ( );
}

CCCamera::~CCCamera ( KDvoid )
{

}

const KDchar* CCCamera::description ( KDvoid )
{
    return CCString::createWithFormat ( "<CCCamera | center = (%.2f,%.2f,%.2f)>", m_fCenterX, m_fCenterY, m_fCenterZ )->getCString ( );
}

KDbool CCCamera::init ( KDvoid )
{
	restore ( );
	return KD_TRUE;
}

KDvoid CCCamera::restore ( KDvoid )
{
	m_fEyeX = m_fEyeY = 0.0f;
	m_fEyeZ = getZEye ( );

	m_fCenterX = m_fCenterY = m_fCenterZ = 0.0f;

	m_fUpX = 0.0f;
	m_fUpY = 1.0f;
	m_fUpZ = 0.0f;

	kmMat4Identity ( &m_tLookupMatrix );

	m_bDirty = KD_FALSE;
}

KDvoid CCCamera::locate ( KDvoid )
{
	if ( m_bDirty )
    {
        kmVec3  tEye, tCenter, tUp;

        kmVec3Fill ( &tEye, m_fEyeX, m_fEyeY , m_fEyeZ );
        kmVec3Fill ( &tCenter, m_fCenterX, m_fCenterY, m_fCenterZ );
        kmVec3Fill ( &tUp, m_fUpX, m_fUpY, m_fUpZ);

        kmMat4LookAt ( &m_tLookupMatrix, &tEye, &tCenter, &tUp );

        m_bDirty = false;
    }

    kmGLMultMatrix( &m_tLookupMatrix );
}

KDfloat CCCamera::getZEye ( KDvoid )
{
	return KD_FLT_EPSILON;
}

KDvoid CCCamera::setEyeXYZ ( KDfloat fEyeX, KDfloat fEyeY, KDfloat fEyeZ )
{
	m_fEyeX = fEyeX;
	m_fEyeY = fEyeY;
	m_fEyeZ = fEyeZ;

	m_bDirty = KD_TRUE;
}

KDvoid CCCamera::setCenterXYZ ( KDfloat fCenterX, KDfloat fCenterY, KDfloat fCenterZ )
{
	m_fCenterX = fCenterX;
	m_fCenterY = fCenterY;
	m_fCenterZ = fCenterZ;

	m_bDirty = KD_TRUE;
}

KDvoid CCCamera::setUpXYZ ( KDfloat fUpX, KDfloat fUpY, KDfloat fUpZ )
{
	m_fUpX = fUpX;
	m_fUpY = fUpY;
	m_fUpZ = fUpZ;

	m_bDirty = KD_TRUE;
}

KDvoid CCCamera::getEyeXYZ ( KDfloat* pEyeX, KDfloat* pEyeY, KDfloat* pEyeZ )
{
	*pEyeX = m_fEyeX;
	*pEyeY = m_fEyeY;
	*pEyeZ = m_fEyeZ;
}

KDvoid CCCamera::getCenterXYZ ( KDfloat* pCenterX, KDfloat* pCenterY, KDfloat* pCenterZ )
{
	*pCenterX = m_fCenterX;
	*pCenterY = m_fCenterY;
	*pCenterZ = m_fCenterZ;
}

KDvoid CCCamera::getUpXYZ ( KDfloat* pUpX, KDfloat* pUpY, KDfloat* pUpZ )
{
	*pUpX = m_fUpX;
	*pUpY = m_fUpY;
	*pUpZ = m_fUpZ;
}

    
NS_CC_END