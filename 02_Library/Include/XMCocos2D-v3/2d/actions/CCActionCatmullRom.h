/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionCatmullRom.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft
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
 * --------------------------------------------------------------------------------- */ 

#ifndef __CCActionCatmullRom_h__
#define __CCActionCatmullRom_h__

#include "CCActionInterval.h"
#include "../base_nodes/CCNode.h"
#include <vector>

NS_CC_BEGIN;

/**
 *	@addtogroup actions
 *	@{
 */

/** 
 *	An Array that contain control points.
 *	Used by CardinalSplineTo and (By) and CatmullRomTo (and By) actions.
 *	@ingroup Actions
 */
class CC_DLL PointArray : public Object, public Clonable
{
	public :
    
		/** 
		 *	creates and initializes a Points array with capacity 
		 *	@js NA
		 */
		static PointArray*				create ( KDuint uCapacity );
    
		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~PointArray ( KDvoid );
		
		/**
		 *	@js NA
		 *	@lua NA
		 */
		PointArray ( KDvoid );
    
		/** 
		 *	initializes a Catmull Rom config with a capacity hint 
		 *	@js NA
		 */
		KDbool							initWithCapacity ( KDuint uCapacity );
    
		/** 
		 *	appends a control point 
		 *	@js NA
		 */
		KDvoid							addControlPoint ( Point tControlPoint );
    
		/** 
		 *	inserts a controlPoint at index 
		 *	@js NA
		 */
		KDvoid							insertControlPoint ( const Point& tControlPoint, KDuint uIndex );
    
		/** 
		 *	replaces an existing controlPoint at index 
		 *	@js NA
		 */
		KDvoid							replaceControlPoint ( const Point& tControlPoint, KDuint uIndex );
    
		/** 
		 *	get the value of a controlPoint at a given index 
		 *	@js NA
		 */
		Point							getControlPointAtIndex ( KDuint uIndex );
    
		/** 
		 *	deletes a control point at a given index 
		 *	@js NA
		 */
		KDvoid							removeControlPointAtIndex ( KDuint uIndex );
    
		/** 
		 *	returns the number of objects of the control point array 
		 *	@js NA
		 */
		KDuint							count ( KDvoid ) const;
    
		/** 
		 *	returns a new copy of the array reversed. User is responsible for releasing this copy 
		 *	@js NA
		 */
		PointArray*						reverse ( KDvoid ) const;
    
		/** 
		 *	reverse the current control point array inline, without generating a new one 
		 *	@js NA
		 */
		KDvoid							reverseInline ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual PointArray*				clone ( KDvoid ) const;

		/**
		 *	@js NA
		 */
		const std::vector<Point*>*		getControlPoints ( KDvoid ) const;

		/**
		 *	@js NA
		 */
		KDvoid							setControlPoints ( std::vector<Point*>* pControlPoints );

	private :

		/** Array that contains the control points */
		std::vector<Point*>*			m_pControlPoints;
};

/**
 *	Cardinal Spline path.
 *	http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Cardinal_spline
 *	@ingroup Actions
 */
class CC_DLL CardinalSplineTo : public ActionInterval
{
	public:

		/**
		 *	creates an action with a Cardinal Spline array of points and tension 
		 *	@code
		 *	when this function bound to js or lua,the input params are changed
		 *	in js: var create(var t,var table)
		 *	in lua: lcaol create(local t, local table)
		 *	@endcode
		 */
		static CardinalSplineTo*		create ( KDfloat fDuration, PointArray* pPoints, KDfloat fTension );

		/**
		 *  @js NA
		 *	@lua NA
		 */
		virtual ~CardinalSplineTo ( KDvoid );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		CardinalSplineTo ( KDvoid );
    
		/** initializes the action with a duration and an array of points */
		KDbool							initWithDuration ( KDfloat duration, PointArray* pPoints, KDfloat fTension );

		virtual KDvoid					updatePosition ( Point& tNewPos );

		inline PointArray*				getPoints ( KDvoid ) { return m_pPoints; }

		/**
		 *	@js NA
		 *	@lua NA
		 */
		inline KDvoid					setPoints ( PointArray* pPoints )
		{
			CC_SAFE_RETAIN ( pPoints );
			CC_SAFE_RELEASE ( m_pPoints );
			m_pPoints = pPoints;
		}

		// Overrides
		virtual CardinalSplineTo*		clone ( KDvoid ) const override;
		virtual CardinalSplineTo*		reverse ( KDvoid ) const override;
		virtual KDvoid					startWithTarget ( Node* pTarget ) override;
		virtual KDvoid					update ( KDfloat fTime ) override;

	protected :

		/** Array of control points */
		PointArray*						m_pPoints;
		KDfloat							m_tDeltaT;
		KDfloat							m_tTension;
		Point							m_tPreviousPosition;
		Point							m_tAccumulatedDiff;
};

/**
 *	Cardinal Spline path.
 *	http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Cardinal_spline
 *	@ingroup Actions
 */
class CC_DLL CardinalSplineBy : public CardinalSplineTo 
{
	public:
    
		/** 
		 *	creates an action with a Cardinal Spline array of points and tension 
		 *	@code
		 *	when this function bound to js or lua,the input params are changed
		 *	in js: var create(var t,var table)
		 *	in lua: lcaol create(local t, local table)
		 *	@endcode
		 */
		static CardinalSplineBy*		create ( KDfloat fDuration, PointArray* pPoints, KDfloat fTension );

		CardinalSplineBy ( KDvoid );
    
		// Overrides
		virtual KDvoid					startWithTarget ( Node* pTarget ) override;
		virtual KDvoid					updatePosition ( Point& tNewPos ) override;
		virtual CardinalSplineBy*		clone ( KDvoid ) const override;
		virtual CardinalSplineBy*		reverse ( KDvoid ) const override;

	protected :

		Point							m_tStartPosition;
};

/**
 *	An action that moves the target with a CatmullRom curve to a destination point.
 *	A Catmull Rom is a Cardinal Spline with a tension of 0.5.
 *	http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull.E2.80.93Rom_spline
 *	@ingroup Actions
 */
class CC_DLL CatmullRomTo : public CardinalSplineTo
{
	public :
    
		/** 
		 *	creates an action with a Cardinal Spline array of points and tension 
		 *	@code
		 *	when this function bound to js or lua,the input params are changed
		 *	in js: var create(var dt,var table)
		 *	in lua: lcaol create(local dt, local table)
		 *	@endcode
		 */
		static CatmullRomTo*			create ( KDfloat fDuration, PointArray* pPoints );

		/** initializes the action with a duration and an array of points */
		KDbool							initWithDuration ( KDfloat fDuration, PointArray* pPoints );

		// Override
		virtual CatmullRomTo*			clone ( KDvoid ) const override;
		virtual CatmullRomTo*			reverse ( KDvoid ) const override;
};

/** 
 *	An action that moves the target with a CatmullRom curve by a certain distance.
 *	A Catmull Rom is a Cardinal Spline with a tension of 0.5.
 *	http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull.E2.80.93Rom_spline
 *	@ingroup Actions
 */
class CC_DLL CatmullRomBy : public CardinalSplineBy
{
	public :

		/**
		 *	creates an action with a Cardinal Spline array of points and tension 
		 *	@code
		 *	when this function bound to js or lua,the input params are changed
		 *	in js: var create(var dt,var table)
		 *	in lua: lcaol create(local dt, local table)
		 *	@endcode
		 */
		static CatmullRomBy*			create ( KDfloat fDuration, PointArray* pPoints );

		/** initializes the action with a duration and an array of points */
		KDbool							initWithDuration ( KDfloat fDuration, PointArray* pPoints );

		// Override
		virtual CatmullRomBy*			clone ( KDvoid ) const override;
		virtual CatmullRomBy*			reverse ( KDvoid ) const override;
};

/** Returns the Cardinal Spline position for a given set of control points, tension and time */
extern CC_DLL Point		ccCardinalSplineAt ( Point& p0, Point& p1, Point& p2, Point& p3, KDfloat tension, KDfloat t );

// end of actions group
/// @}

NS_CC_END;

#endif	// __CCActionCatmullRom_h__