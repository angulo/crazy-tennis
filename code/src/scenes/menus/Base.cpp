/* 
 * Base.cpp -- Implementation of the base menu. 
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

#include "scenes/menus/Base.h"

using namespace CrazyTennis::Scene::Menu;

CEGUI::Window *
Base::_createOptionText(const std::string &text, const std::string &font, const std::string &color, const int &x, const int &y)
{
	CEGUI::Window *result = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
	result->setProperty("Text", CEGUI::String("[colour='") + color + "']" + text);
	result->setProperty("Font", font);
	result->setProperty("UnifiedPosition", "{{0," + Ogre::StringConverter::toString(x) 
		+ "},{0," + Ogre::StringConverter::toString(y) + "}}");
	result->setProperty("FrameEnabled", "False");

	return result;
}

void
Base::_onActionDone(const Controls::Action &action)
{
	switch (action) {
		// Go to next option
		case Controls::RIGHT:
		case Controls::DOWN:
			{
				int newOption = (_currentOption + 1) % _optionsMap.size();
				_setCurrentOption(newOption);
				if (!_optionsMap[newOption]->isVisible()) {
					_onActionDone(action);
				} else {
					SoundPlayer::getSingletonPtr()->play(SOUND_MENU_SLIDE);
				}
			}
			break;
	
		// Go to previous option
		case Controls::LEFT:
		case Controls::UP:
			{
				int newOption;

				if (_currentOption == 0) {
					newOption = _optionsMap.size() - 1;
				} else {
					newOption = _currentOption - 1;
				}

				_setCurrentOption(newOption);
				if (!_optionsMap[newOption]->isVisible()) {
					_onActionDone(action);
				} else {
					SoundPlayer::getSingletonPtr()->play(SOUND_MENU_SLIDE);
				}
			}
			break;

		// Process the current option
		case Controls::SHOT_DRIVE:
		case Controls::START:
			_processCurrentOption();
			SoundPlayer::getSingletonPtr()->play(SOUND_MENU_SELECT);
			break;
		// Back to the previous menu
		case Controls::SHOT_LOB:
			OGF::SceneController::getSingletonPtr()->pop();
			SoundPlayer::getSingletonPtr()->play(SOUND_MENU_BACK);
			break;
		default:
			break;
	}
}

void
Base::_setCurrentOption(const int &option)
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

Base::~Base()
{
}


void
Base::exit()
{
	CEGUI::WindowManager::getSingletonPtr()->destroyWindow(_container);
}

void
Base::pause()
{
	_container->setVisible(false);
}

void
Base::resume()
{
	CEGUI::System::getSingletonPtr()->setGUISheet(_container);
	_container->setVisible(true);
}

bool
Base::keyReleased(const OIS::KeyEvent &event)
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();
	_onActionDone(inputAdapter->inputToAction(event));
	return true;
}

bool
Base::buttonReleased(const OIS::JoyStickEvent &event, int button)
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();
	_onActionDone(inputAdapter->inputToAction(event, button));
	return true;
}

bool
Base::axisMoved(const OIS::JoyStickEvent &event, int axis)
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();
	_onActionDone(inputAdapter->inputToAction(axis, event.state.mAxes[axis].abs));
	return true;
}
