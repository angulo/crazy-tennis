/* 
 * Main.cpp -- Main menu scene implementation file
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

#include "scenes/menus/Main.h"

using namespace CrazyTennis::Scene::Menu;

void
Main::_onActionDone(const Controls::Action &action)
{
	// Prevent going back to previous menu as we dont have any
	if (action == Controls::SHOT_LOB) {
		return;
	} else {
		Base::_onActionDone(action);
	}
}

void
Main::_processCurrentOption()
{
	switch(_currentOption) {
		case OPTION_PLAY:
			{
				Data::Loader *dataLoader = new Data::Loader("../data/game");
				std::list<Data::Player *> players = dataLoader->loadPlayers("players.json");
				
				Data::Match *matchData = new Data::Match(3, false, players.front(), players.back());
				OGF::SceneController::getSingletonPtr()->replace(new CrazyTennis::Scene::Match(matchData));
			}
			break;
		case OPTION_SETTINGS:
			OGF::SceneController::getSingletonPtr()->push(CrazyTennis::Scene::MENU_SETTINGS_MAIN);
			break;
		case OPTION_RECORDS:
			break;
		case OPTION_EXIT:
			_exit = true;
			break;
	}
}

Main::Main()
	:	_exit(false)
{
	_currentOption = OPTION_PLAY;
	_initConfigReader("scenes/menus/main.cfg");
	CEGUI::FontManager::getSingleton().create(_configValue<std::string>("font") + ".font");
}

Main::~Main()
{
}

void
Main::enter()
{
	_container = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "MainContainer");

	_windowBackground = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	_windowBackground->setProperty("Image", "set:Menus image:Splash");
	_windowBackground->setProperty("FrameEnabled", "False");
	_container->addChildWindow(_windowBackground);

	const std::string fontColor = _configValue<std::string>("font_color_unselected");

	_optionsMap[OPTION_PLAY] = _createOptionText("PLAY", _configValue<std::string>("font"), fontColor,
		_configValue<int>("play_x"), _configValue<int>("play_y"));
	_optionsMap[OPTION_RECORDS] = _createOptionText("RECORDS", _configValue<std::string>("font"), fontColor,
		_configValue<int>("records_x"), _configValue<int>("records_y"));
	_optionsMap[OPTION_SETTINGS] = _createOptionText("SETTINGS", _configValue<std::string>("font"), fontColor,
		_configValue<int>("settings_x"), _configValue<int>("settings_y"));
	_optionsMap[OPTION_EXIT] = _createOptionText("EXIT", _configValue<std::string>("font"), fontColor,
		_configValue<int>("exit_x"), _configValue<int>("exit_y"));

	for (std::map<int, CEGUI::Window *>::iterator it = _optionsMap.begin();
		it != _optionsMap.end(); it++) {

		_container->addChildWindow(it->second);
	}

	CEGUI::System::getSingletonPtr()->setGUISheet(_container);
	_setCurrentOption(OPTION_PLAY);
}

bool
Main::frameStarted(const Ogre::FrameEvent &event)
{
	return !_exit;
}
