/* --------------------------------------------------------------------------
 *
 *      File            EnemyBase.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Xing Yan on 12-11-04
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. All rights reserved. 
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
#include "EnemyBase.h"

KDvoid EnemyBase::active ( KDvoid )
{

}

KDbool EnemyBase::beenAttack ( KDint nType )
{
	return KD_FALSE;
}

KDvoid EnemyBase::fire ( KDvoid )
{

}

KDbool EnemyBase::isActive ( KDvoid )
{
	return m_bIsActive;
}

KDvoid EnemyBase::setActive ( KDbool bActive )
{
	m_bIsActive = bActive;
}

KDbool EnemyBase::isFiring ( KDvoid )
{
	return m_bIsFiring;
}

KDvoid EnemyBase::setFiring ( KDbool bFiring )
{
	m_bIsFiring = bFiring;
}

KDint EnemyBase::getType ( KDvoid )
{
	return 0;
}

KDvoid EnemyBase::setType ( KDint nType )
{

}

