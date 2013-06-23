/* 
 * Match.h -- Match data header file.
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

#ifndef _DATA_MATCH_H_
#define _DATA_MATCH_H_

#include <cmath>
#include <list>
#include <vector>

#include "data/MatchListener.h"
#include "data/Player.h"
#include "data/Types.h"

namespace CrazyTennis {
	
	namespace Data {

		typedef std::vector<Player *> PlayersPair;

		class Match {
		
			protected:
				
				PlayersPair _players;
				Player *_currentServer;

				MatchScore _matchScore;
				MatchScore::iterator _currentSet;
				GameScore _gameScore;
				SetScore _tiebreakScore;

				bool _hasTiebreak;
				bool _inTiebreak;
				bool _firstServe;
				bool _isFinished;
				Player *_winner;

				std::list<MatchListener *> _listeners;

				void _notifyListeners();
				bool _isGameFinished() const;
				bool _isSetFinished() const;
				bool _isMatchFinished() const;
				
			public:
				
				Match(const short &games, const bool &hasTiebreak, Player *playerA, Player *playerB);
				~Match();

				MatchStatus wonPoint(const PlayerId &pointWinner);
				MatchStatus missedService();
				MatchStatus getStatus() const;

				Player *getPlayer(const int &number) const;
				void addListener(MatchListener *matchListener);
		};
	};
};

#endif
