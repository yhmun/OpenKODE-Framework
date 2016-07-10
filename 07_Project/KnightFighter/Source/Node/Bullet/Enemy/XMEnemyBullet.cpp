/* --------------------------------------------------------------------------
 *
 *      File            XMEnemyBullet.cpp
 *      Description     XMEnemyBullet
 *      Author          J.S Park
 *      Contact         pparkppark84@gmail.com
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
#include "XMEnemyBullet.h"

KDbool XMEnemyBullet::initWithFile ( const std::string& sFilename )
{
	if ( !XMBullet::initWithFile ( sFilename ) )
	{
		return KD_FALSE;
	}

	b2Body*			pBody = getB2Body ( );

	b2Fixture*		pFixture = pBody->GetFixtureList ( );
	if ( pFixture != KD_NULL )
	{
		b2Filter	tFilter = pFixture->GetFilterData ( );
		tFilter.categoryBits = CATEGORY_EBULLET;
		tFilter.groupIndex = GROUPINDEX_ENEMY;
		pFixture->SetFilterData ( tFilter );
	}

	return KD_TRUE;
}
