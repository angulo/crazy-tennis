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
#include <map>

namespace CrazyTennis {
	
	namespace Dynamics {
		
		typedef std::vector<std::pair<Ogre::Real, Ogre::Real> > CalculationSet;

		class ShotSimulator {
			
			/**
			 * Shot simulation parameters keys.
			 */ 
			enum ParameterKey {
				ORIGIN = 0,
				DESTINATION = 1,
				ANGLE = 2,
				VELOCITY = 3
			};

			/**
			 * Shot simulation parameters values.
			 */
			struct ParameterValue {
				Ogre::Vector3 point;
				Ogre::Real number;
			};

			protected:
				
				std::map<ParameterKey, ParameterValue> _parameters;
			
			public:
			
				ShotSimulator();
				~ShotSimulator();

				ShotSimulator * setOrigin(const Ogre::Vector3 &origin);
				ShotSimulator * setDestination(const Ogre::Vector3 &destination);
				ShotSimulator * setAngle(const Ogre::Real &angle);
				ShotSimulator * setVelocity(const Ogre::Real &velocity);

				CalculationSet calculateSet(const int &slices);
		};
	};
};

#endif
