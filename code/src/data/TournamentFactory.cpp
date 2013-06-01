/* 
 * TournamentFactory.cpp -- Tournament factory implementation
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

#include "data/TournamentFactory.h"

using namespace CrazyTennis::Data;

void
TournamentFactory::_customizePlayer(Player *player, const Difficulty &difficulty)
{
	float multiplier = 1;

	switch(difficulty) {
		
		case DIFFICULTY_EASY:
			multiplier = 0.5;
			break;
		default:
		case DIFFICULTY_MEDIUM:
			multiplier = 1;
			break;
		case DIFFICULTY_HARD:
			multiplier = 2;
			break;
	}

	SkillSet skills = player->getSkills();

	for (SkillSet::iterator it = skills.begin(); it != skills.end(); it++) {
		it->second = std::min(1.f, it->second * multiplier);
	}

	player->setSkills(skills);
}

TournamentFactory::TournamentFactory(const std::list<Player *> basePlayers)
	: _basePlayers(basePlayers)
{
}

TournamentFactory::~TournamentFactory()
{
}

Tournament *
TournamentFactory::create(const TournamentConfig &config, PlayerId currentPlayerId)
{
	// Shuffle the players
	std::vector<Player *> basePlayersShuffable(_basePlayers.begin(), _basePlayers.end());
	std::random_shuffle(basePlayersShuffable.begin(), basePlayersShuffable.end());

	// Select N players from the suffled vector, N == number of steps of the tournament
	// and customize them with the tournament difficulty
	std::vector<Player *> result;

	for (std::vector<Player *>::iterator it = basePlayersShuffable.begin();
		it != basePlayersShuffable.begin() + config.steps; it++) {

		// Implicit copy constructor
		Player *customizedPlayer = new Player(**it);
		_customizePlayer(customizedPlayer, config.difficulty);

		result.push_back(customizedPlayer);
	}

	Tournament *tournament = new Tournament(config,
		std::list<Player *>(result.begin(), result.end()));

	return tournament;
}
