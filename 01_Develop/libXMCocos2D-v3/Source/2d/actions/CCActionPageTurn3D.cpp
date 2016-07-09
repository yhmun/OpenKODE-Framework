/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionPageTurn3D.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      Sindesso Pty Ltd 
 *
 *         http://www.cocos2d-x.org      
 *         http://www.sindesso.com  
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

#include "2d/actions/CCActionPageTurn3D.h"

NS_CC_BEGIN

PageTurn3D* PageTurn3D::create(float duration, const Size& gridSize)
{
    PageTurn3D *pAction = new PageTurn3D();

    if (pAction)
    {
        if (pAction->initWithDuration(duration, gridSize))
        {
            pAction->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE(pAction);
        }
    }

    return pAction;
}

PageTurn3D *PageTurn3D::clone() const
{
	// no copy constructor	
	auto a = new PageTurn3D();
	a->initWithDuration(m_fDuration, m_tGridSize);
	a->autorelease();
	return a;
}

/*
 * Update each tick
 * Time is the percentage of the way through the duration
 */
void PageTurn3D::update(float time)
{
    float tt = KD_MAX(0, time - 0.25f);
    float deltaAy = (tt * tt * 500);
    float ay = -100 - deltaAy;
    
    float deltaTheta = - KD_PI_2_F * kdSqrtf( time) ;
    float theta = /*0.01f */ + KD_PI_2_F +deltaTheta;
    
    float sinTheta = kdSinf(theta);
    float cosTheta = kdCosf(theta);
    
    for (int i = 0; i <= m_tGridSize.width; ++i)
    {
        for (int j = 0; j <= m_tGridSize.height; ++j)
        {
            // Get original vertex
            Vertex3F p = getOriginalVertex(Point(i ,j));
            
            float R = kdSqrtf((p.x * p.x) + ((p.y - ay) * (p.y - ay)));
            float r = R * sinTheta;
            float alpha = kdAsinf( p.x / R );
            float beta = alpha / sinTheta;
            float cosBeta = kdCosf( beta );
            
            // If beta > PI then we've wrapped around the cone
            // Reduce the radius to stop these points interfering with others
            if (beta <= KD_PI_F)
            {
                p.x = ( r * kdSinf(beta));
            }
            else
            {
                // Force X = 0 to stop wrapped
                // points
                p.x = 0;
            }

            p.y = ( R + ay - ( r * (1 - cosBeta) * sinTheta));

            // We scale z here to avoid the animation being
            // too much bigger than the screen due to perspective transform
            p.z = (r * ( 1 - cosBeta ) * cosTheta) / 7;// "100" didn't work for

            //    Stop z coord from dropping beneath underlying page in a transition
            // issue #751
            if( p.z < 0.5f )
            {
                p.z = 0.5f;
            }
            
            // Set new coords
            setVertex(Point(i, j), p);
            
        }
    }
}

NS_CC_END
