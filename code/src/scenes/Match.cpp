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

DynamicObjectPair
Match::_createPhysicObject(const Ogre::String &name, const OGF::ModelId &modelId)
{
	DynamicObjectPair result;

	OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter =
		new OgreBulletCollisions::StaticMeshToShapeConverter(); 

	OGF::ModelBuilderPtr builder(OGF::ModelFactory::getSingletonPtr()->getBuilder(_sceneManager, modelId));
	builder->castShadows(true)
		->parent(_sceneManager->getRootSceneNode()->createChildSceneNode());
	
	result.first = builder->buildNode();
	trimeshConverter->addEntity(static_cast<Ogre::Entity *>(result.first->getAttachedObject(0)));

	result.second = new OgreBulletDynamics::RigidBody(name, _dynamicWorld);
	result.second->setStaticShape(result.first, trimeshConverter->createTrimesh(), 1.0, 0.8);

	delete trimeshConverter;

	return result;
}

void
Match::_createDynamicWorld()
{
	_dynamicWorldDebugDrawer = new OgreBulletCollisions::DebugDrawer();
	_dynamicWorldDebugDrawer->setDrawWireframe(true);	 

	Ogre::SceneNode *debugNode = _sceneManager->getRootSceneNode()->
		createChildSceneNode("debugNode", Ogre::Vector3::ZERO);
	debugNode->attachObject(static_cast<Ogre::SimpleRenderable *>(_dynamicWorldDebugDrawer));

	Ogre::AxisAlignedBox worldBounds = Ogre::AxisAlignedBox(
		Ogre::Vector3(-300, -300, -300), 
		Ogre::Vector3(300,  300, 300)
	);

	Ogre::Vector3 gravity(0, -9.8, 0);

	_dynamicWorld = new OgreBulletDynamics::DynamicsWorld(_sceneManager, worldBounds, gravity);
	_dynamicWorld->setDebugDrawer(_dynamicWorldDebugDrawer);
	_dynamicWorld->setShowDebugShapes(_configValue<bool>("showDynamicWorldDebugShapes"));
}

void
Match::_createScene()
{
	_createDynamicWorld();

	_loadCameras();
	_loadLights();
	_loadStaticObjects();
	_loadDynamicObjects();
}

void
Match::_loadCameras()
{
	_topCamera = _sceneManager->createCamera("TopCamera");

	_topCamera->setPosition(_configValue<float>("camera_x"),
		_configValue<float>("camera_y"), _configValue<float>("camera_z"));
	_topCamera->lookAt(0, 0, 0);

	_topCamera->setNearClipDistance(0.1);
	_topCamera->setFarClipDistance(10000);
	_topCamera->setFOVy(Ogre::Degree(75));

	_topCameraNode = _sceneManager->createSceneNode("TopCameraNode");
	_topCameraNode->attachObject(_topCamera);
	_topCameraNode->setInheritOrientation(false);
	_sceneManager->getRootSceneNode()->addChild(_topCameraNode);

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
	DynamicObjectPair courtIn = _createPhysicObject("CourtIn", Model::COURT_IN);
	DynamicObjectPair courtOut = _createPhysicObject("CourtOut", Model::COURT_OUT);
	DynamicObjectPair lines = _createPhysicObject("Lines", Model::LINES);
	DynamicObjectPair net = _createPhysicObject("Net", Model::NET);

	_ball = new Widget::Ball(_sceneManager, _dynamicWorld);
	OGF::SceneController::getSingletonPtr()->addChild(_ball);

	_ball->setPosition(5, 3.0, 1);
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
	builder->castShadows(true)->position(Ogre::Vector3::ZERO);

	Ogre::StaticGeometry *surrounding = _sceneManager->createStaticGeometry("Surrounding");
	Ogre::Entity *entity;

	// Barrier
	surrounding->addEntity(entity = builder->modelPath(OGF::ModelFactory::getSingletonPtr()->getPath(Model::BARRIER))->buildEntity(),
		Ogre::Vector3::ZERO);
	
	// Seats
	surrounding->addEntity(entity = builder->modelPath(OGF::ModelFactory::getSingletonPtr()->getPath(Model::SEATS))->buildEntity(),
		Ogre::Vector3::ZERO);

	// Stadium
	surrounding->addEntity(entity = builder->modelPath(OGF::ModelFactory::getSingletonPtr()->getPath(Model::STADIUM))->buildEntity(),
		Ogre::Vector3::ZERO);

	surrounding->build();
}

Match::Match()
{
	_initConfigReader("scenes/match.cfg");
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
	_sceneManager->destroyAllCameras();
	_sceneManager->destroyAllStaticGeometry();
	_sceneManager->destroyAllMovableObjects();
	_sceneManager->destroyAllAnimationStates();
	_sceneManager->destroyAllLights();
	_sceneManager->getRootSceneNode()->removeAndDestroyAllChildren();
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
Match::frameEnded(const Ogre::FrameEvent& event)
{
	_dynamicWorld->stepSimulation(event.timeSinceLastFrame);
	return true;
}

bool
Match::frameStarted(const Ogre::FrameEvent &event)
{
	_dynamicWorld->stepSimulation(event.timeSinceLastFrame);
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
