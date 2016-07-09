/* --------------------------------------------------------------------------
 *
 *      File            MainMenuLayer.h
 *
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com 
 *
 *      Created By      Craig Newton on 2012/10/04
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

#ifndef __MainMenuLayer_h__
#define __MainMenuLayer_h__

#define kButtonDifficultyEasy		9001
#define kButtonDifficultyMedium		9002
#define kButtonDifficultyHard		9003

// Represents the Main Menu of the Game
class MainMenuLayer : public CCLayer
{
	public :

		// Create method
		CREATE_FUNC ( MainMenuLayer );
	    
	public :

		// Initialize our Main Menu Layer, and add the initial main menu with the title screen, play button and about button.
		virtual	KDbool		init ( KDvoid );
	    
		// Create, initialize, add to layer and perform the initial animation for the Main Menu.
		KDvoid		addMainMenuChildren ( KDvoid );
	    
		// Animate the main menu items to slide out and remove the main menu children from the layer and cleanup.
		KDvoid		removeMainMenuChildren ( KDvoid );
	    
		// Removes the MainMenuTitle from the layer.
		KDvoid		removeGameTitle ( KDvoid );
	    
		// Removes the MainMenu from the layer.
		KDvoid		removeGameMenu ( KDvoid );
	    
		// Create, initialize, add to layer and perform initial animation to display the start button.
		KDvoid		addStartMenuChildren ( KDvoid );
	    
		// Animate the start menu item to slide out and remove the start menu children from the layer and cleanup.
		KDvoid		removeStartMenuChildren ( KDvoid );
	    
		// Removes the start game menu from the layer, and informs the GameManager to start a new game.
		KDvoid		removeStartGameMenu ( KDvoid );
	    
		// Action to perform when the Play Game Button was touched.
		KDvoid		playGameButtonTouched ( CCObject* pSender );
	    
		// Action to perform when the About Game Button was touched.
		KDvoid		aboutGameButtonTouched ( CCObject* pSender );
	    
		// Action to perform when the Start Easy Game Button was touched.
		KDvoid		easyGameButtonTouched ( CCObject* pSender );
	    
		// Action to perform when the Start Medium Game Button was touched.
		KDvoid		mediumGameButtonTouched ( CCObject* pSender );
	    
		// Action to perform when the Start Hard Game Button was touched.
		KDvoid		hardGameButtonTouched ( CCObject* pSender );
};

#endif	// __MainMenuLayer_h__
