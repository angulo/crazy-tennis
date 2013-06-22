/* 
 * InputAdapter.cpp -- Input adapter implementation
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

#include "InputAdapter.h"

using namespace CrazyTennis;
	
template<> InputAdapter * Ogre::Singleton<InputAdapter>::msSingleton = 0;

void
InputAdapter::_initializeMaps(const std::string &configFilePath)
{
	// Default keymap
	_keyMap[OIS::KC_UP] = Controls::UP;
	_keyMap[OIS::KC_DOWN] = Controls::DOWN;
	_keyMap[OIS::KC_LEFT] = Controls::LEFT;
	_keyMap[OIS::KC_RIGHT] = Controls::RIGHT;
	_keyMap[OIS::KC_ESCAPE] = Controls::START;
	_keyMap[OIS::KC_RETURN] = Controls::SHOT_DRIVE;
	_keyMap[OIS::KC_J] = Controls::SHOT_DRIVE;
	_keyMap[OIS::KC_K] = Controls::SHOT_LOB;

	for (std::map<OIS::KeyCode, Controls::Action>::iterator it = _keyMap.begin();
		it != _keyMap.end(); it++) {

		_keyMapInverse[it->second] = it->first;
	}

	// Default button map
	_buttonMap[1] = Controls::SHOT_DRIVE;
	_buttonMap[2] = Controls::SHOT_LOB;

	for (std::map<int, Controls::Action>::iterator it = _buttonMap.begin();
		it != _buttonMap.end(); it++) {

		_buttonMapInverse[it->second] = it->first;
	}

	// Default axis map
	_axisMap[std::make_pair(0, JOYSTICK_SENSITIVITY)] = Controls::RIGHT;
	_axisMap[std::make_pair(0, -JOYSTICK_SENSITIVITY)] = Controls::LEFT;
	_axisMap[std::make_pair(1, JOYSTICK_SENSITIVITY)] = Controls::DOWN;
	_axisMap[std::make_pair(1, -JOYSTICK_SENSITIVITY)] = Controls::UP;

	for (std::map<std::pair<int, int>, Controls::Action>::iterator it = _axisMap.begin();
		it != _axisMap.end(); it++) {

		_axisMapInverse[it->second] = it->first;
	}
}

InputAdapter::InputAdapter()
{
	_initializeMaps("");
}

InputAdapter::~InputAdapter()
{

}

InputAdapter&
InputAdapter::getSingleton()
{
	if (!msSingleton) {
		InputAdapter *instance = new InputAdapter();
	}

	return *msSingleton;
}

InputAdapter *
InputAdapter::getSingletonPtr()
{
	if (!msSingleton) {
		InputAdapter *instance = new InputAdapter();
	}

	return msSingleton;
}

bool
InputAdapter::hasJoystick() const
{
	return OGF::InputManager::getSingletonPtr()->getJoystick() != NULL;
}

Controls::Action
InputAdapter::inputToAction(const OIS::KeyEvent &inputEvent)
{
	Controls::Action action = Controls::NONE;

	std::map<OIS::KeyCode, Controls::Action>::iterator it = _keyMap.find(inputEvent.key);
	if (it != _keyMap.end()) {
		action = it->second;
	}

	return action;
}

Controls::Action
InputAdapter::inputToAction(const OIS::JoyStickEvent &event, int button)
{
	Controls::Action action = Controls::NONE;

	std::map<int, Controls::Action>::iterator it = _buttonMap.find(button);
	if (it != _buttonMap.end()) {
		action = it->second;
	}

	return action;
}

Controls::Action
InputAdapter::inputToAction(int axis, int value)
{
	Controls::Action action = Controls::NONE;

	for (std::map<std::pair<int, int>, Controls::Action>::iterator it = _axisMap.begin();
		it != _axisMap.end(); it++) {

		std::pair<int, int> axisInput = it->first;
		
		if (axisInput.first == axis) {
			if (value < 0 && axisInput.second < 0 && value <= axisInput.second) {
				action = it->second;
			} else if (value > 0 && axisInput.second > 0 && value >= axisInput.second) {
				action = it->second;
			}
		}
	}

	return action;
}

OIS::KeyCode
InputAdapter::actionToKeyInput(const Controls::Action &action)
{
	OIS::KeyCode input = OIS::KC_NOCONVERT;

	std::map<Controls::Action, OIS::KeyCode>::iterator it = _keyMapInverse.find(action);
	if (it != _keyMapInverse.end()) {
		input = it->second;
	}

	return input;
}

int
InputAdapter::actionToButtonInput(const Controls::Action &action)
{
	int input = -1;

	std::map<Controls::Action, int>::iterator it = _buttonMapInverse.find(action);
	if (it != _buttonMapInverse.end()) {
		input = it->second;
	}

	return input;
}

std::pair<int, int>
InputAdapter::actionToAxisInput(const Controls::Action &action)
{
	std::pair<int,int> input = std::make_pair(-1, -1);

	std::map<Controls::Action, std::pair<int, int> >::iterator it = _axisMapInverse.find(action);
	if (it != _axisMapInverse.end()) {
		input = it->second;
	}

	return input;
}

std::string
InputAdapter::actionToInputText(const Controls::Action &action)
{
	std::string result = "";

	if (hasJoystick()) {
		if (action == Controls::UP || action == Controls::DOWN || action == Controls::LEFT || action == Controls::RIGHT) {
			std::pair<int, int> axisInput = actionToAxisInput(action);
			if (axisInput.first != -1 && axisInput.second != -1) {
				result = "AXIS " + Ogre::StringConverter::toString(axisInput.first);
				result += (axisInput.second > 0 ? " +" :  " -");
			}
		} else {
			int button = actionToButtonInput(action);
			if (button != -1) {
				result = "BUTTON " + Ogre::StringConverter::toString(button);
			}
		}
	}

	if (result != "") {
		result += " or ";
	}
	
	OIS::Keyboard *keyboard = OGF::InputManager::getSingletonPtr()->getKeyboard();
	OIS::KeyCode keyCode = _keyMapInverse[action];
	if (keyCode != -1) {
		result += "KEY " + keyboard->getAsString(keyCode);
	}

	return result;
}

bool
InputAdapter::isActionActive(const Controls::Action &action)
{
	bool isActive = false;

	OIS::Keyboard *keyboard = OGF::InputManager::getSingletonPtr()->getKeyboard();
	OIS::JoyStick *joystick = OGF::InputManager::getSingletonPtr()->getJoystick();

	// First check the keyboard
	isActive = keyboard->isKeyDown(actionToKeyInput(action));

	// Last check the joystick in case its plugged in
	if (!isActive && joystick) {
		OIS::JoyStickState joystickState = joystick->getJoyStickState();
		std::pair<int, int> axisInput = actionToAxisInput(action);

		switch(action) {
			case Controls::UP:
			case Controls::LEFT:
				isActive = joystickState.mAxes[axisInput.first].abs < axisInput.second;
				break;
			case Controls::DOWN:
			case Controls::RIGHT:
				isActive = joystickState.mAxes[axisInput.first].abs > axisInput.second;
				break;
			default:
				{
					int button = actionToButtonInput(action);
					if (button != -1) {
						isActive = joystickState.mButtons[button];
					}
				}
				break;
		}
	}
	
	return isActive;
}

void
InputAdapter::store(const OIS::KeyEvent &inputEvent, const Controls::Action &action)
{
	_keyMap[inputEvent.key] = action;
	_keyMapInverse[action] = inputEvent.key;
}

void
InputAdapter::store(const int &button, const Controls::Action &action)
{
	_buttonMap[button] = action;
	_buttonMapInverse[action] = button;
}

void
InputAdapter::store(const int &axis, const int& value, const Controls::Action &action)
{
	_axisMap[std::make_pair(axis, value)] = action;
	_axisMapInverse[action] = std::make_pair(axis, value);
}
