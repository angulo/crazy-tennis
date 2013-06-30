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
#include "data/Types.h"

namespace CrazyTennis {
	
	namespace Data {
		
		namespace PointState {

			class Machine {
				
				private:
					
					PlayerId _playerA;
					PlayerId _playerB;
					PlayerId _turn;
					PlayerId _winner;

					BouncePlace _whereToServe;
					long int _bounceCount;

					State _currentState;
					State _previousState;

					std::list<Listener *> _listeners;
					
					/**
					 * Handle that a player has lost the point.
					 *
					 * @param loser Player that lost the point.
					 */
					void _hasLost(const PlayerId &loser);

					/**
					 * Handle that a player has won the point.
					 *
					 * @param winner Player that won the point.
					 */
					void _hasWon(const PlayerId &winner);

					/**
					 * Swap the turn.
					 */
					void _swapTurn();

				public:
					
					Machine(const PlayerId &playerA, const PlayerId &playerB);
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
					 * Reset the machine state.
					 *
					 * @param server Player to server.
					 */
					void reset(const PlayerId &server, const BouncePlace &whereToServe);

					/**
					 * Set the player that should hit the ball.
					 *
					 * @param playerToHit Player to hit the ball.
					 */
					void setTurn(const PlayerId &playerToHit);

					/**
					 * Handle a ball hit.
					 *
					 * @param hitter Player who hit the ball.
					 */
					void onBallHit(const PlayerId &hitter);

					/**
					 * Handle a ball bounce.
					 *
					 * @param courtOwner Owner of the court where the ball bounced.
					 * @param where Position where the ball bounced.
					 */
					void onBallBounce(const PlayerId &courtOwner, const BouncePlace &where); 

					/**
					 * Handle a continue event done by a player.
					 *
					 * @param author Player that made the forced continue.
					 */
					void onContinue(const PlayerId &author);

					/**
					 * Handle a pause event.
					 */
					void onGamePaused();

					/**
					 * Handle a resume event.
					 */
					void onGameResumed();
			};
		};
	};
};

#endif
