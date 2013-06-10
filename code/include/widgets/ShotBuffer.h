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
	
namespace CrazyTennis {
	
	namespace Widget {
		
		class ShotBuffer : public OGF::Scene {
			
			public:
				
				ShotBuffer();
				~ShotBuffer();

				void enter();
				void exit();

				bool keyReleased(const OIS::KeyEvent &event);

				/**
				 * Notify about a change in the position of the player
				 * or the ball to update the buffer.
				 * @param playerPosition Player absolute position.
				 * @param ballPosition Ball absolute position.
				 */
				void notifyPosition(const Ogre::Vector3 &playerPosition, const Ogre::Vector3 &ballPosition);
		};
	};
};

#endif