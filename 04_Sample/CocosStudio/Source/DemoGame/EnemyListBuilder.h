/* -----------------------------------------------------------------------------------
 *
 *      File            EnemyListBuilder.h
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

#ifndef __EnemyListBuilder_h__
#define __EnemyListBuilder_h__

#include "Enemy.h"

class EnemyListBuilder
{
	public :

		virtual ~EnemyListBuilder ( KDvoid ) { m_aEnemyList.clear ( ); }

		virtual std::list<Enemy*>	getList ( KDvoid ) { return m_aEnemyList; }
		virtual KDuint				getEnemyNum ( KDvoid ) { return m_aEnemyList.size ( ); }
		virtual Enemy*				getEnemy ( KDvoid );

		KDbool						isTraverseListFinished ( KDvoid );
		KDbool						isEnemyAllDied ( KDvoid ) { return m_aEnemyList.empty ( ); }
		KDvoid						deleteEnemy ( Enemy* pEnemy ) { m_aEnemyList.remove ( pEnemy ); }

	protected :

		KDvoid						initIterPosition ( KDvoid );
		std::list<Enemy*>			m_aEnemyList;
		std::list<Enemy*>::iterator m_pIter;
};

class StageOneEnemyList : public EnemyListBuilder
{
	public :

		StageOneEnemyList ( KDvoid );
};

class StageTwoEnemyList : public EnemyListBuilder
{
	public :

		StageTwoEnemyList ( KDvoid );
};

#endif	// __EnemyListBuilder_h__