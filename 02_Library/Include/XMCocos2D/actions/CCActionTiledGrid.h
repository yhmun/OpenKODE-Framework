/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionTiledGrid.h
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

#ifndef __CCActionTiledGrid_h__
#define __CCActionTiledGrid_h__

#include "CCActionGrid.h"

NS_CC_BEGIN
    
/**
 * @addtogroup actions
 * @{
 */

/** @brief CCShakyTiles3D action */
class CCShakyTiles3D : public CCTiledGrid3DAction
{
	public :

		/** creates the action with a range, whether or not to shake Z vertices, a grid size, and duration */
		static  CCShakyTiles3D*		create ( KDfloat fDuration, const CCSize& tGridSize, KDint nRange, KDbool bShakeZ );

	public :

		/** initializes the action with a range, whether or not to shake Z vertices, a grid size, and duration */
		virtual KDbool		initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDint nRange, KDbool bShakeZ );

		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual KDvoid		update ( KDfloat fTime );

	protected :

		KDint				m_nRandrange;
		KDbool				m_bShakeZ;
};

/** @brief CCShatteredTiles3D action */
class CCShatteredTiles3D : public CCTiledGrid3DAction
{
	public :

		/** creates the action with a range, whether of not to shatter Z vertices, a grid size and duration */
		static  CCShatteredTiles3D*		create ( KDfloat fDuration, const CCSize& tGridSize, KDint nRange, KDbool bShatterZ );

	public :

		/** initializes the action with a range, whether or not to shatter Z vertices, a grid size and duration */
        virtual KDbool		initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDint nRange, KDbool bShatterZ );

		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual KDvoid		update ( KDfloat fTime );

	protected :

		KDint				m_nRandrange;
		KDbool				m_bOnce;
		KDbool				m_bShatterZ;
};

struct Tile;

/** 
 *	@brief	CCShuffleTiles action
 *	Shuffle the tiles in random order
 */
class CCShuffleTiles : public CCTiledGrid3DAction
{
	public :

		virtual ~CCShuffleTiles ( KDvoid );

		/** creates the action with a random seed, the grid size and the duration */
		static   CCShuffleTiles*	create ( KDfloat fDuration, const CCSize& tGridSize, KDuint uSeed );

	public :		

		/** initializes the action with a random seed, the grid size and the duration */
		virtual KDbool		initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDuint uSeed );

		KDvoid				shuffle ( KDuint* pArray, KDuint nLen );

		CCSize				getDelta ( const CCSize& tPos );

		KDvoid				placeTile ( const CCPoint& tPos, Tile* pTile );

		virtual KDvoid		startWithTarget ( CCNode* pTarget );

		virtual KDvoid		update ( KDfloat fTime );

		virtual CCObject*	copyWithZone ( CCZone* pZone );

	protected :

		KDuint				m_uSeed;
		KDuint				m_uTilesCount;
		KDuint*				m_pTilesOrder;
		Tile*				m_pTiles;
};

/** 
 *	@brief	CCFadeOutTRTiles action
 *	Fades out the tiles in a Top-Right direction
 */
class CCFadeOutTRTiles : public CCTiledGrid3DAction
{
	public :

		/** creates the action with the grid size and the duration */
		static  CCFadeOutTRTiles*	create ( KDfloat fDuration, const CCSize& tGridSize );

	public :

		virtual KDfloat		testFunc ( const CCSize& tPos, KDfloat fTime );

		KDvoid				turnOnTile  ( const CCPoint& tPos );
		KDvoid				turnOffTile ( const CCPoint& tPos );

		virtual KDvoid		transformTile ( const CCPoint& tPos, KDfloat fDistance );

		virtual KDvoid		update ( KDfloat fTime );
};

/** 
 *	@brief	CCFadeOutBLTiles action.
 *	Fades out the tiles in a Bottom-Left direction
 */
class CCFadeOutBLTiles : public CCFadeOutTRTiles
{
	public :

		/** creates the action with the grid size and the duration */
        static  CCFadeOutBLTiles*	create ( KDfloat fDuration, const CCSize& tGridSize );

	public :

		virtual KDfloat		testFunc ( const CCSize& tPos, KDfloat fTime );
};

/** 
 *	@brief	CCFadeOutUpTiles action.
 *	Fades out the tiles in upwards direction
 */
class CCFadeOutUpTiles : public CCFadeOutTRTiles
{
	public :

		/** creates the action with the grid size and the duration */
        static  CCFadeOutUpTiles*	create ( KDfloat fDuration, const CCSize& tGridSize );

	public :

		virtual KDfloat		testFunc ( const CCSize& tPos, KDfloat fTime );
    
		virtual KDvoid		transformTile ( const CCPoint& tPos, KDfloat fDistance );
};

/**
 *	@brief	CCFadeOutDownTiles action.
 *	Fades out the tiles in downwards direction
 */
class CCFadeOutDownTiles : public CCFadeOutUpTiles
{
	public :

		/** creates the action with the grid size and the duration */
        static  CCFadeOutDownTiles*		create ( KDfloat fDuration, const CCSize& tGridSize );

	public :

		virtual KDfloat		testFunc ( const CCSize& tPos, KDfloat fTime );
};

/**
 *	@brief	CCTurnOffTiles action.
 *	Turn off the files in random order
 */
class CCTurnOffTiles : public CCTiledGrid3DAction
{
	public :

		virtual ~CCTurnOffTiles ( KDvoid );

		/** creates the action with the grid size and the duration */
		static   CCTurnOffTiles*	create ( KDfloat fDuration, const CCSize& tGridSize );

		/** creates the action with a random seed, the grid size and the duration */
		static   CCTurnOffTiles*	create ( KDfloat fDuration, const CCSize& tGridSize, KDuint uSeed );

	public :

		/** initializes the action with a random seed, the grid size and the duration */
		virtual KDbool		initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDuint uSeed );

		KDvoid				shuffle ( KDuint* pArray, KDuint nLen );

		KDvoid				turnOnTile  ( const CCPoint& tPos );
		KDvoid				turnOffTile ( const CCPoint& tPos );

		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual KDvoid		startWithTarget ( CCNode* pTarget );

		virtual KDvoid		update ( KDfloat fTime );

	protected :

		KDuint				m_uSeed;
		KDuint				m_uTilesCount;
		KDuint*				m_pTilesOrder;
};

/** @brief CCWavesTiles3D action. */
class CCWavesTiles3D : public CCTiledGrid3DAction
{
	public:

		/** creates the action with a number of waves, the waves amplitude, the grid size and the duration */
		static  CCWavesTiles3D*		create ( KDfloat fDuration, const CCSize& tGridSize, KDuint uWaves, KDfloat fAmplitude );

	public :
	
		/** initializes the action with a number of waves, the waves amplitude, the grid size and the duration */
		virtual KDbool		initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDuint uWaves, KDfloat fAmplitude );

		/** waves amplitude */
		inline  KDfloat		getAmplitude ( KDvoid ) { return m_fAmplitude; }
		inline  KDvoid		setAmplitude ( KDfloat fAmplitude ) { m_fAmplitude = fAmplitude; }

		/** waves amplitude rate */
		inline  KDfloat		getAmplitudeRate ( KDvoid ) { return m_fAmplitudeRate; }
		inline  KDvoid		setAmplitudeRate ( KDfloat fAmplitudeRate ) { m_fAmplitudeRate = fAmplitudeRate; }

		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual KDvoid		update ( KDfloat fTime );

	protected :

		KDuint				m_uWaves;
		KDfloat				m_fAmplitude;
		KDfloat				m_fAmplitudeRate;
};

/** 
 *	@brief  CCJumpTiles3D action.
 *	A sin function is executed to move the tiles across the Z axis
 */
class CCJumpTiles3D : public CCTiledGrid3DAction
{
	public :

		/** creates the action with the number of jumps, the sin amplitude, the grid size and the duration */
        static  CCJumpTiles3D*		create ( KDfloat fDuration, const CCSize& tGridSize, KDuint uJumps, KDfloat fAmplitude );

	public :

		/** initializes the action with the number of jumps, the sin amplitude, the grid size and the duration */
		virtual KDbool		initWithDuration ( KDfloat fDuration, const CCSize& tGridSize, KDuint uJumps, KDfloat fAmplitude );

		/** amplitude of the sin*/
		inline  KDfloat		getAmplitude ( KDvoid ) { return m_fAmplitude; }
		inline  KDvoid		setAmplitude ( KDfloat fAmplitude ) { m_fAmplitude = fAmplitude; }

		/** amplitude rate */
		inline  KDfloat		getAmplitudeRate ( KDvoid ) { return m_fAmplitudeRate; }
		inline  KDvoid		setAmplitudeRate ( KDfloat fAmplitudeRate ) { m_fAmplitudeRate = fAmplitudeRate; }

		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual KDvoid		update ( KDfloat fTime );

	protected :

		KDuint				m_uJumps;
		KDfloat				m_fAmplitude;
		KDfloat				m_fAmplitudeRate;
};

/** @brief CCSplitRows action */
class CCSplitRows : public CCTiledGrid3DAction
{
	public :

		/** creates the action with the number of rows to split and the duration */
		static  CCSplitRows*	create ( KDfloat fDuration, KDuint uRows );

	public :

		/** initializes the action with the number of rows to split and the duration */
		virtual KDbool		initWithDuration ( KDfloat fDuration, KDuint uRows );

		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual KDvoid		startWithTarget ( CCNode* pTarget );

		virtual KDvoid		update ( KDfloat fTime );

	protected :

		KDuint				m_uRows;
		CCSize				m_tWinSize;
};

/** @brief CCSplitCols action */
class CCSplitCols : public CCTiledGrid3DAction
{
	public :

		/** creates the action with the number of columns to split and the duration */
		static  CCSplitCols*	create ( KDfloat fDuration, KDuint uCols );

	public :

		/** initializes the action with the number of columns to split and the duration */
		virtual KDbool		initWithDuration ( KDfloat fDuration, KDuint uCols );

		virtual CCObject*	copyWithZone ( CCZone* pZone );

		virtual KDvoid		startWithTarget ( CCNode* pTarget );

		virtual KDvoid		update ( KDfloat fTime );

	protected :

		KDuint				m_uCols;
		CCSize				m_tWinSize;
};

// end of actions group
/// @}

NS_CC_END

#endif // __CCActionTiledGrid_h__
