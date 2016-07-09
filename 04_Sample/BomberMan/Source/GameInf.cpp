/* --------------------------------------------------------------------------
 *
 *      File            GameInf.cpp
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
#include "GameInf.h"

static GameInf*		l_pGameinf = KD_NULL;

GameInf* GameInf::sharedGameInf ( KDvoid )
{
	if ( !l_pGameinf )
	{
		l_pGameinf = new GameInf ( );
		l_pGameinf->init ( );
	}

	return l_pGameinf;
}

KDbool GameInf::init ( KDvoid )
{
	m_nScore = 0;
	m_nLevel = 1;

	return KD_TRUE;
}

