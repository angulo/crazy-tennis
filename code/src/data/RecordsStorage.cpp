/* 
 * RecordsStorage.cpp -- Records data storage implementation.
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

#include "data/RecordsStorage.h"

using namespace CrazyTennis::Data;

void
RecordsStorage::_save(RecordsList records)
{
	_storedFile.close();
	_storedFile.open(RECORDS_FILE, std::fstream::out | std::fstream::trunc);

	RecordsList::iterator it;

	for (it = records.begin(); it != records.end(); it++) {
		_storedFile << it->first << " " << it->second << std::endl;
	}
	
	_storedFile.close();
}

RecordsStorage::RecordsStorage()
{
	_storedFile.open(RECORDS_FILE, std::fstream::in);
	if (!_storedFile.is_open())
		std::cerr << "[ERROR] Records file unaccessible" << std::endl;
}

RecordsStorage::~RecordsStorage()
{
	if (_storedFile.is_open())
		_storedFile.close();
}

RecordsList
RecordsStorage::read()
{
	std::string line;
	std::string name;
	long int points;
	RecordsList result;
	int count = 0;

	if (_storedFile.is_open()) {
		while (std::getline(_storedFile, line)) {
			std::istringstream iss(line);
			iss >> name;
			iss >> points;
			result.push_back(make_pair(name, points));
			count++;
		}
	}

	return result;
}

void
RecordsStorage::addGamePoint(const std::string& name)
{
	RecordsList records = read();
	
	for (RecordsList::iterator it = records.begin(); it != records.end(); it++) {
		std::cout << it->first << " " << it->second << std::endl;
		if (it->first == name) {
			it->second++;
		}
	}
	
	_save(records);
}
