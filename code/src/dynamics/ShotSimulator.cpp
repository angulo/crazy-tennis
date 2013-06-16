/* 
 * ShotSimulator.cpp -- Simulator of shots implementation file
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

#include "dynamics/ShotSimulator.h"

using namespace CrazyTennis::Dynamics;

ShotSimulator::ShotSimulator()
{
}

ShotSimulator::~ShotSimulator()
{
}

ShotSimulator *
ShotSimulator::setOrigin(const Ogre::Vector3 &origin)
{
	ParameterValue value;
	value.point = origin;

	_parameters[ORIGIN] = value;	
	return this;
}

ShotSimulator *
ShotSimulator::setDestination(const Ogre::Vector3 &destination)
{
	ParameterValue value;
	value.point = destination;

	_parameters[DESTINATION] = value;
	return this;
}

ShotSimulator *
ShotSimulator::setAngle(const Ogre::Real &angle)
{
	ParameterValue value;
	value.number = angle;

	_parameters[ANGLE] = value;
	return this;
}

ShotSimulator *
ShotSimulator::setVelocity(const Ogre::Real &velocity)
{
	ParameterValue value;
	value.number = velocity;

	_parameters[VELOCITY] = value;
	return this;
}

CalculationSet
ShotSimulator::calculateSet(const int &slices)
{
	CalculationSet result;

	Ogre::Vector3 origin = _parameters[ORIGIN].point;
	Ogre::Vector3 destination = _parameters[DESTINATION].point;
	Ogre::Vector3 pointingVector = destination - origin;

	Ogre::Vector2 shotVector(sqrt(pow(pointingVector.length(), 2) - pow(pointingVector.y, 2)),
		pointingVector.y);

	// Two more slices to discard -90º and 90º
	Ogre::Real sliceAngle = 180 / static_cast<Ogre::Real>(slices + 1);

	for (int i = 1; i < slices + 1; i++) {
		Ogre::Real angle = Ogre::Degree((i * sliceAngle) - 90.0).valueRadians();

		Ogre::Real velocity =
			sqrt(
				(9.8 * pow(shotVector.x, 2))
					/
				(2.0 * pow(cos(angle), 2) * ((shotVector.x * tan(angle)) - shotVector.y))
			);

		result.push_back(std::make_pair(angle, velocity));
	}

	return result;
}
