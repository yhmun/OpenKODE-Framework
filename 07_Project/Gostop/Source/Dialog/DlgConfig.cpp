/*
 *
 *      File            DlgConfig.cpp
 *      Description     Config dialog.
 *      Version         0.95.1007
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2011 XMSoft.
 *      Copyright (C) 2011 Blue River Ltd.
 *      Copyright (C) 2011 Blueplay Ltd. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 *                     chris@blueplay.co.kr
 *                     	
 */ 

#include "Precompiled.h"
#include "DlgBase.h"
#include "DlgConfig.h"

//
//  "CDlgConfig" dialog layer
//
KDbool CDlgConfig::init ( KDvoid )
{
	m_bRefresh = KD_TRUE;

	if ( CDialog::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}

	const CCSize&  tLyrSize = this->getContentSize ( );
    CCPoint  tPoint;

    for ( KDuint  i = 0; i < 6; i++ )
    {
        CCLabelTTF*  pName = CCLabelTTF::create 
        (
            g_pResManager->getText ( eTXT_Config_Speed, i ),
            g_pResManager->getPath ( eFNT_Gothic_Bold ),
            18
        );
        {
            CC_ASSERT ( pName );
            
            this->addChild ( pName );
            
			tPoint = ccp ( 50.f, tLyrSize.cy - 50.f * i - 120.f );

			pName->ignoreAnchorPointForPosition ( KD_TRUE );
            pName->setPosition ( tPoint );

			switch ( i )
			{
				case 0 :
					{
						CCMenuRadio*  pRadio = CCMenuRadio::create
						(
							this,
							menu_selector ( CDlgConfig::onSpeed ), 
							CCMenuItemImage::create 
							(
								g_pResManager->getPath ( eIMG_BTN_1_Nor ), 
								g_pResManager->getPath ( eIMG_BTN_1_Nor ),
								g_pResManager->getPath ( eIMG_BTN_1_Sel )
							),
							CCMenuItemImage::create 
							(
								g_pResManager->getPath ( eIMG_BTN_2_Nor ), 
								g_pResManager->getPath ( eIMG_BTN_2_Nor ),
								g_pResManager->getPath ( eIMG_BTN_2_Sel )
							),
							CCMenuItemImage::create 
							(
								g_pResManager->getPath ( eIMG_BTN_3_Nor ), 
								g_pResManager->getPath ( eIMG_BTN_3_Nor ),
								g_pResManager->getPath ( eIMG_BTN_3_Sel )
							),
							KD_NULL
						);
						{
							CC_ASSERT ( pRadio );

							this->addChild ( pRadio );

							pRadio->retain ( );
							pRadio->setPosition ( ccp ( tPoint.x + 235.f, tPoint.y + 15.f ) );
							pRadio->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
							pRadio->alignItemsHorizontally ( 10.f );

							m_pRadio = pRadio;
						}
					}
					break;

				case 1 :
				case 2 :
				case 3 :
					{
						CCMenuItemToggle*  pToggle = CCMenuItemToggle::createWithTarget
						(
							this,
							menu_selector ( CDlgConfig::onToggle ),
							CCMenuItemImage::create 
							(
								g_pResManager->getPath ( eIMG_BTN_On_Nor ), 
								g_pResManager->getPath ( eIMG_BTN_Off_Nor )
							),
							CCMenuItemImage::create 
							(
								g_pResManager->getPath ( eIMG_BTN_Off_Nor ), 
								g_pResManager->getPath ( eIMG_BTN_On_Nor )
							),
							KD_NULL
						);
						{
							CC_ASSERT ( pToggle );
						
							pToggle->retain ( );
							pToggle->setTag ( i );

							m_pToggles [ i - 1 ] = pToggle;
						}

						CCMenu*  pMenu = CCMenu::createWithItem ( pToggle );
						{
							CC_ASSERT ( pMenu );

							this->addChild ( pMenu );

							pMenu->setPosition ( ccp ( tPoint.x + 235.f, tPoint.y + 16.f ) );
							pMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
						}
					}
					break;

				case 4 :
				case 5 :
					{
						CCMenuItemImage*  pReset = CCMenuItemImage::create 
						(
							g_pResManager->getPath ( eIMG_BTN_Reset_Nor ), 
							g_pResManager->getPath ( eIMG_BTN_Reset_Sel ),
							this,
							menu_selector ( CDlgConfig::onReset )
						);
						CC_ASSERT ( pReset );

						pReset->setTag ( i );

						CCMenu*  pMenu = CCMenu::createWithItem ( pReset );
						{
							CC_ASSERT ( pMenu );

							this->addChild ( pMenu );

							pMenu->setPosition ( ccp ( tPoint.x + 235.f, tPoint.y + 15.f ) );
							pMenu->setHandlerPriority ( g_pScrManager->getTopDialogPrioty ( ) );
						}
					}
					break;
			}
        }
    }
    
	this->refresh ( );

	return KD_TRUE;
}

KDvoid CDlgConfig::onExit ( KDvoid )
{
	CC_SAFE_RELEASE ( m_pRadio );

	for ( KDuint  i = 0; i < 3; i++ )
	{
		CC_SAFE_RELEASE ( m_pToggles [ i ] );
	}

	g_pEnvManager->save ( );

	CDialog::onExit ( );
}

const KDchar* CDlgConfig::getPathBG ( KDvoid )
{
	return g_pResManager->getPath ( eIMG_BG_Dlg_Config );
}

const KDchar* CDlgConfig::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Title_Config );
}

CCPoint CDlgConfig::getPosTitle ( KDvoid )
{
	return ccp ( 5.f, -3.f );
}

KDvoid CDlgConfig::refresh ( KDvoid )
{
	m_bRefresh = KD_TRUE;

	m_pRadio->setSelectedIndex ( g_pEnvManager->getSpeed ( ) );

	m_pToggles [ 0 ]->setSelectedIndex ( !g_pEnvManager->getSound ( ) );
	m_pToggles [ 1 ]->setSelectedIndex ( !g_pEnvManager->getVoice ( ) );
	m_pToggles [ 2 ]->setSelectedIndex ( !g_pEnvManager->getVibrate ( ) );

	m_bRefresh = KD_FALSE;
}

KDvoid CDlgConfig::onSpeed ( CCObject* pSender )
{
	if ( !m_bRefresh )
	{
		CCMenuRadio*  pRadio = (CCMenuRadio *) pSender;

		g_pEnvManager->setSpeed ( pRadio->getSelectedIndex ( ) );
		g_pResManager->playSound ( eSND_Etc_Click );
	}
}

KDvoid CDlgConfig::onToggle ( CCObject* pSender )
{
	if ( !m_bRefresh )
	{
		CCMenuItemToggle*  pToggle = (CCMenuItemToggle *) pSender;
		KDbool  bOn = !pToggle->getSelectedIndex ( );

		switch ( pToggle->getTag ( ) )
		{
			case 1 :	
				
				g_pEnvManager->setSound ( bOn );	

				if ( bOn )
				{
					g_pResManager->resumeBGM ( );
				}
				else
				{
					g_pResManager->pauseBGM ( );
				}

				break;

			case 2 :	

				g_pEnvManager->setVoice ( bOn );

				break;

			case 3 :	

				g_pEnvManager->setVibrate ( bOn );	

				break;
		}

		g_pResManager->playSound ( eSND_Etc_Click );
	}
}

KDvoid CDlgConfig::onReset ( CCObject* pSender )
{
	g_pResManager->playSound ( eSND_Etc_Click );

	CCNode*  pNode = (CCNode *) pSender;

	switch ( pNode->getTag ( ) )
	{
		case 4 :
	
			g_pEnvManager->resetConfig ( );

			this->refresh ( );

			break;

		case 5 :

			g_pScrManager->setDialog ( eDlgResetGame );

			break;
	}
	
}

//
//  "CDlgResetGame" dialog layer
//
KDbool CDlgResetGame::init ( KDvoid )
{
	if ( CDlgRequest::init ( ) == KD_FALSE )
	{
		return KD_FALSE;
	}
    
    const CCSize&  tLyrSize = this->getContentSize ( );
    
   	CCLabelTTF*  pRequest = CCLabelTTF::create 
	(
        g_pResManager->getText ( eTXT_Request_Reset_Game ),
        g_pResManager->getPath ( eFNT_Gothic_Bold ),
        18
    );
	{
		CC_ASSERT ( pRequest );
        
		this->addChild ( pRequest );
        
		pRequest->setPosition ( ccp ( tLyrSize.cx / 2.f - 5.f, tLyrSize.cy / 2.f + 30.f ) );
	}   

	return KD_TRUE;
}

const KDchar* CDlgResetGame::getTitle ( KDvoid )
{
	return g_pResManager->getText ( eTXT_Config_Game_Reset );
}

KDvoid CDlgResetGame::onYes ( CCObject* pSender )
{
    CDlgRequest::onYes ( pSender );
    
	g_pEnvManager->resetGame ( );
}