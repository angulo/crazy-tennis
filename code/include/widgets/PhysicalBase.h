/* 
 * PhysicalBase.h -- Base class for widgets with physic properties header file
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

#ifndef _WIDGET_PHYSICAL_BASE_H_
#define _WIDGET_PHYSICAL_BASE_H_

#include <OGF/OGF.h>

#include <OgreBulletDynamicsWorld.h>
#include <OgreBulletDynamicsRigidBody.h>

namespace CrazyTennis {
	
	namespace Widget {
		
		class PhysicalBase: public OGF::Widget {
			
			protected:

				OgreBulletDynamics::DynamicsWorld * _dynamicWorld;
				OgreBulletDynamics::RigidBody *_rigidBody;
				
			public:
				
				PhysicalBase(Ogre::SceneManager *sceneManager, OgreBulletDynamics::DynamicsWorld *dynamicWorld);
				virtual ~PhysicalBase();

				/**
				 * Set the position of the ball.
				 * @param position Position
				 */
				virtual void setPosition(const Ogre::Vector3 &position);

				/**
				 * Set the position of the ball.
				 * @param x X-axis position
				 * @param y Y-axis position
				 * @param z Z-axis position
				 */
				virtual void setPosition(const Ogre::Real &x, const Ogre::Real &y, const Ogre::Real& z);

				/**
				 * Get the current position of the physic body.
				 *
				 * @return Current position of the widget
				 */
				Ogre::Vector3 getPosition() const;

				/**
				 * Move the body a certain distance.
				 * @param delta Increment of position.
				 */
				void move(const Ogre::Vector3 &delta);

				/**
				 * Move the body a certain distance.
				 * @param deltaX X-axis increment
				 * @param deltaY Y-axis increment
				 * @param deltaZ Z-axis increment
				 */
				void move(const Ogre::Real &deltaX, const Ogre::Real &deltaY, const Ogre::Real &deltaZ); 

        void rotate(const Ogre::Vector3& axis, const Ogre::Degree& angle);

				/**
				 * Set the velocity of the rigid body.
				 * @param velocity Vector velocity
				 */
				void setLinearVelocity(const Ogre::Vector3 &velocity);
		};
	};
};

#endif
