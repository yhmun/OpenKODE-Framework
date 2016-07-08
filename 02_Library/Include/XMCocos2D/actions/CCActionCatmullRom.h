/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionCatmullRom.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2008      Radu Gruian  : Original code 
 *      Copyright (c) 2011      Vit Valentin : Adapted to cocos2d-x 
 *      Copyright (c) 2012      Ricardo Quesada
 *
 *         http://www.cocos2d-x.org      
 *         http://www.codeproject.com/Articles/30838/Overhauser-Catmull-Rom-Splines-for-Camera-Animatio.So
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

#ifndef __CCActionCatmullRom_h__
#define __CCActionCatmullRom_h__

#include "CCActionInterval.h"
#include "../base_nodes/CCNode.h"

NS_CC_BEGIN

/**
 * @addtogroup actions
 * @{
 */

/** 
 *	An Array that contain control points.
 *	Used by CCCardinalSplineTo and (By) and CCCatmullRomTo (and By) actions.
 *	@ingroup Actions
 */
class CCPointArray : public CCObject
{
	public :

				 CCPointArray ( KDvoid );
		virtual ~CCPointArray ( KDvoid );
		
		/** creates and initializes a Points array with capacity */
		static   CCPointArray*		create ( KDuint uCapacity );

	public :

		/** initializes a Catmull Rom config with a capacity hint */
		KDbool						initWithCapacity ( KDuint uCapacity );
	    
		/** appends a control point */
		KDvoid						addControlPoint ( CCPoint tControlPoint );
	    
		/** inserts a tControlPoint at index */
		KDvoid						insertControlPoint ( const CCPoint& tControlPoint, KDuint uIndex );
	    
		/** replaces an existing tControlPoint at index */
		KDvoid						replaceControlPoint ( const CCPoint& tControlPoint, KDuint uIndex );
	    
		/** get the value of a tControlPoint at a given index */
		CCPoint						getControlPointAtIndex ( KDuint uIndex );
			
		/** deletes a control point at a given index */
		KDvoid						removeControlPointAtIndex ( KDuint uIndex );
	    
		/** returns the number of objects of the control point array */
		KDuint						count ( KDvoid );
	    
		/** returns a new copy of the array reversed. User is responsible for releasing this copy */
		CCPointArray*				reverse ( KDvoid );
			
		/** reverse the current control point array inline, without generating a new one */
		KDvoid						reverseInline ( KDvoid );
	    
		virtual CCObject*			copyWithZone ( CCZone* pZone );
	    
		const std::vector<CCPoint*>*	getControlPoints ( KDvoid );
		KDvoid							setControlPoints ( std::vector<CCPoint*>* pControlPoints );

	private :

		/** Array that contains the control points */
		std::vector<CCPoint*>* 		m_pControlPoints;
};

/** 
 *	Cardinal Spline path.
 *	http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Cardinal_spline
 *	@ingroup Actions
 */
class CCCardinalSplineTo : public CCActionInterval
{
	public :

				 CCCardinalSplineTo ( KDvoid );
		virtual ~CCCardinalSplineTo ( KDvoid );
		
		/** creates an action with a Cardinal Spline array of points and tension */
		static   CCCardinalSplineTo*	create ( KDfloat fDuration, CCPointArray* pPoints, KDfloat fTension );

	public :
	    
		/** initializes the action with a duration and an array of points */
		KDbool							initWithDuration ( KDfloat fDuration, CCPointArray* pPoints, KDfloat fTension );
	    
		// super virtual functions
		virtual CCCardinalSplineTo*		copyWithZone ( CCZone* pZone );

		virtual KDvoid					startWithTarget ( CCNode* pTarget );

		virtual KDvoid					update ( KDfloat fTime );

		virtual CCActionInterval*		reverse ( KDvoid );
	    
		virtual KDvoid					updatePosition ( CCPoint& tNewPos );
	    
		inline CCPointArray*			getPoints ( KDvoid ) { return m_pPoints; }
		inline KDvoid					setPoints ( CCPointArray* pPoints ) 
		{
			CC_SAFE_RETAIN  ( pPoints );
			CC_SAFE_RELEASE ( m_pPoints );
			m_pPoints = pPoints;
		}
	    
	protected :

		/** Array of control points */
		CCPointArray*					m_pPoints;
		KDfloat							m_fDeltaT;
		KDfloat							m_fTension;
		CCPoint							m_tPreviousPosition;
		CCPoint							m_tAccumulatedDiff;
};

/**
 *	Cardinal Spline path.
 *	http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Cardinal_spline
 *	@ingroup Actions
 */
class CCCardinalSplineBy : public CCCardinalSplineTo 
{
	public :

		CCCardinalSplineBy ( KDvoid );

		/** creates an action with a Cardinal Spline array of points and tension */
		static  CCCardinalSplineBy*		create ( KDfloat fDuration, CCPointArray* pPoints, KDfloat fTension );

	public :

		virtual KDvoid					startWithTarget ( CCNode* pTarget );

		virtual CCActionInterval*		reverse ( KDvoid );

		virtual KDvoid					updatePosition ( CCPoint& tNewPos );

	protected :

		CCPoint							m_tStartPosition;
};

/** 
 *	An action that moves the target with a CatmullRom curve to a destination point.
 *	A Catmull Rom is a Cardinal Spline with a tension of 0.5.
 *	http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull.E2.80.93Rom_spline
 *	@ingroup Actions
 */
class CCCatmullRomTo : public CCCardinalSplineTo
{
	public :

		/** creates an action with a Cardinal Spline array of points and tension */
		static CCCatmullRomTo*			create ( KDfloat fDuration, CCPointArray* pPoints );

	public :

		/** initializes the action with a duration and an array of points */
		KDbool							initWithDuration ( KDfloat fDuration, CCPointArray* pPoints );
};

/** 
 *	An action that moves the target with a CatmullRom curve by a certain distance.
 *	A Catmull Rom is a Cardinal Spline with a tension of 0.5.
 *	http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull.E2.80.93Rom_spline
 *	@ingroup Actions
 */
class CCCatmullRomBy : public CCCardinalSplineBy
{
	public :

		/** creates an action with a Cardinal Spline array of points and tension */
		static  CCCatmullRomBy*			create ( KDfloat fDuration, CCPointArray* pPoints );

	public :

		/** initializes the action with a duration and an array of points */
		KDbool							initWithDuration ( KDfloat fDuration, CCPointArray* pPoints );
};

/** Returns the Cardinal Spline position for a given set of control points, tension and time */
extern CCPoint  ccCardinalSplineAt ( CCPoint& tP0, CCPoint& tP1, CCPoint& tP2, CCPoint& tP3, KDfloat fTension, KDfloat fTime );

// end of actions group
/// @}

NS_CC_END

#endif // __CCActionCatmullRom_h__
