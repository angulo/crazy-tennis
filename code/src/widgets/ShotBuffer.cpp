/* 
 * ShotBuffer.cpp -- Shot buffer implementation file
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

#include "widgets/ShotBuffer.h"

using namespace CrazyTennis::Widget;

ShotBuffer::ShotBuffer()
{
	_initConfigReader("widgets/shot_buffer.cfg");
	_playerPosition = Ogre::Vector3(0, 0, 0);
	_ballPosition = Ogre::Vector3(0, 0, 0);
	_maxRadius = _configValue<float>("maxRadius");
	_minRadius = _configValue<float>("minRadius");
	_outOfRange = true;
}

ShotBuffer::~ShotBuffer()
{

}

void
ShotBuffer::enter()
{
	_buffer[Controls::UP] = 0;
	_buffer[Controls::DOWN] = 0;
	_buffer[Controls::LEFT] = 0;
	_buffer[Controls::RIGHT] = 0;
	_buffer[Controls::SHOT_DRIVE] = 0;
	_buffer[Controls::SHOT_LOB] = 0;
}

void
ShotBuffer::exit()
{

}

bool
ShotBuffer::frameStarted(const Ogre::FrameEvent &event)
{
	InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();	
	OIS::Keyboard *keyboard = OGF::InputManager::getSingletonPtr()->getKeyboard();

	// If the ball is in front of the player and the distance is between
	// the max and the min lenght, update the buffer. Otherwise, reset it
	Ogre::Real ballPlayerDistance = (_ballPosition - _playerPosition).length();

	if (((_playerPosition.x < 0 && (_ballPosition.x >= _playerPosition.x)) ||
		_playerPosition.y >= 0 && (_ballPosition.x <= _playerPosition.x)) &&
		ballPlayerDistance < _maxRadius && ballPlayerDistance > _minRadius) {
			
		for (Buffer::iterator it = _buffer.begin(); it != _buffer.end(); it++) {
			OIS::KeyCode key = inputAdapter->actionToKeyInput(it->first);

			// Button is pressed
			if (keyboard->isKeyDown(key)) {
				it->second = std::min(1.0,
					it->second + (0.5 * event.timeSinceLastFrame));
			} else {
				it->second = std::max(0.0,
					it->second - (0.5 * event.timeSinceLastFrame));
			}
		}
	} else if (!_outOfRange) {
		_outOfRange = true;

		for (Buffer::iterator it = _buffer.begin(); it != _buffer.end(); it++) {
			it->second = 0;
		}
	}
}

void
ShotBuffer::notifyPosition(const Ogre::Vector3 &playerPosition, const Ogre::Vector3 &ballPosition)
{
	_playerPosition = playerPosition;
	_ballPosition = ballPosition;
}

Ogre::Real
ShotBuffer::getBufferValue(const Controls::Action &action)
{
	return _buffer[action];
}
