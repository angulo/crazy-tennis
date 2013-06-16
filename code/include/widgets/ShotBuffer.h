/* 
 * ShotBuffer.h -- Header file of the shot buffer
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

#ifndef _WIDGET_SHOT_BUFFER_H_
#define _WIDGET_SHOT_BUFFER_H_

#include <OGF/OGF.h>

#include "InputAdapter.h"
	
namespace CrazyTennis {
	
	namespace Widget {
		
		class ShotBuffer : public OGF::Scene {
			
			typedef std::map<Controls::Action, Ogre::Real> Buffer;
			
			protected:
				
				Buffer _buffer;

				Ogre::Real _minRadius;
				Ogre::Real _maxRadius;

				Ogre::Vector3 _playerPosition;
				Ogre::Vector3 _ballPosition;
				bool _outOfRange;

				Ogre::Real _timeSinceLastUpdate;

			public:
				
				ShotBuffer();
				~ShotBuffer();

				void enter();
				void exit();

				bool frameStarted(const Ogre::FrameEvent &event);

				/**
				 * Notify about a change in the position of the player
				 * or the ball to update the buffer.
				 *
				 * @param playerPosition Player absolute position.
				 * @param ballPosition Ball absolute position.
				 */
				void notifyPosition(const Ogre::Vector3 &playerPosition, const Ogre::Vector3 &ballPosition);

				/**
				 * Get the buffer value of a certain action
				 *
				 * @param action Action to the buffer value from.
				 * @return Buffer value of the action.
				 */
				Ogre::Real getValue(const Controls::Action &action);
		};
	};
};

#endif
