/* --------------------------------------------------------------------------
 *
 *      File            MainScene.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2012 cocos2d-x.org
 *
 *         http://www.cocos2d-x.org  
 *         https://code.google.com/p/tweejump-cocos2dx
 *
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */ 

#ifndef __MainScene_h__
#define __MainScene_h__


#define kNumClouds					12

// Fist platform is 30 pixels from the bottom of the screen
#define kStartingCurrentPlatformY	30

#define kMinPlatformStep			50
#define kMaxPlatformStep			300		// This is the top of the screen (SHOULD BE RESOLUTION INDEPENDANT)
#define kNumPlatforms				10		// Number of platforms
#define kPlatformTopPadding			10

#define kMinBonusStep				20
#define kMaxBonusStep				40

enum 
{
	kSpriteManager		= 0	 ,
	kBird					 ,
	kScoreLabel				 ,
	kCloudsStartTag		= 100,
	kPlatformsStartTag	= 200,
	kBonusStartTag		= 300,
};

enum 
{
	kBonus5				= 0,
	kBonus10				,
	kBonus50				,
	kBonus100				,
	kNumBonuses				,
};

class MainScene : public CCLayer
{
	public :
	    
		CREATE_FUNC ( MainScene );

	public :

		virtual KDbool init ( KDvoid );  

		virtual  KDvoid onClose ( CCObject* pSender );

		KDvoid  initClouds ( KDvoid );
		KDvoid  initCloud  ( KDvoid );

		KDvoid  resetClouds ( KDvoid );
		KDvoid  resetCloud  ( KDvoid );

		KDvoid  step ( KDfloat fDelta );

	protected :

		CCSpriteBatchNode*		m_pSpriteSheet;
		CCSprite*				m_pBackground;

		KDfloat					m_fCurrentCloudY;
		KDint					m_nCurrentCloudTag;
};

#endif  // __MainScene_h__