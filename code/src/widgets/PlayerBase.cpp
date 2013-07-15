/* 
 * PlayerBase.h -- Base player widget implementation file
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

#include "widgets/PlayerBase.h"

using namespace CrazyTennis::Widget;

Ogre::Real
PlayerBase::_getSpeed() const
{
	return _speed;
}

void
PlayerBase::_setInReturnState()
{
	Ogre::Vector3 returnPosition(0, 0, 0);
	Ogre::Real xOffset = _configValue<float>("courtLength") +
		_configValue<float>("returnXOffset");
	Ogre::Real zOffset = _configValue<float>("returnZOffset");
	
	Data::PointState::BouncePlace whereToServe = _matchData->getWhereToServe();

	returnPosition.x = getPosition().x > 0 ? xOffset : -xOffset;

	if (whereToServe == Data::PointState::BOUNCE_IN_RIGHT_SERVE_AREA) {
		returnPosition.z = getPosition().x > 0 ? -zOffset : zOffset;
	} else {
		returnPosition.z = getPosition().x < 0 ? -zOffset : zOffset;
	}

	setPosition(returnPosition);
	_motionManager->stand();
}

void 
PlayerBase::_setInServeState()
{
	Ogre::Vector3 servePosition(0, 0, 0);
	Ogre::Real xOffset = _configValue<float>("courtLength") +
		_configValue<float>("serveXOffset");
	Ogre::Real zOffset = _configValue<float>("serveZOffset");
	
	Data::PointState::BouncePlace whereToServe = _matchData->getWhereToServe();

	servePosition.x = getPosition().x > 0 ? xOffset : -xOffset;

	if (whereToServe == Data::PointState::BOUNCE_IN_RIGHT_SERVE_AREA) {
		servePosition.z = getPosition().x > 0 ? -zOffset : zOffset;
	} else {
		servePosition.z = getPosition().x < 0 ? -zOffset : zOffset;
	}

	setPosition(servePosition);
	_motionManager->stand();
}

void
PlayerBase::_startToServe()
{
	Ogre::Vector3 position = getPosition();

	position.x += getPosition().x > 0 ? - _configValue<float>("ballServeXOffset") : 
		_configValue<float>("ballServeXOffset");
	position.y = 1;
	position.z += getPosition().x > 0 ? - _configValue<float>("ballServeZOffset") : 
		_configValue<float>("ballServeZOffset");

	_ball->setPosition(position);
	_ball->setLinearVelocity(Ogre::Vector3(0, 7, 0));
	_motionManager->serveStart();
}

void
PlayerBase::_serve()
{
	Dynamics::ShotSimulator *simulator = new Dynamics::ShotSimulator();

	Ogre::Vector3 origin = _ball->getPosition();
	Ogre::Vector3 destination = _calculateServeDestination();

	Dynamics::CalculationSet allShots = simulator->setOrigin(origin)
		->setDestination(destination)
		->calculateSet(200);
	
	Dynamics::CalculationSet possibleShots;

	for (int i = 0; i < allShots.size(); i++) {
		Ogre::Real angle = allShots[i].first;
		Ogre::Real velocity = allShots[i].second;

		// Discard impossible angles and directions
		if (!isnan(velocity) && !isnan(angle) && velocity > 0) {
			possibleShots.push_back(allShots[i]);
		}
	}

	int availableShots = possibleShots.size();

	if (availableShots > 0) {
		int shot = (1 - _playerData->getSkills()["serve"]) * 30;

		if (shot != -1) {
			Ogre::Real angle = possibleShots[shot].first;
			Ogre::Real velocity = possibleShots[shot].second;

			_ball->shotTo(destination, angle, velocity);
			SoundPlayer::getSingletonPtr()->play(SOUND_BALL_SERVE);
			_pointStateMachine->onBallHit(_playerData->getId());
			_motionManager->serveEnd();
		}
	}

	delete simulator;
}



PlayerBase::PlayerBase(Ogre::SceneManager *sceneManager, Widget::Ball *ball, Data::Match *matchData,
	Data::Player *playerData, Data::PointState::Machine *pointStateMachine)
:	OGF::Widget(sceneManager), _matchData(matchData), _playerData(playerData), _ball(ball), _pointStateMachine(pointStateMachine)
{
_initConfigReader("widgets/player.cfg");
_speed = _playerData->getSkills()["speed"] * _configValue<float>("maximumRunSpeed");
_pointStateMachine->addListener(this);
}

PlayerBase::~PlayerBase()
{

}

void
PlayerBase::enter()
{
	OGF::ModelBuilderPtr builder(OGF::ModelFactory::getSingletonPtr()->getBuilder(_sceneManager, Model::PLAYER));

	_sceneNode = builder->castShadows(true)
		->parent(_sceneManager->getRootSceneNode()->createChildSceneNode())
		->buildNode();
	
	std::string clothes = _playerData->getVisual()["tshirt"] + "_" +  _playerData->getVisual()["shorts"];
	
	static_cast<Ogre::Entity *>(_sceneNode->getAttachedObject(0))->setMaterialName("shoter_" + clothes);
	
	_motionManager = new Widget::PlayerMotion(_sceneManager, _playerData, _sceneNode, static_cast<Ogre::Entity *>(_sceneNode->getAttachedObject(0)), _ball, this);

	OGF::SceneController::getSingletonPtr()->addChild(_motionManager);
}

void
PlayerBase::exit()
{

}

bool
PlayerBase::frameStarted(const Ogre::FrameEvent &event)
{
	return true;
}

Ogre::Vector3
PlayerBase::getPosition() const
{
	return _sceneNode->getPosition();
}

void
PlayerBase::setPosition(const Ogre::Vector3 &position)
{
	_sceneNode->setPosition(position);
}

void
PlayerBase::setPosition(const Ogre::Real &x, const Ogre::Real &y, const Ogre::Real& z)
{
	setPosition(Ogre::Vector3(x, y, z));
}

void
PlayerBase::rotate(const Ogre::Vector3& axis, const Ogre::Degree& angle)
{
	_sceneNode->rotate(axis, Ogre::Radian(angle));
}

void
PlayerBase::lookAt(const Ogre::Vector3 &point)
{
	_sceneNode->lookAt(point, Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);
}

void
PlayerBase::onChangeState(const Data::PointState::State &previousState, const Data::PointState::State &currentState)
{
	switch(currentState) {
		case Data::PointState::STATE_BEFORE_SERVE:
			if (_matchData->getCurrentServer() == _playerData) {
				_setInServeState();
			} else {
				_setInReturnState();
			}
			break;
	}
}
