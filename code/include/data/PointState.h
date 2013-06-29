/* 
 * PointState.h -- Header file for the point state data definition.
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

#ifndef _DATA_POINT_STATE_H_
#define _DATA_POINT_STATE_H_

#include <list>

#include "data/PointState.h"

namespace CrazyTennis {
	
	namespace Data {

		 namespace PointState {
			
			enum State {
				// Point preparation, players reaching the serve position
				STATE_BEFORE_SERVE = 0,
				// Serve is happening, the ball is not yet hit
				STATE_IN_SERVE = 1,
				// The ball has been hit in the service but didn't collision with anything after
				STATE_WAITING_FOR_SERVE_RESULT = 2,
				// The point is being played
				STATE_IN_POINT = 3,
				// There is a point winner, and players ayer realising of the finish
				STATE_AFTER_POINT = 4,
				// The game has been paused
				STATE_PAUSE = 5
			};

			enum Event {
				// The player has done a continue action
				EVENT_CONTINUE = 0,
				// The player has hit the ball
				EVENT_BALL_HIT = 1,
				// The ball has hit the serve area
				EVENT_BALL_IN_SERVE_AREA = 2,
				// The ball has hit the court
				EVENT_BALL_IN_COURT_AREA = 3,
				// The player has missed the ball
				EVENT_BALL_OUT = 4,
				// The game has been paused
				EVENT_GAME_PAUSED = 6,
				// The game has been resumed
				EVENT_GAME_RESUMED = 7
			};
		};
	};
};

#endif
