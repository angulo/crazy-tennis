/* 
 * SelectPlayer.h -- Header file for the menu to select player.
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

#ifndef _SCENE_MENU_SELECT_PLAYER_H_
#define _SCENE_MENU_SELECT_PLAYER_H_

#include <map>
#include <vector>

#include <OGF/OGF.h>

#include "data/Loader.h"
#include "data/Match.h"
#include "data/Player.h"
#include "scenes/menus/Base.h"
#include "InputAdapter.h"
#include "SceneFactory.h"
#include "SoundPlayer.h"

namespace CrazyTennis {
	
	namespace Scene {
		
		namespace Menu {

			class SelectPlayer : public Base {
				
				protected:

					std::map<Data::PlayerId, Data::Player *> _playersData;
					Data::Player *_currentPlayer;
					CEGUI::Window *_player1Frame;
					CEGUI::Window *_player2Frame;

					std::vector<Data::Player *> _selectedPlayers;
					std::map<Data::Player *, Ogre::Vector2> _playerCoordinates;

					CEGUI::Window * _createPlayerItem(const Data::Player *player);

					CEGUI::Window *_createFrame(const int &number);
					void _onActionDone(const Controls::Action &action);
					void _moveToNextOption();
					void _moveToPreviousOption();
					void _processCurrentOption();
					void _goBack();
					void _renderFrame();
					void _startGame();


				public:

					SelectPlayer();
					~SelectPlayer();

					void enter();
			};
		};
	};
};

#endif
