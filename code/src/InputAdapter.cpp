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
	// Temporal keymap hardcoded
	_keyMap[OIS::KC_UP] = Controls::UP;
	_keyMap[OIS::KC_DOWN] = Controls::DOWN;
	_keyMap[OIS::KC_LEFT] = Controls::LEFT;
	_keyMap[OIS::KC_RIGHT] = Controls::RIGHT;
	_keyMap[OIS::KC_RETURN] = Controls::CONTINUE;
	_keyMap[OIS::KC_ESCAPE] = Controls::BACK;
	_keyMap[OIS::KC_SPACE] = Controls::START;
	_keyMap[OIS::KC_J] = Controls::SHOT_DRIVE;
	_keyMap[OIS::KC_K] = Controls::SHOT_LOB;

	for (std::map<OIS::KeyCode, Controls::Action>::iterator it = _keyMap.begin();
		it != _keyMap.end(); it++) {

		_keyMapInverse[it->second] = it->first;
	}

	_buttonMap[1] = Controls::SHOT_DRIVE;
	_buttonMap[2] = Controls::SHOT_LOB;

	for (std::map<int, Controls::Action>::iterator it = _buttonMap.begin();
		it != _buttonMap.end(); it++) {

		_buttonMapInverse[it->second] = it->first;
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

void
InputAdapter::store(const OIS::KeyEvent &inputEvent, Controls::Action action)
{
	
}
