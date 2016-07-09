/* -----------------------------------------------------------------------------------
 *
 *      File            ColliderToolKits.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2013 XMSoft
 *      Copyright (c) 2010-2013 cocos2d-x.org
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

#include "Precompiled.h"
#include "ColliderToolKits.h"
#include "Enemy.h"
#include "Player.h"

KDvoid MyContactListener::BeginContact ( b2Contact* pContact )
{
	if ( pContact )
	{
		Contact		tContactContainer;
		tContactContainer.fixture [ 0 ] = pContact->GetFixtureA ( );
		tContactContainer.fixture [ 1 ] = pContact->GetFixtureB ( );

		m_aContactList.push_front ( tContactContainer );
	}

	B2_NOT_USED ( pContact );
}


// Return true if contact calculations should be performed between these two shapes.
// If you implement your own collision filter you may want to build from this implementation.
KDbool MyContactFilter::ShouldCollide ( b2Fixture* pFixtureA, b2Fixture* pFixtureB )
{
	m_pFixtureA = pFixtureA;
	m_pFixtureB = pFixtureB;

	m_tFilterA = pFixtureA->GetFilterData ( );
    m_tFilterB = pFixtureB->GetFilterData ( );

    KDbool	bCollide = collideByGroupAndCatagory ( ) && ( collideBullet ( ) || collideByZOrder ( ) );
	return bCollide;
}

KDbool MyContactFilter::collideByGroupAndCatagory ( KDvoid )
{
    if ( m_tFilterA.groupIndex == m_tFilterB.groupIndex && m_tFilterA.groupIndex != 0 )
    {
        return m_tFilterA.groupIndex > 0;
    }

    KDbool	bCollide = ( m_tFilterA.maskBits & m_tFilterB.categoryBits ) != 0 && ( m_tFilterA.categoryBits & m_tFilterB.maskBits ) != 0;
	return bCollide;
}

KDbool MyContactFilter::collideBullet ( KDvoid )
{
	if ( ( m_tFilterA.categoryBits == BULLET ) || (m_tFilterB.categoryBits == BULLET ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

KDbool MyContactFilter::collideByZOrder ( KDvoid )
{
	KDint	nEnemyZorder = 0, nPlayerZorder = 0;

	if ( m_tFilterA.categoryBits == PLAYER )
	{
		Player*		pPlayer = (Player*) m_pFixtureA->GetUserData ( );
		nPlayerZorder = pPlayer->getZOrder ( );
	}
	if ( m_tFilterB.categoryBits == PLAYER )
	{
		Player*		pPlayer = (Player*) m_pFixtureB->GetUserData ( );
		nPlayerZorder = pPlayer->getZOrder ( );
	}

	if ( m_tFilterA.categoryBits == ENEMY )
	{
		Enemy*		pEnemy = (Enemy*) m_pFixtureA->GetUserData ( );
		nEnemyZorder = pEnemy->getZOrder ( );
	}
	if ( m_tFilterB.categoryBits == ENEMY )
	{
		Enemy*		pEnemy = (Enemy*) m_pFixtureB->GetUserData ( );
		nEnemyZorder = pEnemy->getZOrder ( );
	}
	
	if ( kdAbs ( nPlayerZorder - 10 - nEnemyZorder ) <= 30 )
	{
		return true;
	}
	else
	{
		return false;
	}

}
