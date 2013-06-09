/* 
 * PhysicalBase.cpp -- Base class for widgets with physic properties implementation file
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

#include "widgets/PhysicalBase.h"

using namespace CrazyTennis::Widget;

PhysicalBase::PhysicalBase(Ogre::SceneManager *sceneManager, OgreBulletDynamics::DynamicsWorld *dynamicWorld)
	:	OGF::Widget(sceneManager), _dynamicWorld(dynamicWorld)
{
}

PhysicalBase::~PhysicalBase()
{

}

void
PhysicalBase::setPosition(const Ogre::Vector3 &position)
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(OgreBulletCollisions::OgreBtConverter::to(position));
	_rigidBody->getBulletRigidBody()->setWorldTransform(transform);
}

void
PhysicalBase::setPosition(const Ogre::Real &x, const Ogre::Real &y, const Ogre::Real& z)
{
	setPosition(Ogre::Vector3(x, y, z));
}

Ogre::Vector3
PhysicalBase::getPosition() const
{
	return _rigidBody->getCenterOfMassPosition();
}

void
PhysicalBase::move(const Ogre::Vector3 &delta)
{
	Ogre::Vector3 currentPosition = getPosition();
	setPosition(currentPosition + delta);
}

void
PhysicalBase::move(const Ogre::Real &deltaX, const Ogre::Real &deltaY, const Ogre::Real &deltaZ)
{
	move(Ogre::Vector3(deltaX, deltaY, deltaZ));
}