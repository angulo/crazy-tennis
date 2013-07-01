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
}

void
Ball::exit()
{

}

void
Ball::pause()
{

}

void
Ball::resume()
{

}

bool
Ball::frameEnded(const Ogre::FrameEvent& event)
{
	return true;
}

bool
Ball::frameStarted(const Ogre::FrameEvent &event)
{
	return true;
}

bool
Ball::keyPressed(const OIS::KeyEvent &event)
{
	return true;
}

void
Ball::shotTo(const Ogre::Vector3 destination, const Ogre::Real &angle, const Ogre::Real &velocity)
{
	Ogre::Vector3 direction = destination - getPosition();
	Ogre::Real angleToZ = Ogre::Vector3(direction.x, 0, direction.z).angleBetween(Ogre::Vector3(0, 0, 1)).valueRadians();

	direction.x = velocity * cos(angle) * sin(angleToZ);
	direction.z = velocity * cos(angle) * cos(angleToZ);
	direction.y = velocity * sin(angle);
	
	if (getPosition().x > 0) {
		direction.x = -direction.x;
	}

	setLinearVelocity(direction);
}
