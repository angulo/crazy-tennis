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

void
PlayerHuman::_shot(Controls::Action action)
{
	Dynamics::ShotSimulator *simulator = new Dynamics::ShotSimulator();

	Ogre::Vector3 origin = _ball->getPosition();
	Ogre::Vector3 destination(10, 0, 0);

	if (_ball->getPosition().x > 0) {
		destination = (-1) * destination;
	}

	Dynamics::CalculationSet allShots = simulator->setOrigin(origin)
		->setDestination(destination)
		->calculateSet(20);
	
	Dynamics::CalculationSet possibleShots;

	for (int i = 0; i < allShots.size(); i++) {
		Ogre::Real angle = allShots[i].first;
		Ogre::Real velocity = allShots[i].second;

		// Discard impossible angles and directions
		if (!isnan(velocity) && !isnan(angle) && velocity > 0) {
			possibleShots.push_back(allShots[i]);
		}
	}

	if (possibleShots.size() > 0) {
		Ogre::Vector3 direction;
		Ogre::Real angle, velocity;

		_ball->setLinearVelocity(Ogre::Vector3::ZERO);

		if (action == Controls::SHOT_DRIVE) {
			angle = possibleShots[possibleShots.size() / 5].first;
			velocity = possibleShots[possibleShots.size() / 5].second;
		} else if (action == Controls::SHOT_LOB) {
			angle = possibleShots[possibleShots.size() / 1.5].first;
			velocity = possibleShots[possibleShots.size() / 1.5].second;
		}

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
	}
}

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

	Ogre::Vector3 currentPosition = getPosition();
	Ogre::Vector3 movement(0, 0, 0);

	if (inputAdapter->isActionActive(Controls::UP))
		movement.x = currentPosition.x > 0 ? -1 : 1;
	if (inputAdapter->isActionActive(Controls::DOWN))
		movement.x = currentPosition.x > 0 ? 1 : -1;
	if (inputAdapter->isActionActive(Controls::LEFT))
		movement.z = currentPosition.x > 0 ? 1 : -1;
	if (inputAdapter->isActionActive(Controls::RIGHT))
		movement.z = currentPosition.x > 0 ? -1 : 1;
	
	// Only update position in case the movement vector has positive length
	if (movement != Ogre::Vector3::ZERO) {
		movement = event.timeSinceLastFrame * _getSpeed() * movement.normalisedCopy();
		Ogre::Vector3 destination = currentPosition + movement;

		// Prevent trespassing the net
		if (currentPosition.x >= 0) {
			destination.x = std::max(destination.x, _configValue<float>("minimumDistanceToNet"));
		} else {
			destination.x = std::min(destination.x, _configValue<float>("minimumDistanceToNet"));
		}

		setPosition(destination);
	}

	_shotBuffer->notifyPosition(currentPosition, _ball->getPosition());

	return true;
}

bool
PlayerHuman::keyReleased(const OIS::KeyEvent &event)
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();
	Controls::Action action = inputAdapter->inputToAction(event);

	if (action == Controls::SHOT_DRIVE || action == Controls::SHOT_LOB) {
		_shot(action);
	}

	return true;
}

bool
PlayerHuman::buttonPressed(const OIS::JoyStickEvent &event, int button)
{
	return false;
}

bool
PlayerHuman::buttonReleased(const OIS::JoyStickEvent &event, int button)
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();
	Controls::Action action = inputAdapter->inputToAction(event, button);

	if (action == Controls::SHOT_DRIVE || action == Controls::SHOT_LOB) {
		_shot(action);
	}

	return true;
}
