/* 
 * Ball.h -- Ball widget header file
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

#ifndef _WIDGET_BALL_H_
#define _WIDGET_BALL_H_

#include <OGF/OGF.h>

#include <cmath>
#include <queue>

#include <OgreBulletCollisions.h>
#include <OgreBulletDynamics.h>
#include <OgreBulletDynamicsWorld.h>
#include <OgreBulletDynamicsRigidBody.h>
#include <Shapes/OgreBulletCollisionsSphereShape.h>	
#include <Utils/OgreBulletCollisionsMeshToShapeConverter.h>

#include "Model.h"
#include "dynamics/ShotSimulator.h"
#include "widgets/PhysicalBase.h"

namespace CrazyTennis {
	
	namespace Widget {

		class Ball: public PhysicalBase {
			
			protected:
				
				std::list<Ogre::SceneNode *> _ghosts;
				Ogre::Vector3 _direction;
				Ogre::Vector3 _whereToBounce;

				void _createGhosts();
				void _updateGhosts();
			
			public:

				Ball(Ogre::SceneManager *sceneManager, OgreBulletDynamics::DynamicsWorld *dynamicWorld);
				~Ball();

				void enter();
				void exit();
				bool frameStarted(const Ogre::FrameEvent &event);

				/**
				 * Set the visibility of the item.
				 *
				 * @param isVisible Whether the item is visible or not.
				 */
				virtual void setVisible(const bool &isVisible);

				void shotTo(const Ogre::Vector3 destination, const Ogre::Real &angle, const Ogre::Real &velocity);

				Ogre::Vector3 getDirection() const;
				Ogre::Vector3 getWhereToBounce() const;
		};
	};
};

#endif
