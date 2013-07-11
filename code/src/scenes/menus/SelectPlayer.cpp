/* 
 * SelectPlayer.cpp -- Implementation for the menu to select player.
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

#include "scenes/menus/SelectPlayer.h"

using namespace CrazyTennis::Scene::Menu;

CEGUI::Window *
SelectPlayer::_createPlayerItem(const Data::Player *player)
{
	CEGUI::Window *container = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow");
	container->setSize(CEGUI::UVector2(CEGUI::UDim(0, 400), CEGUI::UDim(0, 400)));

	CEGUI::Window *image = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	image->setProperty("Image", "set:Players image:" + player->getImage());
	image->setProperty("FrameEnabled", "False");
	image->setProperty("BackgroundEnabled", "False");
	image->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0, 0)));
	image->setProperty("HorzFormatting", "HorzCentred");
	image->setProperty("VertFormatting", "HorzCentred");

	CEGUI::Window *text = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
	text->setProperty("Text", CEGUI::String("[colour='") + _configValue<std::string>("font_color_player") + "']" + player->getName());
	text->setProperty("Font", _configValue<std::string>("font_player"));
	text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0, 0)));

	container->addChildWindow(image);
	container->addChildWindow(text);

	return container;
}

CEGUI::Window *
SelectPlayer::_createFrame(const int &number)
{
	CEGUI::Window *container = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow");
	container->setSize(CEGUI::UVector2(CEGUI::UDim(0, 400), CEGUI::UDim(0, 400)));

	CEGUI::Window *frame = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	frame->setSize(CEGUI::UVector2(CEGUI::UDim(0, 400), CEGUI::UDim(0, 400)));
	frame->setProperty("Image", "set:Players image:Frame" + Ogre::StringConverter::toString(number));
	frame->setProperty("FrameEnabled", "False");
	frame->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0, 0)));
	frame->setProperty("BackgroundEnabled", "False");
	frame->setProperty("HorzFormatting", "HorzCentred");
	frame->setProperty("VertFormatting", "HorzCentred");

	container->addChildWindow(frame);

	return container;
}

void
SelectPlayer::_onActionDone(const Controls::Action &action)
{
	switch(action) {

		case Controls::LEFT:
			_moveToPreviousOption();
			break;
		case Controls::RIGHT:
			_moveToNextOption();
			break;
		case Controls::SHOT_DRIVE:
			_processCurrentOption();
			break;
		case Controls::START:
		case Controls::SHOT_LOB:
			_goBack();
			break;
	}
}

void
SelectPlayer::_moveToNextOption()
{
	int currentPlayerId = _currentPlayer->getId();
	int nextPlayerId = currentPlayerId + 1 > _optionsMap.size() ? 1 : currentPlayerId + 1;
	_currentPlayer = _playersData[nextPlayerId];

	if (_selectedPlayers.size() == 1 && _selectedPlayers.front()->getId() == nextPlayerId) {
		_moveToNextOption();
	}

	_renderFrame();
}

void
SelectPlayer::_moveToPreviousOption()
{
	int currentPlayerId = _currentPlayer->getId();
	int nextPlayerId = currentPlayerId - 1 == 0 ? _optionsMap.size() : currentPlayerId - 1;
	_currentPlayer = _playersData[nextPlayerId];

	if (_selectedPlayers.size() == 1 && _selectedPlayers.front()->getId() == nextPlayerId) {
		_moveToPreviousOption();
	}

	_renderFrame();
}

void
SelectPlayer::_processCurrentOption()
{
	switch(_selectedPlayers.size()) {
		case 0:
			_selectedPlayers.push_back(_currentPlayer);
			_player2Frame->setVisible(true);
			_moveToNextOption();
			break;
		case 1:
			_selectedPlayers.push_back(_currentPlayer);
			_startGame();
			break;
	}
}

void
SelectPlayer::_goBack()
{
	if (_selectedPlayers.size() == 0) {
		OGF::SceneController::getSingletonPtr()->pop();
	} else {
		_selectedPlayers.pop_back();
		_player2Frame->setVisible(false);
	}
}

void
SelectPlayer::_renderFrame()
{
	CEGUI::Window *frame = _selectedPlayers.size() == 0 ? _player1Frame : _player2Frame;
	frame->setPosition(_optionsMap[_currentPlayer->getId()]->getPosition());
}

void
SelectPlayer::_startGame()
{
	Data::Match *matchData = new Data::Match(1, false, _selectedPlayers.front(), _selectedPlayers.back());
	OGF::SceneController::getSingletonPtr()->replace(new CrazyTennis::Scene::Match(matchData));
}

SelectPlayer::SelectPlayer()
{
	_initConfigReader("scenes/menus/selectPlayer.cfg");
	CEGUI::FontManager::getSingleton().create(_configValue<std::string>("font_player") + ".font");
	CEGUI::FontManager::getSingleton().create(_configValue<std::string>("font_title") + ".font");
}

SelectPlayer::~SelectPlayer()
{

}

void
SelectPlayer::enter()
{
	_container = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "SelectPlayerContainer");

	_windowBackground = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	_windowBackground->setProperty("Image", "set:Menus image:SelectPlayer");
	_windowBackground->setProperty("FrameEnabled", "False");
	_windowBackground->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	_container->addChildWindow(_windowBackground);


	Data::Loader *dataLoader = new Data::Loader("../data/game");
	std::list<Data::Player *> players = dataLoader->loadPlayers("players.json");

	int totalPlayers = players.size();
	int offset = 0;

	for (std::list<Data::Player *>::iterator it = players.begin(); 
		it != players.end(); it++, offset++) {

		_playersData[(*it)->getId()] = *it;

		CEGUI::Window *playerItem = _createPlayerItem(*it);
		_optionsMap[(*it)->getId()] = playerItem;
		
		float xRelPosition = (200 * (offset / 2)) - 50;
		float yRelPosition = offset % 2 ? 500 : 200;

		playerItem->setPosition(CEGUI::UVector2(CEGUI::UDim(0, xRelPosition), CEGUI::UDim(0, yRelPosition)));
		_playerCoordinates[*it] = Ogre::Vector2(xRelPosition, yRelPosition);

		_windowBackground->addChildWindow(playerItem);
	}

	// Create the frames
	_currentPlayer = *(players.begin());
	_player1Frame = _createFrame(1);
	_player2Frame = _createFrame(2);
	_windowBackground->addChildWindow(_player1Frame);
	_windowBackground->addChildWindow(_player2Frame);

	_player2Frame->setVisible(false);
	_renderFrame();

	// Create the title
	CEGUI::Window *title = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
	title->setProperty("Text", CEGUI::String("[colour='") + _configValue<std::string>("font_title_color") + "']" + "SELECT PLAYERS");
	title->setProperty("Font", _configValue<std::string>("font_title"));
	title->setProperty("FrameEnabled", "False");
	title->setProperty("HorzFormatting", "LeftAligned");
	title->setProperty("VertFormatting", "LeftAligned");
	title->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 50), CEGUI::UDim(0, 50)));
	_windowBackground->addChildWindow(title);

	CEGUI::System::getSingletonPtr()->setGUISheet(_container);
}
