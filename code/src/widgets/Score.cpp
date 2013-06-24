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

Score::Score(Data::Match *matchData)
	:	_matchData(matchData)
{
	_initConfigReader("widgets/score.cfg");
}

Score::~Score()
{

}

void
Score::enter()
{
	_container = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "ScoreContainer");

	int sets = _matchData->getStatus().matchScore.size();

	_pointsBackground = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	_pointsBackground->setProperty("Image", "set:Menus image:Score_" + Ogre::StringConverter::toString(sets));
	_pointsBackground->setProperty("FrameEnabled", "False");
	_pointsBackground->setProperty("InheritsAlpha", "False");
	_pointsBackground->setProperty("BackgroundEnabled", "False");
	_pointsBackground->setProperty("VertFormatting", "LeftAligned");
	_pointsBackground->setProperty("HorzFormatting", "LeftAligned");
	_pointsBackground->setPosition(CEGUI::UVector2(CEGUI::UDim(0.02, 0), CEGUI::UDim(0.85, 0)));
	_container->addChildWindow(_pointsBackground);

	CEGUI::System::getSingletonPtr()->setGUISheet(_container);
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
