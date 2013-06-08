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
	:	_sceneManager(sceneManager), _dynamicWorld(dynamicWorld)
{
	_initConfigReader("widgets/ball.cfg");
}

Ball::~Ball()
{

}

void
Ball::preload()
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
		new OgreBulletCollisions::SphereCollisionShape(1.0);
	
	//shape->getBulletShape()->setMargin();

	_rigidBody = new OgreBulletDynamics::RigidBody("Ball", _dynamicWorld);
	_rigidBody->setShape(node, shape, _configValue<float>("restitution"),
		_configValue<float>("friction"), _configValue<float>("weight"));
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
Ball::setPosition(const Ogre::Vector3 &position)
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(OgreBulletCollisions::OgreBtConverter::to(position));
	_rigidBody->getBulletRigidBody()->setWorldTransform(transform);
}

void
Ball::setPosition(const Ogre::Real &x, const Ogre::Real &y, const Ogre::Real& z)
{
	setPosition(Ogre::Vector3(x, y, z));
}
