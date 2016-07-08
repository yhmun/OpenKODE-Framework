/* -----------------------------------------------------------------------------------
 *
 *      File            CCBlade.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2011      Ngo Duc Hiep. All rights reserved.
 *
 *         http://precognitiveresearch.com      
 *
 * -----------------------------------------------------------------------------------
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
 * ----------------------------------------------------------------------------------- */ 

#include "Precompiled.h"
#include "extensions/CCBlade/CCBlade.h"
#include "shaders/CCShaderCache.h"

NS_CC_BEGIN

class CCPointValue : public CCPoint, public CCObject
{
	public :

		static CCPointValue*	create ( const CCPoint& tPoint )
		{
			CCPointValue*	pRet = new CCPointValue ( );

			if ( pRet )
			{
				pRet->setPoint ( tPoint.x, tPoint.y );
				pRet->autorelease ( );
			}
			else
			{
				CC_SAFE_DELETE ( pRet );
			}

			return pRet;
		}
};

#define USE_LAGRANGE				1
#define USE_STL_LIST				0
#define USE_UPDATE_FOR_POP			1

#define POP_TIME_INTERVAL			1 / 60.f
#define DISTANCE_TO_INTERPOLATE		10

inline KDfloat fangle ( CCPoint tVect )
{
	if ( tVect.x == 0.0f && tVect.y == 0.0f )
	{
		return 0;
	}
	
	if ( tVect.x == 0.0f )
	{
		return tVect.y > 0 ? KD_PI_F / 2 : -KD_PI_F / 2;
	}
	
	if ( tVect.y == 0.0 && tVect.x < 0 )
	{
		return -KD_PI_F;
	}
	
	KDfloat		fAngle = kdAtanf ( tVect.y / tVect.x );
    
	return tVect.x < 0 ? fAngle + KD_PI_F : fAngle;
}

inline KDfloat lagrange1 ( CCPoint p1, CCPoint p2, KDfloat x )
{
	return ( x - p1.x ) / ( p2.x - p1.x ) * p2.y + ( x - p2.x ) / ( p1.x - p2.x ) * p1.y;
}

inline KDvoid CCPointSet ( CCPoint* v, KDfloat x, KDfloat y )
{
	v->x = x;
	v->y = y;
}

inline KDvoid f1 ( CCPoint p1, CCPoint p2, KDfloat d, CCPoint* o1, CCPoint* o2 )
{
	KDfloat		l = ccpDistance ( p1, p2 );
	KDfloat		fAngle = fangle ( ccpSub ( p2, p1 ) );
	*o1 = ccpRotateByAngle ( ccp ( p1.x + l, p1.y + d ), p1, fAngle );
	*o2 = ccpRotateByAngle ( ccp ( p1.x + l, p1.y - d ), p1, fAngle );
}

CCBlade* CCBlade::create ( KDint nLimit )
{
	CCBlade*	pRet = new CCBlade ( );

	if ( pRet && pRet->initWithMaximumPoint ( nLimit ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

CCBlade::CCBlade ( KDvoid )
{
	m_nCount			= 0;
	m_pVertices			= KD_NULL;
	m_pCoordinates		= KD_NULL;
	m_bReset			= KD_FALSE;
	m_bFinish			= KD_FALSE;
	m_bWillPop			= KD_FALSE;
	m_fTimeSinceLastPop = 0.f;
	m_fPopTimeInterval	= 0.f;
	m_uPointLimit		= 0;
	m_pTexture			= KD_NULL;
	m_fWidth			= 0.f;
	m_bAutoDim			= KD_FALSE;
	m_pPath				= KD_NULL;
}

CCBlade::~CCBlade ( KDvoid )
{
	CC_SAFE_DELETE ( m_pPath );

    CC_SAFE_FREE ( m_pVertices );
    CC_SAFE_FREE ( m_pCoordinates );

	CC_SAFE_RELEASE ( m_pTexture );
}

KDbool CCBlade::initWithMaximumPoint ( KDint nLimit )
{
	if ( !CCNode::init ( ) )
	{
		return KD_FALSE;
	}

	m_uPointLimit	= nLimit;
	m_fWidth		= 5;
	
    m_pVertices		= (CCPoint*) kdCalloc ( 2 * nLimit + 5, sizeof ( CCPoint ) );
    m_pCoordinates	= (CCPoint*) kdCalloc ( 2 * nLimit + 5, sizeof ( CCPoint ) );
    
    CCPointSet ( m_pCoordinates + 0, 0.00f, 0.5f );
    m_bReset		= KD_FALSE;
    
	m_pPath = new CCArray ( );
    
#if USE_UPDATE_FOR_POP
    m_fPopTimeInterval	= POP_TIME_INTERVAL;    
    m_fTimeSinceLastPop = 0;
    this->scheduleUpdateWithPriority ( 0 );
#endif

	setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTexture ) );

	return KD_TRUE;
}

KDvoid CCBlade::setTexture ( CCTexture2D* pTexture )
{
	CC_SAFE_RELEASE ( m_pTexture );
	m_pTexture = pTexture;
	CC_SAFE_RETAIN ( m_pTexture );
}

KDbool CCBlade::isAutoDim ( KDvoid )
{
	return m_bAutoDim;
}

KDvoid CCBlade::setAutoDim ( KDbool bAutoDim )
{
	m_bAutoDim = bAutoDim;
}

CCArray* CCBlade::getPath ( KDvoid ) const 
{
	return m_pPath;
}

KDvoid CCBlade::populateVertices ( KDvoid )
{
	CCPointValue*  pVal = (CCPointValue*) m_pPath->objectAtIndex ( 0 );

    m_pVertices [ 0 ] = ccp ( pVal->x, pVal->y );

    CCPoint			tPre = m_pVertices [ 0 ];
    
    KDuint			i  = 0;
    CCPointValue*	it = (CCPointValue*) m_pPath->objectAtIndex ( 1 );
	KDfloat			dd = m_fWidth / m_pPath->count ( );

	while ( i < m_pPath->count ( ) - 2 )
	{
		f1 ( tPre, ccp ( it->x, it->y ), m_fWidth - i * dd, m_pVertices + 2 * i + 1, m_pVertices + 2 * i + 2 );
		CCPointSet ( m_pCoordinates + 2 * i + 1, .5f, 1.0f );
		CCPointSet ( m_pCoordinates + 2 * i + 2, .5f, 0.0f );
		
		i++;
		tPre = ccp ( it->x, it->y );
		
		it = (CCPointValue*) m_pPath->objectAtIndex ( i + 1 );
	}
    
    CCPointSet ( m_pCoordinates + 1, 0.25f, 1.0f );
	CCPointSet ( m_pCoordinates + 2, 0.25f, 0.0f );
	
	m_pVertices [ 2 * m_pPath->count ( ) - 3 ] = ccp ( it->x, it->y );

	CCPointSet ( m_pCoordinates + 2 * m_pPath->count ( ) - 3, 0.75f, 0.5f);
}
/*
- (void) shift{
	int index = 2 * _pointLimit - 1;
	for (int i = index; i > 3; i -= 2) {
		vertices[i] = vertices[i-2];
		vertices[i-1] = vertices[i-3];
	}
}

- (void) set_width:(float)newWidth{
    _width = newWidth ;// * CC_CONTENT_SCALE_FACTOR();
}
*/

KDvoid CCBlade::push ( CCPoint v )
{
    m_bWillPop = KD_FALSE;
    
	if ( m_bReset )
	{
		return;
	}
    
#if USE_LAGRANGE
    
    if ( m_pPath->count ( ) == 0 )
	{
        m_pPath->insertObject ( CCPointValue::create ( v ), 0 );
        return;
    }
    
    CCPointValue*		pFirst = (CCPointValue*) m_pPath->objectAtIndex ( 0 );
    if ( ccpDistance ( v, *pFirst ) < DISTANCE_TO_INTERPOLATE ) 
	{
        m_pPath->insertObject ( CCPointValue::create ( v ), 0 );

        if ( m_pPath->count ( ) > m_uPointLimit ) 
		{
            m_pPath->removeLastObject ( );
        }
    }
	else
	{
		CCPoint		tFirst ( pFirst->x, pFirst->y );
        KDint		nNum = (KDint) ( ccpDistance ( v, tFirst ) / DISTANCE_TO_INTERPOLATE );
        CCPoint		iv = ccpMult ( ccpSub ( v, tFirst ), 1.f / ( nNum + 1 ) );
		
		for ( KDint i = 1; i <= nNum + 1; i++ )
		{
			m_pPath->insertObject ( CCPointValue::create ( ccpAdd ( tFirst, ccpMult ( iv, (KDfloat) i ) ) ), 0 );
		}
		while ( m_pPath->count ( ) > m_uPointLimit ) 
		{
			m_pPath->removeLastObject ( );
		}
    }
#else // !USE_LAGRANGE
	m_pPath.push_front ( v );
	if (m_pPath.size ( ) > m_uPointLimit )
	{
		m_pPath.pop_back ();
	}
#endif // !USE_LAGRANGE
    
	
	this->populateVertices ( );
}

KDvoid CCBlade::pop ( KDint n ) 
{
    while ( m_pPath->count ( ) > 0 && n > 0 )
	{
        m_pPath->removeLastObject ( );
        n--;
    }
    
    if ( m_pPath->count ( ) > 2 )
	{
        this->populateVertices ( );
    }
}

KDvoid CCBlade::clear ( KDvoid )
{
	m_pPath->removeAllObjects ( );

	m_bReset = KD_FALSE;

    if ( m_bFinish )
	{
		this->removeFromParentAndCleanup ( KD_TRUE );
	}
}

KDvoid CCBlade::reset ( KDvoid )
{
	m_bReset = KD_TRUE;
}

KDvoid CCBlade::dim ( KDbool bDim )
{
	m_bReset = bDim;
}

KDvoid CCBlade::update ( KDfloat fDelta )
{    
    m_fTimeSinceLastPop += fDelta;
    
    KDfloat		fPrecision = 1 / 60.f;
    KDfloat		fRoundedTimeSinceLastPop = fPrecision * kdRoundf ( m_fTimeSinceLastPop / fPrecision ); // helps because fps flucuate around 1./60.
    
    KDint		nNumberOfPops = (KDint)  ( fRoundedTimeSinceLastPop / m_fPopTimeInterval );
    m_fTimeSinceLastPop = m_fTimeSinceLastPop - nNumberOfPops * m_fPopTimeInterval;
    
    for ( KDint nPop = 0; nPop < nNumberOfPops; nPop++ )
	{        
        if ( ( m_bReset && m_pPath->count ( ) > 0) || ( m_bAutoDim && m_bWillPop ) )
		{
            this->pop ( 1 );
            if ( m_pPath->count ( ) < 3 )
			{
                this->clear ( );

                if ( m_bFinish )
				{
                    return; // if we continue self will have been deallocated
                }
            }
        }
        
    }
}

KDvoid CCBlade::draw ( KDvoid )
{
#if !USE_UPDATE_FOR_POP
    if ( ( m_bReset && m_pPath->count ( ) > 0 ) || ( m_bAutoDim && m_bWillPop ) )
	{
        this->pop ( 1 );
        if ( m_pPath->count ( ) < 3 )
		{
            this->clear ( );
            if ( m_bFinish )
			{
                return;					// if we continue self will have been deallocated
            }
        }
    }
#endif
    
    if ( m_pPath == KD_NULL )
	{
        return;
	}
    
    if ( m_pPath->count ( ) < 3 )
	{
        return;
    }
    
    m_bWillPop = KD_TRUE;

    CC_NODE_DRAW_SETUP ( );

	ccGLEnableVertexAttribs ( kCCVertexAttribFlag_Position |  kCCVertexAttribFlag_TexCoords );
    
    ccGLBindTexture2D ( m_pTexture->getName ( ) );
    ccGLBlendFunc ( CC_BLEND_SRC, CC_BLEND_DST );
    
    ccGLVertexAttribPointer ( kCCVertexAttrib_Position , 2, GL_FLOAT, GL_FALSE, 0, m_pVertices );
    ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, m_pCoordinates );
    
    
    glDrawArrays ( GL_TRIANGLE_STRIP, 0, 2 * m_pPath->count ( ) - 2 );

    CC_INCREMENT_GL_DRAWS ( 1 );
}

KDvoid CCBlade::finish ( KDvoid )
{
    m_bFinish = KD_FALSE;
}

NS_CC_END