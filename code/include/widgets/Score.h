/* 
 * Score.h -- Score UI widget header file.
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

#ifndef _WIDGETS_SCORE_H_
#define _WIDGETS_SCORE_H_

#include <OGF/OGF.h>

#include "data/Match.h"
#include "data/MatchListener.h"
#include "data/Types.h"

namespace CrazyTennis {
	
	namespace Widget {
		
		class Score : public OGF::Scene, public Data::MatchListener {
			
			protected:

				Data::Match *_matchData;

				CEGUI::Window *_container;
				CEGUI::Window *_background;
				CEGUI::Window *_serverMark;

				std::vector<CEGUI::Window *> _names;;
				std::vector<CEGUI::Window *> _games;
				std::vector<CEGUI::Window *> _points;

				void _createBackground();
				void _createGames();
				void _createNames();
				void _createPoints();
				void _createServerMark();
				CEGUI::Window * _createText(const std::string &text, const float &xRel, const float &yRel, const std::string &font);

				void _refresh(const Data::MatchStatus &matchStatus);

				void _refreshGameNumber(CEGUI::Window *item, const int &number);
				void _refreshGamePoints(CEGUI::Window *item0, CEGUI::Window *item1, const int &number0, const int &number1);

			public:
				
				Score(Data::Match *matchData);
				~Score();

				void enter();
				void exit();
				void pause();
				void resume();


				bool frameStarted(const Ogre::FrameEvent &event);
				void onMatchEvent(Data::MatchStatus matchStatus);
		};
	};
};

#endif
