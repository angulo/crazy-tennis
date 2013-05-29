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
#include <iostream>

using namespace CrazyTennis::Data;

Loader::Loader(const std::string &path)
	:	_path(path)
{
	// Set the global C and C++ locale to the user-configured locale,
  // so we can use std::cout with UTF-8, via Glib::ustring, without exceptions.
	std::locale::global(std::locale(""));
}

Loader::~Loader()
{

}

std::list<Player *>
Loader::loadPlayers(const std::string &fileName)
{
	std::list<Player *> result;
	std::string filePath = _path + "/" + fileName;
	xmlpp::TextReader reader(filePath);

	reader.read();
	reader.move_to_element();

	while(reader.read()) {

		int id;
		std::string name;

		if (reader.has_attributes()) {
			reader.move_to_first_attribute();

			do {
				std::string attrName = reader.get_name();
				std::string attrValue = reader.get_value();

				if (attrName == "id") {
					id = atoi(attrValue.c_str());
				} else if (attrName == "name") {
					name = attrValue;
				}

			} while (reader.move_to_next_attribute());

			result.push_back(new Player(id, name));
			std::cout << id << " " << name << std::endl;
			reader.move_to_element();
		}
	}

	return result;
}
