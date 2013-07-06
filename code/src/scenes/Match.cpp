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
	_courtIn = _createPhysicObject("CourtIn", Model::COURT_IN);
	_courtOut = _createPhysicObject("CourtOut", Model::COURT_OUT);
	DynamicObjectPair net = _createPhysicObject("Net", Model::NET, 0.3);

	_ball = new Widget::Ball(_sceneManager, _dynamicWorld);
	OGF::SceneController::getSingletonPtr()->addChild(_ball);

	_ball->setPosition(_configValue<float>("court_middle_x"), 5, _configValue<float>("court_middle_z"));

	Widget::PlayerBase *player1 = new Widget::PlayerHuman(_sceneManager,
		_dynamicWorld, _ball, _data, _data->getPlayer(0), _pointStateMachine);

	OGF::SceneController::getSingletonPtr()->addChild(player1);
	player1->setPosition(12, 2, 0);
	player1->rotate(Ogre::Vector3(0, 1, 0), Ogre::Degree(180));

	Widget::PlayerBase *player2 = new Widget::PlayerCpu(_sceneManager,
		_dynamicWorld, _ball, _data, _data->getPlayer(1), _pointStateMachine);

	OGF::SceneController::getSingletonPtr()->addChild(player2);
	player2->setPosition(-12, 2, 0);
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
	
	// Lines
	surrounding->addEntity(entity = builder->modelPath(OGF::ModelFactory::getSingletonPtr()->getPath(Model::LINES))->buildEntity(),
		Ogre::Vector3(0, 0.015, 0));
	
	// Sky
	entity = builder->modelPath(OGF::ModelFactory::getSingletonPtr()->getPath(Model::SKY))->buildEntity();
	entity->setMaterialName("sky");
	Ogre::SceneNode *node = _sceneManager->getRootSceneNode()->createChildSceneNode();
	node->setPosition(0, -300, 0);
	node->attachObject(entity);

	surrounding->build();

	builder->parent(_sceneManager->getRootSceneNode()->createChildSceneNode());

	// Referee
	builder->modelPath(OGF::ModelFactory::getSingletonPtr()->getPath(Model::REFEREE))->buildNode();
	// Referee chair
	builder->modelPath(OGF::ModelFactory::getSingletonPtr()->getPath(Model::CHAIR))->buildNode();
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

void
Match::_checkBallStatus()
{
	// Dont check for ball collisions until being near the floor
	Ogre::Vector3 ballPosition = _ball->getPosition();
	if (ballPosition.y >= 0.1) {
		return;
	}

	bool bounce = false, bounceIn = false, bounceOut = false;

	Ogre::Vector3 collisionPoint = ballPosition;
	btCollisionWorld *bulletWorld = _dynamicWorld->getBulletCollisionWorld();
	int totalCollisions = bulletWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < totalCollisions; i++) {
		btPersistentManifold *collision = bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject *objectA = const_cast<btCollisionObject *>(collision->getBody0());
		btCollisionObject *objectB = const_cast<btCollisionObject *>(collision->getBody1());
    
		OgreBulletCollisions::Object *collisionObjectA = _dynamicWorld->findObject(objectA);
		OgreBulletCollisions::Object *collisionObjectB = _dynamicWorld->findObject(objectB);

		if (collisionObjectA == _ball->getRigidBody() || collisionObjectB == _ball->getRigidBody()) {
			if (collisionObjectA == _courtIn.second || collisionObjectB == _courtIn.second) {
				bounceIn = bounce = true;
			} else if (collisionObjectA == _courtOut.second || collisionObjectB == _courtOut.second) {
				bounceOut = bounce = true;
			}
		}
	}

	if (bounce) {
		Data::PlayerId courtOwner = collisionPoint.x > 0 ?
			_data->getPlayer(0)->getId() : _data->getPlayer(1)->getId();

		Data::PointState::BouncePlace where;

		if (bounceIn) {
			if (abs(collisionPoint.x) < _configValue<float>("court_middle_x")) {
				if (collisionPoint.x * collisionPoint.z >= 0) {
					where = Data::PointState::BOUNCE_IN_LEFT_SERVE_AREA;
				} else {
					where = Data::PointState::BOUNCE_IN_RIGHT_SERVE_AREA;
				}
			} else {
				where = Data::PointState::BOUNCE_IN_COURT;
			}
		} else {
			where = Data::PointState::BOUNCE_OUT;
		}

		_pointStateMachine->onBallBounce(courtOwner, where);
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
	_pointStateMachine = new Data::PointState::Machine(_data->getPlayer(0)->getId(), _data->getPlayer(1)->getId());
	_pointStateMachine->addListener(this);

	_createScene();
	_pointStateMachine->reset(_data->getCurrentServer()->getId(), Data::PointState::BOUNCE_IN_RIGHT_SERVE_AREA);
}

void
Match::exit()
{
	delete _pointStateMachine;
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
	return true;
}

bool
Match::frameStarted(const Ogre::FrameEvent &event)
{
	_dynamicWorld->stepSimulation(event.timeSinceLastFrame, 4, 1.0 / 240.0);
	_checkBallStatus();

	return true;
}

bool
Match::keyReleased(const OIS::KeyEvent &event)
{
	Controls::Action action = InputAdapter::getSingletonPtr()->inputToAction(event);
	_onActionDone(action);

	return true;
}

bool
Match::buttonReleased(const OIS::JoyStickEvent &event, int button)
{
	Controls::Action action = InputAdapter::getSingletonPtr()->inputToAction(event, button);
	_onActionDone(action);

	return true;
}
