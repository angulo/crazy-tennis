/* 
 * Player.cpp -- Player data implementation file
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

#include "data/Player.h"

using namespace CrazyTennis::Data;

Player::Player(const PlayerId &id, const std::string &name, const SkillSet &skills, const VisualSet &visual, const std::string &image)
	:	_id(id), _name(name), _skills(skills), _visual(visual), _image(image)
{
}

Player::~Player()
{

}

std::string
Player::getName() const
{
	return _name;
}

PlayerId
Player::getId() const
{
	return _id;
}

SkillSet
Player::getSkills() const
{
	return _skills;
}

VisualSet
Player::getVisual() const
{
	return _visual;
}

void
Player::setSkills(SkillSet skills)
{
	_skills = skills;
}

std::string
Player::getImage() const
{
	return _image;
}
