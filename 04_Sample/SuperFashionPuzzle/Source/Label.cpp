/* --------------------------------------------------------------------------
 *
 *      File            Label.cpp
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft. 
 *      Copyright (c) 2010      Ricardo Ruiz López, 2010. All rights reserved.
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
#include "Label.h"

KDbool Label::initWithFontSize ( KDfloat fFontSize, KDint nOffset, const KDchar* szFontName, const ccColor3B& tForegroundColor, const ccColor3B& tBackgroundColor )
{
	if ( !CCLabelTTF::initWithString ( "", szFontName, fFontSize ) )
	{
		return KD_FALSE; 
	}
		
	this->setAnchorPoint ( ccp ( 0, 0.5 ) );
	this->setColor ( tForegroundColor );

	for ( KDint i = 0; i < 8; i++ )
	{
		CCLabelTTF*		pLabel = CCLabelTTF::create ( "", szFontName, fFontSize );
	
		KDint	nOffsetH = 0;
		KDint	nOffsetV = 0;

		// clock-wise from 0 (north) to 7
		switch ( i )
		{
			case 0 :	nOffsetH = 0;			nOffsetV = nOffset;		break;
			case 1 :	nOffsetH = nOffset;		nOffsetV = nOffset;		break;
			case 2 :	nOffsetH = nOffset;		nOffsetV = 0;			break;
			case 3 :	nOffsetH = nOffset;		nOffsetV = -nOffset;	break;
			case 4 :	nOffsetH = 0;			nOffsetV = -nOffset;	break;
			case 5 :	nOffsetH = -nOffset;	nOffsetV = -nOffset;	break;
			case 6 :	nOffsetH = -nOffset;	nOffsetV = 0;			break;
			case 7 :	nOffsetH = -nOffset;	nOffsetV = nOffset;		break;			
		}

		pLabel->setPosition ( ccp ( nOffsetH, nOffsetV ) );
		pLabel->setAnchorPoint ( ccp ( 0, 0 ) );
		pLabel->setColor ( tBackgroundColor );
		this->addChild ( pLabel, this->getZOrder ( ) - 1 );
	}

	return KD_TRUE;
}
	
KDvoid Label::setString ( const KDchar* szString )
{
	CCLabelTTF::setString ( szString );

	CCObject*	pObject;
	CCARRAY_FOREACH ( this->getChildren ( ), pObject )
	{
		CCLabelTTF*		pLabel = (CCLabelTTF*) pObject;
		pLabel->setString ( szString );
	}
}
