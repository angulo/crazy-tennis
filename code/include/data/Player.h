/* 
 * Player.h -- Player data header file
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

#ifndef _DATA_PLAYER_H_
#define _DATA_PLAYER_H_

#include <string>

#include "data/Types.h"

namespace CrazyTennis {
	
	namespace Data {

		class Player {
			
			protected:
				
				PlayerId _id;
				std::string _name;
				SkillSet _skills;
				VisualSet _visual;
				std::string _image;
			
			public:
			
				Player(const PlayerId &id, const std::string &name, const SkillSet &skills, const VisualSet &visual, const std::string &image);
				~Player();

				std::string getName() const;
				PlayerId getId() const;
				SkillSet getSkills() const;
				VisualSet getVisual() const;
				std::string getImage() const;

				void setSkills(SkillSet skills);
		};

	};

};

#endif
