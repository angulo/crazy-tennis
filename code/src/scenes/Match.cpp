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
Match::_createPhysicObject(const Ogre::String &name, const OGF::ModelId &modelId, const float &restitution)
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
	result.second->setStaticShape(result.first, trimeshConverter->createTrimesh(), restitution, 0.8);

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
	_loadUserInterface();
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
	_topCamera->setFOVy(Ogre::Degree(45));

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
	DynamicObjectPair net = _createPhysicObject("Net", Model::NET, 0.3);

	_ball = new Widget::Ball(_sceneManager, _dynamicWorld);
	OGF::SceneController::getSingletonPtr()->addChild(_ball);

	_ball->setPosition(10, 5, 0);

	Widget::PlayerBase *player1 = new Widget::PlayerHuman(_sceneManager,
		_dynamicWorld, _ball, _data->getPlayer(0));

	OGF::SceneController::getSingletonPtr()->addChild(player1);
	player1->setPosition(12, 1, 0);

	Widget::PlayerBase *player2 = new Widget::PlayerCpu(_sceneManager,
		_dynamicWorld, _ball, _data->getPlayer(1));

	OGF::SceneController::getSingletonPtr()->addChild(player2);
	player2->setPosition(-12, 1, 0);
}

void
Match::_loadLights()
{
	_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	_sceneManager->setShadowColour(Ogre::ColourValue(0.0, 0.0, 0.0) );
	_sceneManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	
	Ogre::Light *sunLight = _sceneManager->createLight("Light_1");
	sunLight->setPosition(20, 100, 20);
	sunLight->setType(Ogre::Light::LT_DIRECTIONAL);
	sunLight->setDirection(-10, -100, -10);
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
	
	// Sky
	entity = builder->modelPath(OGF::ModelFactory::getSingletonPtr()->getPath(Model::SKY))->buildEntity();
	entity->setMaterialName("sky");
	Ogre::SceneNode *node = _sceneManager->getRootSceneNode()->createChildSceneNode();
	node->setPosition(0, -300, 0);
	node->attachObject(entity);

	surrounding->build();

	surrounding = _sceneManager->createStaticGeometry("Surrounding2");
	// Referee
	surrounding->addEntity(entity = builder->modelPath(OGF::ModelFactory::getSingletonPtr()->getPath(Model::REFEREE))->buildEntity(),
		Ogre::Vector3::ZERO);

	// Referee chair
	surrounding->addEntity(entity = builder->modelPath(OGF::ModelFactory::getSingletonPtr()->getPath(Model::CHAIR))->buildEntity(),
		Ogre::Vector3::ZERO);
	
	surrounding->setCastShadows(true);
	surrounding->build();
}

void
Match::_loadUserInterface()
{
	Widget::Score *scoreWidget = new Widget::Score(_data);
	OGF::SceneController::getSingletonPtr()->addChild(scoreWidget);
	_data->addListener(scoreWidget);
}

void
Match::_onActionDone(const Controls::Action &action)
{
	switch (action) {
		case Controls::START:
			OGF::SceneController::getSingletonPtr()->replace(CrazyTennis::Scene::MENU_MAIN);
			break;
		default:
			break;
	}
}

Match::Match(Data::Match *data)
	:	_data(data)
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
	delete _dynamicWorld->getDebugDrawer();
	delete _dynamicWorld;

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
	_dynamicWorld->stepSimulation(event.timeSinceLastFrame, 4, 1.0 / 240.0);
	return true;
}

bool
Match::frameStarted(const Ogre::FrameEvent &event)
{
	_dynamicWorld->stepSimulation(event.timeSinceLastFrame, 4, 1.0 / 240.0);
	return true;
}

bool
Match::keyPressed(const OIS::KeyEvent &event)
{
	Controls::Action action = InputAdapter::getSingletonPtr()->inputToAction(event);
	_onActionDone(action);

	return true;
}

bool
Match::buttonPressed(const OIS::JoyStickEvent &event, int button)
{
	Controls::Action action = InputAdapter::getSingletonPtr()->inputToAction(event, button);
	_onActionDone(action);

	return true;
}
