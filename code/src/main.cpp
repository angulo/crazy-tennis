/* 
 * main.cpp -- Main file of the project with system initialization
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

#include <OGF/OGF.h>

#include "Model.h"
#include "SceneFactory.h"
#include "SoundPlayer.h"

int
main(int argc, char **argv)
{
	std::srand(unsigned(std::time(0)));

	OGF::ISceneFactory *sceneFactory = new CrazyTennis::SceneFactory();

	if (!OGF::Bootstrap::getSingletonPtr()->init("resources.cfg", "Crazy Tennis", sceneFactory, CrazyTennis::Scene::SPLASH)) {
		return 1;
	}

	CEGUI::SchemeManager::getSingletonPtr()->create("TaharezLook.scheme");
	OGF::ModelFactory::getSingletonPtr()->initialize(CrazyTennis::Model::getModelMap());
	CrazyTennis::SoundPlayer::getSingletonPtr()->preload();

	OGF::Bootstrap::getSingletonPtr()->run();
	OGF::Bootstrap::getSingletonPtr()->shutdown();

	return 0;
}
