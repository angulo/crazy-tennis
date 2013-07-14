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
PlayerHuman::_calculateShotDestination()
{
	Ogre::Real errorRate = (drand48() / 10.0) + (drand48() * (1 - _playerData->getSkills()["precision"]) * _configValue<float>("maxDistanceError"));

	Ogre::Real halfWidth(_configValue<float>("courtWidth") / 2.0);
	Ogre::Real length(_configValue<float>("courtLength"));
	Ogre::Real horizontalBalance = _shotBuffer->getValue(Controls::RIGHT) - 
		_shotBuffer->getValue(Controls::LEFT);
	Ogre::Real verticalBalance = _shotBuffer->getValue(Controls::UP) - 
		_shotBuffer->getValue(Controls::DOWN);
	
	Ogre::Vector3 destination(((0.5 * length) * (1 + verticalBalance)) + errorRate, 0,
		(halfWidth * horizontalBalance) + errorRate);

	if (_ball->getPosition().x > 0) {
		destination = (-1) * destination;
	}

	return destination;
}

bool
PlayerHuman::_canMoveTo(const Ogre::Vector3 &destination)
{
	bool canMove = true;

	bool beforeServe = _pointStateMachine->getCurrentState() == Data::PointState::STATE_BEFORE_SERVE &&
		_matchData->getCurrentServer() == _playerData;
	bool inServe = _pointStateMachine->getCurrentState() == Data::PointState::STATE_IN_SERVE &&
		_matchData->getCurrentServer() == _playerData;
	Ogre::Vector3 currentPosition = getPosition();

	if (beforeServe) {
		if (currentPosition.x > 0) {
			if (_matchData->getWhereToServe() == Data::PointState::BOUNCE_IN_LEFT_SERVE_AREA) {
				canMove = destination.z > 0;			
			} else {
				canMove = destination.z < 0;			
			}
		} else {
			if (_matchData->getWhereToServe() == Data::PointState::BOUNCE_IN_LEFT_SERVE_AREA) {
				canMove = destination.z > 0;			
			} else {
				canMove = destination.z < 0;			
			}
		}

		canMove = canMove && abs(destination.z) < _configValue<float>("courtWidth") / 2.0 &&
			destination.x == currentPosition.x;

	} else if (inServe) {
		canMove = false;
	}

	return canMove;
}

bool
PlayerHuman::_canShoot(const Controls::Action &action)
{
	return _shotBuffer->getValue(action) > 0 && getPosition().squaredDistance(_ball->getPosition()) < 5;
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

		if (_canMoveTo(destination)) {
			// Prevent trespassing the net
			if (currentPosition.x >= 0) {
				destination.x = std::max(destination.x, _configValue<float>("minimumDistanceToNet"));
			} else {
				destination.x = std::min(destination.x, _configValue<float>("minimumDistanceToNet"));
			}

			setPosition(destination);
		}
	}
}

void
PlayerHuman::_onActionStart(const Controls::Action &action)
{
	if (!_directionBlocked &&
		(action == Controls::SHOT_DRIVE || action == Controls::SHOT_LOB)) {

		_directionBlocked = true;

		bool beforeServe = _pointStateMachine->getCurrentState() == Data::PointState::STATE_BEFORE_SERVE &&
			_matchData->getCurrentServer() == _playerData;
		bool inPoint = _pointStateMachine->getCurrentState() == Data::PointState::STATE_IN_POINT;

		if (beforeServe) {
			_pointStateMachine->onContinue(_playerData->getId());
			_startToServe();
		} else if (inPoint) {
			_motionManager->driveHitStart();
		}
	}
}

void
PlayerHuman::_onActionEnd(const Controls::Action &action)
{
	if (action == Controls::SHOT_DRIVE || action == Controls::SHOT_LOB) {
		bool inServe = _pointStateMachine->getCurrentState() == Data::PointState::STATE_IN_SERVE &&
			_matchData->getCurrentServer() == _playerData;
		bool inPoint = _pointStateMachine->getCurrentState() == Data::PointState::STATE_IN_POINT;

		if (inServe) {
			_serve();
		} else if (inPoint) {
			if (_canShoot(action)) {
				_shoot(action);
			}
			_motionManager->driveHitEnd();
		}
	}

	_directionBlocked = false;
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
PlayerHuman::_serve()
{
	Dynamics::ShotSimulator *simulator = new Dynamics::ShotSimulator();

	Ogre::Vector3 origin = _ball->getPosition();
	Ogre::Vector3 destination = _serverMark->getPosition();

	Dynamics::CalculationSet allShots = simulator->setOrigin(origin)
		->setDestination(destination)
		->calculateSet(200);
	
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
		int shot = (1 - _playerData->getSkills()["serve"]) * 10;

		if (shot != -1) {
			Ogre::Real angle = possibleShots[shot].first;
			Ogre::Real velocity = possibleShots[shot].second;

			_ball->shotTo(destination, angle, velocity);
			SoundPlayer::getSingletonPtr()->play(SOUND_BALL_SERVE);
			_pointStateMachine->onBallHit(_playerData->getId());
			_motionManager->serveEnd();
		}
	}
}

void
PlayerHuman::_shoot(const Controls::Action &action)
{
	Dynamics::ShotSimulator *simulator = new Dynamics::ShotSimulator();

	Ogre::Vector3 origin = _ball->getPosition();
	Ogre::Vector3 destination = _calculateShotDestination();

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
			_ball->shotTo(destination, angle, velocity);
			SoundPlayer::getSingletonPtr()->play(SOUND_BALL_SHOT);
			_pointStateMachine->onBallHit(_playerData->getId());
		}
	}
}

void
PlayerHuman::_startToServe()
{
	Ogre::Vector3 position = getPosition();

	position.x += getPosition().x > 0 ? - _configValue<float>("ballServeXOffset") : 
		_configValue<float>("ballServeXOffset");
	position.y = 1;
	position.z += getPosition().x > 0 ? - _configValue<float>("ballServeZOffset") : 
		_configValue<float>("ballServeZOffset");

	_ball->setPosition(position);
	_ball->setLinearVelocity(Ogre::Vector3(0, 7, 0));
	_motionManager->serveStart();
}

PlayerHuman::PlayerHuman(Ogre::SceneManager *sceneManager, Widget::Ball *ball, Data::Match *matchData,
	Data::Player *playerData, Data::PointState::Machine *pointStateMachine)
	:	PlayerBase(sceneManager, ball, matchData, playerData, pointStateMachine),
		_directionBlocked(false)
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
	OGF::SceneController::getSingletonPtr()->addChild(_shotBuffer);
	_serverMark = new Widget::ServerMark(_sceneManager, _matchData, _playerData->getId());
	OGF::SceneController::getSingletonPtr()->addChild(_serverMark);
	_pointStateMachine->addListener(_serverMark);

	_setInServeState();
}

void
PlayerHuman::exit()
{

}

bool
PlayerHuman::frameStarted(const Ogre::FrameEvent &event)
{
	PlayerBase::frameStarted(event);
	if (!_directionBlocked)
		_move(event.timeSinceLastFrame);

	_shotBuffer->notifyPosition(getPosition(), _ball->getPosition());
	return true;
}

bool
PlayerHuman::keyPressed(const OIS::KeyEvent &event)
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();
	_onActionStart(inputAdapter->inputToAction(event));

	return true;
}

bool
PlayerHuman::keyReleased(const OIS::KeyEvent &event)
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();
	_onActionEnd(inputAdapter->inputToAction(event));

	return true;
}

bool
PlayerHuman::buttonPressed(const OIS::JoyStickEvent &event, int button)
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();
	_onActionStart(inputAdapter->inputToAction(event, button));
	
	return true;
}

bool
PlayerHuman::buttonReleased(const OIS::JoyStickEvent &event, int button)
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();
	_onActionEnd(inputAdapter->inputToAction(event, button));

	return true;
}

void
PlayerHuman::onChangeState(const Data::PointState::State &previousState, const Data::PointState::State &currentState)
{
	PlayerBase::onChangeState(previousState, currentState);

	switch(currentState) {
		case Data::PointState::STATE_IN_SERVE:
			_directionBlocked = true;
			break;
		default:
			_directionBlocked = false;
			break;
	}
}
