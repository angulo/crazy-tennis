/* 
 * PlayerCpu.cpp -- CPU player implementation file
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

#include "widgets/PlayerCpu.h"

using namespace CrazyTennis::Widget;

bool
PlayerCpu::_canStartHit()
{
	return !_hitting && _ball->getPosition().squaredDistance(getPosition()) <= 16;
}

bool
PlayerCpu::_canEndHit()
{
	return _ball->getPosition().squaredDistance(getPosition()) <= 4;
}

void
PlayerCpu::_startHit()
{
	_hitting = true;
	_motionManager->driveHitStart();
}


void
PlayerCpu::_endHit()
{
	_hitting = false;
	_motionManager->driveHitEnd();
	_shot();
}

void
PlayerCpu::_shot()
{
	Ogre::Vector3 ballPosition = _ball->getPosition();
	Ogre::Vector3 origin = ballPosition;

	Ogre::Vector3 destination(6, 0, 0);

	Dynamics::ShotSimulator *simulator = new Dynamics::ShotSimulator();
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
		int shot = availableShots / 3;
		Ogre::Real angle = possibleShots[shot].first;
		Ogre::Real velocity = possibleShots[shot].second;

		_ball->shotTo(destination, angle, velocity);
		SoundPlayer::getSingletonPtr()->play(SOUND_BALL_SHOT);
		_pointStateMachine->onBallHit(_playerData->getId());
	}
}

PlayerCpu::PlayerCpu(Ogre::SceneManager *sceneManager, Widget::Ball *ball, Data::Match *matchData,
	Data::Player *playerData, Data::PointState::Machine *pointStateMachine)
	:	PlayerBase(sceneManager, ball, matchData, playerData, pointStateMachine), _hitting(false)
{

}

PlayerCpu::~PlayerCpu()
{

}

void
PlayerCpu::enter()
{
	PlayerBase::enter();
}

void
PlayerCpu::exit()
{

}

bool
PlayerCpu::frameStarted(const Ogre::FrameEvent &event)
{
	if (_pointStateMachine->getCurrentState() == Data::PointState::STATE_IN_POINT) {
		if (_pointStateMachine->getTurn() == _playerData->getId()) {
			if (!_hitting) {
				Ogre::Vector3 bounceDirectionIncrement = _ball->getDirection().normalisedCopy() * 3;
				Ogre::Vector3 bouncePlace = _ball->getWhereToBounce();
				Ogre::Vector3 directionToBounce = bouncePlace - bounceDirectionIncrement;

				directionToBounce -= getPosition();
				directionToBounce.y = 0;

				if (directionToBounce.squaredLength() > 1) {
					Ogre::Vector3 increment = directionToBounce.normalisedCopy() * _getSpeed() * event.timeSinceLastFrame;
					setPosition(getPosition() + increment);
				}
			}

			if (_canStartHit()) {
				_startHit();
			} else if (_canEndHit()) {
				_endHit();
			}
		} else {
			_hitting = false;
			Ogre::Vector3 directionToCenter;
			if (getPosition().x > 0) {
				directionToCenter = Ogre::Vector3(_configValue<float>("courtLength"), 0, 0) - getPosition();
			} else {
				directionToCenter = Ogre::Vector3(- _configValue<float>("courtLength"), 0, 0) - getPosition();
			}

			directionToCenter.y = 0;

			if (directionToCenter.squaredLength() > 1) {
				Ogre::Vector3 increment = directionToCenter.normalisedCopy() * _getSpeed() * event.timeSinceLastFrame;
				setPosition(getPosition() + increment);
			}
		}
	} else {
		_hitting = false;
	}

	return true;
}
