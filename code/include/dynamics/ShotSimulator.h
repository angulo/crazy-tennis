/* 
 * ShotSimulator.h -- Simulator of shots header file.
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

#ifndef _DYNAMICS_SHOT_SIMULATOR_H_
#define _DYNAMICS_SHOT_SIMULATOR_H_

#include <OGF/OGF.h>

#include <cmath>

namespace CrazyTennis {
	
	namespace Dynamics {
		
		typedef std::vector<std::pair<Ogre::Angle, Ogre::Real> > CalculationSet;

		class ShotSimulator {
			
			public:
			
				ShotSimulator();
				~ShotSimulator();

				void setOrigin(const Ogre::Vector3 &origin);
				void setDestination(const Ogre::Vector3 &origin);
				void setAngle(const Ogre::Angle &angle);
				void setVelocity(const Ogre::Real &velocity);

				Ogre::Angle calculateAngle();
				Ogre::Real calculateVelocity();
				CalculationSet calculateSet(const int &slices);
		};
	};
};

#endif
