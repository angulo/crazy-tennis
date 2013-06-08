/* 
 * SceneFactory.h -- Scene factory header file
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

#ifndef _SCENE_FACTORY_H_
#define _SCENE_FACTORY_H_

#include <OGF/OGF.h>

#include "scenes/Scenes.h"

namespace CrazyTennis {

	namespace Scene {
		enum { 
			SPLASH = 0,
			MENU_MAIN = 1,
			MATCH = 2
		};
	};

	namespace Widget {
		enum {
			BALL = 3
		};
	};

	class SceneFactory : public OGF::ISceneFactory {
		
		public:

			OGF::Scene * create(OGF::SceneId sceneId);
	};
};

#endif
