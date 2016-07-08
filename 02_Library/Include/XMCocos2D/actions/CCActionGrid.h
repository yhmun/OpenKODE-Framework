/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionGrid.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
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
 * ----------------------------------------------------------------------------------- */ 

#ifndef __CCActionGrid_h__
#define __CCActionGrid_h__

#include "CCActionInterval.h"
#include "CCActionInstant.h"

NS_CC_BEGIN
    
/**
 * @addtogroup actions
 * @{
 */

class CCGridBase;

/** @brief Base class for Grid actions */
class CCGridAction : public CCActionInterval
{
	public :

		/** creates the action with size and duration */
		static  CCGridAction*		create ( KDfloat fDuration, const CCSize& tGridSize );

	public :

		/** initializes the action with size and duration */
		virtual KDbool				initWithDuration ( KDfloat fDuration, const CCSize& tGridSize );

		virtual CCObject*			copyWithZone ( CCZone* pZone );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual CCActionInterval*	reverse ( KDvoid );

		/** returns the grid */
		virtual CCGridBase*			getGrid ( KDvoid );

	protected :

		CCSize						m_tGridSize;
};

/** 
 *	@brief	Base class for CCGrid3D actions.
 *			Grid3D actions can modify a non-tiled grid.
 */
class CCGrid3DAction : public CCGridAction
{
	public :

		/** creates the action with size and duration */
        static  CCGrid3DAction*		create ( KDfloat fDuration, const CCSize& tGridSize );

	public :

		/** returns the grid */
		virtual CCGridBase*			getGrid ( KDvoid );

		/** returns the vertex than belongs to certain position in the grid */
		ccVertex3F					vertex ( const CCPoint& tPos );

		/** returns the non-transformed vertex than belongs to certain position in the grid */
		ccVertex3F					originalVertex ( const CCPoint& tPos );

		/** sets a new vertex to a certain position of the grid */
		KDvoid						setVertex ( const CCPoint& tPos, const ccVertex3F& tVertex );
};

/** @brief Base class for CCTiledGrid3D actions */
class CCTiledGrid3DAction : public CCGridAction
{
	public :

		/** creates the action with size and duration */
        static  CCTiledGrid3DAction*	create ( KDfloat fDuration, const CCSize& tGridSize );

	public :

		/** returns the tile that belongs to a certain position of the grid */
		ccQuad3						tile ( const CCPoint& tPos );

		/** returns the non-transformed tile that belongs to a certain position of the grid */
		ccQuad3						originalTile ( const CCPoint& tPos );

		/** sets a new tile to a certain position of the grid */
		KDvoid						setTile ( const CCPoint& tPos, const ccQuad3& tCoords );

		/** returns the grid */
		virtual CCGridBase*			getGrid ( KDvoid );
};

/** @brief CCAccelDeccelAmplitude action */
class CCAccelDeccelAmplitude : public CCActionInterval
{
	public :

		virtual ~CCAccelDeccelAmplitude ( KDvoid );
		
		/** creates the action with an inner action that has the amplitude property, and a duration time */
		static  CCAccelDeccelAmplitude*  create ( CCAction* pAction, KDfloat fDuration );

	public :

		/** initializes the action with an inner action that has the amplitude property, and a duration time */
		virtual KDbool				initWithAction ( CCAction* pAction, KDfloat fDuration );

		/** get amplitude rate */
		inline  KDfloat				getRate ( KDvoid ) { return m_fRate; }

		/** set amplitude rate */
		inline  KDvoid				setRate ( KDfloat fRate ) { m_fRate = fRate; }

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

	protected :

		KDfloat						m_fRate;
		CCActionInterval*			m_pOther;
};

/** @brief CCAccelAmplitude action */
class CCAccelAmplitude : public CCActionInterval
{
	public :

		virtual ~CCAccelAmplitude ( KDvoid );
	
		/** creates the action with an inner action that has the amplitude property, and a duration time */
		static  CCAccelAmplitude*	create ( CCAction* pAction, KDfloat fDuration );

	public :

		/** initializes the action with an inner action that has the amplitude property, and a duration time */
		virtual KDbool				initWithAction ( CCAction* pAction, KDfloat fDuration );

		/** get amplitude rate */
		inline  KDfloat				getRate ( KDvoid ) { return m_fRate; }

		/** set amplitude rate */
		inline  KDvoid				setRate ( KDfloat fRate ) { m_fRate = fRate; }

		virtual KDvoid				startWithTarget ( CCNode* pTarget );    

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

	protected :

		KDfloat						m_fRate;
		CCActionInterval*			m_pOther;
};

/** @brief CCDeccelAmplitude action */
class CCDeccelAmplitude : public CCActionInterval
{
	public :

		virtual ~CCDeccelAmplitude ( KDvoid );
	
		/**
		 *	creates the action with an inner action that has the amplitude property, and a duration time 
		 *	@deprecated: This interface will be deprecated sooner or later.
		 */
        static  CCDeccelAmplitude*		create ( CCAction* pAction, KDfloat fDuration );

	public :

		/** initializes the action with an inner action that has the amplitude property, and a duration time */
		virtual KDbool				initWithAction ( CCAction* pAction, KDfloat fDuration );

		/** get amplitude rate */
		inline  KDfloat				getRate ( KDvoid ) { return m_fRate; }

		/** set amplitude rate */
		inline  KDvoid				setRate ( KDfloat fRate ) { m_fRate = fRate; }

		virtual KDvoid				startWithTarget ( CCNode* pTarget );   

		virtual KDvoid				update ( KDfloat fTime );

		virtual CCActionInterval*	reverse ( KDvoid );

	protected :

		KDfloat						m_fRate;
		CCActionInterval*			m_pOther;
};

/** @brief	 CCStopGrid action.
 *	@warning Don't call this action if another grid action is active.
 *			 Call if you want to remove the the grid effect. Example:
 *			 CCSequence::actions(Lens::action(...), CCStopGrid::action(...), KD_NULL);
*/
class CCStopGrid : public CCActionInstant
{
	public :

		/** Allocates and initializes the action */
		static  CCStopGrid*			create ( KDvoid );

	public :

		virtual KDvoid				startWithTarget ( CCNode* pTarget );
};

/** @brief CCReuseGrid action */
class CCReuseGrid : public CCActionInstant
{
	public :

		/** creates an action with the number of times that the current grid will be reused */
		static  CCReuseGrid*		create ( KDint nTimes );

	public :

		/** initializes an action with the number of times that the current grid will be reused */
        virtual KDbool				initWithTimes ( KDint nTimes );

		virtual KDvoid				startWithTarget ( CCNode* pTarget );

	protected :

		KDint						m_nTimes;
};

// end of actions group
/// @}

NS_CC_END

#endif // __CCActionGrid_h__
