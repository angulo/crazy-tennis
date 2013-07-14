/* 
 * PlayerMotion.h -- Header file for the player motion manager.
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

namespace CrazyTennis {
	namespace Widget {
		class PlayerMotion;
	};
};

#ifndef _WIDGETS_PLAYER_MOTION_H_
#define _WIDGETS_PLAYER_MOTION_H_

#include <map>

#include <OGF/OGF.h>

#include "data/Player.h"
#include "widgets/Ball.h"
#include "widgets/PlayerBase.h"

namespace CrazyTennis {

	namespace Widget {
		
		class PlayerMotion : public OGF::Widget {

			enum Animation {
				ANIMATION_RUN,
				ANIMATION_SERVE,
				ANIMATION_SHOT_BACK,
				ANIMATION_SHOT_DRIVE,
				ANIMATION_SHOT_LOB,
				ANIMATION_SIDE_STEP,
				ANIMATION_STAND
			};
			
			protected:

				Data::Player *_data;
				
				Ogre::SceneNode *_node;
				Ogre::Entity *_entity;

				Widget::Ball *_ball;
				Widget::PlayerBase *_player;

				std::map<Animation, Ogre::AnimationState *> _animations;	

				bool _driveHitSecondPhase;

				void _disableAllAnimations();

				void _updateAnimationServe(const Ogre::Real &timePassed);
				void _updateAnimationStand(const Ogre::Real &timePassed);
				void _updateAnimationShotDrive(const Ogre::Real &timePassed);
				void _updateAnimationShotBack(const Ogre::Real &timePassed);

			public:
				
				PlayerMotion(Ogre::SceneManager *sceneManager, Data::Player *data, Ogre::SceneNode *node,
					Ogre::Entity *entity, Widget::Ball *ball, Widget::PlayerBase *player);
				~PlayerMotion();

				void enter();
				void exit();
				bool frameStarted(const Ogre::FrameEvent &event);

				
				/**
				 * Start the serve throwing the ball up.
				 */
				void serveStart();

				/**
				 * Hit the ball serving.
				 */
				void serveEnd();

				/**
				 * Set the player in a standing position.
				 */
				void stand();

				/**
				 * Hit the ball with a drive shot.
				 */
				void driveHitStart();

				/**
				 * Hit the ball with a drive shot (second phase)
				 */
				void driveHitEnd();

				/**
				 * Hit the ball with a lob shot.
				 */
				void lobHit();

				/**
				 * Make the player run to a destination.
				 *
				 * @param destination Destination of the player.
				 */
				void runTo(const Ogre::Vector3 &destination);

				/**
				 * Make the player continue moving to a certain direction after some time passed.
				 *
				 * @param direction Delta movement direction.
				 * @param timeSinceLastMove Time increment to consider when making the movement.
				 */
				void runDeltaTo(const Ogre::Vector3 &direction, const Ogre::Real &timeSinceLastMove);
		};
	};
};

#endif
