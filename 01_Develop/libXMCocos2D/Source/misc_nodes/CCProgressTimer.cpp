/* -----------------------------------------------------------------------------------
 *
 *      File            CCProgresstimer.cpp  
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Lam Pham
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
#include "misc_nodes/CCProgressTimer.h"
#include "textures/CCTextureCache.h"
#include "support/CCPointExtension.h"
#include "shaders/CCShaderCache.h"

NS_CC_BEGIN
    
#define kProgressTextureCoordsCount        4
const KDchar kCCProgressTextureCoords = 0x4b;

CCProgressTimer::CCProgressTimer ( KDvoid )
{
	m_eType				= kCCProgressTimerTypeRadial;
	m_fPercentage		= 0.0f;
	m_pSprite			= KD_NULL;
	m_nVertexDataCount	= 0;
	m_pVertexData		= KD_NULL;
	m_tMidpoint			= CCPointZero;
	m_tBarChangeRate	= CCPointZero;
	m_bReverseDirection	= KD_FALSE;
}

CCProgressTimer* CCProgressTimer::create ( const KDchar* szPath )
{
	CCSprite*  pSprite = CCSprite::create ( szPath );

	return pSprite ? CCProgressTimer::create ( pSprite ) : KD_NULL;
}

CCProgressTimer* CCProgressTimer::create ( CCSprite* pSprite )
{
    CCProgressTimer*  pRet = new CCProgressTimer ( );

    if ( pRet->initWithSprite ( pSprite ) )
    {
        pRet->autorelease ( );
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }        

    return pRet;
}

KDbool CCProgressTimer::initWithSprite ( CCSprite* pSprite )
{
    setPercentage ( 0.0f );
    m_pVertexData = KD_NULL;
    m_nVertexDataCount = 0;

    setAnchorPoint ( ccp ( 0.5f, 0.5f ) );
    m_eType = kCCProgressTimerTypeRadial;
    m_bReverseDirection = KD_FALSE;
    setMidpoint ( ccp ( 0.5f, 0.5f ) );
    setBarChangeRate ( ccp ( 1, 1 ) );
    setSprite ( pSprite );
    // shader program
    setShaderProgram ( CCShaderCache::sharedShaderCache ( )->programForKey ( kCCShader_PositionTextureColor ) );

    return KD_TRUE;
}


CCProgressTimer::~CCProgressTimer ( KDvoid )
{
    CC_SAFE_FREE ( m_pVertexData );
    CC_SAFE_RELEASE ( m_pSprite );
}

KDvoid CCProgressTimer::setPercentage ( KDfloat fPercentage )
{
    if ( m_fPercentage != fPercentage )
    {
        m_fPercentage = clampf ( fPercentage, 0, 100 );
        updateProgress ( );
    }
}

KDvoid CCProgressTimer::setSprite ( CCSprite* pSprite )
{
    if ( m_pSprite != pSprite )
    {
        CC_SAFE_RETAIN ( pSprite );
        CC_SAFE_RELEASE ( m_pSprite );
        m_pSprite = pSprite;
        setContentSize ( m_pSprite->getContentSize ( ) );

        //    Every time we set a new sprite, we free the current vertex data
        if ( m_pVertexData )
        {
            CC_SAFE_FREE ( m_pVertexData );
            m_nVertexDataCount = 0;
        }
    }        
}

KDvoid CCProgressTimer::setType ( CCProgressTimerType eType )
{
    if ( eType != m_eType )
    {
        //    release all previous information
        if ( m_pVertexData )
        {
            CC_SAFE_FREE ( m_pVertexData );
            m_pVertexData = KD_NULL;
            m_nVertexDataCount = 0;
        }

        m_eType = eType;
    }
}

KDvoid CCProgressTimer::setReverseProgress ( KDbool bReverse )
{
    if ( m_bReverseDirection != bReverse ) 
	{
        m_bReverseDirection = bReverse;

        //    release all previous information
        CC_SAFE_FREE ( m_pVertexData );
        m_nVertexDataCount = 0;
    }
}

KDvoid CCProgressTimer::setColor ( const ccColor3B& tColor )
{
	m_pSprite->setColor ( tColor );
	updateColor ( );
}

const ccColor3B& CCProgressTimer::getColor ( KDvoid ) const
{
	return m_pSprite->getColor ( );
}

KDvoid CCProgressTimer::setOpacity ( GLubyte cOpacity )
{
	m_pSprite->setOpacity ( cOpacity );
	updateColor ( );
}

GLubyte CCProgressTimer::getOpacity ( KDvoid ) const
{
	return m_pSprite->getOpacity ( );
}

// Interval

///
//    @returns the vertex position from the texture coordinate
///
ccTex2F CCProgressTimer::textureCoordFromAlphaPoint ( CCPoint tAlpha )
{
    ccTex2F  tRet = { 0.0f, 0.0f };
    if ( !m_pSprite )
	{
        return tRet;
    }

    ccV3F_C4B_T2F_Quad  tQuad = m_pSprite->getQuad ( );
    CCPoint  tMin = ccp ( tQuad.bl.texCoords.u, tQuad.bl.texCoords.v );
    CCPoint  tMax = ccp ( tQuad.tr.texCoords.u, tQuad.tr.texCoords.v );

    //  Fix bug #1303 so that progress timer handles sprite frame texture rotation
    if ( m_pSprite->isTextureRectRotated ( ) )
	{
        CC_SWAP ( tAlpha.x, tAlpha.y, KDfloat );
    }

    return tex2 ( tMin.x * ( 1.f - tAlpha.x ) + tMax.x * tAlpha.x, tMin.y * ( 1.f - tAlpha.y ) + tMax.y * tAlpha.y );
}

ccVertex2F CCProgressTimer::vertexFromAlphaPoint ( CCPoint tAlpha )
{
    ccVertex2F  tRet = { 0.0f, 0.0f };
    if ( !m_pSprite ) 
	{
        return tRet;
    }

    ccV3F_C4B_T2F_Quad  tQuad = m_pSprite->getQuad ( );
    CCPoint  tMin = ccp ( tQuad.bl.vertices.x, tQuad.bl.vertices.y );
    CCPoint  tMax = ccp ( tQuad.tr.vertices.x, tQuad.tr.vertices.y );
    tRet.x = tMin.x * ( 1.f - tAlpha.x ) + tMax.x * tAlpha.x;
    tRet.y = tMin.y * ( 1.f - tAlpha.y ) + tMax.y * tAlpha.y;

    return tRet;
}

KDvoid CCProgressTimer::updateColor ( KDvoid )
{
    if ( !m_pSprite )
	{
        return;
    }

    if ( m_pVertexData )
    {
        ccColor4B  tColor = m_pSprite->getQuad ( ).tl.colors;
        for ( KDint i = 0; i < m_nVertexDataCount; ++i )
        {
            m_pVertexData[i].colors = tColor;
        }            
    }
}

KDvoid CCProgressTimer::updateProgress ( KDvoid )
{
    switch ( m_eType )
    {
		case kCCProgressTimerTypeRadial :
			updateRadial ( );
			break;

		case kCCProgressTimerTypeBar :
			updateBar ( );
			break;

		default :
			break;
    }
}

KDvoid CCProgressTimer::setAnchorPoint ( CCPoint tAnchorPoint )
{
    CCNode::setAnchorPoint ( tAnchorPoint );
}

CCPoint CCProgressTimer::getMidpoint ( KDvoid )
{
    return m_tMidpoint;
}

KDvoid CCProgressTimer::setMidpoint ( CCPoint tMidPoint )
{
    m_tMidpoint = ccpClamp ( tMidPoint, CCPointZero, ccp ( 1, 1 ) );
}

///
//    Update does the work of mapping the texture onto the triangles
//    It now doesn't occur the cost of free/alloc data every update cycle.
//    It also only changes the percentage point but no other points if they have not
//    been modified.
//    
//    It now deals with flipped texture. If you run into this problem, just use the
//    sprite property and enable the methods flipX, flipY.
///
KDvoid CCProgressTimer::updateRadial ( KDvoid )
{
    if ( !m_pSprite )
	{
        return;
    }

    KDfloat  fAlpha = m_fPercentage / 100.f;

    KDfloat  fAngle = 2.f * KD_PI_F * ( m_bReverseDirection ? fAlpha : 1.0f - fAlpha );

    //    We find the vector to do a hit detection based on the percentage
    //    We know the first vector is the one @ 12 o'clock ( top,mid ) so we rotate
    //    from that by the progress angle around the m_tMidpoint pivot
    CCPoint  topMid = ccp ( m_tMidpoint.x, 1.f );
    CCPoint  tPercentagePt = ccpRotateByAngle ( topMid, m_tMidpoint, fAngle );


    KDint    nIndex = 0;
    CCPoint  tHit = CCPointZero;

    if ( fAlpha == 0.f ) 
	{
        //    More efficient since we don't always need to check intersection
        //    If the alpha is zero then the hit point is top mid and the index is 0.
        tHit   = topMid;
        nIndex = 0;
    } 
	else if ( fAlpha == 1.f ) 
	{
        //    More efficient since we don't always need to check intersection
        //    If the alpha is one then the hit point is top mid and the index is 4.
        tHit   = topMid;
        nIndex = 4;
    } 
	else 
	{
        //    We run a for loop checking the edges of the texture to find the
        //    intersection point
        //    We loop through five points since the top is split in half

        KDfloat  fMin = KD_FLT_MAX;

        for ( KDint i = 0; i <= kProgressTextureCoordsCount; ++i ) 
		{
            KDint    pIndex = ( i + ( kProgressTextureCoordsCount - 1 ) ) % kProgressTextureCoordsCount;

            CCPoint  tEdgePtA = boundaryTexCoord ( i % kProgressTextureCoordsCount );
            CCPoint  tEdgePtB = boundaryTexCoord ( pIndex );

            //    Remember that the top edge is split in half for the 12 o'clock position
            //    Let's deal with that here by finding the correct endpoints
            if ( i == 0 )
			{
                tEdgePtB = ccpLerp ( tEdgePtA, tEdgePtB, 1 - m_tMidpoint.x );
            }
			else if ( i == 4 )
			{
                tEdgePtA = ccpLerp ( tEdgePtA, tEdgePtB, 1 - m_tMidpoint.x );
            }

            //    s and t are returned by ccpLineIntersect
            KDfloat   s = 0, t = 0;
            if ( ccpLineIntersect ( tEdgePtA, tEdgePtB, m_tMidpoint, tPercentagePt, &s, &t ) )
            {

                //    Since our hit test is on rays we have to deal with the top edge
                //    being in split in half so we have to test as a segment
                if ( ( i == 0 || i == 4 ) ) 
				{
                    //    s represents the point between edgePtA--edgePtB
                    if ( ! ( 0.f <= s && s <= 1.f ) )
					{
                        continue;
                    }
                }
                //    As long as our t isn't negative we are at least finding a
                //    correct hitpoint from m_tMidpoint to percentagePt.
                if ( t >= 0.f )
				{
                    //    Because the percentage line and all the texture edges are
                    //    rays we should only account for the shortest intersection
                    if ( t < fMin ) 
					{
                        fMin   = t;
                        nIndex = i;
                    }
                }
            }
        }

        //    Now that we have the minimum magnitude we can use that to find our intersection
        tHit = ccpAdd ( m_tMidpoint, ccpMult ( ccpSub ( tPercentagePt, m_tMidpoint ), fMin ) );

    }


    //    The size of the vertex data is the index from the hitpoint
    //    the 3 is for the m_tMidpoint, 12 o'clock point and hitpoint position.

    KDbool sameIndexCount = KD_TRUE;
    if ( m_nVertexDataCount != nIndex + 3 )
	{
        sameIndexCount = KD_FALSE;
        CC_SAFE_FREE ( m_pVertexData );
        m_nVertexDataCount = 0;
    }


    if ( !m_pVertexData )
	{
        m_nVertexDataCount = nIndex + 3;
        m_pVertexData = (ccV2F_C4B_T2F*) kdMalloc ( m_nVertexDataCount * sizeof ( ccV2F_C4B_T2F ) );
        CCAssert ( m_pVertexData, "CCProgressTimer. Not enough memory" );
    }
    updateColor ( );

    if ( !sameIndexCount ) 
	{

        //    First we populate the array with the m_tMidpoint, then all
        //    vertices/texcoords/colors of the 12 'o clock start and edges and the hitpoint
        m_pVertexData[0].texCoords = textureCoordFromAlphaPoint ( m_tMidpoint );
        m_pVertexData[0].vertices = vertexFromAlphaPoint ( m_tMidpoint );

        m_pVertexData[1].texCoords = textureCoordFromAlphaPoint ( topMid );
        m_pVertexData[1].vertices = vertexFromAlphaPoint ( topMid );

        for ( KDint i = 0; i < nIndex; ++i )
		{
            CCPoint alphaPoint = boundaryTexCoord ( i );
            m_pVertexData[i+2].texCoords = textureCoordFromAlphaPoint ( alphaPoint );
            m_pVertexData[i+2].vertices = vertexFromAlphaPoint ( alphaPoint );
        }
    }

    //    hitpoint will go last
    m_pVertexData [ m_nVertexDataCount - 1 ].texCoords = textureCoordFromAlphaPoint ( tHit );
    m_pVertexData [ m_nVertexDataCount - 1 ].vertices  = vertexFromAlphaPoint       ( tHit );

}

///
//    Update does the work of mapping the texture onto the triangles for the bar
//    It now doesn't occur the cost of free/alloc data every update cycle.
//    It also only changes the percentage point but no other points if they have not
//    been modified.
//    
//    It now deals with flipped texture. If you run into this problem, just use the
//    sprite property and enable the methods flipX, flipY.
///
KDvoid CCProgressTimer::updateBar ( KDvoid )
{
    if ( !m_pSprite )
	{
        return;
    }

    KDfloat  fAlpha = m_fPercentage / 100.0f;
    CCPoint  tAlphaOffset = ccpMult ( ccp ( 1.0f * ( 1.0f - m_tBarChangeRate.x ) + fAlpha * m_tBarChangeRate.x, 1.0f * ( 1.0f - m_tBarChangeRate.y ) + fAlpha * m_tBarChangeRate.y ), 0.5f );
    CCPoint  tMin = ccpSub ( m_tMidpoint, tAlphaOffset );
    CCPoint  tMax = ccpAdd ( m_tMidpoint, tAlphaOffset );

    if ( tMin.x < 0.f )
	{
        tMax.x += -tMin.x;
        tMin.x = 0.f;
    }

    if ( tMax.x > 1.f )
	{
        tMin.x -= tMax.x - 1.f;
        tMax.x = 1.f;
    }

    if ( tMin.y < 0.f )
	{
        tMax.y += -tMin.y;
        tMin.y = 0.f;
    }

    if ( tMax.y > 1.f )
	{
        tMin.y -= tMax.y - 1.f;
        tMax.y = 1.f;
    }

    if ( !m_bReverseDirection ) 
	{
        if ( !m_pVertexData )
		{
            m_nVertexDataCount = 4;
            m_pVertexData = (ccV2F_C4B_T2F*) kdMalloc ( m_nVertexDataCount * sizeof ( ccV2F_C4B_T2F ) );
            CCAssert ( m_pVertexData, "CCProgressTimer. Not enough memory" );
        }
        //    TOPLEFT
        m_pVertexData[0].texCoords = textureCoordFromAlphaPoint ( ccp ( tMin.x, tMax.y ) );
        m_pVertexData[0].vertices  = vertexFromAlphaPoint       ( ccp ( tMin.x, tMax.y ) );

        //    BOTLEFT
        m_pVertexData[1].texCoords = textureCoordFromAlphaPoint ( ccp ( tMin.x, tMin.y ) );
        m_pVertexData[1].vertices  = vertexFromAlphaPoint       ( ccp ( tMin.x, tMin.y ) );

        //    TOPRIGHT
        m_pVertexData[2].texCoords = textureCoordFromAlphaPoint ( ccp ( tMax.x, tMax.y ) );
        m_pVertexData[2].vertices  = vertexFromAlphaPoint       ( ccp ( tMax.x, tMax.y ) );

        //    BOTRIGHT
        m_pVertexData[3].texCoords = textureCoordFromAlphaPoint ( ccp ( tMax.x, tMin.y ) );
        m_pVertexData[3].vertices  = vertexFromAlphaPoint       ( ccp ( tMax.x, tMin.y ) );
    } 
	else 
	{
        if ( !m_pVertexData ) 
		{
            m_nVertexDataCount = 8;
            m_pVertexData = (ccV2F_C4B_T2F*) kdMalloc ( m_nVertexDataCount * sizeof ( ccV2F_C4B_T2F ) );
            CCAssert ( m_pVertexData, "CCProgressTimer. Not enough memory" );
            //    TOPLEFT 1
            m_pVertexData[0].texCoords = textureCoordFromAlphaPoint ( ccp ( 0, 1 ) );
            m_pVertexData[0].vertices  = vertexFromAlphaPoint       ( ccp ( 0, 1 ) );

            //    BOTLEFT 1
            m_pVertexData[1].texCoords = textureCoordFromAlphaPoint ( ccp ( 0, 0 ) );
            m_pVertexData[1].vertices  = vertexFromAlphaPoint       ( ccp ( 0, 0 ) );

            //    TOPRIGHT 2
            m_pVertexData[6].texCoords = textureCoordFromAlphaPoint ( ccp ( 1, 1 ) );
            m_pVertexData[6].vertices  = vertexFromAlphaPoint       ( ccp ( 1, 1 ) );

            //    BOTRIGHT 2
            m_pVertexData[7].texCoords = textureCoordFromAlphaPoint ( ccp ( 1, 0 ) );
            m_pVertexData[7].vertices  = vertexFromAlphaPoint       ( ccp ( 1, 0 ) );
        }

        //    TOPRIGHT 1
        m_pVertexData[2].texCoords = textureCoordFromAlphaPoint ( ccp ( tMin.x, tMax.y ) );
        m_pVertexData[2].vertices  = vertexFromAlphaPoint       ( ccp ( tMin.x, tMax.y ) );

        //    BOTRIGHT 1
        m_pVertexData[3].texCoords = textureCoordFromAlphaPoint ( ccp ( tMin.x, tMin.y ) );
        m_pVertexData[3].vertices  = vertexFromAlphaPoint       ( ccp ( tMin.x, tMin.y ) );

        //    TOPLEFT 2
        m_pVertexData[4].texCoords = textureCoordFromAlphaPoint ( ccp ( tMax.x, tMax.y ) );
        m_pVertexData[4].vertices  = vertexFromAlphaPoint       ( ccp ( tMax.x, tMax.y ) );

        //    BOTLEFT 2
        m_pVertexData[5].texCoords = textureCoordFromAlphaPoint ( ccp ( tMax.x, tMin.y ) );
        m_pVertexData[5].vertices  = vertexFromAlphaPoint       ( ccp ( tMax.x, tMin.y ) );
    }

    updateColor ( );
}

CCPoint CCProgressTimer::boundaryTexCoord ( KDchar cIndex )
{
    if ( cIndex < kProgressTextureCoordsCount ) 
	{
        if ( m_bReverseDirection )
		{
            return ccp ( ( kCCProgressTextureCoords >> ( 7 - ( cIndex << 1 ) ) ) & 1, ( kCCProgressTextureCoords >> ( 7 - ( ( cIndex << 1 ) + 1 ) ) ) & 1 );
        } 
		else 
		{
            return ccp ( ( kCCProgressTextureCoords >> ( ( cIndex << 1 ) + 1 ) ) & 1, ( kCCProgressTextureCoords >> ( cIndex << 1 ) ) & 1 );
        }
    }

    return CCPointZero;
}

KDvoid CCProgressTimer::draw ( KDvoid )
{
    if ( ! m_pVertexData || ! m_pSprite )
	{
        return;
	}

    CC_NODE_DRAW_SETUP ( );

    ccGLBlendFunc ( m_pSprite->getBlendFunc ( ).src, m_pSprite->getBlendFunc ( ).dst );

    ccGLEnableVertexAttribs ( kCCVertexAttribFlag_PosColorTex );

    ccGLBindTexture2D ( m_pSprite->getTexture ( )->getName ( ) );

#ifdef EMSCRIPTEN
	setGLBufferData ( (KDvoid*) m_pVertexData, ( m_nVertexDataCount * sizeof ( ccV2F_C4B_T2F ) ), 0 );

	KDint	nOffset = 0;
	ccGLVertexAttribPointer ( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof ( ccV2F_C4B_T2F ), (GLvoid*) nOffset );

	nOffset += sizeof ( ccVertex2F );
	ccGLVertexAttribPointer ( kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof ( ccV2F_C4B_T2F ), (GLvoid*) nOffset );

	nOffset += sizeof ( ccColor4B );
	ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof ( ccV2F_C4B_T2F ), (GLvoid*) nOffset );
#else
    ccGLVertexAttribPointer ( kCCVertexAttrib_Position , 2, GL_FLOAT        , GL_FALSE, sizeof ( m_pVertexData[0] ), &m_pVertexData[0].vertices  );
    ccGLVertexAttribPointer ( kCCVertexAttrib_TexCoords, 2, GL_FLOAT        , GL_FALSE, sizeof ( m_pVertexData[0] ), &m_pVertexData[0].texCoords );
    ccGLVertexAttribPointer ( kCCVertexAttrib_Color    , 4, GL_UNSIGNED_BYTE, GL_TRUE , sizeof ( m_pVertexData[0] ), &m_pVertexData[0].colors    );
#endif // EMSCRIPTEN

    if ( m_eType == kCCProgressTimerTypeRadial )
    {
        glDrawArrays ( GL_TRIANGLE_FAN, 0, m_nVertexDataCount );
    } 
    else if ( m_eType == kCCProgressTimerTypeBar )
    {
        if ( !m_bReverseDirection ) 
        {
            glDrawArrays ( GL_TRIANGLE_STRIP, 0, m_nVertexDataCount );
        } 
        else 
        {
            glDrawArrays ( GL_TRIANGLE_STRIP, 0, m_nVertexDataCount / 2 );
            glDrawArrays ( GL_TRIANGLE_STRIP, 4, m_nVertexDataCount / 2 );
            // 2 draw calls
            CC_INCREMENT_GL_DRAWS ( 1 );
        }
    }

    CC_INCREMENT_GL_DRAWS ( 1 );
}

NS_CC_END

