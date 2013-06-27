/* 
 * Model.cpp -- Models enumeration and id-path implementation
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

#include "Model.h"

using namespace CrazyTennis;

OGF::ModelMap
Model::getModelMap()
{
	OGF::ModelMap modelMap;

	modelMap[BALL] = "ball.mesh";
	modelMap[BARRIER] = "barrier.mesh";
	modelMap[CHAIR] = "chair.mesh";
	modelMap[COURT_IN] = "court.mesh";
	modelMap[COURT_OUT] = "court_out.mesh";
	modelMap[LINES] = "lines.mesh";
	modelMap[NET] = "net.mesh";
	modelMap[PLAYER] = "shoter.mesh";
	modelMap[REFEREE] = "referee.mesh";
	modelMap[SEATS] = "seats.mesh";
	modelMap[SKY] = "sky.mesh";
	modelMap[STADIUM] = "stadium.mesh";

	return modelMap;
}
