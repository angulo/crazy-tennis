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
MenuMain::_createOptionText(const std::string &text, const std::string &font, const int &x, const int &y)
{
	CEGUI::Window *result = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
	result->setProperty("Text", CEGUI::String("[colour='") + _configValue<std::string>("font_color_unselected") + "']" + text);
	result->setProperty("Font", font);
	result->setProperty("UnifiedPosition", "{{0," + Ogre::StringConverter::toString(x) 
		+ "},{0," + Ogre::StringConverter::toString(y) + "}}");
	result->setProperty("FrameEnabled", "False");

	return result;
}

void
MenuMain::_setCurrentOption(Option option)
{
	// Reset the previously selected option
	CEGUI::Window *currentOptionWindow = _optionsMap[_currentOption];
	CEGUI::String currentOptionText = currentOptionWindow->getText();
	currentOptionWindow->setText(currentOptionText.replace(9, 8, 
		_configValue<std::string>("font_color_unselected")));

	_currentOption = option;

	// Set the new option
	currentOptionWindow = _optionsMap[_currentOption];
	currentOptionText = currentOptionWindow->getText();
	currentOptionWindow->setText(currentOptionText.replace(9, 8, 
		_configValue<std::string>("font_color_selected")));
}

void
MenuMain::_processCurrentOption()
{
	switch(_currentOption) {
		case OPTION_PLAY:
			OGF::SceneController::getSingletonPtr()->replace(CrazyTennis::Scene::MATCH);
			break;
		case OPTION_SETTINGS:
			break;
		case OPTION_RECORDS:
			break;
		case OPTION_EXIT:
			_exit = true;
			break;
	}
}

MenuMain::MenuMain()
	:	_exit(false), _currentOption(OPTION_PLAY)
{
	_initConfigReader("scenes/menus/main.cfg");
	CEGUI::FontManager::getSingleton().create(_configValue<std::string>("font_small") + ".font");
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

	_optionsMap[OPTION_PLAY] = _createOptionText("PLAY", _configValue<std::string>("font_big"), _configValue<int>("play_x"), _configValue<int>("play_y"));
	_optionsMap[OPTION_RECORDS] = _createOptionText("RECORDS", _configValue<std::string>("font_small"),  _configValue<int>("records_x"), _configValue<int>("records_y"));
	_optionsMap[OPTION_SETTINGS] = _createOptionText("SETTINGS", _configValue<std::string>("font_small"),  _configValue<int>("settings_x"), _configValue<int>("settings_y"));
	_optionsMap[OPTION_EXIT] = _createOptionText("EXIT", _configValue<std::string>("font_small"),  _configValue<int>("exit_x"), _configValue<int>("exit_y"));

	for (std::map<Option, CEGUI::Window *>::iterator it = _optionsMap.begin();
		it != _optionsMap.end(); it++) {

		_container->addChildWindow(it->second);
	}

	CEGUI::System::getSingletonPtr()->setGUISheet(_container);

	_setCurrentOption(OPTION_PLAY);
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
MenuMain::frameStarted(const Ogre::FrameEvent &event)
{
	return !_exit;
}

bool
MenuMain::keyPressed(const OIS::KeyEvent &event)
{
	switch (InputAdapter::getSingletonPtr()->inputToAction(event)) {

		// Go to next option
		case Controls::RIGHT:
		case Controls::DOWN:
			{
				Option newOption = static_cast<Option>((_currentOption + 1) % _optionsMap.size());
				_setCurrentOption(newOption);
			}
			break;
	
		// Go to previous option
		case Controls::LEFT:
		case Controls::UP:
			{
				Option newOption;

				if (_currentOption == 0) {
					newOption = static_cast<Option>(_optionsMap.size() - 1);
				} else {
					newOption = static_cast<Option>(_currentOption - 1);
				}

				_setCurrentOption(newOption);
			}
			break;

		// Process the current option
		case Controls::CONTINUE:
			_processCurrentOption();
			break;
		
		default:
		case Controls::NONE:
			break;
	}

	return true;
}
