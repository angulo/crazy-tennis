/* 
 * PlayerBase.h -- Base player widget header file
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

#ifndef _WIDGET_PLAYER_BASE_H_
#define _WIDGET_PLAYER_BASE_H_

#include <OGF/OGF.h>

#include <OgreBulletCollisions.h>
#include <OgreBulletDynamics.h>
#include <Utils/OgreBulletCollisionsMeshToShapeConverter.h>

#include "data/Player.h"
#include "widgets/Ball.h"
#include "widgets/PhysicalBase.h"

namespace CrazyTennis {
	
	namespace Widget {

		class PlayerBase: public PhysicalBase {
			
			protected:
				
				Data::Player *_data;
				Widget::Ball *_ball;

			public:

				PlayerBase(Ogre::SceneManager *sceneManager, OgreBulletDynamics::DynamicsWorld *dynamicWorld, Widget::Ball *ball, Data::Player *data);
				~PlayerBase();

				/**
				 * Creates the rigid body of the player using the 
				 * provided player data as configuration.
				 */
				virtual void enter();
				void exit();

				bool frameStarted(const Ogre::FrameEvent &event);
		};
	};
};

#endif
