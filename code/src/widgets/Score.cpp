/* 
 * Score.cpp -- Score UI widget implementation file
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

#include "widgets/Score.h"

using namespace CrazyTennis::Widget;

void
Score::_createBackground()
{
	int sets = _matchData->getStatus().matchScore.size();

	_background = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	_background->setProperty("Image", "set:Menus image:Score_" + Ogre::StringConverter::toString(sets));
	_background->setProperty("FrameEnabled", "False");
	_background->setProperty("InheritsAlpha", "False");
	_background->setProperty("BackgroundEnabled", "False");
	_background->setProperty("VertFormatting", "LeftAligned");
	_background->setProperty("HorzFormatting", "LeftAligned");
	_background->setPosition(CEGUI::UVector2(CEGUI::UDim(0.04, 0), CEGUI::UDim(0.85, 0)));
	_container->addChildWindow(_background);
}

void
Score::_createGames()
{
	std::string fontNumbers = _configValue<std::string>("font_numbers");
	_games.push_back(_createText("0", 100, 13, fontNumbers));	
	_games.push_back(_createText("0", 100, 62, fontNumbers));	

	_background->addChildWindow(_games[0]);
	_background->addChildWindow(_games[1]);
}

void
Score::_createNames()
{
	std::string name1 = "HUM";
	std::string name2 = "CPU";

	std::string fontNames = _configValue<std::string>("font_names");
	_names.push_back(_createText(name1, 20, 13, fontNames));
	_names.push_back(_createText(name2, 20, 62, fontNames));

	_background->addChildWindow(_names[0]);
	_background->addChildWindow(_names[1]);
}

void
Score::_createPoints()
{
	std::string fontNumbers = _configValue<std::string>("font_numbers");
	int sets = _matchData->getStatus().matchScore.size();
	int pointsPosition = _configValue<int>("points_position_" + Ogre::StringConverter::toString(sets));

	_points.push_back(_createText("0", pointsPosition, 13, fontNumbers));	
	_points.push_back(_createText("0", pointsPosition, 62, fontNumbers));	

	_background->addChildWindow(_points[0]);
	_background->addChildWindow(_points[1]);
}

void
Score::_createServerMark()
{
	_serverMark = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	_serverMark->setProperty("Image", "set:Menus image:Ball");
	_serverMark->setProperty("FrameEnabled", "False");
	_serverMark->setProperty("InheritsAlpha", "False");
	_serverMark->setProperty("BackgroundEnabled", "False");
	_serverMark->setProperty("VertFormatting", "LeftAligned");
	_serverMark->setProperty("HorzFormatting", "LeftAligned");

	if (_matchData->getCurrentServer() == _matchData->getPlayer(0)) {
		_serverMark->setPosition(CEGUI::UVector2(CEGUI::UDim(0.04, -16), CEGUI::UDim(0.85, 6)));
	} else {
		_serverMark->setPosition(CEGUI::UVector2(CEGUI::UDim(0.04, -16), CEGUI::UDim(0.85, 54)));
	}

	_container->addChildWindow(_serverMark);
}

void
Score::_createPointWinnerMessage()
{
	_pointWinnerMessage = _createText(std::string(""), 300, 650, _configValue<std::string>("font_point_winner"));
	_pointWinnerMessage->setProperty("VertFormatting", "LeftAligned");
	_pointWinnerMessage->setProperty("HorzFormatting", "LeftAligned");
	_pointWinnerMessage->setVisible(false);
	_container->addChildWindow(_pointWinnerMessage);
}

CEGUI::Window *
Score::_createText(const std::string &text, const float &xRel, const float &yRel, const std::string &font)
{
	CEGUI::Window *result = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
	result->setProperty("Text", CEGUI::String("[colour='") + _configValue<std::string>("font_color") + "']" + text);
	result->setProperty("Font", font);
	result->setPosition(CEGUI::UVector2(CEGUI::UDim(0, xRel), CEGUI::UDim(0, yRel)));
	result->setProperty("FrameEnabled", "False");
	result->setProperty("VertFormatting", "LeftAligned");
	result->setProperty("HorzFormatting", "LeftAligned");

	return result;
}

void
Score::_refresh(const Data::MatchStatus &matchStatus)
{
	for (int i = 0, count = 0; i < matchStatus.matchScore.size(); i++, count += 2) {
		_refreshGameNumber(_games[count], matchStatus.matchScore[i][0]);
		_refreshGameNumber(_games[count + 1], matchStatus.matchScore[i][1]);
	}

	_refreshGamePoints(_points[0], _points[1], matchStatus.gameScore[0], matchStatus.gameScore[1]);

	if (_matchData->getCurrentServer() == _matchData->getPlayer(0)) {
		_serverMark->setPosition(CEGUI::UVector2(CEGUI::UDim(0.04, -16), CEGUI::UDim(0.85, 6)));
	} else {
		_serverMark->setPosition(CEGUI::UVector2(CEGUI::UDim(0.04, -16), CEGUI::UDim(0.85, 54)));
	}
}

void
Score::_refreshGameNumber(CEGUI::Window *item, const int &number)
{
	item->setText(CEGUI::String("[colour='") + _configValue<std::string>("font_color") + "']" + Ogre::StringConverter::toString(number));
}

void
Score::_refreshGamePoints(CEGUI::Window *item0, CEGUI::Window *item1, const int &number0, const int &number1)
{
	std::string text0, text1;
	
	if (number0 >= 3 && number1 >= 3) {
		text0 = text1 = "40";
		if (number0 > number1) {
			text0 = "A";
		} else if (number0 < number1) {
			text1 = "A";
		}
	} else {
		switch(number0) {
			case 0: text0 = "0"; break;
			case 1: text0 = "15"; break;
			case 2: text0 = "30"; break;
			case 3: text0 = "40"; break;
		}

		switch(number1) {
			case 0: text1 = "0"; break;
			case 1: text1 = "15"; break;
			case 2: text1 = "30"; break;
			case 3: text1 = "40"; break;
		}
	}

	item0->setText(CEGUI::String("[colour='") + _configValue<std::string>("font_color") + "']" + text0);
	item1->setText(CEGUI::String("[colour='") + _configValue<std::string>("font_color") + "']" + text1);
}

Score::Score(Data::Match *matchData)
	:	_matchData(matchData), _pointWinnerTimer(NULL)
{
	_initConfigReader("widgets/score.cfg");
	CEGUI::FontManager::getSingleton().create(_configValue<std::string>("font_names") + ".font");
	CEGUI::FontManager::getSingleton().create(_configValue<std::string>("font_numbers") + ".font");
	CEGUI::FontManager::getSingleton().create(_configValue<std::string>("font_point_winner") + ".font");
}

Score::~Score()
{

}

void
Score::enter()
{
	_container = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "ScoreContainer");
	CEGUI::System::getSingletonPtr()->setGUISheet(_container);

	_createBackground();
	_createGames();
	_createNames();
	_createPoints();
	_createServerMark();
	_createPointWinnerMessage();
}

void
Score::exit()
{
	CEGUI::WindowManager::getSingletonPtr()->destroyWindow(_container);
}

void
Score::pause()
{
	_container->setVisible(false);
}

void
Score::resume()
{
	CEGUI::System::getSingletonPtr()->setGUISheet(_container);
	_container->setVisible(true);
}

bool
Score::frameStarted(const Ogre::FrameEvent &event)
{
	if (_pointWinnerTimer) {
		Ogre::Real time = _pointWinnerTimer->getMilliseconds();
		if (time >= _configValue<float>("timeBeforeShowingPointWinnerMilliseconds") &&
			time <= _configValue<float>("timeToDisappearPointWinnerMilliseconds")) {
			
			if (!_pointWinnerMessage->isVisible()) {
				_pointWinnerMessage->setVisible(true);
			}
		} else if (_pointWinnerMessage->isVisible()) {
			_pointWinnerMessage->setVisible(false);
			delete _pointWinnerTimer;
			_pointWinnerTimer = NULL;
		}
	}
}

void
Score::onMatchEvent(Data::MatchStatus matchStatus)
{
	_refresh(matchStatus);
}

void
Score::onWonPoint(const Data::PlayerId &winner)
{
	if (_matchData->getStatus().firstServe) {
		Data::Player *winnerPlayer = _matchData->getPlayer(0)->getId() == winner ?
			_matchData->getPlayer(0) : _matchData->getPlayer(1);
		
		std::string condition =_matchData->getPlayer(0)->getId() == winner ?
			"HUM" : "CPU";
		
		_pointWinnerMessage->setText("Point for " + winnerPlayer->getName() + " (" + condition + ")");
		_pointWinnerMessage->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 150), CEGUI::UDim(0, 350)));
	} else {
		_pointWinnerMessage->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 450), CEGUI::UDim(0, 350)));
		_pointWinnerMessage->setText("Fault!");
	}
		
	_pointWinnerTimer = new Ogre::Timer();
	_pointWinnerTimer->reset();
}
