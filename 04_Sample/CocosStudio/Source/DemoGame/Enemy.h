/* -----------------------------------------------------------------------------------
 *
 *      File            Enemy.h
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

#ifndef __Enemy_h__
#define __Enemy_h__

class Enemy	: public Layer
{
	public :

		virtual KDbool			init ( KDvoid );
		virtual KDvoid			update ( KDfloat fDelta );
		virtual KDvoid			onExit ( KDvoid );
	
		virtual KDvoid			beHitted ( KDvoid ) = 0;
		virtual KDvoid			defineAction ( Action* pAction = KD_NULL ) = 0;
		KDbool					isEnemyDied ( KDvoid ) { return m_nHitPoints <= 0; }
		Armature*				getArmature ( KDvoid ) { return m_pEnemyArmature; }

	protected :

		KDint					m_nHitPoints;
		Armature*				m_pEnemyArmature;
};

class KnightEnemy : public Enemy
{
	public :

		static KnightEnemy*		create ( Point tStartPostion, KDint nHP = 100, KDfloat fScaleX = 1.0f, KDfloat fScaleY = 1.0f );

		virtual KDbool			init ( Point tStartPostion, KDint nHP, KDfloat fScaleX, KDfloat fScaleY );
	
		virtual KDvoid			beHitted ( KDvoid );
	
		KDvoid					defineAction ( Action* pAction = KD_NULL );
};

class RobotEnemy : public Enemy
{
	public :

		static RobotEnemy*		create ( Point tStartPostion, KDint nHP = 100, KDfloat fScaleX = 1.0f, KDfloat fScaleY = 1.0f );

		virtual KDbool			init ( Point tStartPostion, KDint nHP, KDfloat fScaleX, KDfloat fScaleY );
	
		virtual KDvoid			beHitted ( KDvoid );
	
		KDvoid					defineAction ( Action* pAction = KD_NULL );
};

#endif	// __Enemy_h__