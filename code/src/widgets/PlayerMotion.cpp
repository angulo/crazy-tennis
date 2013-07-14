/* 
 * PlayerMotion.cpp -- Player motion manager implementation
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

#include "widgets/PlayerMotion.h"

using namespace CrazyTennis::Widget;

void
PlayerMotion::_disableAllAnimations()
{
	for (std::map<Animation, Ogre::AnimationState *>::iterator it = _animations.begin();
		it != _animations.end(); it++) {

		it->second->setEnabled(false);
		it->second->setWeight(0);
		it->second->setTimePosition(0);
	}
}

void
PlayerMotion::_updateAnimationStand(const Ogre::Real &timePassed)
{
	Ogre::AnimationState *animation = _animations[ANIMATION_STAND];

	if (!animation->hasEnded()) {
		if (animation->getTimePosition() + timePassed >= animation->getLength()) {
			animation->setTimePosition(0.05);
		} else {
			animation->addTime(timePassed);
		}
	} else {
		animation->setEnabled(true);
		animation->setTimePosition(0.0);
	}
}

void
PlayerMotion::_updateAnimationShotDrive(const Ogre::Real &timePassed)
{
	Ogre::AnimationState *animation = _animations[ANIMATION_SHOT_DRIVE];

	if (!animation->hasEnded()) {
		Ogre::Real percentRemaining = 1 - (animation->getTimePosition() / animation->getLength());
		if (percentRemaining <= 0.5) {
			if (_driveHitSecondPhase) {
				animation->addTime(timePassed * 40);
			}
		} else {
			animation->addTime(timePassed * 40);
		}
	} else {
		animation->setEnabled(false);
		animation->setWeight(0);
		_animations[ANIMATION_STAND]->setWeight(1.0);
		animation->setTimePosition(0.0);
	}
}

PlayerMotion::PlayerMotion(Ogre::SceneManager *sceneManager, Data::Player *data, Ogre::SceneNode *node,
	Ogre::Entity *entity, Widget::Ball *ball, Widget::PlayerBase *player)
		:	OGF::Widget(sceneManager), _data(data), _node(node), _entity(entity), _ball(ball), _player(player)
{

}

PlayerMotion::~PlayerMotion()
{

}

void
PlayerMotion::enter()
{
	/*
	_animations[ANIMATION_RUN] = _entity->getAnimationState("run");
	_animations[ANIMATION_SHOT_LOB] = _entity->getAnimationState("shot_lob");
	_animations[ANIMATION_SIDE_STEP] = _entity->getAnimationState("side_step");
	*/

	_animations[ANIMATION_SHOT_DRIVE] = _entity->getAnimationState("shot_drive");
	_animations[ANIMATION_STAND] = _entity->getAnimationState("stand");

	_disableAllAnimations();
	stand();
}

void
PlayerMotion::exit()
{

}

bool
PlayerMotion::frameStarted(const Ogre::FrameEvent &event)
{
	for (std::map<Animation, Ogre::AnimationState *>::iterator it = _animations.begin();
		it != _animations.end(); it++) {

		if (it->second->getEnabled()) {
			switch(it->first) {
				case ANIMATION_STAND:
					_updateAnimationStand(event.timeSinceLastFrame);
					break;
				case ANIMATION_SHOT_DRIVE:
					_updateAnimationShotDrive(event.timeSinceLastFrame);
					break;
				default:
					break;
			}
		}
	}

	return true;
}

void
PlayerMotion::stand()
{
	_disableAllAnimations();
	_animations[ANIMATION_STAND]->setEnabled(true);
	_animations[ANIMATION_STAND]->setTimePosition(0.0);
	_animations[ANIMATION_STAND]->setWeight(1.0);

	_driveHitSecondPhase = false;
	_backHitSecondPhase = false;
}

void
PlayerMotion::driveHitStart()
{
	_animations[ANIMATION_SHOT_DRIVE]->setEnabled(true);
	_animations[ANIMATION_SHOT_DRIVE]->setTimePosition(0.0);
	_animations[ANIMATION_STAND]->setWeight(0.2);
	_animations[ANIMATION_SHOT_DRIVE]->setWeight(0.8);
	_animations[ANIMATION_SHOT_DRIVE]->setLoop(false);
	_driveHitSecondPhase = false;
}

void
PlayerMotion::driveHitEnd()
{
	_driveHitSecondPhase = true;
}

void
PlayerMotion::lobHit()
{

}

void
PlayerMotion::runTo(const Ogre::Vector3 &destination)
{

}

void
PlayerMotion::runDeltaTo(const Ogre::Vector3 &direction, const Ogre::Real &timeSinceLastMove)
{

}
