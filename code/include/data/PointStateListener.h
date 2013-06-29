/* 
 * PointStateListener.h -- Header file for the point state listener interface.
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

#ifndef _DATA_POINT_STATE_LISTENER_H_
#define _DATA_POINT_STATE_LISTENER_H_

#include "data/PointState.h"
	
namespace CrazyTennis {
	
	namespace Data {
		
		namespace PointState {
		
			class Listener {
				
				virtual void onChangePointState(const PointState::State &previousState, const PointState::State &currentState) = 0;
			};
		};
	};
};

#endif

