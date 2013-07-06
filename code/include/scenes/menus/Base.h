/* 
 * Base.h -- Header file for menus with options.
 *
 * Copyright (C) 2013 Javier Angulo Lucerón <javier.angulo1@gmail.com>
 * 
 * This file is part of Crazy Tennis
 * * Crazy Tennis is free software: you can redistribute it and/or modify it under the terms of * the GNU General Public License as published by the Free Software Foundation, either 
 * version 3 of the License, or (at your option) any later version.
 * Crazy Tennis is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with Crazy Tennis. 
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SCENE_MENU_BASE_H_
#define _SCENE_MENU_BASE_H_

#include <OGF/OGF.h>

#include "InputAdapter.h"
#include "data/Data.h"

namespace CrazyTennis {
	
	namespace Scene {
		
		namespace Menu {

			class Base: public OGF::Scene {
				
				protected:
					
					CEGUI::Window *_windowBackground;
					CEGUI::Window *_container;

					int _currentOption;
					std::map<int, CEGUI::Window *> _optionsMap;

					CEGUI::Window * _createOptionText(const std::string &text, const std::string &font, const std::string &color, const int &x, const int &y);

					virtual void _onActionDone(const Controls::Action &action);
					virtual void _processCurrentOption() = 0;
					virtual void _setCurrentOption(const int &option);

				public:
					
					virtual ~Base();

					virtual void enter() = 0;
					void exit();
					void pause();
					void resume();

					virtual bool keyReleased(const OIS::KeyEvent &event);
					virtual bool buttonReleased(const OIS::JoyStickEvent &event, int button);
					virtual bool axisMoved(const OIS::JoyStickEvent &event, int axis);
			};
		};
	};
};

#endif
