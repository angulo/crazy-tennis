/* 
 * Types.h -- Data types definition
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

#ifndef _DATA_TYPES_H_
#define _DATA_TYPES_H_

#include <map>
#include <string>
#include <vector>

namespace CrazyTennis {
	
	namespace Data {

		typedef unsigned short PlayerId;
		
		/** Set of game playing skills */
		typedef std::map<std::string, float> SkillSet;
		typedef std::map<std::string, std::string> VisualSet;

		/** Game difficult level */
		enum Difficulty
		{
			DIFFICULTY_EASY = 0,
			DIFFICULTY_MEDIUM = 1,
			DIFFICULTY_HARD = 2
		};

		/** Tournament configuration */
		struct TournamentConfig
		{
			short steps;
			Difficulty difficulty;
			short sets;
			short games;
			bool tiebreak;
		};
		
		enum PointValue {
			POINT_0 = 0,
			POINT_15 = 15,
			POINT_30 = 30,
			POINT_40 = 40,
			POINT_A = 50
		};

		typedef std::vector<PointValue> GameResult;
		typedef std::vector<short> SetResult;
		typedef std::vector<SetResult> MatchResult;

		/**
			* Current status of a match.
			*/
		struct MatchStatus {
			PlayerId server;
			PlayerId winner;
			MatchResult matchResult;
			GameResult gameResult;
			bool inTiebreak;
			bool firstServe;
			bool isFinished;
		};
	};
};

#endif
