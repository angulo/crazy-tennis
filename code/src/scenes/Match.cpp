/* 
 * Match.cpp -- Match scene implementation file
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

#include "scenes/Match.h"

using namespace CrazyTennis::Scene;


void
Match::_createScene()
{
	_loadCameras();
	_loadLights();
	_loadStaticObjects();
	_loadDynamicObjects();
}

void
Match::_loadCameras()
{
	_topCamera = _sceneManager->createCamera("TopCamera");

	_topCamera->setPosition(20, 5, 0);
	_topCamera->lookAt(0, 0, 0);

	_topCamera->setNearClipDistance(0.1);
	_topCamera->setFarClipDistance(10000);
	_topCamera->setFOVy(Ogre::Degree(75));

	_topCameraNode = _sceneManager->createSceneNode("TopCameraNode");
	_topCameraNode->attachObject(_topCamera);
	_topCameraNode->setInheritOrientation(false);

	Ogre::Viewport *viewport;
	Ogre::RenderWindow *renderWindow = Ogre::Root::getSingletonPtr()->getAutoCreatedWindow();

	if (renderWindow->getNumViewports() > 0) {
		viewport = renderWindow->getViewport(0);
		viewport->setCamera(_topCamera);
	} else {
		viewport = renderWindow->addViewport(_topCamera);
	}

	_topCamera->setAspectRatio(Ogre::Real(viewport->getActualWidth()) /
		Ogre::Real(viewport->getActualHeight()));
	
}

void
Match::_loadDynamicObjects()
{

}

void
Match::_loadLights()
{
	_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
	_sceneManager->setShadowTextureCount(2);
	_sceneManager->setShadowTextureSize(512);

	_sceneManager->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5) );
	_sceneManager->setAmbientLight(Ogre::ColourValue(0.7, 0.7, 0.7));
	
	Ogre::Light *sunLight = _sceneManager->createLight("Sun");
	sunLight->setPosition(20, 10, 20);
	sunLight->setType(Ogre::Light::LT_DIRECTIONAL);
	sunLight->setDirection(-20, 0, -20);
	sunLight->setSpotlightInnerAngle(Ogre::Degree(25.0f));
	sunLight->setSpotlightOuterAngle(Ogre::Degree(100.0f));
	sunLight->setSpotlightFalloff(0.0);
	sunLight->setCastShadows(true);
}

void
Match::_loadStaticObjects()
{
	OGF::ModelBuilderPtr builder(OGF::ModelFactory::getSingletonPtr()->getBuilder(_sceneManager));

	builder->modelPath("stadium.mesh")
		->castShadows(true)
		->parent(_sceneManager->getRootSceneNode()->createChildSceneNode())
		->position(Ogre::Vector3(0, 0, 0))
		->queryFlags(1)
		->buildNode();
}l

Match::Match()
{

}

Match::~Match()
{

}

void
Match::preload()
{

}

void
Match::enter()
{
	_createScene();
}

void
Match::exit()
{

}

void
Match::pause()
{

}

void
Match::resume()
{

}

bool
Match::frameStarted(const Ogre::FrameEvent &event)
{
	return true;
}

bool
Match::keyPressed(const OIS::KeyEvent &event)
{
	switch (InputAdapter::getSingletonPtr()->inputToAction(event)) {
		case Controls::BACK:
			OGF::SceneController::getSingletonPtr()->replace(CrazyTennis::Scene::MENU_MAIN);
			break;
	}

	return true;
}
