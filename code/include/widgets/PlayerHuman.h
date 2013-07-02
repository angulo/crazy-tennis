/* 
 * PlayerHuman.h -- Human player header file
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

#ifndef _WIDGET_PLAYER_HUMAN_H_
#define _WIDGET_PLAYER_HUMAN_H_

#include <OGF/OGF.h>

#include "InputAdapter.h"
#include "data/Match.h"
#include "data/PointStateMachine.h"
#include "dynamics/ShotSimulator.h"
#include "widgets/PlayerBase.h"
#include "widgets/ShotBuffer.h"

namespace CrazyTennis {
	
	namespace Widget {

		class PlayerHuman : public PlayerBase {
			
			protected:

				OGF::SceneId _shotBufferId;
				Widget::ShotBuffer *_shotBuffer;
				bool _directionBlocked;

				/**
				 * Calculate the shot destination depending on the current shot
				 * buffer status.
				 *
				 * @return Destination of the shot.
				 */
				Ogre::Vector3 _calculateDestination();

				/**
				 * Check if the player can shot at the moment.
				 * @return true if the player can shot, false otherwise.
				 */
				bool _canShoot(const Controls::Action &action);

				/**
				 * Move the player according to the current buffer value.
				 */
				void _move(const Ogre::Real &timeSinceLastFrame);

				/**
				 * Select the shot to be executed depending on the shot buffer status
				 * and the desired type of shot.
				 *
				 * @param action Action to be executed.
				 * @param availableShots Number of different shots to be selected.
				 * @return Index of the most appropiate shot to be done at the moment. -1 if no possible shot.
				 */
				int _selectShot(const Controls::Action &action, const int &availableShots);
			
				/**
				 * Execute a shot by hitting the ball in the appropiate direction,
				 * depending on the current shot buffer status.
				 *
				 * @param action Action to be executed.
				 */
				void _shoot(const Controls::Action &action);

			public:
				
				PlayerHuman(Ogre::SceneManager *sceneManager, OgreBulletDynamics::DynamicsWorld *dynamicWorld,
					Widget::Ball *ball, Data::Match *matchData, Data::Player *playerData, Data::PointState::Machine *pointStateMachine);
				~PlayerHuman();

				void enter();
				void exit();

				bool frameStarted(const Ogre::FrameEvent &event);
				bool keyPressed(const OIS::KeyEvent &event);
				bool keyReleased(const OIS::KeyEvent &event);
				bool buttonPressed(const OIS::JoyStickEvent &event, int button);
				bool buttonReleased(const OIS::JoyStickEvent &event, int button);
		};
	};
};

#endif
