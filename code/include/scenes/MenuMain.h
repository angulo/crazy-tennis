/* 
 * MenuMain.h -- Main menu scene header file
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

#ifndef _SCENE_MENU_MAIN_H_
#define _SCENE_MENU_MAIN_H_

#include <OGF/OGF.h>

#include "InputAdapter.h"
#include "SceneFactory.h"
#include "data/Data.h"

namespace CrazyTennis {
	
	namespace Scene {

		class MenuMain: public OGF::Scene {
			
			enum Option {
				OPTION_PLAY = 0,
				OPTION_RECORDS = 1,
				OPTION_SETTINGS = 2,
				OPTION_EXIT = 3
			};

			private:
				
				CEGUI::Window *_windowBackground;
				CEGUI::Window *_container;

				Option _currentOption;
				std::map<Option, CEGUI::Window *> _optionsMap;
				bool _exit;

				CEGUI::Window * _createOptionText(const std::string &text, const std::string &font, const int &x, const int &y);

				void _onActionDone(const Controls::Action &action);
				void _processCurrentOption();
				void _setCurrentOption(const Option &option);

			public:
				
				MenuMain();
				~MenuMain();

				void enter();
				void exit();
				void pause();
				void resume();

				bool frameStarted(const Ogre::FrameEvent &event);
				bool keyPressed(const OIS::KeyEvent &event);
				bool buttonPressed(const OIS::JoyStickEvent &event, int button);
				bool axisMoved(const OIS::JoyStickEvent &event, int axis);
		};
	};
};

#endif
