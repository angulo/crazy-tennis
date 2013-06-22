/* 
 * SettingsMain.cpp -- Settings main menu scene implementation file
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

#include "scenes/menus/SettingsMain.h"

using namespace CrazyTennis::Scene::Menu;

void
SettingsMain::_processCurrentOption()
{
	switch(_currentOption) {
		case OPTION_CONTROLS:
			OGF::SceneController::getSingletonPtr()->push(CrazyTennis::Scene::MENU_SETTINGS_CONTROLS);
			break;
		case OPTION_SOUND:
			break;
		case OPTION_BACK:
			OGF::SceneController::getSingletonPtr()->pop();
			break;
	}
}

SettingsMain::SettingsMain()
{
	_currentOption = OPTION_CONTROLS;
	_initConfigReader("scenes/menus/settings.cfg");
	CEGUI::FontManager::getSingleton().create(_configValue<std::string>("font") + ".font");
}

SettingsMain::~SettingsMain()
{
}

void
SettingsMain::enter()
{
	_container = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "SettingsMainContainer");

	_windowBackground = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	_windowBackground->setProperty("Image", "set:Menus image:Splash");
	_windowBackground->setProperty("FrameEnabled", "False");
	_container->addChildWindow(_windowBackground);

	const std::string fontColor = _configValue<std::string>("font_color_unselected");

	_optionsMap[OPTION_CONTROLS] = _createOptionText("CONTROLS", _configValue<std::string>("font"), fontColor,
		_configValue<int>("controls_x"), _configValue<int>("controls_y"));
	_optionsMap[OPTION_SOUND] = _createOptionText("SOUND", _configValue<std::string>("font"), fontColor,
		_configValue<int>("sound_x"), _configValue<int>("sound_y"));
	_optionsMap[OPTION_BACK] = _createOptionText("BACK", _configValue<std::string>("font"), fontColor,
		_configValue<int>("back_x"), _configValue<int>("back_y"));

	for (std::map<int, CEGUI::Window *>::iterator it = _optionsMap.begin();
		it != _optionsMap.end(); it++) {

		_container->addChildWindow(it->second);
	}

	CEGUI::System::getSingletonPtr()->setGUISheet(_container);
	_setCurrentOption(OPTION_CONTROLS);
}
