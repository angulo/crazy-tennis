/* 
 * PlayerHuman.cpp -- Human player implementation file
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

#include "widgets/PlayerHuman.h"

using namespace CrazyTennis::Widget;

PlayerHuman::PlayerHuman(Ogre::SceneManager *sceneManager, OgreBulletDynamics::DynamicsWorld *dynamicWorld, Widget::Ball *ball, Data::Player *data)
	:	PlayerBase(sceneManager, dynamicWorld, ball, data)
{

}

PlayerHuman::~PlayerHuman()
{

}

void
PlayerHuman::enter()
{
	PlayerBase::enter();
	
	_shotBuffer = new Widget::ShotBuffer();
	_shotBufferId = OGF::SceneController::getSingletonPtr()->addChild(_shotBuffer);
}

void
PlayerHuman::exit()
{

}

bool
PlayerHuman::frameStarted(const Ogre::FrameEvent &event)
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();
	OIS::Keyboard *keyboard = OGF::InputManager::getSingletonPtr()->getKeyboard();

	Ogre::Vector3 currentPosition = getPosition();
	Ogre::Vector3 movement(0, 0, 0);

	if (keyboard->isKeyDown(inputAdapter->actionToInput(Controls::UP)))
		movement.x = currentPosition.x > 0 ? -1 : 1;
	if (keyboard->isKeyDown(inputAdapter->actionToInput(Controls::DOWN)))
		movement.x = currentPosition.x > 0 ? 1 : -1;
	if (keyboard->isKeyDown(inputAdapter->actionToInput(Controls::LEFT)))
		movement.z = currentPosition.x > 0 ? 1 : -1;
	if (keyboard->isKeyDown(inputAdapter->actionToInput(Controls::RIGHT)))
		movement.z = currentPosition.x > 0 ? -1 : 1;
	
	movement = event.timeSinceLastFrame * _getSpeed() * movement.normalisedCopy();

	Ogre::Vector3 destination = currentPosition + movement;

	// Prevent trespassing the net
	if (currentPosition.x >= 0) {
		destination.x = std::max(destination.x, _configValue<float>("minimumDistanceToNet"));
	} else {
		destination.x = std::min(destination.x, _configValue<float>("minimumDistanceToNet"));
	}

	setPosition(destination);

	return true;
}

bool
PlayerHuman::keyPressed(const OIS::KeyEvent &event)
{
	return true;
	Dynamics::ShotSimulator *simulator = new Dynamics::ShotSimulator();

	_ball->setLinearVelocity(Ogre::Vector3::ZERO);

	Ogre::Vector3 origin = _ball->getPosition();
	Ogre::Vector3 destination(0, 0, 0);

	if (event.key == OIS::KC_UP) {
		destination.x = 11;
	} else if (event.key == OIS::KC_LEFT) {
		destination.x = 11;
		destination.z = -2;
	} else if (event.key == OIS::KC_RIGHT) {
		destination.x = 11;
		destination.z = 2;
	} else {
		return true;
	}

	if (_ball->getPosition().x > 0) {
		destination = (-1) * destination;
	}

	Dynamics::CalculationSet test = simulator->setOrigin(origin)
		->setDestination(destination)
		->calculateSet(10);
	
	Ogre::Vector3 direction;

	for (int i = 7; i > 0; i--) {
		Ogre::Real velocity = test[i].second;
		Ogre::Real angle = test[i].first;

		// Discard impossible angles and directions
		if (!isnan(velocity) && !isnan(angle) && velocity > 0) {
			direction = destination - origin;
			Ogre::Real angleToZ = Ogre::Vector3(direction.x, 0, direction.z).angleBetween(Ogre::Vector3(0, 0, 1)).valueRadians();

			direction.x = velocity * cos(angle) * sin(angleToZ);
			direction.z = velocity * cos(angle) * cos(angleToZ);
			direction.y = velocity * sin(angle);
			
			if (_ball->getPosition().x > 0) {
				direction.x = -direction.x;
				direction.z = -direction.z;
			}

			_ball->setLinearVelocity(direction);
			break;
		}
	}

	return true;
}
