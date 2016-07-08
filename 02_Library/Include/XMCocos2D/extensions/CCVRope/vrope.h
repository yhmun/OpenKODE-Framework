/* -----------------------------------------------------------------------------------
 *
 *      File            vrope.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2012      Creeng Ltd. ( Ported to cocos2d-x 12/7/2012 )
 *      Copyright (c) 2012      Flightless Ltd. ( Modified 20/4/2012 )
 *      Copyright (c) 2010      Clever Hamster Games. ( Created by patrick 14/10/2010 )
 *
 *         http://www.www.flightless.co.nz 
 *         http://www.creeng.com      
 *
 * -----------------------------------------------------------------------------------
 * 
 *      Permission is hereby granted, free of charge, to any person obtaining a copy 
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights 
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is furnished
 *      to do so, subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be included in all 
 *      copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *      INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 *      A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *      HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
 *      OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 *      SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

#ifndef __vrope_h__
#define __vrope_h__

#include "vstick.h"
#include "../../cocoa/CCArray.h"
#include "../../sprite_nodes/CCSprite.h"
#include "../../sprite_nodes/CCSpriteBatchNode.h"
#include "XMBox2D/Box2D.h"

NS_CC_BEGIN

#define kCCPTMRatio		32

class VRope : public CCObject
{
	public :
				 VRope ( KDvoid );
		virtual ~VRope ( KDvoid );

		static	 VRope*		create ( b2Body* pBodyA, b2Body* pBodyB, CCSpriteBatchNode* pRopeBatchNode, KDfloat fPTMRatio = kCCPTMRatio );
		static	 VRope*		create ( b2Joint* pJointAB, CCSpriteBatchNode* pRopeBatchNode, KDfloat fPTMRatio = kCCPTMRatio );
		static	 VRope*		create ( b2RopeJoint* pJoint, CCSpriteBatchNode* pRopeBatchNode, KDfloat fPTMRatio = kCCPTMRatio );
		static	 VRope*		create ( const CCPoint& tPointA, const CCPoint& tPointB, CCSpriteBatchNode* pRopeBatchNode, KDfloat fPTMRatio = kCCPTMRatio );
		static	 VRope*		create ( b2RopeJoint* pJoint, CCSpriteBatchNode* pRopeBatchNode, CCArray* pPoints, CCArray* pSticks, CCArray* pSprites, KDfloat fPTMRatio = kCCPTMRatio );

	public :

		virtual	 KDbool		initWithBodies		( b2Body* pBodyA, b2Body* pBodyB, CCSpriteBatchNode* pRopeBatchNode, KDfloat fPTMRatio = kCCPTMRatio );
		virtual	 KDbool		initWithJoint		( b2Joint* pJointAB, CCSpriteBatchNode* pRopeBatchNode, KDfloat fPTMRatio = kCCPTMRatio );
		virtual	 KDbool		initWithRopeJoint	( b2RopeJoint* pJoint, CCSpriteBatchNode* pRopeBatchNode, KDfloat fPTMRatio = kCCPTMRatio );
		virtual	 KDbool		initWithPoints		( const CCPoint& tPointA, const CCPoint& tPointB, CCSpriteBatchNode* pRopeBatchNode, KDfloat fPTMRatio = kCCPTMRatio );
		virtual	 KDbool		initWithArrays		( b2RopeJoint* pJoint, CCSpriteBatchNode* pRopeBatchNode, CCArray* pPoints, CCArray* pSticks, CCArray* pSprites, KDfloat fPTMRatio = kCCPTMRatio );

		KDvoid				update ( KDfloat fDelta );		
		KDvoid				updateWithPreIntegratedGravity ( KDfloat fDelta );													// Flightless, update rope by pre-integrating the gravity each step (optimised for changing gravity)
		KDvoid				updateWithPreIntegratedGravity ( KDfloat fDelta, KDfloat fGravityX, KDfloat fGravityY );			// Flightless, update rope by pre-integrating the gravity each step (optimised for changing gravity)
		KDvoid				updateWithPreIntegratedOriginGravity ( KDfloat fDelta );											// Flightless, update rope by pre-integrating the gravity each step (optimised for changing gravity)
			    
		KDvoid				reset ( KDvoid );

		KDvoid				createRope ( const CCPoint& tPointA, const CCPoint& tPointB, KDfloat fDistance = 0.f );
		
		KDvoid				resetWithPoints ( const CCPoint& tPointA, const CCPoint& tPointB );
		
		KDvoid				updateWithPoints ( const CCPoint& tPointA, const CCPoint& tPointB, KDfloat fDelta );
		KDvoid				updateWithPoints ( const CCPoint& tPointA, const CCPoint& tPointB, KDfloat gxdt, KDfloat gydt );	// Flightless, update with support for pre-integrating the gravity each step (optimised for changing gravity)

		KDvoid				debugDraw ( KDvoid );
		
		KDvoid				updateSprites ( KDvoid );

		KDvoid				removeSprites ( KDvoid );

		CCArray*			getSticks ( KDvoid );
	    
		b2RopeJoint*		getRopeJoint ( KDvoid );
		KDvoid				setRopeJoint ( b2RopeJoint* pJoint );

		b2Joint*			getJoint ( KDvoid );
		KDvoid				setJoint ( b2Joint* pJoint );

		VRope*				cutRopeInStick ( VStick* pStick, b2Body* pBodyA, b2Body* pBodyB );

	private :

		KDvoid				getPoints ( CCPoint& tPointA, CCPoint& tPointB );

	private :

		KDint					m_nNumPoints;		
		CCArray*				m_pPoints;
		CCArray*				m_pSticks;
		CCArray*				m_pRopeSprites;
		CCSpriteBatchNode*		m_pSpriteBatch;
		KDfloat					m_fAntiSagHack;
		b2Body*					m_pBodyA;
		b2Body*					m_pBodyB;
		b2Joint*				m_pJointAB;
		b2RopeJoint*			m_pJoint;
		KDfloat					m_fPTMRatio;		// Pixels to Meters ratio
};


NS_CC_END

#endif  // __vrope_h__