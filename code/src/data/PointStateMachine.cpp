/* 
 * PointStateMachine.cpp -- Implementation file for the point state machine.
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

#include "data/PointStateMachine.h"

using namespace CrazyTennis::Data::PointState;

void
Machine::_notifyAll() const
{
}

Machine::Machine()
	:	_current(STATE_IN_SERVE), _previous(STATE_IN_SERVE)
{
}

Machine::~Machine()
{
}

void
Machine::addListener(Listener *listener)
{
	_listeners.push_back(listener);		
}

State
Machine::getCurrentState() const
{
	return _current;
}

void
Machine::setCurrentState(const State &state)
{
	bool shouldNotify = state != _current;
	_previous = _current;
	_current = state;
}

State
Machine::onEvent(const Event &event)
{
	switch(_current) {
		case STATE_BEFORE_SERVE:
			switch(event) {
				case EVENT_CONTINUE:
					setCurrentState(STATE_IN_SERVE);
					break;
			}
			break;

		case STATE_IN_SERVE:
			switch(event) {
				case EVENT_BALL_HIT:
					setCurrentState(STATE_WAITING_FOR_SERVE_RESULT);
					break;
			}
			break;

		case STATE_WAITING_FOR_SERVE_RESULT:
			switch(event) {
				case EVENT_BALL_IN_SERVE_AREA:
					setCurrentState(STATE_IN_POINT);
					break;
				case EVENT_BALL_IN_COURT_AREA:
				case EVENT_BALL_OUT:
					setCurrentState(STATE_BEFORE_SERVE);
					break;
			}
			break;

		case STATE_IN_POINT:
			switch(event) {
				case EVENT_BALL_OUT:
					setCurrentState(STATE_AFTER_POINT);
					break;
			}

		case STATE_AFTER_POINT:
			switch(event) {
				case EVENT_CONTINUE:
					setCurrentState(STATE_BEFORE_SERVE);
					break;
			}
			
			break;
		case STATE_PAUSE:
			switch(event) {
				case EVENT_GAME_RESUMED:
					setCurrentState(_previous);
					break;
			}
			break;
	};
}
