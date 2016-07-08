/* --------------------------------------------------------------------------
 *
 *      File            ObjectCallback.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
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
#include "ObjectCallback.h"

ObjectCallback* ObjectCallback::create ( CCObject* pObject, SEL_CallFunc pCallback )
{
	ObjectCallback*	pRet = new ObjectCallback ( );

	if ( pRet && pRet->initWithObject ( pObject, pCallback ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool ObjectCallback::initWithObject ( CCObject* pObject, SEL_CallFunc pCallback )
{	
	m_pObject	= pObject;
	m_pCallback	= pCallback;

	return KD_TRUE;
}

CCObject* ObjectCallback::getObject ( KDvoid )
{
	return m_pObject;
}

KDvoid ObjectCallback::setObject ( CCObject* pObject )
{
	m_pObject = pObject;
}

SEL_CallFunc ObjectCallback::getCallback ( KDvoid )
{
	return m_pCallback;
}

KDvoid ObjectCallback::setCallback ( SEL_CallFunc pCallback )
{
	m_pCallback = pCallback;
}