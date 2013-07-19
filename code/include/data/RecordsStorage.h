/* 
 * RecordsStorage.h -- Records data storage.
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


#ifndef _RECORDS_STORAGE_H
#define _RECORDS_STORAGE_H

#define RECORDS_FILE "../data/records.dat"

#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace CrazyTennis {
	
	namespace Data {

		typedef std::list<std::pair<std::string, long int> > RecordsList;
		
		class RecordsStorage {

			private:
				
				std::fstream _storedFile;
				void _save(RecordsList records);
			
			public:

				RecordsStorage();
				~RecordsStorage();

				RecordsList read();
				void addGamePoint(const std::string& name);
		};
	};
};

#endif
