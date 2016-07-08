/* -----------------------------------------------------------------------------------
 *
 *      File            CCActionTiledGrid.h
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

#ifndef __CCActionTiledGrid_h__
#define __CCActionTiledGrid_h__

#include "CCActionGrid.h"

NS_CC_BEGIN

/**
 *	@addtogroup actions
 *	@{
 */

/** @brief ShakyTiles3D action */
class CC_DLL ShakyTiles3D : public TiledGrid3DAction
{
	public :

		/** creates the action with a range, whether or not to shake Z vertices, a grid size, and duration */
		static ShakyTiles3D*		create ( KDfloat fDuration, const Size& tGridSize, KDint nRange, KDbool bShakeZ );

		/** initializes the action with a range, whether or not to shake Z vertices, a grid size, and duration */
		KDbool						initWithDuration ( KDfloat fDuration, const Size& tGridSize, KDint nRange, KDbool bShakeZ );

		// Override
		virtual ShakyTiles3D*		clone ( KDvoid ) const override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		KDint						m_nRandrange;
		KDbool						m_bShakeZ;
};

/** @brief ShatteredTiles3D action */
class CC_DLL ShatteredTiles3D : public TiledGrid3DAction
{
	public :

		/** creates the action with a range, whether of not to shatter Z vertices, a grid size and duration */
		static ShatteredTiles3D*	create ( KDfloat fDuration, const Size& tGridSize, KDint nRange, KDbool bShatterZ );

		/** initializes the action with a range, whether or not to shatter Z vertices, a grid size and duration */
		KDbool						initWithDuration ( KDfloat fDuration, const Size& tGridSize, KDint nRange, KDbool bShatterZ );

		// Override
		virtual ShatteredTiles3D*	clone ( KDvoid ) const override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		KDint						m_nRandrange;
		KDbool						m_bOnce;
		KDbool						m_bShatterZ;
};

struct Tile;

/** 
 *	@brief ShuffleTiles action
 *	Shuffle the tiles in random order
 */
class CC_DLL ShuffleTiles : public TiledGrid3DAction
{
	public :

		/** creates the action with a random seed, the grid size and the duration */
		static ShuffleTiles*		create ( KDfloat fDuration, const Size& tGridSize, KDuint uSeed );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		virtual ~ShuffleTiles ( KDvoid );

		/** initializes the action with a random seed, the grid size and the duration */
		KDbool						initWithDuration ( KDfloat fDuration, const Size& tGridSize, KDuint uSeed );

		KDvoid						shuffle ( KDuint* pArray, KDuint uLen );
		Size						getDelta ( const Size& tPos ) const;
		KDvoid						placeTile ( const Point& tPos, Tile* t );

		// Overrides
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual ShuffleTiles*		clone ( KDvoid ) const override;

	protected :

		KDuint						m_uSeed;
		KDuint						m_uTilesCount;
		KDuint*						m_pTilesOrder;
		Tile*						m_pTiles;
};

/** 
 *	@brief FadeOutTRTiles action
 *	Fades out the tiles in a Top-Right direction
 */
class CC_DLL FadeOutTRTiles : public TiledGrid3DAction
{
	public :

		/** creates the action with the grid size and the duration */
		static FadeOutTRTiles*		create ( KDfloat fDuration, const Size& tGridSize );

		virtual KDfloat				testFunc ( const Size& tPos, KDfloat fTime );
		KDvoid						turnOnTile ( const Point& tPos );
		KDvoid						turnOffTile ( const Point& tPos );
		virtual KDvoid				transformTile ( const Point& tPos, KDfloat fDistance );

		// Overrides
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual FadeOutTRTiles*		clone ( KDvoid ) const override;
};

/**
 *	@brief FadeOutBLTiles action.
 *	Fades out the tiles in a Bottom-Left direction
 */
class CC_DLL FadeOutBLTiles : public FadeOutTRTiles
{
	public :

		/** creates the action with the grid size and the duration */
		static FadeOutBLTiles*		create ( KDfloat fDuration, const Size& tGridSize );

		// Overrides
		virtual KDfloat				testFunc ( const Size& tPos, KDfloat fTime ) override;
		virtual FadeOutBLTiles*		clone ( KDvoid ) const override;
};

/**
 *	@brief FadeOutUpTiles action.
 *	Fades out the tiles in upwards direction
 */
class CC_DLL FadeOutUpTiles : public FadeOutTRTiles
{
	public :

		/** creates the action with the grid size and the duration */
		static FadeOutUpTiles*		create ( KDfloat fDuration, const Size& tGridSize );

		virtual KDvoid				transformTile ( const Point& tPos, KDfloat fDistance );

		// Overrides
		virtual FadeOutUpTiles*		clone ( KDvoid ) const override;
		virtual KDfloat				testFunc ( const Size& tPos, KDfloat fTime ) override;
};

/** 
 *	@brief FadeOutDownTiles action.
 *	Fades out the tiles in downwards direction
 */
class CC_DLL FadeOutDownTiles : public FadeOutUpTiles
{
	public :

		/** creates the action with the grid size and the duration */
		static FadeOutDownTiles*	create ( KDfloat fDuration, const Size& tGridSize );

		// Overrides
		virtual FadeOutDownTiles*	clone ( KDvoid ) const override;
		virtual KDfloat				testFunc ( const Size& tPos, KDfloat fTime ) override;
};

/** 
 *	@brief TurnOffTiles action.
 *	Turn off the files in random order
 */
class CC_DLL TurnOffTiles : public TiledGrid3DAction
{
	public :

		/** creates the action with the grid size and the duration */
		static TurnOffTiles*		create ( KDfloat fDuration, const Size& tGridSize );

		/** creates the action with a random seed, the grid size and the duration */
		static TurnOffTiles*		create ( KDfloat fDuration, const Size& tGridSize, KDuint uSeed );

		/**
		 *	@js NA
		 *	@lua NA
		 */
		~TurnOffTiles ( KDvoid );

		/** initializes the action with a random seed, the grid size and the duration */
		KDbool						initWithDuration ( KDfloat fDuration, const Size& tGridSize, KDuint uSeed );

		KDvoid						shuffle ( KDuint* pArray, KDuint uLen );
		KDvoid						turnOnTile ( const Point& tPos );
		KDvoid						turnOffTile ( const Point& tPos );

		// Overrides
		virtual TurnOffTiles*		clone ( KDvoid ) const override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		KDuint						m_uSeed;
		KDuint						m_uTilesCount;
		KDuint*						m_pTilesOrder;
};

/** @brief WavesTiles3D action. */
class CC_DLL WavesTiles3D : public TiledGrid3DAction
{
	public :

		/** creates the action with a number of waves, the waves amplitude, the grid size and the duration */
		static WavesTiles3D*		create ( KDfloat fDuration, const Size& tGridSize, KDuint waves, KDfloat fAmplitude );

		/** initializes the action with a number of waves, the waves amplitude, the grid size and the duration */
		KDbool						initWithDuration ( KDfloat fDuration, const Size& tGridSize, KDuint waves, KDfloat fAmplitude );

		/** waves amplitude */
		inline KDfloat				getAmplitude ( KDvoid ) const { return m_fAmplitude; }
		inline KDvoid				setAmplitude ( KDfloat fAmplitude ) { m_fAmplitude = fAmplitude; }

		/** waves amplitude rate */
		inline KDfloat				getAmplitudeRate ( KDvoid ) const { return m_fAmplitudeRate; }
		inline KDvoid				setAmplitudeRate ( KDfloat fAmplitudeRate ) { m_fAmplitudeRate = fAmplitudeRate; }

		// Override
		virtual WavesTiles3D*		clone ( KDvoid ) const override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		KDuint						m_uWaves;
		KDfloat						m_fAmplitude;
		KDfloat						m_fAmplitudeRate;
};

/** 
 *	@brief JumpTiles3D action.
 *	A sin function is executed to move the tiles across the Z axis
 */
class CC_DLL JumpTiles3D : public TiledGrid3DAction
{
	public :

		/** creates the action with the number of jumps, the sin amplitude, the grid size and the duration */
		static JumpTiles3D*			create ( KDfloat fDuration, const Size& tGridSize, KDuint uNumberOfJumps, KDfloat fAmplitude );

		/** initializes the action with the number of jumps, the sin amplitude, the grid size and the duration */
		KDbool						initWithDuration ( KDfloat fDuration, const Size& tGridSize, KDuint uNumberOfJumps, KDfloat fAmplitude );

		/** amplitude of the sin*/
		inline KDfloat				getAmplitude ( KDvoid ) const { return m_fAmplitude; }
		inline KDvoid				setAmplitude ( KDfloat fAmplitude ) { m_fAmplitude = fAmplitude; }

		/** amplitude rate */
		inline KDfloat				getAmplitudeRate ( KDvoid ) const { return m_fAmplitudeRate; }
		inline KDvoid				setAmplitudeRate ( KDfloat fAmplitudeRate ) { m_fAmplitudeRate = fAmplitudeRate; }

		// Override
		virtual JumpTiles3D*		clone ( KDvoid ) const override;
		virtual KDvoid				update ( KDfloat fTime ) override;

	protected :

		KDuint						m_uJumps;
		KDfloat						m_fAmplitude;
		KDfloat						m_fAmplitudeRate;
};

/** @brief SplitRows action */
class CC_DLL SplitRows : public TiledGrid3DAction
{
	public :

		/** creates the action with the number of rows to split and the duration */
		static SplitRows*			create ( KDfloat fDuration, KDuint uRows );

		/** initializes the action with the number of rows to split and the duration */
		KDbool						initWithDuration ( KDfloat fDuration, KDuint uRows );

		// Overrides
		virtual SplitRows*			clone ( KDvoid ) const override;
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;

	protected :

		KDuint						m_uRows;
		Size						m_tWinSize;
};

/** @brief SplitCols action */
class CC_DLL SplitCols : public TiledGrid3DAction
{
	public :

		/** creates the action with the number of columns to split and the duration */
		static SplitCols*			create ( KDfloat fDuration, KDuint uCols );

		/** initializes the action with the number of columns to split and the duration */
		KDbool						initWithDuration ( KDfloat fDuration, KDuint uCols );

		// Overrides
		virtual SplitCols*			clone ( KDvoid ) const override;
		virtual KDvoid				update ( KDfloat fTime ) override;
		virtual KDvoid				startWithTarget ( Node* pTarget ) override;

	protected :

		KDuint						m_uCols;
		Size						m_tWinSize;
};

// end of actions group
/// @}

NS_CC_END

#endif // __ACTION_CCTILEDGRID_ACTION_H__
