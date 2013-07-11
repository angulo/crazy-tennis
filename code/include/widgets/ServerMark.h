/* 
 * ServerMark.h -- Server mark header file.
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

#ifndef _WIDGETS_SERVER_MARK_H_
#define _WIDGETS_SERVER_MARK_H_

#include <OGF/OGF.h>

#include "data/Match.h"
#include "data/PointState.h"
#include "data/PointStateListener.h"
#include "data/Types.h"
#include "InputAdapter.h"
#include "Model.h"

namespace CrazyTennis {

	namespace Widget {
		
		class ServerMark : public OGF::Widget, public Data::PointState::Listener {
			
			protected:

				Ogre::SceneNode *_node;
				Data::PlayerId _server;
				Data::Match *_matchData;

				bool _shouldMove;

				void _reset();

			public:
				
				ServerMark(Ogre::SceneManager *sceneManager, Data::Match *matchData, const Data::PlayerId &server);
				~ServerMark();

				void enter();
				void exit();

				bool frameStarted(const Ogre::FrameEvent &event);

				Ogre::Vector3 getPosition() const;
				void onChangeState(const Data::PointState::State &previousState, const Data::PointState::State &currentState);
		};
	};

};

#endif
