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
Machine::_hasLost(const PlayerId &loser)
{
	_hasWon(loser == _playerA ? _playerB : _playerA);
}

void
Machine::_hasWon(const PlayerId &winner)
{
	_winner = winner;
	setCurrentState(STATE_AFTER_POINT);
}

void
Machine::_swapTurn()
{
	setTurn(_turn == _playerA ? _playerB : _playerA);
}

Machine::Machine(const PlayerId &playerA, const PlayerId &playerB)
	:	_playerA(playerA), _playerB(playerB), _turn(playerA),
		_currentState(STATE_BEFORE_SERVE), _previousState(STATE_BEFORE_SERVE),
		_bounceCount(0)
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
	return _currentState;
}

void
Machine::setCurrentState(const State &state)
{
	bool shouldNotify = state != _currentState;
	_previousState = _currentState;
	_currentState = state;

	for (std::list<Listener *>::iterator it = _listeners.begin(); it != _listeners.end(); it++) {
		(*it)->onChangeState(_previousState, _currentState);

		if (_currentState == STATE_AFTER_POINT) {
			(*it)->onWonPoint(_winner);
		}
	}

}

CrazyTennis::Data::PlayerId
Machine::getWinner() const
{
	return _winner;
}

void
Machine::reset(const PlayerId &server, const BouncePlace &whereToServe)
{
	setTurn(server);
	setCurrentState(STATE_BEFORE_SERVE);
	_whereToServe = whereToServe;
	_bounceCount = 0;
}

void
Machine::setTurn(const PlayerId &playerToHit)
{
	_turn = playerToHit;
}

CrazyTennis::Data::PlayerId
Machine::getTurn() const
{
	return _turn;
}

void
Machine::onBallHit(const PlayerId &hitter)
{
	if (_currentState == STATE_IN_POINT) {
		_bounceCount = 0;
		if (_turn == hitter) {
			_swapTurn();
		} else {
			_hasWon(_turn);
		}
	} else if (_currentState == STATE_IN_SERVE) {
		_bounceCount = 0;
		setCurrentState(STATE_WAITING_FOR_SERVE_RESULT);
		_swapTurn();
	} else if (_currentState == STATE_WAITING_FOR_SERVE_RESULT) {
		_hasLost(_turn);
	}
}

void
Machine::onBallBounce(const PlayerId &courtOwner, const BouncePlace &where)
{
	if (_currentState == STATE_WAITING_FOR_SERVE_RESULT) {
		if (courtOwner != _turn || where != _whereToServe) {
			_hasWon(_turn);
		} else {
			setCurrentState(STATE_IN_POINT);
			++_bounceCount;
		}
	} else if (_currentState == STATE_IN_POINT) {
		++_bounceCount;

		if (_turn != courtOwner) {
			_hasWon(_turn);
		} else {
			if (_bounceCount == 1) {
				if (where == BOUNCE_OUT) {
					_hasWon(_turn);
				}
			} else {
				_hasLost(_turn);
			}
		}
	}
}

void
Machine::onContinue(const PlayerId &author)
{
	if (_currentState == STATE_BEFORE_SERVE) {
		setCurrentState(STATE_IN_SERVE);
	}
}

void
Machine::onGamePaused()
{
	if (_currentState != STATE_PAUSE) {
		setCurrentState(STATE_PAUSE);
	}
}

void
Machine::onGameResumed()
{
	if (_currentState == STATE_PAUSE) {
		setCurrentState(_previousState);
	}
}
