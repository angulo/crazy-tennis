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

PlayerCpu::PlayerCpu(Ogre::SceneManager *sceneManager, OgreBulletDynamics::DynamicsWorld *dynamicWorld, Widget::Ball *ball,
	Data::Match *matchData, Data::Player *playerData, Data::PointState::Machine *pointStateMachine)
	:	PlayerBase(sceneManager, dynamicWorld, ball, matchData, playerData, pointStateMachine)
{

}

PlayerCpu::~PlayerCpu()
{

}

void
PlayerCpu::enter()
{
	PlayerBase::enter();
	
	_shotBuffer = new Widget::ShotBuffer();
	_shotBufferId = OGF::SceneController::getSingletonPtr()->addChild(_shotBuffer);
}

void
PlayerCpu::exit()
{

}

bool
PlayerCpu::frameStarted(const Ogre::FrameEvent &event)
{
	Ogre::Vector3 ballPosition = _ball->getPosition();
	Ogre::Vector3 origin = ballPosition;

	if (ballPosition.x <= -13) {
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
			_pointStateMachine->onBallHit(_playerData->getId());
		}
	}

	return true;
}

bool
PlayerCpu::keyPressed(const OIS::KeyEvent &event)
{
	return true;
}
