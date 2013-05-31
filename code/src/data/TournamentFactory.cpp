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

	return NULL;
}
