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
	_background->setPosition(CEGUI::UVector2(CEGUI::UDim(0.02, 0), CEGUI::UDim(0.85, 0)));
	_container->addChildWindow(_background);
}

void
Score::_createGames()
{
	std::string fontNumbers = _configValue<std::string>("font_numbers");
	_games.push_back(_createText("1", 100, 13, fontNumbers));	
	_games.push_back(_createText("1", 100, 62, fontNumbers));	

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

	_points.push_back(_createText("15", pointsPosition, 13, fontNumbers));	
	_points.push_back(_createText("A", pointsPosition, 62, fontNumbers));	

	_background->addChildWindow(_points[0]);
	_background->addChildWindow(_points[1]);
}

void
Score::_createServerMark()
{
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

Score::Score(Data::Match *matchData)
	:	_matchData(matchData)
{
	_initConfigReader("widgets/score.cfg");
	CEGUI::FontManager::getSingleton().create(_configValue<std::string>("font_names") + ".font");
	CEGUI::FontManager::getSingleton().create(_configValue<std::string>("font_numbers") + ".font");
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

}

void
Score::onMatchEvent(Data::MatchStatus matchStatus)
{

}
