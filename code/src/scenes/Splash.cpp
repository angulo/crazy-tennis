/* 
 * Splash.cpp -- Splash screen menu scene
 *
 * Copyright (C) 2013 Javier Angulo Lucerón <javier.angulo1@gmail.com>
 * 
 * This file is part of Crazy Tennis
 *
 * Crazy Tennis is free software: you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation, either 
 * version 3 of the License, or (at your option) any later version.
 * Crazy Tennis is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with Crazy Tennis. 
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "scenes/Splash.h"

using namespace CrazyTennis::Scene;

Splash::Splash()
{
	_initConfigReader("scenes/main_menu.cfg");
	CEGUI::FontManager::getSingleton().create("SFSportsNight-70.font");
}

Splash::~Splash()
{
}

void
Splash::enter()
{
	_container = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "SplashContainer");

	_windowBackground = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	_windowBackground->setProperty("Image", "set:Menus image:Splash");
	_windowBackground->setProperty("FrameEnabled", "False");

	_container->addChildWindow(_windowBackground);
	CEGUI::System::getSingletonPtr()->setGUISheet(_container);
}

void
Splash::exit()
{
	CEGUI::WindowManager::getSingletonPtr()->destroyWindow(_container);
}

void
Splash::pause()
{
	_container->setVisible(false);
}

void
Splash::resume()
{
	CEGUI::System::getSingletonPtr()->setGUISheet(_container);
	_container->setVisible(true);
}

bool
Splash::frameStarted(const Ogre::FrameEvent& event)
{

}

bool
Splash::keyPressed(const OIS::KeyEvent &event)
{
	switch(event.key) {

		case OIS::KC_RETURN:
			break;

		default:
			break;
	}
	
	return true;
}
