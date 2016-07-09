/* --------------------------------------------------------------------------
 *
 *      File            ContectListener.cpp
 *      Description     ContectListener
 *      Author          J.S. Park
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
#include "ContectListener.h"
#include "SampleLayer.h"

ContectListener::ContectListener ( SampleLayer* pParent )
{
	m_pParent = pParent;
}

ContectListener::~ContectListener ( KDvoid )
{

}

KDvoid ContectListener::BeginContact ( b2Contact* contact )
{
	CCLOG ( "ContectListener::BeginContact" );

	m_pParent->m_nMaxJump = 0;
}

KDvoid ContectListener::EndContact ( b2Contact* contact )
{
	CCLOG ( "ContectListener::EndContact" );
}

KDvoid ContectListener::PreSolve ( b2Contact* contect, const b2Manifold* oldManifold )
{
	//CCLOG ( "ContectListener::PreSolve" );
}

KDvoid ContectListener::PostSolve ( b2Contact* contact, const b2ContactImpulse* impulse )
{
	//CCLOG ( "ContectListener::PostSolve" );
}