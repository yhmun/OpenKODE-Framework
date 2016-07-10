/* --------------------------------------------------------------------------
 *
 *      File            XMStageScene.h
 *      Description     XMStageScene
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

#ifndef __XMStageScene_h__
#define __XMStageScene_h__

class XMBackground;
class XMPlayer;
class XMStageLoader;
class XMContactListener;
class XMContactFilter;
class XMEnemy;
class XMStageUi;

class XMStageScene : public Scene
{
	public :
		
		XMStageScene ( KDvoid );
		virtual ~XMStageScene ( KDvoid );

		CREATE_FUNC ( XMStageScene );

	protected :

		virtual KDbool				init ( KDvoid );
		virtual KDvoid				initVariable ( KDvoid );
		virtual	KDvoid				initPhysics ( KDvoid );
		virtual	KDvoid				initBackground ( KDint nStage );
		virtual KDvoid				initDPadAndButtons ( KDvoid );
		virtual KDvoid				initTouchListener ( KDvoid );
		virtual KDvoid				initStageLoader ( KDint nStage );
		virtual KDvoid				initPlayer ( KDvoid );
		virtual KDvoid				initStageUi ( );

		virtual	KDvoid				onExit ( KDvoid );

		virtual KDvoid				onTouchesBegan ( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid				onTouchesMoved ( const std::vector<Touch*>& aTouches, Event* pEvent );
		virtual KDvoid				onTouchesEnded ( const std::vector<Touch*>& aTouches, Event* pEvent );

		KDvoid						createButtonWithPosition ( const Point& tPosition, const KDchar* szUpFrame, const KDchar* szDownFrame, const KDchar* szName );

		virtual KDvoid				draw ( KDvoid );

		virtual KDvoid				update ( KDfloat fDelta );
		KDvoid						updateStageLoader ( KDvoid );
		KDvoid						operatePlayer ( KDvoid );
		KDvoid						processContactList ( KDfloat fDelta );





		KDvoid						startGame ( KDvoid );
		KDvoid						endGame ( KDvoid );
		KDvoid						addEnemy ( Object* pObject );


	private :

		XMBackground*				m_pBackground;
		GameDPad*					m_pDPad;
		std::vector<GameButton*>	m_aButtons;
		XMStageUi*					m_pXMStageUi;
		XMPlayer*					m_pPlayer;

		XMStageLoader*				m_pStageLoader;

		KDbool						m_bIsStart;

		b2World*					m_pWorld;
		XMContactListener*			m_pContactListener;
		XMContactFilter*			m_pContactFilter;
		Box2DDebugDraw*				m_pDebugDraw;

};

#endif  // __XMStageScene_h__