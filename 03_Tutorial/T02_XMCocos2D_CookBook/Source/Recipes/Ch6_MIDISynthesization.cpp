/* --------------------------------------------------------------------------
 *
 *      File            Ch6_MIDISynthesization.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Nate Burba
 *      Contact         Cocos2dCookbook@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2011      COCOS2D COOKBOOK. All rights reserved. 
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

#include "Precompiled.h"
#include "Ch6_MIDISynthesization.h"

static const KDint	kWhiteKeyNumbers  [ ]	= { 0, 2, 4, 5, 7, 9, 11 };
static const KDint	kWhiteKeyCount			= sizeof ( kWhiteKeyNumbers ) / sizeof ( KDint );
static const KDint	kBlackKey1Numbers [ ]	= { 1, 3 };
static const KDint	kBlackKey1Count			= sizeof ( kBlackKey1Numbers ) / sizeof ( KDint );
static const KDint	kBlackKey2Numbers [ ]	= { 6, 8, 10 };
static const KDint	kBlackKey2Count			= sizeof ( kBlackKey2Numbers ) / sizeof ( KDint );

KDbool Ch6_MIDISynthesization::init ( KDvoid )
{	
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	// Show message
	this->showMessage ( "Play a tune on the keyboard" );

	// Init sample generator
	m_pSampleGenerator = MIDISampleGenerator::create ( );
	m_pSampleGenerator->retain ( );

	// Menu items
	CCMenuItemFont::setFontSize ( 25 );
	CCMenuItemFont::setFontName ( "Marker Felt.ttf" );

	CCMenu*		pMenu = CCMenu::create 
	(
		CCMenuItemFont::create ( "Left"		, this, menu_selector ( Ch6_MIDISynthesization::slideLeft  ) ),
		CCMenuItemFont::create ( "Randomize", this, menu_selector ( Ch6_MIDISynthesization::randomize  ) ),
		CCMenuItemFont::create ( "Right"	, this, menu_selector ( Ch6_MIDISynthesization::slideRight ) ),
		KD_NULL
	);
	pMenu->alignItemsHorizontallyWithPadding ( 100 );
	pMenu->setPosition ( ccp ( 240, 230 ) );
	this->addChild ( pMenu );
	
	// Init keyboard
	this->initKeyboard ( );

	return KD_TRUE;
}

KDvoid Ch6_MIDISynthesization::onExit ( KDvoid )
{
	CC_SAFE_DELETE ( m_pWhiteKeys );
	CC_SAFE_DELETE ( m_pBlackKeys );
	CC_SAFE_DELETE ( m_pKeyTouches );

	CC_SAFE_RELEASE ( m_pSampleGenerator );

	Recipe::onExit ( );
}	

KDvoid Ch6_MIDISynthesization::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	// Process multiple touches
	CCArray*	pAllObjects = pTouches->allObjects ( );
	for ( KDuint i = 0; i < pAllObjects->count ( ); i++ )
	{
		CCTouch*	pTouch = (CCTouch*) pAllObjects->objectAtIndex ( i );
		CCPoint		tPoint = pTouch->getLocation ( );

		// Touch hash tracking
		KDint			nHashKey = pTouch->getID ( );
		KDbool			bKeyPressed = KD_FALSE;
		CCObject*		pObject;

		// Process black keys first
		CCARRAY_FOREACH ( m_pBlackKeys, pObject )
		{
			if ( bKeyPressed )
			{
				break; 
			}

			CCSprite*	pBlackKey = (CCSprite*) pObject;
			if ( this->touchedSprite ( pBlackKey, tPoint ) )
			{
				bKeyPressed = this->keyPressed ( pBlackKey, nHashKey );
			}
		}

		CCARRAY_FOREACH ( m_pWhiteKeys, pObject )
		{
			if ( bKeyPressed )
			{
				break; 
			}

			CCSprite*	pWhiteKey = (CCSprite*) pObject;
			if ( this->touchedSprite ( pWhiteKey, tPoint ) )
			{
				bKeyPressed = this->keyPressed ( pWhiteKey, nHashKey );
			}
		}
	}
}

KDvoid Ch6_MIDISynthesization::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	// Process multiple touches
	CCArray*	pAllObjects = pTouches->allObjects ( );
	for ( KDuint i = 0; i < pAllObjects->count ( ); i++ )
	{
		CCTouch*	pTouch = (CCTouch*) pAllObjects->objectAtIndex ( i );
		CCPoint		tPoint = pTouch->getLocation ( );	

		// Touch hash tracking
		KDint			nHashKey = pTouch->getID ( );
		KDbool			bKeyPressed = KD_FALSE;
		CCObject*		pObject;
				
		// Process black keys first
		CCARRAY_FOREACH ( m_pBlackKeys, pObject )
		{
			if ( bKeyPressed )
			{
				break; 
			}

			CCSprite*	pBlackKey = (CCSprite*) pObject;
			if ( this->touchedSprite ( pBlackKey, tPoint ) )
			{
				CCInteger*	pStoredKeyNumber = (CCInteger*) m_pKeyTouches->objectForKey ( nHashKey );
				if ( pStoredKeyNumber )
				{
					// This hash is already touching something
					if ( pStoredKeyNumber->getValue ( ) != pBlackKey->getTag ( ) )
					{
						// Moved onto a key from another key
						this->keyReleased ( nHashKey, KD_FALSE );
						this->keyPressed ( pBlackKey, nHashKey );
					}
				}
				else
				{
					// Moved onto a key from nothing
					this->keyPressed  ( pBlackKey, nHashKey );					
				}

				bKeyPressed = KD_TRUE;
			}
		}

		CCARRAY_FOREACH ( m_pWhiteKeys, pObject )
		{
			if ( bKeyPressed )
			{
				break; 
			}

			CCSprite*	pWhiteKey = (CCSprite*) pObject;
			if ( this->touchedSprite ( pWhiteKey, tPoint ) )
			{
				CCInteger*	pStoredKeyNumber = (CCInteger*) m_pKeyTouches->objectForKey ( nHashKey );
				if ( pStoredKeyNumber )
				{
					// This hash is already touching something
					if ( pStoredKeyNumber->getValue ( ) != pWhiteKey->getTag ( ) )
					{
						// Moved onto a key from another key
						this->keyReleased ( nHashKey, KD_FALSE );
						this->keyPressed ( pWhiteKey, nHashKey );
					}
				}
				else
				{
					// Moved onto a key from nothing
					this->keyPressed  ( pWhiteKey, nHashKey );					
				}

				bKeyPressed = KD_TRUE;
			}
		}
		
		// If we moved off of a key we stop playing the last note we pressed
		if ( !bKeyPressed )
		{
			this->keyReleased ( nHashKey, KD_TRUE ); 
		}
	}
}

KDvoid Ch6_MIDISynthesization::ccTouchesEnded ( CCSet* pTouches, CCEvent* pEvent )
{
	// Stop sounds and remove sprites
	CCArray*	pAllObjects = pTouches->allObjects ( );
	for ( KDuint i = 0; i < pAllObjects->count ( ); i++ )
	{
		CCTouch*	pTouch = (CCTouch*) pAllObjects->objectAtIndex ( i );

		this->keyReleased ( pTouch->getID ( ), KD_TRUE );
	}
}

KDvoid Ch6_MIDISynthesization::initKeyboard ( KDvoid )
{
	// Data structure init
	m_pWhiteKeys  = new CCArray ( );
	m_pBlackKeys  = new CCArray ( );
	m_pKeyTouches = new CCDictionary ( );
		
	// Main keyboard node
	m_pKeyboard = CCNode::create ( );
	m_pKeyboard->setPosition ( ccp ( 0, 120 ) );
	
	// Helper values
	m_fKeySpriteScale = 0.40f;

	KDfloat		fKeySize = 112 * m_fKeySpriteScale;
	KDfloat		fWhiteKeyHeight = 336 * m_fKeySpriteScale;
	KDint		nNoteStart  = 16;
	KDint		nNextOctave = 12;
	CCPoint		tBlackKey1Offset = ccp ( fKeySize * 0.6666f, 68.666f * m_fKeySpriteScale );
	CCPoint		tBlackKey2Offset = ccp ( fKeySize * 0.6666f + fKeySize * 3, 68.666f * m_fKeySpriteScale );
	
	// Add white keys
	for ( KDint x = 0; x < 2; x++ )
	{
		for ( KDint i = 0; i < kWhiteKeyCount; i++ )
		{
			KDint		nToneNumber = kWhiteKeyNumbers [ i ] + nNoteStart + x * nNextOctave;
			CCSprite*	pWhiteKey = CCSprite::create ( "piano_white_key.png" );
			pWhiteKey->setScale ( m_fKeySpriteScale );
			pWhiteKey->setPosition ( ccp ( ( i + kWhiteKeyCount * x )  * fKeySize, 0 ) );
			pWhiteKey->setTag ( nToneNumber );
		
			m_pKeyboard->addChild ( pWhiteKey, 1 ); 
			m_pWhiteKeys->addObject ( pWhiteKey );
		}
	}
	
	// Add first black key set
	for ( KDint x = 0; x < 2; x++ )
	{
		for ( KDint i = 0; i < kBlackKey1Count; i++ )
		{
			KDint		nToneNumber = kBlackKey1Numbers [ i ] + nNoteStart + x * nNextOctave;
			CCSprite*	pBlackKey = CCSprite::create ( "piano_black_key.png" );
			pBlackKey->setScale ( m_fKeySpriteScale );
			pBlackKey->setPosition ( ccp ( ( i + kWhiteKeyCount * x ) * fKeySize + tBlackKey1Offset.x, tBlackKey1Offset.y ) );
			pBlackKey->setTag ( nToneNumber );
		
			m_pKeyboard->addChild ( pBlackKey, 2 ); 
			m_pBlackKeys->addObject ( pBlackKey );
		}
	}
	
	// Add second black key set
	for ( KDint x = 0; x < 2; x++ )
	{
		for ( KDint i = 0; i < kBlackKey2Count; i++ )
		{
			KDint		nToneNumber = kBlackKey2Numbers [ i ] + nNoteStart + x * nNextOctave;
			CCSprite*	pBlackKey = CCSprite::create ( "piano_black_key.png" );
			pBlackKey->setScale ( m_fKeySpriteScale );
			pBlackKey->setPosition ( ccp ( ( i + kWhiteKeyCount * x ) * fKeySize + tBlackKey2Offset.x, tBlackKey2Offset.y ) );
			pBlackKey->setTag ( nToneNumber );

			m_pKeyboard->addChild ( pBlackKey, 2 ); 
			m_pBlackKeys->addObject ( pBlackKey );
		}
	}
	
	// Black background
	CCSprite*	pBg = CCSprite::create ( "blank.png" );
	pBg->setColor ( ccc3 ( 0, 0, 0 ) );
	pBg->setTextureRect ( CCRectMake ( 0, 0, kWhiteKeyCount * 2 * fKeySize, fWhiteKeyHeight + 20 ) );
	pBg->setPosition ( ccp ( 727.5 * m_fKeySpriteScale, 0 ) );
	m_pKeyboard->addChild ( pBg, 0 ); 
	
	// Finally, add keyboard
	this->addChild ( m_pKeyboard );	
}

KDvoid Ch6_MIDISynthesization::randomize ( CCObject* pSender )
{
	// Randomize values including Modulation, Oscillation, Filter, etc
	m_pSampleGenerator->randomize ( );
}

KDvoid Ch6_MIDISynthesization::slideLeft ( CCObject* pSender )
{
	// Slide keyboard to the left
	KDfloat		fKeySize = 112 * m_fKeySpriteScale;
	m_pKeyboard->runAction ( CCEaseInOut::create ( CCMoveBy::create ( 0.25f, ccp ( fKeySize, 0 ) ), 2 ) );
}

KDvoid Ch6_MIDISynthesization::slideRight ( CCObject* pSender )
{
	// Slide keyboard to the right
	KDfloat		fKeySize = 112 * m_fKeySpriteScale;
	m_pKeyboard->runAction ( CCEaseInOut::create ( CCMoveBy::create ( 0.25f, ccp ( -fKeySize, 0 ) ), 2 ) );
}

KDbool Ch6_MIDISynthesization::keyPressed ( CCSprite* pKey, KDint nHashKey )
{
	// Set darker key color
	pKey->setColor ( ccc3 ( 255, 100, 100 ) );
	
	// Play note
	m_pSampleGenerator->noteOn ( pKey->getTag ( ) );
	
	// Keep track of touch
	m_pKeyTouches->setObject ( CCInteger::create ( pKey->getTag ( ) ), nHashKey ); 
	return KD_TRUE;
}

KDbool Ch6_MIDISynthesization::keyReleased ( KDint nHashKey, KDbool bRemove )
{
	// Set lighter key color
	KDbool		bKeyReleased = KD_FALSE;

	CCInteger*	pNote = (CCInteger*) m_pKeyTouches->objectForKey ( nHashKey );
	if ( !pNote )
	{
		return bKeyReleased;
	}

	KDint		nNote = pNote->getValue ( );
	CCObject*	pObject;

	CCARRAY_FOREACH ( m_pBlackKeys, pObject )
	{
		if ( bKeyReleased )
		{
			break; 
		}

		CCSprite*	pBlackKey = (CCSprite*) pObject;
		if ( pBlackKey->getTag ( ) == nNote )
		{
			pBlackKey->setColor ( ccc3 ( 255, 255, 255 ) );
			bKeyReleased = KD_TRUE;
		}
	}

	CCARRAY_FOREACH ( m_pWhiteKeys, pObject )
	{
		if ( bKeyReleased )
		{
			break; 
		}

		CCSprite*	pWhiteKey = (CCSprite*) pObject;
		if ( pWhiteKey->getTag ( ) == nNote )
		{
			pWhiteKey->setColor ( ccc3 ( 255, 255, 255 ) );
			bKeyReleased = KD_TRUE;
		}
	}

	if ( bKeyReleased )
	{
		// Stop playing note
		m_pSampleGenerator->noteOff ( nNote );
		
		// Remove tracking
		if ( bRemove )
		{
			m_pKeyTouches->removeObjectForKey ( nHashKey ); 
		}
	}
	
	return bKeyReleased;
}

// Tells us whether or not we touched a sprite
KDbool Ch6_MIDISynthesization::touchedSprite ( CCSprite* pSprite, const CCPoint& tTouch )
{	
	CCSize		tSpriteSize  = pSprite->getContentSize ( );
	KDfloat		fSpriteScale = pSprite->getScale ( );
	CCPoint		tSpritePos   = pSprite->getPosition ( );
	CCPoint		tParentPos   = pSprite->getParent ( )->getPosition ( );

	CCSize		tSize  = CCSize  ( tSpriteSize.cx * fSpriteScale, tSpriteSize.cy * fSpriteScale );
	CCPoint		tPoint = CCPoint ( tSpritePos.x - tSize.cx / 2 + tParentPos.x, tSpritePos.y - tSize.cy / 2 + tParentPos.y );	
	CCRect		tRect  = CCRect  ( tPoint.x, tPoint.y, tSize.cx, tSize.cy ); 

	return pointIsInRect ( tTouch, tRect );
}
