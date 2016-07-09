/* --------------------------------------------------------------------------
 *
 *      File            KWVector.h
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      giginet - 10/12/08
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2010-2013 Kawaz. All rights reserved.                             
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
#include "KWVector.h"

KWVector* KWVector::create ( KDvoid )
{
	KWVector*	pRet = new KWVector ( );

	if ( pRet && pRet->init ( ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KWVector* KWVector::create ( const CCPoint& tPoint )
{
	KWVector*	pRet = new KWVector ( );

	if ( pRet && pRet->initWithPoint ( tPoint ) )
	{
		pRet->autorelease ( );
	}
	else
	{
		CC_SAFE_DELETE ( pRet );
	}

	return pRet;
}

KDbool KWVector::init ( KDvoid )
{
	x = 0;
	y = 0;

	return KD_TRUE;
}

KDbool KWVector::initWithPoint ( const CCPoint& tPoint )
{
	x = tPoint.x;
	y = tPoint.y;

	return KD_TRUE;
}

KWVector* KWVector::set ( const CCPoint& tPoint )
{
	x = tPoint.x;
	y = tPoint.y;

	return this;
}

KWVector* KWVector::clone ( KDvoid )
{
	return KWVector::create ( ccp ( x, y ) );
}

KWVector* KWVector::add ( KWVector* v )
{
	x += v->x;
	y += v->y;

	return this;
}

KWVector* KWVector::sub ( KWVector* v )
{
	x -= v->x;
	y -= v->y;

	return this;
}

KDfloat KWVector::scalar ( KWVector* v )
{
	return x * v->x + y * v->y;
}

KDfloat KWVector::cross ( KWVector* v )
{
	return x * v->y - y * v->x;
}

KWVector* KWVector::scale ( KDfloat n )
{
	x *= n;
	y *= n;

	return this;
}

KDfloat KWVector::length ( KDvoid )
{
	return kdSqrtf ( x * x + y * y );
}

KWVector* KWVector::normalize ( KDvoid )
{
	if ( this->length ( ) == 0 )
	{
		x = 0;
		y = 0;
		return this;
	}
	else
	{
		return this->scale ( 1 / this->length ( ) );
	}
}

KWVector* KWVector::resize ( KDfloat n )
{
	return this->normalize ( )->scale ( n );
}

KDfloat KWVector::angle ( KDvoid )
{
	return kdAtan2f ( y, x );
}

KWVector* KWVector::rotate ( KDfloat deg )
{
	KDfloat		rad = KD_PI_F * deg / 180;
	KDfloat		tmpx = x;
	
	x = kdSinf ( rad ) * y + kdCosf ( rad ) * x;
	y = kdCosf ( rad ) * y - kdSinf ( rad ) * tmpx;

	return this;
}

KWVector* KWVector::reverse ( KDvoid )
{
	x *= -1;
	y *= -1;

	return this;
}

KWVector* KWVector::zero ( KDvoid )
{
	return this->set ( ccp ( 0, 0 ) );
}

KWVector* KWVector::max ( KDfloat max )
{
	if ( this->length ( ) > max )
	{
		this->resize ( max );
	}

	return this;
}

KWVector* KWVector::min ( KDfloat min )
{
	if ( this->length ( ) < min )
	{
		this->resize ( min );
	}

	return this;
}

KDbool KWVector::isEqual ( KWVector* v )
{
	return v->x == x && v->y == y;
}

const KDchar* KWVector::description ( KDvoid )
{
	return ccszf ( "(%f, %f)", x, y );
}


