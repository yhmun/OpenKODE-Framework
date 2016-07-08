/* -----------------------------------------------------------------------------------
 *
 *      File            CCTransitionPageTurn.cpp
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

#include "2d/layers_scenes_transitions_nodes/CCTransitionPageTurn.h"
#include "2d/CCDirector.h"
#include "2d/actions/CCActionInterval.h"
#include "2d/actions/CCActionInstant.h"
#include "2d/actions/CCActionGrid.h"
#include "2d/actions/CCActionPageTurn3D.h"

NS_CC_BEGIN

float TransitionPageTurn::POLYGON_OFFSET_FACTOR = -20.f;
float TransitionPageTurn::POLYGON_OFFSET_UNITS = -20.f;

TransitionPageTurn::TransitionPageTurn()
{
}

TransitionPageTurn::~TransitionPageTurn()
{
}

/** creates a base transition with duration and incoming scene */
TransitionPageTurn * TransitionPageTurn::create(float t, Scene *scene, bool backwards)
{
    TransitionPageTurn * pTransition = new TransitionPageTurn();
    pTransition->initWithDuration(t,scene,backwards);
    pTransition->autorelease();
    return pTransition;
}

/** initializes a transition with duration and incoming scene */
bool TransitionPageTurn::initWithDuration(float t, Scene *scene, bool backwards)
{
    // XXX: needed before [super init]
    m_bBack = backwards;

    if (TransitionScene::initWithDuration(t, scene))
    {
        // do something
    }
    return true;
}

void TransitionPageTurn::sceneOrder()
{
    m_bIsInSceneOnTop = m_bBack;
}

void TransitionPageTurn::draw()
{
    Scene::draw();
    
    if( m_bIsInSceneOnTop ) {
        m_pOutScene->visit();
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(POLYGON_OFFSET_FACTOR, POLYGON_OFFSET_UNITS);
        m_pInScene->visit();
        glDisable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(0, 0);
    } else {
        m_pInScene->visit();
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(POLYGON_OFFSET_FACTOR, POLYGON_OFFSET_UNITS);
        m_pOutScene->visit();
        glDisable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(0, 0);
    }
}

void TransitionPageTurn::onEnter()
{
    TransitionScene::onEnter();
    Size s = Director::getInstance()->getWinSize();
    int x,y;
    if (s.width > s.height)
    {
        x=16;
        y=12;
    }
    else
    {
        x=12;
        y=16;
    }

    ActionInterval *action  = this->actionWithSize(Size(x,y));

    if (! m_bBack )
    {
        m_pOutScene->runAction
        (
            Sequence::create
            (
                action,
                CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
                StopGrid::create(),
                NULL
            )
        );
    }
    else
    {
        // to prevent initial flicker
        m_pInScene->setVisible(false);
        m_pInScene->runAction
        (
            Sequence::create
            (
                Show::create(),
                action,
                CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
                StopGrid::create(),
                NULL
            )
        );
    }
}


ActionInterval* TransitionPageTurn:: actionWithSize(const Size& vector)
{
    if (m_bBack)
    {
        // Get hold of the PageTurn3DAction
        return ReverseTime::create
        (
            PageTurn3D::create(m_fDuration, vector)
        );
    }
    else
    {
        // Get hold of the PageTurn3DAction
        return PageTurn3D::create(m_fDuration, vector);
    }
}

NS_CC_END
