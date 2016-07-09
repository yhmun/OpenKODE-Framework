/* --------------------------------------------------------------------------
 *
 *      File            T01_HelloWorld.h
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
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

#ifndef __T01_HelloWorld_h__
#define __T01_HelloWorld_h__

/** Example 001 HelloWorld

	This Tutorial shows how to set up the IDE for using the Irrlicht Engine and how
	to write a simple HelloWorld program with it. The program will show how to use
	the basics of the VideoDriver, the GUIEnvironment, and the SceneManager.
	Microsoft Visual Studio is used as an IDE, but you will also be able to
	understand everything if you are using a different one or even another
	operating system than windows.

	You have to include the header file <irrlicht.h> in order to use the engine. The
	header file can be found in the Irrlicht Engine SDK directory \c include. To let
	the compiler find this header file, the directory where it is located has to be
	specified. This is different for every IDE and compiler you use. Let's explain
	shortly how to do this in Microsoft Visual Studio:

	- If you use Version 6.0, select the Menu Extras -> Options.
	  Select the directories tab, and select the 'Include' Item in the combo box.
	  Add the \c include directory of the irrlicht engine folder to the list of
	  directories. Now the compiler will find the Irrlicht.h header file. We also
	  need the irrlicht.lib to be found, so stay in that dialog, select 'Libraries'
	  in the combo box and add the \c lib/VisualStudio directory.
	  \image html "vc6optionsdir.jpg"
	  \image latex "vc6optionsdir.jpg"
	  \image html "vc6include.jpg"
	  \image latex "vc6include.jpg"

	- If your IDE is Visual Studio .NET, select Tools -> Options.
	  Select the projects entry and then select VC++ directories. Select 'show
	  directories for include files' in the combo box, and add the \c include
	  directory of the irrlicht engine folder to the list of directories. Now the
	  compiler will find the Irrlicht.h header file. We also need the irrlicht.lib
	  to be found, so stay in that dialog, select 'show directories for Library
	  files' and add the \c lib/VisualStudio directory.
	  \image html "vcnetinclude.jpg"
	  \image latex "vcnetinclude.jpg"

	That's it. With your IDE set up like this, you will now be able to develop
	applications with the Irrlicht Engine.

	Lets start!

	After we have set up the IDE, the compiler will know where to find the Irrlicht
	Engine header files so we can include it now in our code.
*/
class CT01_HelloWorld : public CTutorial
{
	public :

		CT01_HelloWorld ( KDvoid );

	public :

		virtual video::SColor  getClear ( KDvoid );

		virtual const wchar_t*  getTitle ( KDvoid );
};

#endif