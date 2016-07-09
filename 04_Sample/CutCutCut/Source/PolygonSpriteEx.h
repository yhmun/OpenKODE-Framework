/* -----------------------------------------------------------------------------------
 *
 *      File            PolygonSpriteEx.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2012      Allen Benson G Tan on 5/19/12 
 *      Copyright (c) 2012      WhiteWidget Inc. All rights reserved.   
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

#ifndef __PolygonSpriteEx_h__
#define __PolygonSpriteEx_h__

enum State
{
    kStateIdle			= 0	,
    kStateTossed
};

enum Type
{
    kTypeWatermelon		= 0	,
    kTypeStrawberry			,
    kTypePineapple			,
    kTypeGrapes				,
    kTypeBanana				,
    kTypeBomb
};

class PolygonSpriteEx : public PolygonSprite
{
	public :
				 PolygonSpriteEx ( KDvoid );
		virtual ~PolygonSpriteEx ( KDvoid );
	    
		static PolygonSpriteEx*		createWithTexture ( CCTexture2D* pTexture, b2Body* pBody, KDbool bOriginal, KDfloat fPTMRatio = 32 );

	public :

		virtual KDbool				initWithTexture ( CCTexture2D* pTexture, b2Body* pBody, KDbool bOriginal, KDfloat fPTMRatio = 32 );

		virtual b2Body*				createBodyForWorld ( b2World* pWorld, b2Vec2 tPosition, KDfloat fRotation, b2Vec2* pVertices, KDint nVertexCount, KDfloat fDensity, KDfloat fFriction, KDfloat fRestitution );

	protected :

		CC_SYNTHESIZE_BOOL	 ( m_bSliceEntered	, SliceEntered	);
		CC_SYNTHESIZE_BOOL	 ( m_bSliceExited	, SliceExited	);

		CC_SYNTHESIZE 		 ( b2Vec2	, m_tEntryPoint		, EntryPoint	 );
		CC_SYNTHESIZE	 	 ( b2Vec2	, m_tExitPoint		, ExitPoint		 );
		CC_SYNTHESIZE		 ( KDfloat	, m_fSliceEntryTime	, SliceEntryTime );
		CC_SYNTHESIZE		 ( State	, m_eState			, State			 );
		CC_SYNTHESIZE		 ( Type		, m_eType			, Type			 );

		CC_SYNTHESIZE_RETAIN ( CCParticleSystemQuad*, m_pSplurt, Splurt );
};

#endif	// __PolygonSpriteEx_h__


