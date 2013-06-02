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

#include "SceneFactory.h"

namespace CrazyTennis {
	
	namespace Scene {

		class MenuMain: public OGF::Scene {

			private:
				
				CEGUI::Window *_windowBackground;
				CEGUI::Window *_container;

				CEGUI::Window * _createOptionText(const std::string &text, const int &x, const int &y, const std::string &color);

			public:
				
				MenuMain();
				~MenuMain();

				void enter();
				void exit();
				void pause();
				void resume();

				bool keyPressed(const OIS::KeyEvent &event);
		};
	};
};

#endif
