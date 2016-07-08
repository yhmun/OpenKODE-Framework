/* --------------------------------------------------------------------------
 *
 *      File            Ch7_GridPathfinding.cpp
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
#include "Ch7_GridPathfinding.h"

KDbool Ch7_GridPathfinding::init ( KDvoid )
{	
	// Superclass initialization and message
	if ( !Recipe::init ( ) )
	{
		return KD_FALSE;
	}

	this->showMessage ( "Tap and hold to draw walls.\nPress 'Find Path' to run the simulation." );

	//Initial variables
	m_tGridSize			= ccp ( 25, 15 );
	m_fNodeSpace		= 16.0f;
	m_tTouchedNode		= ccp ( 0, 0 );
	m_tStartCoord		= ccp ( 2, 2 );
	m_tEndCoord			= ccp ( m_tGridSize.x - 3, m_tGridSize.y - 3 );
	m_bTouchedNodeIsNew = KD_FALSE;
	m_pFoundPath		= new CCArray ( );
	m_bAddMode			= KD_TRUE;
	
	// Seperate grid node
	m_pGridNode = CCNode::create ( );
	m_pGridNode->setPosition ( ccp ( 35, 15 ) );
	this->addChild ( m_pGridNode, 3 ); 

	// Create 2D array (grid)
	m_pGrid = new CCArray ( (KDint) m_tGridSize.x ); 
	for ( KDint x = 0; x < (KDint) m_tGridSize.x; x++ )
	{
		m_pGrid->addObject ( CCArray::createWithCapacity ( (KDint) (KDint) m_tGridSize.y ) );
	}

	// Create AStar nodes and place them in the grid
	for ( KDint x = 0; x < (KDint) m_tGridSize.x; x++ )
	{
		for ( KDint y = 0; y < (KDint) m_tGridSize.y; y++ )
		{
			// Add a node
			AStarNode*	pNode = AStarNode::create ( );
			pNode->setPosition ( ccp ( x * m_fNodeSpace + m_fNodeSpace / 2, y * m_fNodeSpace + m_fNodeSpace / 2 ) );
			( (CCArray*) m_pGrid->objectAtIndex ( x ) )->addObject ( pNode );
		}
	}
	
	// Add neighbor nodes
	for ( KDint x = 0; x < (KDint) m_tGridSize.x; x++ )
	{
		for ( KDint y = 0; y < (KDint) m_tGridSize.y; y++ )
		{			
			// Add a node
			AStarNode*	pNode = (AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( x ) )->objectAtIndex ( y );
			
			// Add self as neighbor to neighboring nodes
			this->addNeighbor ( pNode, x - 1, y - 1 );		// Top-Left
			this->addNeighbor ( pNode, x - 1, y     );		// Left
			this->addNeighbor ( pNode, x - 1, y + 1 );		// Bottom-Left
			this->addNeighbor ( pNode, x    , y - 1 );		// Top
			
			this->addNeighbor ( pNode, x    , y + 1 );		// Bottom
			this->addNeighbor ( pNode, x + 1, y - 1 );		// Top-Right
			this->addNeighbor ( pNode, x + 1, y     );		// Right
			this->addNeighbor ( pNode, x + 1, y + 1 );		// Bottom-Right		
		}
	}

	// Add visual represenation of nodes
	this->addGridArt ( );
	
	// Menu items 
	CCMenuItemFont::setFontSize ( 30 );

	CCMenuItemFont*		pFindPathItem = CCMenuItemFont::create ( "Find Path", this, menu_selector ( Ch7_GridPathfinding::findPath ) );
	pFindPathItem->setScale ( 0.65f );
	
	CCMenuItemToggle*	pSwitchModeItem = CCMenuItemToggle::createWithTarget 
	(
		this, 
		menu_selector ( Ch7_GridPathfinding::switchMode ), 
		CCMenuItemFont::create ( "Switch Mode: Remove Wall" ),
		CCMenuItemFont::create ( "Switch Mode: Add Wall" ), 
		KD_NULL 
	);
	pSwitchModeItem->setScale ( 0.65f );
	
	CCMenu*				pMenu = CCMenu::create ( pFindPathItem, pSwitchModeItem, KD_NULL );
	pMenu->alignItemsVertically ( 0 );
	pMenu->setPosition ( ccp ( 350, 290 ) );
	this->addChild ( pMenu, Z_HUD );

	// Add draw layer
	this->addDrawLayer ( );
	
	// Schedule step method
	this->schedule ( schedule_selector ( Ch7_GridPathfinding::step ) );

	return KD_TRUE;
}

KDvoid Ch7_GridPathfinding::onExit ( KDvoid )
{
	CC_SAFE_DELETE ( m_pGrid );
	CC_SAFE_DELETE ( m_pFoundPath );
	CC_SAFE_DELETE ( m_pSprites );

	Recipe::onExit ( );
}

/// Process touching and moving to add/remove walls 
KDvoid Ch7_GridPathfinding::ccTouchesBegan ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
    CCPoint		tPoint = pTouch->getLocation ( );

	KDint		x = (KDint) ( ( tPoint.x - m_pGridNode->getPosition ( ).x ) / m_fNodeSpace );
	KDint		y = (KDint) ( ( tPoint.y - m_pGridNode->getPosition ( ).y ) / m_fNodeSpace );

	CCPoint		tTouchedNode = ccp ( x, y );
	
	m_bTouchedNodeIsNew = KD_TRUE;
	m_tTouchedNode = tTouchedNode;
}

KDvoid Ch7_GridPathfinding::ccTouchesMoved ( CCSet* pTouches, CCEvent* pEvent )
{
	CCTouch*	pTouch = (CCTouch*) pTouches->anyObject ( );
    CCPoint		tPoint = pTouch->getLocation ( );

	KDint		x = (KDint) ( ( tPoint.x - m_pGridNode->getPosition ( ).x ) / m_fNodeSpace );
	KDint		y = (KDint) ( ( tPoint.y - m_pGridNode->getPosition ( ).y ) / m_fNodeSpace );

	CCPoint		tTouchedNode = ccp ( x, y );

	if ( tTouchedNode.x != m_tTouchedNode.x || tTouchedNode.y != m_tTouchedNode.y )
	{
		m_bTouchedNodeIsNew = KD_TRUE;
	}

	m_tTouchedNode = tTouchedNode;
}

KDvoid Ch7_GridPathfinding::drawLayer ( KDvoid )
{
	// If we have less than two nodes in our path we return
	if ( !m_pFoundPath || m_pFoundPath->count ( ) < 2 )
	{
		return;
	}

	ccDrawColor4B ( 0, 0, 255, 255 );	
	
	// Draw all edges of the path
	for ( KDuint i = 0; i < m_pFoundPath->count ( ) - 1; i++ )
	{
		CCPoint		tPoint1 = ( (CCPointValue*) m_pFoundPath->objectAtIndex ( i     ) )->getValue ( ); 
		CCPoint		tPoint2 = ( (CCPointValue*) m_pFoundPath->objectAtIndex ( i + 1 ) )->getValue ( ); 
		ccDrawLine ( tPoint1, tPoint2 );
	}
}

KDvoid Ch7_GridPathfinding::step ( KDfloat fDelta )
{
	// Flip a touched node
	if ( m_bTouchedNodeIsNew )
	{
		this->flipNodeWithTouchedNode ( m_tTouchedNode );
		m_bTouchedNodeIsNew = KD_FALSE;
	}
}

/// Add draw layer 
KDvoid Ch7_GridPathfinding::addDrawLayer ( KDvoid )
{
	m_pDrawLayer = DrawLayer::create ( );
	m_pDrawLayer->setPosition ( ccp ( 0, 0 ) );
	m_pDrawLayer->setRecipe ( this );
	m_pGridNode->addChild ( m_pDrawLayer, 10 );
}

/// Switch between adding nodes and adding walls 
KDvoid Ch7_GridPathfinding::switchMode ( CCObject* pSender )
{
	m_bAddMode = !m_bAddMode;
}

/// Find a path from the startNode to the endNode 
KDvoid Ch7_GridPathfinding::findPath ( CCObject* pSender )
{
	AStarNode*		pStartNode = (AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( (KDint) m_tStartCoord.x ) )->objectAtIndex ( (KDint) m_tStartCoord.y ); 
	AStarNode*		pEndNode   = (AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( (KDint) m_tEndCoord.x   ) )->objectAtIndex ( (KDint) m_tEndCoord.y   );

	if ( m_pFoundPath )
	{
		m_pFoundPath->removeAllObjects ( );
		m_pFoundPath->release ( );
	}
	m_pFoundPath = KD_NULL;
	
	// Run the pathfinding algorithm
	m_pFoundPath = AStarPathNode::findPathFrom ( pStartNode, pEndNode );
	CC_SAFE_RETAIN ( m_pFoundPath );
	
	if ( !m_pFoundPath )
	{
		this->showMessage ( "No Path Found" );
	}
	else
	{
		this->showMessage ( "Found Path" );
	}
}

/// Helper method for adding neighbor nodes 
KDvoid Ch7_GridPathfinding::addNeighbor ( AStarNode* pNode, KDint x, KDint y )
{
	if ( x >= 0 && y >= 0 && x < m_tGridSize.x && y < m_tGridSize.y )
	{
		AStarNode*		pNeighbor = (AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( x ) )->objectAtIndex ( y );
		pNode->getNeighbors ( )->addObject ( pNeighbor );
	}
}

/// Add sprites which correspond to grid nodes 
KDvoid Ch7_GridPathfinding::addGridArt ( KDvoid )
{	
	m_pSprites = new CCDictionary ( );

	for ( KDint x = 0; x < (KDint) m_tGridSize.x; x++ )
	{
		for ( KDint y = 0; y < (KDint) m_tGridSize.y; y++ )
		{
			AStarNode*	pNode = (AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( x ) )->objectAtIndex ( y );

			CCSprite*	pSprite = CCSprite::create ( "gridNode.png" );
			pSprite->setPosition ( pNode->getPosition ( ) );
			if ( pNode->isActive ( ) )
			{
				pSprite->setColor ( ccc3 ( 200, 200, 200 ) );
			}
			else
			{
				pSprite->setColor ( ccc3 ( 100, 100, 100 ) );
			}
			m_pGridNode->addChild ( pSprite );
			m_pSprites->setObject ( pSprite, ccszf ( "(%d,%d)", x, y ) );
		}
	}
	
	// Add start point at (0,0)
	CCSprite*	pStartSprite =  CCSprite::create ( "start_button.png" );
	pStartSprite->setPosition ( ccp ( m_tStartCoord.x * m_fNodeSpace + m_fNodeSpace / 2, m_tStartCoord.y * m_fNodeSpace + m_fNodeSpace / 2 ) );
	m_pGridNode->addChild ( pStartSprite );
	
	// Add end point at (gridSize.x-1,gridSize.y-1)
	CCSprite*	pEndSprite =  CCSprite::create ( "end_button.png" );
	pEndSprite->setPosition ( ccp ( m_tEndCoord.x * m_fNodeSpace + m_fNodeSpace / 2, m_tEndCoord.y * m_fNodeSpace + m_fNodeSpace / 2 ) );
	m_pGridNode->addChild ( pEndSprite );
}

/// Flip a node when touched 
KDvoid Ch7_GridPathfinding::flipNodeWithTouchedNode ( const CCPoint& tPoint )
{
	KDint	x = (KDint) tPoint.x;
	KDint	y = (KDint) tPoint.y;
	
	if ( x == 0 && y == 0 )
	{
		return;
	}
	if ( x == m_tGridSize.x - 1 && y == m_tGridSize.y - 1 )
	{
		return;
	}
	
	if ( x < 0 || y < 0 || x > m_tGridSize.x - 1 || y > m_tGridSize.y - 1 )
	{
		return;
	}
	
	AStarNode*		pNode = (AStarNode*) ( (CCArray*) m_pGrid->objectAtIndex ( x ) )->objectAtIndex ( y );
	CCSprite*		pSprite = (CCSprite*) m_pSprites->objectForKey ( ccszf ( "(%d,%d)", x, y ) );
	
	if ( pNode->isActive ( ) && m_bAddMode )
	{
		// Remove node as neighbor and vice versa
		pNode->setActive ( KD_FALSE );
		pSprite->setColor ( ccc3 ( 100, 100, 100 ) );
	}
	else if ( !m_bAddMode )
	{
		pNode->setActive ( KD_TRUE );
		// Change sprite color
		pSprite->setColor ( ccc3 ( 200, 200, 200 ) );
	}
}