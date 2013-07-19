/* 
 * Records.cpp -- Records menu implementation.
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

#include "scenes/menus/Records.h"

using namespace CrazyTennis::Scene::Menu;

CEGUI::Window *
Records::_createRecordEntry(const std::string name, const int &points, const int &number)
{
	CEGUI::Window *container = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow");

	std::ostringstream tempStream;
	std::string pointsString = Ogre::StringConverter::toString(points);

	CEGUI::Window *nameText = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
	nameText->setProperty("Text", "[colour='" + _configValue<std::string>("font_color_record") + "']" + name);
	nameText->setProperty("Font", _configValue<std::string>("font_record"));
	nameText->setProperty("UnifiedAreaRect", "{{0,0},{0,0},{1,0},{1,0}}");

	CEGUI::Window *pointsText = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
	pointsText->setProperty("Text", "[colour='" + _configValue<std::string>("font_color_record") + "']" + pointsString);
	pointsText->setProperty("Font", _configValue<std::string>("font_record"));
	pointsText->setProperty("UnifiedAreaRect", "{{0, 370},{0,0},{1,0},{1,0}}");

	container->addChildWindow(nameText);
	container->addChildWindow(pointsText);
	container->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 275), CEGUI::UDim(0, -300 + number * 60)));

	return container;
}

void
Records::_processCurrentOption()
{
	switch(_currentOption) {
		case OPTION_BACK:
			OGF::SceneController::getSingletonPtr()->pop();
			break;
	}
}

void
Records::_paintRecords()
{
	// Load the records
	Data::RecordsStorage *recordsStorage = new Data::RecordsStorage();
	Data::RecordsList records = recordsStorage->read();
	delete recordsStorage;

	int counter = 1;
	for (Data::RecordsList::iterator it = records.begin(); it != records.end(); it++, counter++) {
		_container->addChildWindow(_createRecordEntry(it->first, it->second, counter));
	}

}

Records::Records()
{
	_initConfigReader("scenes/menus/records.cfg");
	CEGUI::FontManager::getSingleton().create(_configValue<std::string>("font") + ".font");
	CEGUI::FontManager::getSingleton().create(_configValue<std::string>("font_record") + ".font");
}

Records::~Records()
{
}

void
Records::enter()
{
	_container = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "RecordsContainer");

	_windowBackground = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	_windowBackground->setProperty("Image", "set:Menus image:SelectPlayer");
	_windowBackground->setProperty("FrameEnabled", "False");
	_container->addChildWindow(_windowBackground);

	_optionsMap[OPTION_BACK] = _createOptionText("BACK", _configValue<std::string>("font"), _configValue<std::string>("font_color_selected"),
		_configValue<int>("back_x"), _configValue<int>("back_y"));

	for (std::map<int, CEGUI::Window *>::iterator it = _optionsMap.begin();
		it != _optionsMap.end(); it++) {

		_container->addChildWindow(it->second);
	}

	_paintRecords();

	CEGUI::System::getSingletonPtr()->setGUISheet(_container);
	_currentOption = OPTION_BACK;
	_setCurrentOption(OPTION_BACK);
}
