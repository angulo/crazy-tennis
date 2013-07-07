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

#include "data/Data.h"
#include "scenes/menus/Base.h"
#include "SceneFactory.h"
#include "SoundPlayer.h"

namespace CrazyTennis {
	
	namespace Scene {
		
		namespace Menu {

			class Main: public Base {
				
				enum Option {
					OPTION_PLAY = 0,
					OPTION_RECORDS = 1,
					OPTION_SETTINGS = 2,
					OPTION_EXIT = 3
				};

				protected:
					
					bool _exit;
					void _onActionDone(const Controls::Action &action);
					void _processCurrentOption();

				public:
					
					Main();
					~Main();

					void enter();
					bool frameStarted(const Ogre::FrameEvent &event);
			};
		};
	};
};

#endif
