/* 
 * Match.cpp -- Match data implementation file
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

#include "data/Match.h"

using namespace CrazyTennis::Data;

void
Match::_notifyListeners()
{
	MatchStatus status = getStatus();
	for(std::list<MatchListener *>::iterator it = _listeners.begin(); it != _listeners.end(); it++)
		(*it)->onMatchEvent(status);
}

bool
Match::_isGameFinished() const
{
	return (std::abs(_gameScore[0] - _gameScore[1]) >= 2  &&
		std::max(_gameScore[0], _gameScore[1]) >= 3);
}

bool
Match::_isSetFinished() const
{
	SetScore setScore = *_currentSet;
	return std::abs(setScore[0] - setScore[1]) >= 2 &&
		std::max(setScore[0], setScore[1]) >= 6;
}

bool
Match::_isMatchFinished() const
{
	return _matchScore.end() - _currentSet == 1 && _isSetFinished();
}

Match::Match(const short &games, const bool &hasTiebreak, Player *playerA, Player *playerB)
	:	_hasTiebreak(hasTiebreak), _inTiebreak(false), _firstServe(true), _isFinished(false)
{
	_players.push_back(playerA);
	_players.push_back(playerB);
	_currentServer = playerA;

	for (int i = 0; i < games; i++) {
		SetScore setScore;
		setScore.push_back(0);
		setScore.push_back(0);
		_matchScore.push_back(setScore);
	}

	_gameScore.push_back(0);
	_gameScore.push_back(0);
}

Match::~Match()
{

}

MatchStatus
Match::wonPoint(const PlayerId &pointWinner)
{
	_firstServe = true;
	int playerOffset = _players[0]->getId() == pointWinner ? 0 : 1;
	_gameScore[playerOffset] += 1;

	if (_isGameFinished()) {
		_gameScore[0] = _gameScore[1] = 0;
		(*_currentSet)[playerOffset] += 1;

		if (_isSetFinished()) {
			if (_isMatchFinished()) {
				_isFinished = true;
				_winner = _players[playerOffset];
			} else {
				_currentSet++;
			}
		} else {
			_currentServer = _players[1 - playerOffset];
		}
	}

	_notifyListeners();
}

MatchStatus
Match::missedService()
{
	if (_firstServe) {
		_firstServe = false;
	} else {
		int playerOffset = _players[0] == _currentServer? 0 : 1;
		wonPoint(_players[1 - playerOffset]->getId());
	}

	_notifyListeners();
}

MatchStatus
Match::getStatus () const
{
	MatchStatus status;
	status.server = _currentServer->getId();
	status.winner = _isFinished ? _winner->getId() : -1;
	status.matchScore = _matchScore;
	status.gameScore = _gameScore;
	status.inTiebreak = _inTiebreak;
	status.firstServe = _firstServe;
	status.isFinished = _isFinished;

	return status;
}

Player *
Match::getPlayer(const int &number) const
{
	return _players[number];
}

void
Match::addListener(MatchListener *matchListener)
{
	_listeners.push_back(matchListener);	
}
