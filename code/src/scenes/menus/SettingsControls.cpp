/* 
 * SettingsControls.cpp -- Settings controls menu scene implementation file
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

#include "scenes/menus/SettingsControls.h"

using namespace CrazyTennis::Scene::Menu;

void
SettingsControls::_createInputValues(const int &yBase, const int &yIncrement)
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();

	int total = 0;
	for (std::map<int, CEGUI::Window *>::iterator it = _optionsMap.begin(); it != _optionsMap.end(); it++) {
		Controls::Action action = _optionToAction(it->first);
		if (action != Controls::NONE) {
			std::string textInput = inputAdapter->actionToInputText(_optionToAction(it->first));
			if (textInput.length() > _configValue<int>("maxInputValueTextLength")) {
				textInput = textInput.substr(0, _configValue<int>("maxInputValueTextLength") - 2) + "..";
			}
			_inputValues[it->first] = _createOptionText(textInput, _configValue<std::string>("font_input_value"),
				_configValue<std::string>("font_color_input_value"), _configValue<int>("input_value_x"), yBase + (total++ * yIncrement));
			_container->addChildWindow(_inputValues[it->first]);
		}
	}
}

CrazyTennis::Controls::Action
SettingsControls::_optionToAction(const int &option)
{
	Controls::Action result = Controls::NONE;

	switch (option) {
		case OPTION_UP:
			result = Controls::UP;
			break;
		case OPTION_DOWN:
			result = Controls::DOWN;
			break;
		case OPTION_LEFT:
			result = Controls::LEFT;
			break;
		case OPTION_RIGHT:
			result = Controls::RIGHT;
			break;
		case OPTION_SHOT_DRIVE:
			result = Controls::SHOT_DRIVE;
			break;
		case OPTION_SHOT_LOB:
			result = Controls::SHOT_LOB;
			break;
		case OPTION_START:
			result = Controls::START;
			break;
	}

	return result;
}

void
SettingsControls::_savePendingChanges()
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();

	for (std::map<Controls::Action, OIS::KeyCode>::iterator it =  _pendingKeyChanges.begin();
		it != _pendingKeyChanges.end(); it++) {
	
		inputAdapter->store(it->second, it->first);
	}

	for (std::map<Controls::Action, int>::iterator it =  _pendingButtonChanges.begin();
		it != _pendingButtonChanges.end(); it++) {

		inputAdapter->store(it->second, it->first);
	}

	for (std::map<Controls::Action, std::pair<int, int> >::iterator it =  _pendingAxisChanges.begin();
		it != _pendingAxisChanges.end(); it++) {
		
		inputAdapter->store(it->second.first, it->second.second, it->first);
	}
}

void
SettingsControls::_onActionDone(const Controls::Action &action)
{
	if (!_editing) {
		Base::_onActionDone(action);
	}
}

void
SettingsControls::_processCurrentOption()
{
	switch(_currentOption) {
		case OPTION_SAVE:
			_savePendingChanges();
		case OPTION_BACK:
			OGF::SceneController::getSingletonPtr()->pop();
			break;
		default:
			_editing = true;
		_inputValues[_currentOption]->setText("[colour='FFFF0000']PRESS KEY OR BUTTON..");
			break;
	}
}

SettingsControls::SettingsControls()
{
	_currentOption = OPTION_UP;	
	_editing = false;
	_initConfigReader("scenes/menus/settingsControls.cfg");
	CEGUI::FontManager::getSingleton().create(_configValue<std::string>("font") + ".font");
	CEGUI::FontManager::getSingleton().create(_configValue<std::string>("font_input_value") + ".font");
}

SettingsControls::~SettingsControls()
{

}

void
SettingsControls::enter()
{
	_container = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "SettingsControlsContainer");

	_windowBackground = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	_windowBackground->setProperty("Image", "set:Menus image:Splash");
	_windowBackground->setProperty("FrameEnabled", "False");
	_container->addChildWindow(_windowBackground);

	const std::string fontColor = _configValue<std::string>("font_color_unselected");

	int offset = 0;
	int yBase = _configValue<int>("action_y_base");
	int yIncrement = _configValue<int>("action_y_increment");

	_optionsMap[OPTION_UP] = _createOptionText("UP", _configValue<std::string>("font"), fontColor,
		_configValue<int>("action_x"), yBase + (yIncrement * offset++));
	_optionsMap[OPTION_DOWN] = _createOptionText("DOWN", _configValue<std::string>("font"), fontColor,
		_configValue<int>("action_x"), yBase + (yIncrement * offset++));
	_optionsMap[OPTION_LEFT] = _createOptionText("LEFT", _configValue<std::string>("font"), fontColor,
		_configValue<int>("action_x"), yBase + (yIncrement * offset++));
	_optionsMap[OPTION_RIGHT] = _createOptionText("RIGHT", _configValue<std::string>("font"), fontColor,
		_configValue<int>("action_x"), yBase + (yIncrement * offset++));
	_optionsMap[OPTION_SHOT_DRIVE] = _createOptionText("DRIVE", _configValue<std::string>("font"), fontColor,
		_configValue<int>("action_x"), yBase + (yIncrement * offset++));
	_optionsMap[OPTION_SHOT_LOB] = _createOptionText("LOB", _configValue<std::string>("font"), fontColor,
		_configValue<int>("action_x"), yBase + (yIncrement * offset++));
	_optionsMap[OPTION_START] = _createOptionText("MENU", _configValue<std::string>("font"), fontColor,
		_configValue<int>("action_x"), yBase + (yIncrement * offset++));
	_optionsMap[OPTION_BACK] = _createOptionText("CANCEL", _configValue<std::string>("font_actions_value"), fontColor,
		_configValue<int>("back_x"), _configValue<int>("back_y"));
	_optionsMap[OPTION_SAVE] = _createOptionText("SAVE", _configValue<std::string>("font_actions_value"), fontColor,
		_configValue<int>("save_x"), _configValue<int>("save_y"));
	_optionsMap[OPTION_SAVE]->setVisible(false);

	for (std::map<int, CEGUI::Window *>::iterator it = _optionsMap.begin();
		it != _optionsMap.end(); it++) {

		_container->addChildWindow(it->second);
	}

	_createInputValues(yBase, yIncrement);

	CEGUI::System::getSingletonPtr()->setGUISheet(_container);
	_setCurrentOption(OPTION_UP);
}


bool
SettingsControls::keyPressed(const OIS::KeyEvent &event)
{
	if (!_editing) {
		Base::keyPressed(event);
	} else {
		_pendingKeyChanges[_optionToAction(_currentOption)] = event.key;
		_editing = false;
		_inputValues[_currentOption]->setText("[colour='FFFF0000']MODIFIED");
		_optionsMap[OPTION_SAVE]->setVisible(true);
	}
	
	 return true;
}

bool
SettingsControls::buttonPressed(const OIS::JoyStickEvent &event, int button)
{
	if (!_editing) {
		Base::buttonPressed(event, button);
	} else {
		_pendingButtonChanges[_optionToAction(_currentOption)] = button;
		_editing = false;
		_inputValues[_currentOption]->setText("[colour='FFFF0000']MODIFIED");
		_optionsMap[OPTION_SAVE]->setVisible(true);
	}

	return true;
}

bool
SettingsControls::axisMoved(const OIS::JoyStickEvent &event, int axis)
{
	if (!_editing) {
		Base::axisMoved(event, axis);
	} else {
		_pendingAxisChanges[_optionToAction(_currentOption)] = std::make_pair(axis, event.state.mAxes[axis].abs);
		_editing = false;
		_inputValues[_currentOption]->setText("[colour='FFFF0000']MODIFIED");
		_optionsMap[OPTION_SAVE]->setVisible(true);
	}
	return true;
}
