/* 
 * SceneFactory.cpp -- Scene factory implementation file
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

#include "SceneFactory.h"

using namespace CrazyTennis;

OGF::Scene *
SceneFactory::create(OGF::SceneId sceneId)
{
	OGF::Scene *scene = NULL;

	switch(sceneId) {
		case Scene::SPLASH:
			scene = new Scene::Splash();
			break;
		case Scene::MENU_MAIN:
			scene = new Scene::MenuMain();
			break;
		default:
			break;
	}

	return scene;
}
