/* -----------------------------------------------------------------------------------
 *
 *      File            PolygonSprite.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2012      zeiteisens. All rights reserved. ( Created by Hanno Bruns on 24.06.12 )
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

#ifndef __PolygonSprite_h__
#define __PolygonSprite_h__

#include "../CCPRKit/PRFilledPolygon.h"
#include "XMBox2D/Box2D.h"

NS_CC_BEGIN

class PolygonSprite : public PRFilledPolygon
{
	public :
				 PolygonSprite ( KDvoid );
		virtual ~PolygonSprite ( KDvoid );
	    

		static PolygonSprite*		create ( const KDchar* szFilename, b2Body* pBody, KDbool bOriginal, KDfloat fPTMRatio = 32 );

		static PolygonSprite*		createWithTexture ( CCTexture2D* pTexture, b2Body* pBody, KDbool bOriginal, KDfloat fPTMRatio = 32 );
	
		static PolygonSprite*		createWithWorld ( b2World* pWorld );

	public :

		virtual KDbool				initWithFile ( const KDchar* szFilename, b2Body* pBody, KDbool bOriginal, KDfloat fPTMRatio = 32 );

		virtual KDbool				initWithTexture ( CCTexture2D* pTexture, b2Body* pBody, KDbool bOriginal, KDfloat fPTMRatio = 32 );		
		
		virtual KDbool				initWithWorld ( b2World* pWorld );

		virtual b2Body*				createBodyForWorld ( b2World* pWorld, b2Vec2 tPosition, KDfloat fRotation, b2Vec2* pVertices, KDint nVertexCount, KDfloat fDensity, KDfloat fFriction, KDfloat fRestitution );

		virtual KDvoid				activateCollisions ( KDvoid );

		virtual KDvoid				deactivateCollisions ( KDvoid );
	    
		virtual KDvoid				setPosition ( const CCPoint& tPosition );

		virtual CCAffineTransform	nodeToParentTransform ( KDvoid );
	    
	protected :

		CC_SYNTHESIZE_BOOL	( m_bOriginal, Original );
		CC_SYNTHESIZE		( b2Body*, m_pBody, Body );		
		CC_SYNTHESIZE		( b2Vec2 , m_tCentroid, Centroid );

		KDfloat						m_fPTMRatio;
};


NS_CC_END

#endif	// __PolygonSprite_h__
