/* -----------------------------------------------------------------------------------
 *
 *      File            Enemy.cpp
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
#include "Enemy.h"

KDbool Enemy::init ( KDvoid )
{
	if ( !Layer::init ( ) )
	{
		return false;
	}

	scheduleUpdate ( );

	return true;
}

KDvoid Enemy::update ( KDfloat fDelta )
{
	this->setZOrder ( (KDint) Director::getInstance ( )->getVisibleSize ( ).height - m_pEnemyArmature->getPosition ( ).y );
}

KDvoid Enemy::onExit ( KDvoid )
{
	unscheduleUpdate ( );
	Layer::onExit ( );
}



KnightEnemy* KnightEnemy::create ( Point tStartPostion, KDint nHP, KDfloat fScaleX, KDfloat fScaleY )
{
	KnightEnemy*	pRet = new KnightEnemy ( );

    if ( pRet && pRet->init ( tStartPostion, nHP, fScaleX, fScaleY ) )
    {
        pRet->autorelease ( );        
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool KnightEnemy::init ( Point tStartPostion, KDint nHP, KDfloat fScaleX, KDfloat fScaleY )
{
	if ( !Enemy::init ( ) )
	{
		return false;
	}

	ArmatureDataManager::getInstance( ) ->addArmatureFileInfo ( "armature/hero/Hero.ExportJson" );
	m_pEnemyArmature = Armature::create ( "Hero" );
	m_pEnemyArmature->getAnimation ( )->playByIndex ( 1 );

	m_nHitPoints = nHP;
	m_pEnemyArmature->setPosition ( tStartPostion );
	m_pEnemyArmature->setScaleX ( fScaleX );
	m_pEnemyArmature->setScaleY ( fScaleY );
	this->addChild ( m_pEnemyArmature );
	
	return true;
}

KDvoid KnightEnemy::beHitted ( KDvoid )
{
	m_nHitPoints -= 10;
}

KDvoid KnightEnemy::defineAction ( Action* pAction )
{
	if ( pAction == KD_NULL )
	{
        //define knightenemy form right to left
        auto	pActionToLeft = MoveBy::create ( 5, Point ( Director::getInstance ( )->getVisibleSize ( ).width * -1.3, 0 ) );
        auto	pActionScaleN = ScaleTo::create ( 0.3f, -0.5f, 0.5f );
        auto	pActionScaleP = ScaleTo::create ( 0.3f, 0.5f, 0.5f );
        auto	pActionToRight = MoveBy::create ( 5, Point ( Director::getInstance ( )->getVisibleSize ( ).width * 1.3, 0 ) );
        pAction = RepeatForever::create ( Sequence::create ( pActionToRight, pActionScaleN, pActionToLeft, pActionScaleP, nullptr ) );
	}
	m_pEnemyArmature->runAction ( pAction );
}


RobotEnemy* RobotEnemy::create ( Point tStartPostion, KDint nHP, KDfloat fScaleX, KDfloat fScaleY )
{
	RobotEnemy*		pRet = new RobotEnemy ( );

    if ( pRet && pRet->init ( tStartPostion, nHP, fScaleX, fScaleY ) )
    {
        pRet->autorelease ( );        
    }
    else
    {
        CC_SAFE_DELETE ( pRet );
    }

	return pRet;
}

KDbool RobotEnemy::init ( Point tStartPostion, KDint nHP, KDfloat fScaleX, KDfloat fScaleY )
{
	if ( !Enemy::init ( ) )
	{
		return false;
	}

	ArmatureDataManager::getInstance ( )->addArmatureFileInfo ( "armature/monster/MonsterEnemy.ExportJson" );

	m_pEnemyArmature = Armature::create ( "MonsterEnemy" );
	m_pEnemyArmature->getAnimation ( )->playByIndex ( 1 );

	m_nHitPoints = nHP;
	m_pEnemyArmature->setPosition ( tStartPostion );
	m_pEnemyArmature->setScaleX ( fScaleX );
	m_pEnemyArmature->setScaleY ( fScaleY );
	this->addChild ( m_pEnemyArmature );
	
	return true;
}

KDvoid RobotEnemy::beHitted ( KDvoid )
{
	m_nHitPoints -= 10;
}

KDvoid RobotEnemy::defineAction ( Action* pAction )
{

}