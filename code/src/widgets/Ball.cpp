/* 
 * Ball.cpp -- Ball widget implementation file
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

#include "widgets/Ball.h"

using namespace CrazyTennis::Widget;

void
Ball::_createGhosts()
{
	OGF::ModelBuilderPtr builder(OGF::ModelFactory::getSingletonPtr()->getBuilder(_sceneManager, CrazyTennis::Model::BALL));
	builder->castShadows(false)
		->parent(_sceneManager->getRootSceneNode()->createChildSceneNode());
	
	for (int i = 0; i < _configValue<int>("ghostsNumber"); i++) {
		_ghosts.push_back(builder->buildNode());
	}
}

void
Ball::_updateGhosts()
{
	Ogre::Vector3 currentPosition = getPosition();
	Ogre::SceneNode *lastGhost = _ghosts.front();
	lastGhost->setPosition(getPosition());
	_ghosts.pop_front();
	_ghosts.push_back(lastGhost);

	int count = 9;
	for (std::list<Ogre::SceneNode *>::iterator it = _ghosts.begin(); it != _ghosts.end(); it++, count--) {
		Ogre::Entity *object = static_cast<Ogre::Entity *>((*it)->getAttachedObject(0));
		object->getSubEntity(0)->setMaterialName("vert_" + Ogre::StringConverter::toString(count));
		object->getSubEntity(1)->setMaterialName("blanc_" + Ogre::StringConverter::toString(count));
	}
}


Ball::Ball(Ogre::SceneManager *sceneManager, OgreBulletDynamics::DynamicsWorld *dynamicWorld)
	:	PhysicalBase(sceneManager, dynamicWorld)
{
	_initConfigReader("widgets/ball.cfg");
}

Ball::~Ball()
{

}

void
Ball::enter()
{
	OGF::ModelBuilderPtr builder(OGF::ModelFactory::getSingletonPtr()->getBuilder(_sceneManager, CrazyTennis::Model::BALL));
	builder->castShadows(true)
		->parent(_sceneManager->getRootSceneNode()->createChildSceneNode());
	
	Ogre::SceneNode *node = builder->buildNode();
	OgreBulletCollisions::SphereCollisionShape *shape =
		new OgreBulletCollisions::SphereCollisionShape(_configValue<float>("radius"));
	
	_rigidBody = new OgreBulletDynamics::RigidBody("Ball", _dynamicWorld);
	_rigidBody->setShape(node, shape, _configValue<float>("restitution"),
		_configValue<float>("friction"), _configValue<float>("weight"));
	_rigidBody->disableDeactivation();

	_createGhosts();
}

void
Ball::exit()
{

}

bool
Ball::frameStarted(const Ogre::FrameEvent &event)
{
	static float timeSinceLastGhostPaint = 0;

	timeSinceLastGhostPaint += event.timeSinceLastFrame;
	if (timeSinceLastGhostPaint >= _configValue<float>("ghostsTimeDistance")) {
		_updateGhosts();
		timeSinceLastGhostPaint = 0;
	}

	return true;
}

void
Ball::setVisible(const bool &isVisible)
{
	PhysicalBase::setVisible(isVisible);	
	for (std::list<Ogre::SceneNode *>::iterator it = _ghosts.begin(); it != _ghosts.end(); it++) {
		(*it)->setVisible(isVisible);
	}
}

void
Ball::shotTo(const Ogre::Vector3 destination, const Ogre::Real &angle, const Ogre::Real &velocity)
{
	_whereToBounce = destination;
	Ogre::Vector3 direction = destination - getPosition();
	Ogre::Real angleToZ = Ogre::Vector3(direction.x, 0, direction.z).angleBetween(Ogre::Vector3(0, 0, 1)).valueRadians();

	direction.x = velocity * cos(angle) * sin(angleToZ);
	direction.z = velocity * cos(angle) * cos(angleToZ);
	direction.y = velocity * sin(angle);
	_direction = direction;

	if (getPosition().x > 0) {
		direction.x = -direction.x;
	}

	setLinearVelocity(direction);
}

Ogre::Vector3
Ball::getDirection() const
{
	return _direction;
}

Ogre::Vector3
Ball::getWhereToBounce() const
{
	return _whereToBounce;
}
