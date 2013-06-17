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

Ogre::Vector3 
PlayerHuman::_calculateDestination()
{
	Ogre::Real halfWidth(_configValue<float>("courtWidth") / 2.0);
	Ogre::Real length(_configValue<float>("courtLength"));

	Ogre::Real horizontalBalance = _shotBuffer->getValue(Controls::RIGHT) - 
		_shotBuffer->getValue(Controls::LEFT);
	Ogre::Real verticalBalance = _shotBuffer->getValue(Controls::UP) - 
		_shotBuffer->getValue(Controls::DOWN) - 0.5;
	
	Ogre::Vector3 destination(length * (1 + verticalBalance), 0, halfWidth * horizontalBalance);

	if (_ball->getPosition().x > 0) {
		destination = (-1) * destination;
	}

	return destination;
}

bool
PlayerHuman::_canShoot(const Controls::Action &action)
{
	return _shotBuffer->getValue(action) > 0;	
}

void
PlayerHuman::_move(const Ogre::Real &timeSinceLastFrame)
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
		movement = timeSinceLastFrame * _getSpeed() * movement.normalisedCopy();
		Ogre::Vector3 destination = currentPosition + movement;

		// Prevent trespassing the net
		if (currentPosition.x >= 0) {
			destination.x = std::max(destination.x, _configValue<float>("minimumDistanceToNet"));
		} else {
			destination.x = std::min(destination.x, _configValue<float>("minimumDistanceToNet"));
		}

		setPosition(destination);
	}
}

int
PlayerHuman::_selectShot(const Controls::Action &action, const int &availableShots)
{
	int shot, lowLimit, highLimit;
	Ogre::Real bufferValue = _shotBuffer->getValue(action);

	if (bufferValue == 0) {
		shot = -1;
	} else {
		switch(action) {
			case Controls::SHOT_DRIVE:
				lowLimit = _configValue<float>("driveShotLowLimit") * (availableShots - 1);
				highLimit = _configValue<float>("driveShotHighLimit") * (availableShots - 1);
				shot = lowLimit + ((1.0 - bufferValue) * (highLimit - lowLimit));
				break;	
			case Controls::SHOT_LOB:
				lowLimit = _configValue<float>("lobShotLowLimit") * (availableShots - 1);
				highLimit = _configValue<float>("lobShotHighLimit") * (availableShots - 1);
				shot = lowLimit + (bufferValue * (highLimit - lowLimit));
				break;
			default:
				break;
		}
	}

	return shot;
}


void
PlayerHuman::_shoot(const Controls::Action &action)
{
	Dynamics::ShotSimulator *simulator = new Dynamics::ShotSimulator();

	Ogre::Vector3 origin = _ball->getPosition();
	Ogre::Vector3 destination = _calculateDestination();

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

	int availableShots = possibleShots.size();

	if (availableShots > 0) {
		int shot = _selectShot(action, availableShots);

		if (shot != -1) {
			Ogre::Real angle = possibleShots[shot].first;
			Ogre::Real velocity = possibleShots[shot].second;
			Ogre::Vector3 direction = destination - origin;
			Ogre::Real angleToZ = Ogre::Vector3(direction.x, 0, direction.z).angleBetween(Ogre::Vector3(0, 0, 1)).valueRadians();

			direction.x = velocity * cos(angle) * sin(angleToZ);
			direction.z = velocity * cos(angle) * cos(angleToZ);
			direction.y = velocity * sin(angle);
			
			if (_ball->getPosition().x > 0) {
				direction.x = -direction.x;
			}

			_ball->setLinearVelocity(direction);
		}
	}
}

PlayerHuman::PlayerHuman(Ogre::SceneManager *sceneManager, OgreBulletDynamics::DynamicsWorld *dynamicWorld, Widget::Ball *ball, Data::Player *data)
	:	PlayerBase(sceneManager, dynamicWorld, ball, data), _directionBlocked(false)
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
	if (!_directionBlocked)
		_move(event.timeSinceLastFrame);

	_shotBuffer->notifyPosition(getPosition(), _ball->getPosition());
	return true;
}

bool
PlayerHuman::keyPressed(const OIS::KeyEvent &event)
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();
	Controls::Action action = inputAdapter->inputToAction(event);
	
	if (!_directionBlocked &&
		(action == Controls::SHOT_DRIVE || action == Controls::SHOT_LOB)) {

		_directionBlocked = true;
	}

	return true;
}

bool
PlayerHuman::keyReleased(const OIS::KeyEvent &event)
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();
	Controls::Action action = inputAdapter->inputToAction(event);

	if (action == Controls::SHOT_DRIVE || action == Controls::SHOT_LOB) {
		if (_canShoot(action)) {
			_shoot(action);
		}
	}

	_directionBlocked = false;

	return true;
}

bool
PlayerHuman::buttonPressed(const OIS::JoyStickEvent &event, int button)
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();
	Controls::Action action = inputAdapter->inputToAction(event, button);
	
	if (!_directionBlocked &&
		(action == Controls::SHOT_DRIVE || action == Controls::SHOT_LOB)) {

		_directionBlocked = true;
	}

	return true;
}

bool
PlayerHuman::buttonReleased(const OIS::JoyStickEvent &event, int button)
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();
	Controls::Action action = inputAdapter->inputToAction(event, button);

	if (action == Controls::SHOT_DRIVE || action == Controls::SHOT_LOB) {
		if (_canShoot(action)) {
			_shoot(action);
		}
	}

	_directionBlocked = false;

	return true;
}
