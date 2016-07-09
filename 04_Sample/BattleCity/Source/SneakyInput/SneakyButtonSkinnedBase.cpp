/* --------------------------------------------------------------------------
 *
 *      File            SneakyButtonSkinnedBase.cpp
 *      Description     
 *      Ported By       Young-Hwan Mun
 *      Created By      Nick Pannuto on 2010/02/19
 *      Contact         xmsoft77@gmail.com
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2013 XMSoft.
 *      Copyright (c) 2010-2013 cocos2d-x.org
 *      Copyright (c) 2010      Sneakyness, llc. All rights reserved.
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
#include "SneakyButtonSkinnedBase.h"
#include "SneakyButton.h"

NS_CC_BEGIN

SneakyButtonSkinnedBase* SneakyButtonSkinnedBase::create ( KDvoid )
{
	SneakyButtonSkinnedBase*	pRet = new SneakyButtonSkinnedBase ( );

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

KDbool SneakyButtonSkinnedBase::init ( KDvoid ) 
{
	m_pDefaultSprite	= KD_NULL;
	m_pActivatedSprite	= KD_NULL;
	m_pDisabledSprite	= KD_NULL;
	m_pPressSprite		= KD_NULL;
	m_pButton			= KD_NULL;

	if ( !CCSprite::init ( ) )
	{
		return KD_FALSE;
	}	

	this->schedule ( schedule_selector ( SneakyButtonSkinnedBase::watchSelf ) );		

	return KD_TRUE;
}

KDvoid SneakyButtonSkinnedBase::setContentSize ( const CCSize& tContentSize )
{
	m_tContentSize = tContentSize;

	if ( m_pDefaultSprite )
	{
		m_pDefaultSprite->setContentSize ( tContentSize );
	}

	if ( m_pButton )
	{
		m_pButton->setRadius ( tContentSize.cx / 2 );
	}
}

KDvoid SneakyButtonSkinnedBase::watchSelf ( KDfloat fDelta )
{
	if ( !m_pButton )
	{
		return;
	}

	if ( !m_pButton->getStatus ( ) )
	{
		if ( m_pDisabledSprite )
		{
			m_pDisabledSprite->setVisible ( KD_TRUE );
		}
	}
	else 
	{
		if ( !m_pButton->isActive ( ) )
		{
			if ( m_pPressSprite )
			{
				m_pPressSprite->setVisible ( KD_FALSE );
			}

			if ( m_pButton->getValue ( ) == 0 )
			{
				if ( m_pActivatedSprite )
				{
					m_pActivatedSprite->setVisible ( KD_FALSE );
				}

				if ( m_pDefaultSprite )
				{
					m_pDefaultSprite->setVisible ( KD_TRUE );
				}
			}
			else 
			{
				if ( m_pActivatedSprite )
				{
					m_pActivatedSprite->setVisible ( KD_TRUE );
				}
			}
		}
		else
		{
			if ( m_pDefaultSprite )
			{
				m_pDefaultSprite->setVisible ( KD_FALSE );
			}

			if ( m_pPressSprite )
			{
				m_pPressSprite->setVisible ( KD_TRUE );
			}
		}
	}	
}

KDvoid SneakyButtonSkinnedBase::setDefaultSprite ( CCSprite* pSprite )
{
	if ( m_pDefaultSprite )
	{
		this->removeChild ( m_pDefaultSprite, KD_TRUE );
	}

	if ( pSprite )
	{
		this->addChild ( pSprite, 0 );		

		this->setContentSize ( pSprite->getContentSize ( ) );
	}

	m_pDefaultSprite = pSprite; 
}

CCSprite* SneakyButtonSkinnedBase::getDefaultSprite ( KDvoid )
{
	return m_pDefaultSprite;
}

KDvoid SneakyButtonSkinnedBase::setActivatedSprite ( CCSprite* pSprite )
{
	if ( m_pActivatedSprite )
	{
		this->removeChild ( m_pActivatedSprite, KD_TRUE );
	}

	if ( pSprite )
	{
		this->addChild ( pSprite, 1 );

		this->setContentSize ( pSprite->getContentSize ( ) );
	}

	m_pActivatedSprite = pSprite; 
}

CCSprite* SneakyButtonSkinnedBase::getActivatedSprite ( KDvoid )
{
	return m_pActivatedSprite;
}

KDvoid SneakyButtonSkinnedBase::setDisabledSprite ( CCSprite* pSprite )
{
	if ( m_pDisabledSprite )
	{
		this->removeChild ( m_pDisabledSprite, KD_TRUE );
	}

	if ( pSprite )
	{
		this->addChild ( pSprite, 2 );

		this->setContentSize ( pSprite->getContentSize ( ) );
	}

	m_pDisabledSprite = pSprite; 
}

CCSprite* SneakyButtonSkinnedBase::getDisabledSprite ( KDvoid )
{	
	return m_pDisabledSprite;
}

KDvoid SneakyButtonSkinnedBase::setPressSprite ( CCSprite* pSprite )
{
	if ( m_pPressSprite )
	{
		this->removeChild ( m_pPressSprite, KD_TRUE );
	}

	if ( pSprite )
	{
		this->addChild ( pSprite, 3 );

		this->setContentSize ( pSprite->getContentSize ( ) );
	}

	m_pPressSprite = pSprite; 
}

CCSprite* SneakyButtonSkinnedBase::getPressSprite ( KDvoid )
{
	return m_pPressSprite;
}

KDvoid SneakyButtonSkinnedBase::setButton ( SneakyButton* pButton )
{
	if ( m_pButton )
	{
		this->removeChild ( m_pButton, KD_TRUE );
	}

	if ( pButton )
	{		
		this->addChild ( pButton, 4 );

		if ( m_pDefaultSprite )
		{
			pButton->setRadius ( m_pDefaultSprite->boundingBox ( ).size.cx / 2 );
		}
	}

	m_pButton = pButton; 
}

SneakyButton* SneakyButtonSkinnedBase::getButton ( KDvoid )
{
	return m_pButton;
}

NS_CC_END