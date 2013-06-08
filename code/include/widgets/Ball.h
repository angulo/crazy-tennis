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

#include <OgreBulletCollisions.h>
#include <OgreBulletDynamics.h>
#include <OgreBulletDynamicsWorld.h>
#include <OgreBulletDynamicsRigidBody.h>

#include "Shapes/OgreBulletCollisionsSphereShape.h"	
#include <Utils/OgreBulletCollisionsMeshToShapeConverter.h>

#include "Model.h"

namespace CrazyTennis {
	
	namespace Widget {

		class Ball: public OGF::Widget {
			
			private:

				Ogre::SceneManager *_sceneManager;
				OgreBulletDynamics::DynamicsWorld * _dynamicWorld;
				OgreBulletDynamics::RigidBody *_rigidBody;
				
			public:

				/**
				 * Constructor
				 *
				 * @param sceneManager Manager of the scene the ball belongs to.
				 * @param dynamicWorld Bullet dynamic world instance.
				 */
				Ball(Ogre::SceneManager *sceneManager, OgreBulletDynamics::DynamicsWorld *dynamicWorld);
				~Ball();

				void preload();
				void enter();
				void exit();
				void pause();
				void resume();

				bool frameEnded(const Ogre::FrameEvent& event);
				bool frameStarted(const Ogre::FrameEvent &event);
				bool keyPressed(const OIS::KeyEvent &event);

				/**
				 * Set the position of the ball.
				 * @param position Position
				 */
				void setPosition(const Ogre::Vector3 &position);

				/**
				 * Set the position of the ball.
				 * @param x X-axis position
				 * @param y Y-axis position
				 * @param z Z-axis position
				 */
				void setPosition(const Ogre::Real &x, const Ogre::Real &y, const Ogre::Real& z);
		};
	};
};

#endif
