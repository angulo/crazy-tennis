/* 
 * Splash.cpp -- Splash screen menu scene
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

#include "scenes/Splash.h"

using namespace CrazyTennis::Scene;

CEGUI::Window *
Splash::_createTitleText(const std::string &text, const int &x, const int &y, const std::string &color)
{
	CEGUI::Window *result = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText");
	result->setProperty("Text", CEGUI::String("[colour='") + color + "']" + text);
	result->setProperty("Font", _configValue<std::string>("font_name"));
	result->setProperty("UnifiedAreaRect", "{{0," + Ogre::StringConverter::toString(x) 
		+ "},{0," + Ogre::StringConverter::toString(y) + "},{1,0},{1,0}}");
	result->setProperty("FrameEnabled", "False");

	return result;
}

void
Splash::_loadAnimations(CEGUI::Window *topText, CEGUI::Window *bottomText)
{
	CEGUI::AnimationManager::getSingletonPtr()->loadAnimationsFromXML("Menu.xml");

	CEGUI::AnimationInstance *animationTop = CEGUI::AnimationManager::getSingleton().instantiateAnimation("SplashTextTop");
	animationTop->setTargetWindow(topText);

	CEGUI::AnimationInstance *animationBottom = CEGUI::AnimationManager::getSingleton().instantiateAnimation("SplashTextBottom");
	animationBottom->setTargetWindow(bottomText);

	animationTop->start();
	animationBottom->start();
}

Splash::Splash()
{
	_initConfigReader("scenes/menus/splash.cfg");
	CEGUI::FontManager::getSingleton().create(_configValue<std::string>("font_name") + ".font");
}

Splash::~Splash()
{
}

void
Splash::enter()
{
	_container = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "SplashContainer");

	_windowBackground = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage");
	_windowBackground->setProperty("Image", "set:Menus image:Splash");
	_windowBackground->setProperty("FrameEnabled", "False");

	_container->addChildWindow(_windowBackground);

	CEGUI::Window *crazyText = _createTitleText("CRAZY", _configValue<int>("crazy_text_x"), _configValue<int>("crazy_text_y"),
		_configValue<std::string>("crazy_text_font_color"));
	CEGUI::Window *tennisText = _createTitleText("TENNIS", _configValue<int>("tennis_text_x"), _configValue<int>("tennis_text_y"),
		_configValue<std::string>("tennis_text_font_color"));

	_container->addChildWindow(crazyText);
	_container->addChildWindow(tennisText);

	CEGUI::System::getSingletonPtr()->setGUISheet(_container);

	_loadAnimations(crazyText, tennisText);
}

void
Splash::exit()
{
	CEGUI::WindowManager::getSingletonPtr()->destroyWindow(_container);
}

void
Splash::pause()
{
	_container->setVisible(false);
}

void
Splash::resume()
{
	CEGUI::System::getSingletonPtr()->setGUISheet(_container);
	_container->setVisible(true);
}

bool
Splash::frameStarted(const Ogre::FrameEvent& event)
{

}

bool
Splash::keyPressed(const OIS::KeyEvent &event)
{
	switch(event.key) {

		case OIS::KC_RETURN:
			break;

		default:
			break;
	}
	
	return true;
}
