/* --------------------------------------------------------------------------
 *
 *      File            XMCompanyLogo.cpp
 *      Description     XMCompanyLogo
 *      Author          Pill-Gwan Jeon
 *      Contact         jpg3087@gmail.com
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
#include "XMCompanyLogo.h"
#include "Node/Scene/XMLoading.h"

XMCompanyLogo::XMCompanyLogo ( KDvoid )
{

}

XMCompanyLogo::~XMCompanyLogo ( KDvoid )
{

}

KDbool init ( KDvoid );

KDbool XMCompanyLogo::init ( KDvoid )
{
	if ( !Scene::init ( ) )
	{
		return KD_FALSE;
	}

	Sprite*		pSpriteCompanyLogo = Sprite::create ( "Xmlogo.jpg" );
	pSpriteCompanyLogo->setPosition ( Point ( m_tContentSize / 2 ) );
	this->addChild ( pSpriteCompanyLogo );

	scheduleOnce ( schedule_selector ( XMCompanyLogo::changeScene ), 1.0f );

	return KD_TRUE;
}

KDvoid XMCompanyLogo::changeScene ( KDfloat fDelta )
{
	CCLOG ( "CompanyLogo" );
	Director::getInstance ( )->replaceScene ( XMLoading::create ( ) );
}
