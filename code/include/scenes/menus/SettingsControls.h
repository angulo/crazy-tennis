/* 
 * SettingsControls.h -- Settings controls menu scene header file
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

#ifndef _SCENE_MENU_SETTINGS_CONTROLS_H_
#define _SCENE_MENU_SETTINGS_CONTROLS_H_

#include <OGF/OGF.h>

#include "InputAdapter.h"
#include "SceneFactory.h"
#include "scenes/menus/Base.h"

namespace CrazyTennis {
	
	namespace Scene {
		
		namespace Menu {

			class SettingsControls: public Base {
				
				enum Option {
					OPTION_UP = 0,
					OPTION_DOWN = 1,
					OPTION_LEFT = 2,
					OPTION_RIGHT = 3,
					OPTION_SHOT_DRIVE = 4,
					OPTION_SHOT_LOB = 5,
					OPTION_START = 6,
					OPTION_BACK = 7
				};

				protected:

					bool _editing;

					/**
					 * @var Input values needed to generate the actions
					 * that can be configure.
					 */
					std::map<int, CEGUI::Window *> _inputValues;

					/**
					 * Initialize the input values map and create the visual representation
					 * of each of the input keys.
					 *
					 * @param yBase Y-coordinate base to place the input values.
					 * @param yIncrement Y-coordinate increment after each value.
					 */
					void _createInputValues(const int &yBase, const int &yIncrement);
					
					/**
					 * Map from menu options to actions.
					 * @param option Menu option.
					 * @return Action corresponding to the menu option.
					 */
					CrazyTennis::Controls::Action _optionToAction(const int &option);

					void _onActionDone(const Controls::Action &action);
					void _processCurrentOption();

				public:
					
					SettingsControls();
					~SettingsControls();

					void enter();
					bool keyPressed(const OIS::KeyEvent &event);
					bool buttonPressed(const OIS::JoyStickEvent &event, int button);
					bool axisMoved(const OIS::JoyStickEvent &event, int axis);
			};
		};
	};
};

#endif
