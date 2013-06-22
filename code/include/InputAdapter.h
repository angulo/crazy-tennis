/* 
 * InputAdapter.h -- Header file for the input adapter
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

#ifndef _INPUT_ADAPTER_H_
#define _INPUT_ADAPTER_H_

#define JOYSTICK_SENSITIVITY 2500

#include <OGF/OGF.h>

namespace CrazyTennis {

	namespace Controls {

		/**
		 * Set of possible semantic actions available 
		 * in the game
		 */
		enum Action { 
			NONE = 0,
			UP = 1,
			DOWN = 2,
			LEFT = 3,
			RIGHT = 4,
			SHOT_DRIVE = 5,
			SHOT_LOB = 6,
			START = 7
		};
	};

	class InputAdapter : public Ogre::Singleton<InputAdapter> {
		
		private:	

			/**
			 * Map that stores the current associations between input events
			 * and semantic actions
			 */
			std::map<OIS::KeyCode, Controls::Action> _keyMap;
			std::map<Controls::Action, OIS::KeyCode> _keyMapInverse;
			std::map<int, Controls::Action> _buttonMap;
			std::map<Controls::Action, int> _buttonMapInverse;
			std::map<std::pair<int, int>, Controls::Action> _axisMap;
			std::map<Controls::Action, std::pair<int, int> > _axisMapInverse;

			/**
			 * Initialize the map between input events and semantic actions
			 *
			 * @param configFilePath Path to the config file containing the 
			 *	actions already mapped.
			 */
			void _initializeMaps(const std::string &configFilePath);

		public:
		
			InputAdapter();
			~InputAdapter();
			
			static InputAdapter & getSingleton();
			static InputAdapter * getSingletonPtr();

			/**
			 * Check if there is a joystick plugged in.
			 * @return true if there is a joystick plugged in, false otherwise.
			 */
			bool hasJoystick() const;
			
			/**
			 * Get the semantic action associated to a key event.
			 *
			 * @param inputEvent Input event.
			 * @return Semantic action associated to the input event.
			 */
			Controls::Action inputToAction(const OIS::KeyEvent &inputEvent);

			/**
			 * Get the semantic action associated to a joystick event.
			 *
			 * @param inputEvent Input event.
			 * @return Semantic action associated to the input event.
			 */
			Controls::Action inputToAction(const OIS::JoyStickEvent &inputEvent, int button);

			/**
			 * Get the semantic action associated to a joystick event.
			 *
			 * @param axis Moved axis.
			 * @param value Value of the axis.
			 * @return Semantic action associated to the input event.
			 */
			Controls::Action inputToAction(int axis, int value);

			/**
			 * Get the input needed to generate a certain action.
			 *
			 * @param action Domain action.
			 * @return Input keycode to press to generate the action.
			 */
			OIS::KeyCode actionToKeyInput(const Controls::Action &action);

			/**
			 * Get the input needed to generate a certain action.
			 *
			 * @param action Domain action.
			 * @return Input keycode to press to generate the action.
			 */
			int actionToButtonInput(const Controls::Action &action);

			/**
			 * Get the input needed to generate a certain action.
			 *
			 * @param action Domain action.
			 * @return Axis value to generate the provided action.
			 */
			std::pair<int, int>
			actionToAxisInput(const Controls::Action &action);

			/**
			 * Get the textual representation of the necessary input to generate
			 * a certain action.
			 * @param action Semantic action.
			 * @return Textual representation of the input for the provided action.
			 */
			std::string
			actionToInputText(const Controls::Action &action);

			/**
			 * Check if the provided action is being done right now, that is,
			 * if the input corresponding to a certain action is pressed/clicked
			 * right now.
			 * 
			 * @param action action to check
			 * @return true if the action is active, false otherwise
			 */
			bool isActionActive(const Controls::Action &ction);

			/**
			 * Store a new association between an action and a semantic event.
			 *
			 * @param inputEvent Input event to map from.
			 * @param action Semantic action to map to.
			 */
			void store(const OIS::KeyEvent &inputEvent, const Controls::Action &action);

			/**
			 * Store a new association between an action and a semantic event.
			 *
			 * @param button Joystick button pressed.
			 * @param action Semantic action to map to.
			 */
			void store(const int &button, const Controls::Action &action);

			/**
			 * Store a new association between an action and a semantic event.
			 *
			 * @param axis Axis identification.
			 * @param value Value of the axis.
			 */
			void store(const int &axis, const int& value, const Controls::Action &action);
	};
};

#endif
