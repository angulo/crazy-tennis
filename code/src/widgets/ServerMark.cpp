/* 
 * ServerMark.cpp -- Server mark implementation file
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

#include "widgets/ServerMark.h"

using namespace CrazyTennis::Widget;

void
ServerMark::_reset()
{
	Ogre::Real initialX = _configValue<float>("initialX");
	Ogre::Real initialZ = _configValue<float>("initialZ");
	Ogre::Vector3 position(initialX, 0.10, initialZ);

	if (_server == _matchData->getPlayer(0)->getId()) {
		if (_matchData->getWhereToServe() == Data::PointState::BOUNCE_IN_LEFT_SERVE_AREA) {
			position.x = -initialX;
			position.z = -initialZ;
		} else {
			position.x = -initialX;
		}
	} else if (_matchData->getWhereToServe() == Data::PointState::BOUNCE_IN_LEFT_SERVE_AREA) {
		position.z = -initialZ;
	}

	_node->setPosition(position);
}

ServerMark::ServerMark(Ogre::SceneManager *sceneManager, Data::Match *matchData, const Data::PlayerId &server)
	:	OGF::Widget(sceneManager), _matchData(matchData), _server(server), _shouldMove(false)
{
	_initConfigReader("widgets/serverMark.cfg");
}

ServerMark::~ServerMark()
{
}

void
ServerMark::enter()
{
	OGF::ModelBuilderPtr builder(OGF::ModelFactory::getSingletonPtr()->getBuilder(_sceneManager, CrazyTennis::Model::SERVER_MARK));
	builder->castShadows(false)
		->position(Ogre::Vector3::ZERO)
		->visible(false)
		->parent(_sceneManager->getRootSceneNode()->createChildSceneNode());
	
	_node = builder->buildNode();
}

void
ServerMark::exit()
{

}

bool
ServerMark::frameStarted(const Ogre::FrameEvent &event)
{
	if (!_shouldMove) {
		return true;
	}

	Ogre::Real speed = _configValue<float>("speed");

	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();

	Ogre::Vector3 currentPosition = getPosition();
	Ogre::Vector3 movement(0, 0, 0);

	if (inputAdapter->isActionActive(Controls::UP))
		movement.x = -1;
	if (inputAdapter->isActionActive(Controls::DOWN))
		movement.x = 1;
	if (inputAdapter->isActionActive(Controls::LEFT))
		movement.z = 1;
	if (inputAdapter->isActionActive(Controls::RIGHT))
		movement.z = -1;

	// Only update position in case the movement vector has positive length
	if (movement != Ogre::Vector3::ZERO) {
		movement = event.timeSinceLastFrame * speed * movement.normalisedCopy();
		Ogre::Vector3 destination = currentPosition + movement;

		_node->setPosition(destination);
	}
	_node->yaw(Ogre::Degree(360 * event.timeSinceLastFrame));

}

Ogre::Vector3
ServerMark::getPosition() const
{
	return _node->getPosition();
}

void
ServerMark::onChangeState(const Data::PointState::State &previousState, const Data::PointState::State &currentState)
{
	if (currentState == Data::PointState::STATE_IN_SERVE && 
		_matchData->getCurrentServer()->getId() == _server) {

		_reset();
		_node->setVisible(true);
		_shouldMove = true;
	} else {
		_shouldMove = false;
		_node->setVisible(false);
	}
}
