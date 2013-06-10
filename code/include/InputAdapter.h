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
			CONTINUE = 5,
			BACK = 6,
			START = 7,
			SHOT_DRIVE = 8,
			SHOT_LOB = 9
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

			/**
			 * Initialize the map between input events and semantic actions
			 *
			 * @param configFilePath Path to the config file containing the 
			 *	actions already mapped.
			 */
			void _initializeKeyMap(const std::string &configFilePath);
		
		public:
		
			InputAdapter();
			~InputAdapter();
			
			static InputAdapter & getSingleton();
			static InputAdapter * getSingletonPtr();

			/**
			 * Get the semantic action associated to a key event.
			 *
			 * @param inputEvent Input event.
			 * @return Semantic action associated to the input event.
			 */
			Controls::Action inputToAction(const OIS::KeyEvent &inputEvent);

			/**
			 * Get the input needed to generate a certain action.
			 *
			 * @param action Domain action.
			 * @return Input keycode to press to generate the action.
			 */
			OIS::KeyCode actionToInput(const Controls::Action &action);

			/**
			 * Store a new association between an action and a semantic event.
			 *
			 * @param inputEvent Input event to map from.
			 * @param action Semantic action to map to.
			 */
			void store(const OIS::KeyEvent &inputEvent, Controls::Action action);
	};
};

#endif
