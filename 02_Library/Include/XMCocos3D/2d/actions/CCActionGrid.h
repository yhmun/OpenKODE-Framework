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

#ifndef __CCActionGrid_h__
#define __CCActionGrid_h__

#include "CCActionInterval.h"
#include "CCActionInstant.h"

NS_CC_BEGIN

class GridBase;

/**
 *	@addtogroup actions
 *	@{
 */

/** @brief Base class for Grid actions */
class CC_DLL GridAction : public ActionInterval
{
	public :

		/** initializes the action with size and duration */
		KDbool							initWithDuration ( KDfloat fDuration, const Size& tGridSize );

		/** returns the grid */
		virtual GridBase*				getGrid ( KDvoid );

		// overrides
		virtual GridAction*				clone ( KDvoid ) const override = 0;
		virtual GridAction*				reverse ( KDvoid ) const override;
		virtual KDvoid					startWithTarget ( Node* pTarget ) override;

	protected :

		Size							m_tGridSize;
};

/** 
 *	@brief Base class for Grid3D actions.
 *	Grid3D actions can modify a non-tiled grid.
 */
class CC_DLL Grid3DAction : public GridAction
{
	public :

		/** returns the grid */
		virtual GridBase*				getGrid ( KDvoid );

		/**
		 *	returns the vertex than belongs to certain position in the grid 
		 *	@js NA
		 *	@lua NA
		 */
		Vertex3F						getVertex ( const Point& tPosition ) const;

		/**
		 *	returns the non-transformed vertex than belongs to certain position in the grid 
		 *	@js NA
		 *	@lua NA
		 */
		Vertex3F						getOriginalVertex(const Point& position) const;

		/** 
		 *	sets a new vertex to a certain position of the grid 
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid							setVertex(const Point& position, const Vertex3F& vertex);

		// Overrides
		virtual Grid3DAction*			clone ( KDvoid ) const override = 0;
};

/** @brief Base class for TiledGrid3D actions */
class CC_DLL TiledGrid3DAction : public GridAction
{
	public :

		/**
		 *	creates the action with size and duration 
		 *	@js NA
		 *	@lua NA
		 */
		static TiledGrid3DAction*		create ( KDfloat fDuration, const Size& tGridSize );

		/** 
		 *	returns the tile that belongs to a certain position of the grid 
		 *	@js NA
		 *	@lua NA
		 */
		Quad3							getTile ( const Point& tPosition ) const;

		/**
		 *	returns the non-transformed tile that belongs to a certain position of the grid 
		 *	@js NA
		 *	@lua NA
		 */
		Quad3							getOriginalTile ( const Point& tPosition ) const;

		/** 
		 *	sets a new tile to a certain position of the grid 
		 *	@js NA
		 *	@lua NA
		 */
		KDvoid							setTile ( const Point& tPosition, const Quad3& tCoords );

		/** returns the grid */
		virtual GridBase*				getGrid ( KDvoid );

		// Override
		virtual TiledGrid3DAction*		clone ( KDvoid ) const override = 0;
};

/** @brief AccelDeccelAmplitude action */
class CC_DLL AccelDeccelAmplitude : public ActionInterval
{
	public :

		/** creates the action with an inner action that has the amplitude property, and a duration time */
		static AccelDeccelAmplitude*	create ( Action* pAction, KDfloat fDuration );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~AccelDeccelAmplitude ( KDvoid );

		/** initializes the action with an inner action that has the amplitude property, and a duration time */
		KDbool							initWithAction ( Action* pAction, KDfloat fDuration );

		/** returns a new clone of the action */
		virtual AccelDeccelAmplitude*	clone ( KDvoid ) const;

		/** returns a new reversed action */
		virtual AccelDeccelAmplitude*	reverse ( KDvoid ) const;

		/** get amplitude rate */
		inline KDfloat					getRate ( KDvoid ) const { return m_fRate; }
		/** set amplitude rate */
		inline KDvoid					setRate ( KDfloat fRate ) { m_fRate = fRate; }

		// Overrides
		virtual KDvoid					startWithTarget ( Node* pTarget ) override;
		virtual KDvoid					update ( KDfloat fTime ) override;

	protected :

		KDfloat							m_fRate;
		ActionInterval*					m_pOther;
};

/** @brief AccelAmplitude action */
class CC_DLL AccelAmplitude : public ActionInterval
{
	public :

		/** creates the action with an inner action that has the amplitude property, and a duration time */
		static AccelAmplitude*			create(Action *action, float duration);

		/**
		 * @js NA
		 * @lua NA
		 */
		virtual ~AccelAmplitude ( KDvoid );

		/** initializes the action with an inner action that has the amplitude property, and a duration time */
		KDbool							initWithAction ( Action* pAction, KDfloat fDuration );

		/** get amplitude rate */
		inline KDfloat					getRate ( KDvoid ) const { return m_fRate; }

		/** set amplitude rate */
		inline KDvoid					setRate ( KDfloat fRate ) { m_fRate = fRate; }

		// Overrides
		virtual KDvoid					startWithTarget ( Node* pTarget ) override;
		virtual KDvoid					update ( KDfloat fTime ) override;
		virtual AccelAmplitude*			clone ( KDvoid ) const override;
		virtual AccelAmplitude*			reverse ( KDvoid ) const override;

	protected :

		KDfloat							m_fRate;
		ActionInterval*					m_pOther;
};

/** @brief DeccelAmplitude action */
class CC_DLL DeccelAmplitude : public ActionInterval
{
	public :

		/** creates the action with an inner action that has the amplitude property, and a duration time */
		static DeccelAmplitude*			create ( Action* pAction, KDfloat fDuration );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~DeccelAmplitude();

		/** initializes the action with an inner action that has the amplitude property, and a duration time */
		bool							initWithAction ( Action* pAction, KDfloat fDuration );

		/** get amplitude rate */
		inline KDfloat					getRate ( KDvoid ) const { return m_fRate; }

		/** set amplitude rate */
		inline KDvoid					setRate ( KDfloat fRate ) { m_fRate = fRate; }

		// overrides
		virtual KDvoid					startWithTarget ( Node* pTarget ) override;
		virtual KDvoid					update ( KDfloat fTime ) override;
		virtual DeccelAmplitude*		clone ( KDvoid ) const;
		virtual DeccelAmplitude*		reverse ( KDvoid ) const;

	protected :

		KDfloat							m_fRate;
		ActionInterval*					m_pOther;
};

/** 
 *	@brief StopGrid action.
 *	@warning Don't call this action if another grid action is active.
 *	Call if you want to remove the the grid effect. Example:
 *	Sequence::actions(Lens::action(...), StopGrid::action(...), NULL);
 */
class CC_DLL StopGrid : public ActionInstant
{
	public :

		/** Allocates and initializes the action */
		static StopGrid*				create ( KDvoid );

		// Overrides
		virtual KDvoid					startWithTarget ( Node* pTarget ) override;
		virtual StopGrid*				clone ( KDvoid ) const override;
		virtual StopGrid*				reverse ( KDvoid ) const override;
};

/** @brief ReuseGrid action */
class CC_DLL ReuseGrid : public ActionInstant
{
	public :

		/** creates an action with the number of times that the current grid will be reused */
		static ReuseGrid*				create ( KDint nTimes );

		/** initializes an action with the number of times that the current grid will be reused */
		bool							initWithTimes ( KDint nTimes );

		// Override
		virtual KDvoid					startWithTarget ( Node* pTarget ) override;
		virtual ReuseGrid*				clone ( KDvoid ) const override;
		virtual ReuseGrid*				reverse ( KDvoid ) const override;

	protected :

		KDint							m_nTimes;
};

//	end of actions group
/// @}

NS_CC_END

#endif	// __CCActionGrid_h__
