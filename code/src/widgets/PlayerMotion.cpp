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

PlayerMotion::PlayerMotion(Ogre::SceneManager *sceneManager, Data::Player *data, Ogre::SceneNode *node,
	Ogre::Entity *entity, Widget::Ball *ball)
		:	OGF::Widget(sceneManager), _data(data), _node(node), _entity(entity), _ball(ball)
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
	_animations[ANIMATION_SHOT_DRIVE] = _entity->getAnimationState("shot_drive");
	_animations[ANIMATION_SHOT_LOB] = _entity->getAnimationState("shot_lob");
	_animations[ANIMATION_SIDE_STEP] = _entity->getAnimationState("side_step");
	*/
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
	if (!_animations[ANIMATION_STAND]->hasEnded()) {
		if (_animations[ANIMATION_STAND]->getTimePosition() + event.timeSinceLastFrame >= _animations[ANIMATION_STAND]->getLength()) {
			_animations[ANIMATION_STAND]->setTimePosition(0.05);
		} else {
			_animations[ANIMATION_STAND]->addTime(event.timeSinceLastFrame);
		}
	} else {
		_animations[ANIMATION_STAND]->setEnabled(true);
		_animations[ANIMATION_STAND]->setTimePosition(0.0);
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
}

void
PlayerMotion::driveHit()
{

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
