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
	_outOfRange = false;
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
	_timeSinceLastUpdate = 0;
}

void
ShotBuffer::exit()
{

}

bool
ShotBuffer::frameStarted(const Ogre::FrameEvent &event)
{
	_timeSinceLastUpdate += event.timeSinceLastFrame;
	if (_timeSinceLastUpdate >= _configValue<float>("maxTimeBetweenUpdates")) {
		InputAdapter *inputAdapter = InputAdapter::getSingletonPtr();	

		Ogre::Real ballPlayerDistance = (_ballPosition - _playerPosition).length();

			// Check minimum and maximum distance to hit
		if (ballPlayerDistance < _maxRadius && ballPlayerDistance > _minRadius &&
			// Check that the ball and the player are in the same side
			((_ballPosition.x > 0 && _playerPosition.x > 0) || (_ballPosition.x < 0 && _playerPosition.x < 0))) { 
				
			_outOfRange = false;

			Ogre::Real increment = _configValue<float>("incrementPerSecond") * 
				_timeSinceLastUpdate;

			// Update all the buffers with the incremented/decreased value, 
			// depending on the action input status
			for (Buffer::iterator it = _buffer.begin(); it != _buffer.end(); it++) {
				if (inputAdapter->isActionActive(it->first)) {
					it->second = std::min(Ogre::Real(1.0), it->second + increment);
				} else {
					it->second = std::max(Ogre::Real(0.0), it->second - increment);
				}
			}
		// In case the player is out of range, reset the buffers
		} else if (!_outOfRange) {
			_outOfRange = true;

			for (Buffer::iterator it = _buffer.begin(); it != _buffer.end(); it++) {
				it->second = 0;
			}
		}

		_timeSinceLastUpdate = 0;
	}

	return true;
}

void
ShotBuffer::notifyPosition(const Ogre::Vector3 &playerPosition, const Ogre::Vector3 &ballPosition)
{
	_playerPosition = playerPosition;
	_ballPosition = ballPosition;
}

Ogre::Real
ShotBuffer::getValue(const Controls::Action &action)
{
	return _buffer[action];
}
