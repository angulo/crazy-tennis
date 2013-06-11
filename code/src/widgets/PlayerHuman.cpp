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
	return true;
}

bool
PlayerHuman::keyPressed(const OIS::KeyEvent &event)
{
	if ((getPosition() - _ball->getPosition()).length() < 0.5) {
		Dynamics::ShotSimulator *simulator = new Dynamics::ShotSimulator();

		_ball->setLinearVelocity(Ogre::Vector3::ZERO);

		Ogre::Vector3 origin = _ball->getPosition();
		Ogre::Vector3 destination(0, 0, 0);

		if (event.key == OIS::KC_UP) {
			destination.x = 5;
		} else if (event.key == OIS::KC_LEFT) {
			destination.x = 5;
			destination.z = -2;
		} else if (event.key == OIS::KC_RIGHT) {
			destination.x = 5;
			destination.z = 2;
		} else {
			return true;
		}

		if (getPosition().x > 0) {
			destination *= -1;
		}

		Dynamics::CalculationSet test = simulator->setOrigin(origin)
			->setDestination(destination)
			->calculateSet(10);


		for (int i = 0; i < 10; i++) {
			Ogre::Real velocity = test[i].second;
			Ogre::Real angle = test[i].first;

			// Discard impossible angles and directions
			if (!isnan(velocity) && !isnan(angle)) {

				Ogre::Vector3 unitary = destination - origin;
				unitary.y = velocity * sin(angle);
				unitary.normalise();
				unitary = velocity * unitary * 0.95;

				_ball->setLinearVelocity(unitary);
			}
		}
	}

	return true;
}
