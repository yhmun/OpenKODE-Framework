/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionTiledGrid.cpp
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

#include "2d/actions/CCActionTiledGrid.h"
#include "2d/CCDirector.h"
#include "2d/effects/CCGrid.h"

NS_CC_BEGIN

struct Tile
{
    Point    position;
    Point    startPosition;
    Size    delta;
};

// implementation of ShakyTiles3D

ShakyTiles3D* ShakyTiles3D::create(float duration, const Size& gridSize, int nRange, bool bShakeZ)
{
    ShakyTiles3D *pAction = new ShakyTiles3D();

    if (pAction)
    {
        if (pAction->initWithDuration(duration, gridSize, nRange, bShakeZ))
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

bool ShakyTiles3D::initWithDuration(float duration, const Size& gridSize, int nRange, bool bShakeZ)
{
    if (TiledGrid3DAction::initWithDuration(duration, gridSize))
    {
        m_nRandrange = nRange;
        m_bShakeZ = bShakeZ;

        return true;
    }

    return false;
}

ShakyTiles3D* ShakyTiles3D::clone() const
{
	// no copy constructor	
	auto a = new ShakyTiles3D();
    a->initWithDuration(m_fDuration, m_tGridSize, m_nRandrange, m_bShakeZ);
	a->autorelease();
	return a;
}

void ShakyTiles3D::update(float time)
{
    CC_UNUSED_PARAM(time);
    int i, j;

    for (i = 0; i < m_tGridSize.width; ++i)
    {
        for (j = 0; j < m_tGridSize.height; ++j)
        {
            Quad3 coords = getOriginalTile(Point(i, j));

            // X
            coords.bl.x += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
            coords.br.x += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
            coords.tl.x += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
            coords.tr.x += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;

            // Y
            coords.bl.y += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
            coords.br.y += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
            coords.tl.y += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
            coords.tr.y += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;

            if (m_bShakeZ)
            {
                coords.bl.z += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
                coords.br.z += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
                coords.tl.z += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
                coords.tr.z += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
            }
                        
            setTile(Point(i, j), coords);
        }
    }
}

// implementation of ShatteredTiles3D

ShatteredTiles3D* ShatteredTiles3D::create(float duration, const Size& gridSize, int nRange, bool bShatterZ)
{
    ShatteredTiles3D *pAction = new ShatteredTiles3D();

    if (pAction)
    {
        if (pAction->initWithDuration(duration, gridSize, nRange, bShatterZ))
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

bool ShatteredTiles3D::initWithDuration(float duration, const Size& gridSize, int nRange, bool bShatterZ)
{
    if (TiledGrid3DAction::initWithDuration(duration, gridSize))
    {
        m_bOnce = false;
        m_nRandrange = nRange;
        m_bShatterZ = bShatterZ;

        return true;
    }

    return false;
}

ShatteredTiles3D* ShatteredTiles3D::clone() const
{
	// no copy constructor	
	auto a = new ShatteredTiles3D();
	a->initWithDuration(m_fDuration, m_tGridSize, m_nRandrange, m_bShatterZ);
	a->autorelease();
	return a;
}

void ShatteredTiles3D::update(float time)
{
    CC_UNUSED_PARAM(time);
    int i, j;

    if (m_bOnce == false)
    {
        for (i = 0; i < m_tGridSize.width; ++i)
        {
            for (j = 0; j < m_tGridSize.height; ++j)
            {
                Quad3 coords = getOriginalTile(Point(i ,j));
                
                // X
                coords.bl.x += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
                coords.br.x += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
                coords.tl.x += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
                coords.tr.x += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
                
                // Y
                coords.bl.y += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
                coords.br.y += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
                coords.tl.y += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
                coords.tr.y += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;

                if (m_bShatterZ) 
                {
                    coords.bl.z += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
                    coords.br.z += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
                    coords.tl.z += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
                    coords.tr.z += ( kdRand() % (m_nRandrange*2) ) - m_nRandrange;
                }
                
                setTile(Point(i, j), coords);
            }
        }
        
        m_bOnce = true;
    }
}

// implementation of ShuffleTiles

ShuffleTiles* ShuffleTiles::create(float duration, const Size& gridSize, unsigned int seed)
{
    ShuffleTiles *pAction = new ShuffleTiles();

    if (pAction)
    {
        if (pAction->initWithDuration(duration, gridSize, seed))
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

bool ShuffleTiles::initWithDuration(float duration, const Size& gridSize, unsigned int seed)
{
    if (TiledGrid3DAction::initWithDuration(duration, gridSize))
    {
        m_uSeed = seed;
        m_pTilesOrder = NULL;
        m_pTiles = NULL;

        return true;
    }

    return false;
}

ShuffleTiles* ShuffleTiles::clone() const
{
	// no copy constructor
	auto a = new ShuffleTiles();
	a->initWithDuration(m_fDuration, m_tGridSize, m_uSeed);
	a->autorelease();
	return a;
}

ShuffleTiles::~ShuffleTiles(void)
{
    CC_SAFE_DELETE_ARRAY(m_pTilesOrder);
    CC_SAFE_DELETE_ARRAY(m_pTiles);
}

void ShuffleTiles::shuffle(unsigned int *pArray, unsigned int nLen)
{
    int i;
    for( i = nLen - 1; i >= 0; i-- )
    {
        unsigned int j = kdRand() % (i+1);
        unsigned int v = pArray[i];
        pArray[i] = pArray[j];
        pArray[j] = v;
    }
}

Size ShuffleTiles::getDelta(const Size& pos) const
{
    Point    pos2;

    unsigned int idx = pos.width * m_tGridSize.height + pos.height;

    pos2.x = (float)(m_pTilesOrder[idx] / (int)m_tGridSize.height);
    pos2.y = (float)(m_pTilesOrder[idx] % (int)m_tGridSize.height);

    return Size((int)(pos2.x - pos.width), (int)(pos2.y - pos.height));
}

void ShuffleTiles::placeTile(const Point& pos, Tile *t)
{
    Quad3 coords = getOriginalTile(pos);

    Point step = m_pTarget->getGrid()->getStep();
    coords.bl.x += (int)(t->position.x * step.x);
    coords.bl.y += (int)(t->position.y * step.y);

    coords.br.x += (int)(t->position.x * step.x);
    coords.br.y += (int)(t->position.y * step.y);

    coords.tl.x += (int)(t->position.x * step.x);
    coords.tl.y += (int)(t->position.y * step.y);

    coords.tr.x += (int)(t->position.x * step.x);
    coords.tr.y += (int)(t->position.y * step.y);

    setTile(pos, coords);
}

void ShuffleTiles::startWithTarget(Node *target)
{
    TiledGrid3DAction::startWithTarget(target);

    if (m_uSeed != (unsigned int)-1)
    {
        kdSrand(m_uSeed);
    }

    m_uTilesCount = m_tGridSize.width * m_tGridSize.height;
    m_pTilesOrder = new unsigned int[m_uTilesCount];
    int i, j;
    unsigned int k;

    /**
     * Use k to loop. Because _tilesCount is unsigned int,
     * and i is used later for int.
     */
    for (k = 0; k < m_uTilesCount; ++k)
    {
        m_pTilesOrder[k] = k;
    }

    shuffle(m_pTilesOrder, m_uTilesCount);

    m_pTiles = (struct Tile *)new Tile[m_uTilesCount];
    Tile *tileArray = (Tile*) m_pTiles;

    for (i = 0; i < m_tGridSize.width; ++i)
    {
        for (j = 0; j < m_tGridSize.height; ++j)
        {
            tileArray->position = Point((float)i, (float)j);
            tileArray->startPosition = Point((float)i, (float)j);
            tileArray->delta = getDelta(Size(i, j));
            ++tileArray;
        }
    }
}

void ShuffleTiles::update(float time)
{
    int i, j;

    Tile *tileArray = (Tile*)m_pTiles;

    for (i = 0; i < m_tGridSize.width; ++i)
    {
        for (j = 0; j < m_tGridSize.height; ++j)
        {
            tileArray->position = Point((float)tileArray->delta.width, (float)tileArray->delta.height) * time;
            placeTile(Point(i, j), tileArray);
            ++tileArray;
        }
    }
}

// implementation of FadeOutTRTiles

FadeOutTRTiles* FadeOutTRTiles::create(float duration, const Size& gridSize)
{
    FadeOutTRTiles *pAction = new FadeOutTRTiles();

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

FadeOutTRTiles* FadeOutTRTiles::clone() const
{
	// no copy constructor
	auto a = new FadeOutTRTiles();
	a->initWithDuration(m_fDuration, m_tGridSize);
	a->autorelease();
	return a;
}

float FadeOutTRTiles::testFunc(const Size& pos, float time)
{
    Point n = Point((float)m_tGridSize.width, (float)m_tGridSize.height) * time;
    if ((n.x + n.y) == 0.0f)
    {
        return 1.0f;
    }

    return kdPowf((pos.width + pos.height) / (n.x + n.y), 6);
}

void FadeOutTRTiles::turnOnTile(const Point& pos)
{
    setTile(pos, getOriginalTile(pos));
}

void FadeOutTRTiles::turnOffTile(const Point& pos)
{
    Quad3 coords;
    kdMemset(&coords, 0, sizeof(Quad3));
    setTile(pos, coords);
}

void FadeOutTRTiles::transformTile(const Point& pos, float distance)
{
    Quad3 coords = getOriginalTile(pos);
    Point step = m_pTarget->getGrid()->getStep();

    coords.bl.x += (step.x / 2) * (1.0f - distance);
    coords.bl.y += (step.y / 2) * (1.0f - distance);

    coords.br.x -= (step.x / 2) * (1.0f - distance);
    coords.br.y += (step.y / 2) * (1.0f - distance);

    coords.tl.x += (step.x / 2) * (1.0f - distance);
    coords.tl.y -= (step.y / 2) * (1.0f - distance);

    coords.tr.x -= (step.x / 2) * (1.0f - distance);
    coords.tr.y -= (step.y / 2) * (1.0f - distance);

    setTile(pos, coords);
}

void FadeOutTRTiles::update(float time)
{
    int i, j;

    for (i = 0; i < m_tGridSize.width; ++i)
    {
        for (j = 0; j < m_tGridSize.height; ++j)
        {
            float distance = testFunc(Size(i, j), time);
            if ( distance == 0 )
            {
                turnOffTile(Point(i, j));
            } else 
            if (distance < 1)
            {
                transformTile(Point(i, j), distance);
            }
            else
            {
                turnOnTile(Point(i, j));
            }
        }
    }
}

// implementation of FadeOutBLTiles

FadeOutBLTiles* FadeOutBLTiles::create(float duration, const Size& gridSize)
{
    FadeOutBLTiles *pAction = new FadeOutBLTiles();

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

FadeOutBLTiles* FadeOutBLTiles::clone() const
{
	// no copy constructor
	auto a = new FadeOutBLTiles();
	a->initWithDuration(m_fDuration, m_tGridSize);
	a->autorelease();
	return a;
}

float FadeOutBLTiles::testFunc(const Size& pos, float time)
{
    Point n = Point((float)m_tGridSize.width, (float)m_tGridSize.height) * (1.0f - time);
    if ((pos.width + pos.height) == 0)
    {
        return 1.0f;
    }

    return kdPowf((n.x + n.y) / (pos.width + pos.height), 6);
}

// implementation of FadeOutUpTiles

FadeOutUpTiles* FadeOutUpTiles::create(float duration, const Size& gridSize)
{
    FadeOutUpTiles *pAction = new FadeOutUpTiles();

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

FadeOutUpTiles* FadeOutUpTiles::clone() const
{
	// no copy constructor
	auto a = new FadeOutUpTiles();
	a->initWithDuration(m_fDuration, m_tGridSize);
	a->autorelease();
	return a;
}

float FadeOutUpTiles::testFunc(const Size& pos, float time)
{
    Point n = Point((float)m_tGridSize.width, (float)m_tGridSize.height) * time;
    if (n.y == 0.0f)
    {
        return 1.0f;
    }

    return kdPowf(pos.height / n.y, 6);
}

void FadeOutUpTiles::transformTile(const Point& pos, float distance)
{
    Quad3 coords = getOriginalTile(pos);
    Point step = m_pTarget->getGrid()->getStep();

    coords.bl.y += (step.y / 2) * (1.0f - distance);
    coords.br.y += (step.y / 2) * (1.0f - distance);
    coords.tl.y -= (step.y / 2) * (1.0f - distance);
    coords.tr.y -= (step.y / 2) * (1.0f - distance);

    setTile(pos, coords);
}

// implementation of FadeOutDownTiles

FadeOutDownTiles* FadeOutDownTiles::create(float duration, const Size& gridSize)
{
    FadeOutDownTiles *pAction = new FadeOutDownTiles();

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

FadeOutDownTiles* FadeOutDownTiles::clone() const
{
	// no copy constructor
	auto a = new FadeOutDownTiles();
	a->initWithDuration(m_fDuration, m_tGridSize);
	a->autorelease();
	return a;
}

float FadeOutDownTiles::testFunc(const Size& pos, float time)
{
    Point n = Point((float)m_tGridSize.width, (float)m_tGridSize.height) * (1.0f - time);
    if (pos.height == 0)
    {
        return 1.0f;
    }

    return kdPowf(n.y / pos.height, 6);
}

// implementation of TurnOffTiles

TurnOffTiles* TurnOffTiles::create(float duration, const Size& gridSize)
{
    TurnOffTiles* pAction = new TurnOffTiles();
    if (pAction->initWithDuration(duration, gridSize, 0))
    {
        pAction->autorelease();
    }
    else
    {
        CC_SAFE_RELEASE(pAction);
    }
    return pAction;
}

TurnOffTiles* TurnOffTiles::create(float duration, const Size& gridSize, unsigned int seed)
{
    TurnOffTiles *pAction = new TurnOffTiles();

    if (pAction)
    {
        if (pAction->initWithDuration(duration, gridSize, seed))
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

bool TurnOffTiles::initWithDuration(float duration, const Size& gridSize, unsigned int seed)
{
    if (TiledGrid3DAction::initWithDuration(duration, gridSize))
    {
        m_uSeed = seed;
        m_pTilesOrder = NULL;

        return true;
    }

    return false;
}

TurnOffTiles* TurnOffTiles::clone() const
{
	// no copy constructor
	auto a = new TurnOffTiles();
    a->initWithDuration(m_fDuration, m_tGridSize, m_uSeed );
	a->autorelease();
	return a;
}

TurnOffTiles::~TurnOffTiles(void)
{
    CC_SAFE_DELETE_ARRAY(m_pTilesOrder);
}

void TurnOffTiles::shuffle(unsigned int *pArray, unsigned int nLen)
{
    int i;
    for (i = nLen - 1; i >= 0; i--)
    {
        unsigned int j = kdRand() % (i+1);
        unsigned int v = pArray[i];
        pArray[i] = pArray[j];
        pArray[j] = v;
    }
}

void TurnOffTiles::turnOnTile(const Point& pos)
{
    setTile(pos, getOriginalTile(pos));
}

void TurnOffTiles::turnOffTile(const Point& pos)
{
    Quad3 coords;

    kdMemset(&coords, 0, sizeof(Quad3));
    setTile(pos, coords);
}

void TurnOffTiles::startWithTarget(Node *target)
{
    unsigned int i;

    TiledGrid3DAction::startWithTarget(target);

    if (m_uSeed != (unsigned int)-1)
    {
        kdSrand(m_uSeed);
    }

    m_uTilesCount = m_tGridSize.width * m_tGridSize.height;
    m_pTilesOrder = new unsigned int[m_uTilesCount];

    for (i = 0; i < m_uTilesCount; ++i)
    {
        m_pTilesOrder[i] = i;
    }

    shuffle(m_pTilesOrder, m_uTilesCount);
}

void TurnOffTiles::update(float time)
{
    unsigned int i, l, t;

    l = (unsigned int)(time * (float)m_uTilesCount);

    for( i = 0; i < m_uTilesCount; i++ )
    {
        t = m_pTilesOrder[i];
        Point tilePos = Point( (unsigned int)(t / m_tGridSize.height), t % (unsigned int)m_tGridSize.height );

        if ( i < l )
        {
            turnOffTile(tilePos);
        }
        else
        {
            turnOnTile(tilePos);
        }
    }
}

// implementation of WavesTiles3D

WavesTiles3D* WavesTiles3D::create(float duration, const Size& gridSize, unsigned int waves, float amplitude)
{
    WavesTiles3D *pAction = new WavesTiles3D();

    if (pAction)
    {
        if (pAction->initWithDuration(duration, gridSize, waves, amplitude))
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

bool WavesTiles3D::initWithDuration(float duration, const Size& gridSize, unsigned int waves, float amplitude)
{
    if (TiledGrid3DAction::initWithDuration(duration, gridSize))
    {
        m_uWaves = waves;
        m_fAmplitude = amplitude;
        m_fAmplitudeRate = 1.0f;

        return true;
    }

    return false;
}

WavesTiles3D* WavesTiles3D::clone() const
{
	// no copy constructor
	auto a = new WavesTiles3D();
    a->initWithDuration(m_fDuration, m_tGridSize, m_uWaves, m_fAmplitude);
	a->autorelease();
	return a;
}

void WavesTiles3D::update(float time)
{
    int i, j;

    for( i = 0; i < m_tGridSize.width; i++ )
    {
        for( j = 0; j < m_tGridSize.height; j++ )
        {
            Quad3 coords = getOriginalTile(Point(i, j));

            coords.bl.z = (kdSinf(time * KD_PI_F  *m_uWaves * 2 + 
                (coords.bl.y+coords.bl.x) * .01f) * m_fAmplitude * m_fAmplitudeRate );
            coords.br.z    = coords.bl.z;
            coords.tl.z = coords.bl.z;
            coords.tr.z = coords.bl.z;

            setTile(Point(i, j), coords);
        }
    }
}

// implementation of JumpTiles3D

JumpTiles3D* JumpTiles3D::create(float duration, const Size& gridSize, unsigned int numberOfJumps, float amplitude)
{
    JumpTiles3D *pAction = new JumpTiles3D();

    if (pAction)
    {
        if (pAction->initWithDuration(duration, gridSize, numberOfJumps, amplitude))
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

bool JumpTiles3D::initWithDuration(float duration, const Size& gridSize, unsigned int numberOfJumps, float amplitude)
{
    if (TiledGrid3DAction::initWithDuration(duration, gridSize))
    {
        m_uJumps = numberOfJumps;
        m_fAmplitude = amplitude;
        m_fAmplitudeRate = 1.0f;

        return true;
    }

    return false;
}

JumpTiles3D* JumpTiles3D::clone() const
{
	// no copy constructor
	auto a = new JumpTiles3D();
    a->initWithDuration(m_fDuration, m_tGridSize, m_uJumps, m_fAmplitude);
	a->autorelease();
	return a;
}

void JumpTiles3D::update(float time)
{
    int i, j;

    float sinz =  (kdSinf(KD_PI_F * time * m_uJumps * 2) * m_fAmplitude * m_fAmplitudeRate );
    float sinz2 = (kdSinf(KD_PI_F * (time * m_uJumps * 2 + 1)) * m_fAmplitude * m_fAmplitudeRate );

    for( i = 0; i < m_tGridSize.width; i++ )
    {
        for( j = 0; j < m_tGridSize.height; j++ )
        {
            Quad3 coords = getOriginalTile(Point(i, j));

            if ( ((i+j) % 2) == 0 )
            {
                coords.bl.z += sinz;
                coords.br.z += sinz;
                coords.tl.z += sinz;
                coords.tr.z += sinz;
            }
            else
            {
                coords.bl.z += sinz2;
                coords.br.z += sinz2;
                coords.tl.z += sinz2;
                coords.tr.z += sinz2;
            }

            setTile(Point(i, j), coords);
        }
    }
}

// implementation of SplitRows

SplitRows* SplitRows::create(float duration, unsigned int nRows)
{
    SplitRows *pAction = new SplitRows();

    if (pAction)
    {
        if (pAction->initWithDuration(duration, nRows))
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

bool SplitRows::initWithDuration(float duration, unsigned int nRows)
{
    m_uRows = nRows;

    return TiledGrid3DAction::initWithDuration(duration, Size(1, nRows));
}

SplitRows* SplitRows::clone() const
{
	// no copy constructor
	auto a = new SplitRows();
    a->initWithDuration(m_fDuration, m_uRows);
	a->autorelease();
	return a;
}

void SplitRows::startWithTarget(Node *target)
{
    TiledGrid3DAction::startWithTarget(target);
    m_tWinSize = Director::getInstance()->getWinSizeInPixels();
}

void SplitRows::update(float time)
{
    unsigned int j;

    for (j = 0; j < m_tGridSize.height; ++j)
    {
        Quad3 coords = getOriginalTile(Point(0, j));
        float    direction = 1;

        if ( (j % 2 ) == 0 )
        {
            direction = -1;
        }

        coords.bl.x += direction * m_tWinSize.width * time;
        coords.br.x += direction * m_tWinSize.width * time;
        coords.tl.x += direction * m_tWinSize.width * time;
        coords.tr.x += direction * m_tWinSize.width * time;

        setTile(Point(0, j), coords);
    }
}

// implementation of SplitCols

SplitCols* SplitCols::create(float duration, unsigned int nCols)
{
    SplitCols *pAction = new SplitCols();

    if (pAction)
    {
        if (pAction->initWithDuration(duration, nCols))
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

bool SplitCols::initWithDuration(float duration, unsigned int nCols)
{
    m_uCols = nCols;
    return TiledGrid3DAction::initWithDuration(duration, Size(nCols, 1));
}

SplitCols* SplitCols::clone() const
{
	// no copy constructor	
	auto a = new SplitCols();
    a->initWithDuration(m_fDuration, m_uCols);
	a->autorelease();
	return a;
}

void SplitCols::startWithTarget(Node *target)
{
    TiledGrid3DAction::startWithTarget(target);
    m_tWinSize = Director::getInstance()->getWinSizeInPixels();
}

void SplitCols::update(float time)
{
    unsigned int i;

    for (i = 0; i < m_tGridSize.width; ++i)
    {
        Quad3 coords = getOriginalTile(Point(i, 0));
        float    direction = 1;

        if ( (i % 2 ) == 0 )
        {
            direction = -1;
        }

        coords.bl.y += direction * m_tWinSize.height * time;
        coords.br.y += direction * m_tWinSize.height * time;
        coords.tl.y += direction * m_tWinSize.height * time;
        coords.tr.y += direction * m_tWinSize.height * time;

        setTile(Point(i, 0), coords);
    }
}

NS_CC_END
