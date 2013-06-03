/* 
 * InputAdapter.cpp -- Input adapter implementation
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

#include "InputAdapter.h"

using namespace CrazyTennis;
	
template<> InputAdapter * Ogre::Singleton<InputAdapter>::msSingleton = 0;

void
InputAdapter::initializeKeyMap(const std::string &configFilePath)
{
		
}

InputAdapter::InputAdapter()
{

}

InputAdapter::~InputAdapter()
{

}

InputAdapter&
InputAdapter::getSingleton()
{
	if (!msSingleton) {
		InputAdapter *instance = new InputAdapter();
	}

	return *msSingleton;
}


InputAdapter *
InputAdapter::getSingletonPtr()
{
	if (!msSingleton) {
		InputAdapter *instance = new InputAdapter();
	}

	return msSingleton;
}


Controls::Action
InputAdapter::inputToAction(const OIS::KeyEvent &inputEvent)
{

}

void
InputAdapter::store(const OIS::KeyEvent &inputEvent, Controls::Action action)
{
	
}
