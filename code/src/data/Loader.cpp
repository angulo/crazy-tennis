/* 
 * Loader.cpp -- Data loader implementation file
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

#include "data/Loader.h"

using namespace CrazyTennis::Data;

Loader::Loader(const std::string &path)
	:	_path(path)
{

}

Loader::~Loader()
{

}

std::list<Player *>
Loader::loadPlayers(const std::string &fileName)
{
	std::list<Player *> result;
	std::string filePath = _path + "/" + fileName;

	Json::Value root;
	Json::Reader reader;
	std::ifstream inputStream(filePath.c_str(), std::ifstream::in);


	if (!reader.parse(inputStream, root)) {
		std::cout  << "Errors when parsing players file\n" << reader.getFormattedErrorMessages();
		throw "Abort!";
	}

	const Json::Value players = root["players"];

	for (Json::Value::iterator it = players.begin(); it != players.end(); it++) {
		SkillSet skillSet;
		VisualSet visualSet;

		const Json::Value player = *it;
		PlayerId id = player["id"].asUInt();
		std::string name = player["name"].asString();
		std::string image = player["image"].asString();
		
		const Json::Value skills = player["skills"];
		skillSet["speed"] = skills["speed"].asFloat();
		skillSet["precision"] = skills["precision"].asFloat();
		skillSet["power"] = skills["power"].asFloat();
		skillSet["serve"] = skills["serve"].asFloat();

		const Json::Value visual = player["visual"];
		visualSet["tshirt"] = visual["tshirt"].asString();
		visualSet["shorts"] = visual["tshirt"].asString();

		result.push_back(new Player(id, name, skillSet, visualSet, image));
	}


	return result;
}
