/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionGrid.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2009      On-Core
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

#include "2d/actions/CCActionGrid.h"
#include "2d/CCDirector.h"
#include "2d/effects/CCGrid.h"

NS_CC_BEGIN
// implementation of GridAction

bool GridAction::initWithDuration(float duration, const Size& gridSize)
{
    if (ActionInterval::initWithDuration(duration))
    {
        m_tGridSize = gridSize;

        return true;
    }

    return false;
}

void GridAction::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);

    GridBase *newgrid = this->getGrid();

    Node *t = m_pTarget;
    GridBase *targetGrid = t->getGrid();

    if (targetGrid && targetGrid->getReuseGrid() > 0)
    {
        if (targetGrid->isActive() && targetGrid->getGridSize().width == m_tGridSize.width
            && targetGrid->getGridSize().height == m_tGridSize.height /*&& dynamic_cast<GridBase*>(targetGrid) != NULL*/)
        {
            targetGrid->reuse();
        }
        else
        {
            CCASSERT(0, "");
        }
    }
    else
    {
        if (targetGrid && targetGrid->isActive())
        {
            targetGrid->setActive(false);
        }

        t->setGrid(newgrid);
        t->getGrid()->setActive(true);
    }
}

GridAction* GridAction::reverse() const
{
    // FIXME: This conversion isn't safe.
	return (GridAction*)ReverseTime::create( this->clone() );
}

GridBase* GridAction::getGrid()
{
    // Abstract class needs implementation
    CCASSERT(0, "");

    return nullptr;
}

// implementation of Grid3DAction

GridBase* Grid3DAction::getGrid()
{
    return Grid3D::create(m_tGridSize);
}

Vertex3F Grid3DAction::getVertex(const Point& position) const
{
    Grid3D *g = (Grid3D*)m_pTarget->getGrid();
    return g->getVertex(position);
}

Vertex3F Grid3DAction::getOriginalVertex(const Point& position) const
{
    Grid3D *g = (Grid3D*)m_pTarget->getGrid();
    return g->getOriginalVertex(position);
}

void Grid3DAction::setVertex(const Point& position, const Vertex3F& vertex)
{
    Grid3D *g = (Grid3D*)m_pTarget->getGrid();
    g->setVertex(position, vertex);
}

// implementation of TiledGrid3DAction

GridBase* TiledGrid3DAction::getGrid(void)
{
    return TiledGrid3D::create(m_tGridSize);
}

Quad3 TiledGrid3DAction::getTile(const Point& pos) const
{
    TiledGrid3D *g = (TiledGrid3D*)m_pTarget->getGrid();
    return g->getTile(pos);
}

Quad3 TiledGrid3DAction::getOriginalTile(const Point& pos) const
{
    TiledGrid3D *g = (TiledGrid3D*)m_pTarget->getGrid();
    return g->getOriginalTile(pos);
}

void TiledGrid3DAction::setTile(const Point& pos, const Quad3& coords)
{
    TiledGrid3D *g = (TiledGrid3D*)m_pTarget->getGrid();
    return g->setTile(pos, coords);
}

// implementation AccelDeccelAmplitude

AccelDeccelAmplitude* AccelDeccelAmplitude::create(Action *action, float duration)
{
    AccelDeccelAmplitude *ret = new AccelDeccelAmplitude();
    if (ret)
    {
        if (ret->initWithAction(action, duration))
        {
            ret->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(ret);
        }
    }

    return ret;
}

bool AccelDeccelAmplitude::initWithAction(Action *action, float duration)
{
    if (ActionInterval::initWithDuration(duration))
    {
        m_fRate = 1.0f;
        m_pOther = (ActionInterval*)(action);
        action->retain();

        return true;
    }

    return false;
}

AccelDeccelAmplitude* AccelDeccelAmplitude::clone() const
{
	// no copy constructor
	auto a = new AccelDeccelAmplitude();
	a->initWithAction(m_pOther->clone(), m_fRate);
	a->autorelease();
	return a;
}

AccelDeccelAmplitude::~AccelDeccelAmplitude()
{
    CC_SAFE_RELEASE(m_pOther);
}

void AccelDeccelAmplitude::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    m_pOther->startWithTarget(target);
}

void AccelDeccelAmplitude::update(float time)
{
    float f = time * 2;

    if (f > 1)
    {
        f -= 1;
        f = 1 - f;
    }

    ((AccelDeccelAmplitude*)(m_pOther))->setAmplitudeRate(kdPowf(f, m_fRate));
}

AccelDeccelAmplitude* AccelDeccelAmplitude::reverse() const
{
    return AccelDeccelAmplitude::create(m_pOther->reverse(), m_fDuration);
}

// implementation of AccelAmplitude

AccelAmplitude* AccelAmplitude::create(Action *action, float duration)
{
    AccelAmplitude *ret = new AccelAmplitude();
    if (ret)
    {
        if (ret->initWithAction(action, duration))
        {
            ret->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(ret);
        }
    }

    return ret;
}

bool AccelAmplitude::initWithAction(Action *action, float duration)
{
    if (ActionInterval::initWithDuration(duration))
    {
        m_fRate = 1.0f;
        m_pOther = (ActionInterval*)(action);
        action->retain();

        return true;
    }

    return false;
}

AccelAmplitude* AccelAmplitude::clone() const
{
	// no copy constructor
	auto a = new AccelAmplitude();
	a->initWithAction(m_pOther->clone(), m_fDuration);
	a->autorelease();
	return a;
}

AccelAmplitude::~AccelAmplitude(void)
{
    CC_SAFE_DELETE(m_pOther);
}

void AccelAmplitude::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    m_pOther->startWithTarget(target);
}

void AccelAmplitude::update(float time)
{
    ((AccelAmplitude*)(m_pOther))->setAmplitudeRate(kdPowf(time, m_fRate));
    m_pOther->update(time);
}

AccelAmplitude* AccelAmplitude::reverse() const
{
    return AccelAmplitude::create(m_pOther->reverse(), m_fDuration);
}

// DeccelAmplitude

DeccelAmplitude* DeccelAmplitude::create(Action *action, float duration)
{
    DeccelAmplitude *ret = new DeccelAmplitude();
    if (ret)
    {
        if (ret->initWithAction(action, duration))
        {
            ret->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(ret);
        }
    }

    return ret;
}

bool DeccelAmplitude::initWithAction(Action *action, float duration)
{
    if (ActionInterval::initWithDuration(duration))
    {
        m_fRate = 1.0f;
        m_pOther = (ActionInterval*)(action);
        action->retain();

        return true;
    }

    return false;
}

DeccelAmplitude::~DeccelAmplitude()
{
    CC_SAFE_RELEASE(m_pOther);
}

void DeccelAmplitude::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    m_pOther->startWithTarget(target);
}

void DeccelAmplitude::update(float time)
{
    ((DeccelAmplitude*)(m_pOther))->setAmplitudeRate(kdPowf((1 - time), m_fRate));
    m_pOther->update(time);
}

DeccelAmplitude* DeccelAmplitude::clone() const
{
	// no copy constructor	
	auto a = new DeccelAmplitude();
	a->initWithAction(m_pOther->clone(), m_fDuration);
	a->autorelease();
	return a;
}

DeccelAmplitude* DeccelAmplitude::reverse() const
{
    return DeccelAmplitude::create(m_pOther->reverse(), m_fDuration);
}

// implementation of StopGrid

void StopGrid::startWithTarget(Node *target)
{
    ActionInstant::startWithTarget(target);

    GridBase *grid = m_pTarget->getGrid();
    if (grid && grid->isActive())
    {
        grid->setActive(false);
    }
}

StopGrid* StopGrid::create()
{
    StopGrid* pAction = new StopGrid();
    pAction->autorelease();

    return pAction;
}

StopGrid* StopGrid::clone() const
{
	return StopGrid::create();
}

StopGrid* StopGrid::reverse() const
{
	// no reverse, just clone it
	return this->clone();
}

// implementation of ReuseGrid

ReuseGrid* ReuseGrid::create(int times)
{
    ReuseGrid *action = new ReuseGrid();
    if (action)
    {
        if (action->initWithTimes(times))
        {
            action->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(action);
        }
    }

    return action;
}

bool ReuseGrid::initWithTimes(int times)
{
    m_nTimes = times;

    return true;
}

void ReuseGrid::startWithTarget(Node *target)
{
    ActionInstant::startWithTarget(target);

    if (m_pTarget->getGrid() && m_pTarget->getGrid()->isActive())
    {
        m_pTarget->getGrid()->setReuseGrid(m_pTarget->getGrid()->getReuseGrid() + m_nTimes);
    }
}

ReuseGrid* ReuseGrid::clone() const
{
	return ReuseGrid::create(m_nTimes);
}

ReuseGrid* ReuseGrid::reverse() const
{
	// no reverse, just clone it
	return this->clone();
}

NS_CC_END
