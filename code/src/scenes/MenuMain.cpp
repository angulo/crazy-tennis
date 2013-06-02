/* 
 * MenuMain.cpp -- Main menu scene implementation file
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

#include "scenes/MenuMain.h"

using namespace CrazyTennis::Scene;

CEGUI::Window *
MenuMain::_createOptionText(const std::string &text, const int &x, const int &y, const std::string &color)
{
	CEGUI::Window *result = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
	result->setProperty("Text", CEGUI::String("[colour='") + color + "']" + text);
	result->setProperty("Font", _configValue<std::string>("font_name"));
	result->setProperty("UnifiedAreaRect", "{{0," + Ogre::StringConverter::toString(x) 
		+ "},{0," + Ogre::StringConverter::toString(y) + "},{1,0},{1,0}}");
	result->setProperty("FrameEnabled", "False");

	return result;
}

MenuMain::MenuMain()
{
	_initConfigReader("scenes/menus/main.cfg");
	CEGUI::FontManager::getSingleton().create(_configValue<std::string>("font_name") + ".font");
}

MenuMain::~MenuMain()
{
}

void
MenuMain::enter()
{
	_container = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "MenuMainContainer");

	_windowBackground = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	_windowBackground->setProperty("Image", "set:Menus image:Splash");
	_windowBackground->setProperty("FrameEnabled", "False");

	_container->addChildWindow(_windowBackground);

	CEGUI::System::getSingletonPtr()->setGUISheet(_container);
}

void
MenuMain::exit()
{
	CEGUI::WindowManager::getSingletonPtr()->destroyWindow(_container);
}

void
MenuMain::pause()
{
	_container->setVisible(false);
}

void
MenuMain::resume()
{
	CEGUI::System::getSingletonPtr()->setGUISheet(_container);
	_container->setVisible(true);
}

bool
MenuMain::keyPressed(const OIS::KeyEvent &event)
{
	return true;
}
