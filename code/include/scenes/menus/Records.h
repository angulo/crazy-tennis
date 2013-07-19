/* 
 * Records.h -- Records menu header file.
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

#ifndef _SCENE_MENU_RECORDS_MAIN_H_
#define _SCENE_MENU_RECORDS_MAIN_H_

#include <OGF/OGF.h>

#include "data/RecordsStorage.h"
#include "scenes/menus/Base.h"
#include "SceneFactory.h"

namespace CrazyTennis {
	
	namespace Scene {
		
		namespace Menu {

			class Records: public Base {
				
				enum Options {
					OPTION_BACK = 0
				};
				
				protected:
					
					CEGUI::Window * _createRecordEntry(const std::string name, const int &points, const int &number);
					void _processCurrentOption();
					void _paintRecords();

				public:
					
					Records();
					~Records();

					void enter();
			};
		};
	};
};

#endif
