/* 
 * PointStateMachine.h -- Header file for the point state machine.
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

#ifndef _DATA_POINT_STATE_MACHINE_H_
#define _DATA_POINT_STATE_MACHINE_H_

#include <list>

#include "data/PointState.h"
#include "data/PointStateListener.h"

namespace CrazyTennis {
	
	namespace Data {
		
		namespace PointState {

			class Machine {
				
				private:
					
					State _current;
					State _previous;

					std::list<Listener *> _listeners;
					
					/**
					* Notify all the listeners about a change in the 
					* current state.
					*/
					void _notifyAll() const;

				public:
					
					Machine();
					~Machine();

					/**
					* Add a listener for the current state changes.
					*
					* @param listener Point state listener.
					*/
					void addListener(Listener *listener);

					/**
					* Get the current point state.
					*
					* @return Current point state.
					*/
					State getCurrentState() const;

					/**
					* Set the current point status.
					*
					* @param state State to be set.
					*/
					void setCurrentState(const State &state);

					/**
					* Update the state machine with an event that has happened.
					*
					* @param event Event that has happened recently.
					* @return New status after the update.
					*/
					State onEvent(const Event &event);
			};
		};
	};
};

#endif
