/* 
 * PlayerCpu.cpp -- CPU player implementation file
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

#include "widgets/PlayerCpu.h"

using namespace CrazyTennis::Widget;

PlayerCpu::PlayerCpu(Ogre::SceneManager *sceneManager, OgreBulletDynamics::DynamicsWorld *dynamicWorld, Widget::Ball *ball, Data::Player *data)
	:	PlayerBase(sceneManager, dynamicWorld, ball, data)
{

}

PlayerCpu::~PlayerCpu()
{

}

void
PlayerCpu::enter()
{
	PlayerBase::enter();
	
	_shotBuffer = new Widget::ShotBuffer();
	_shotBufferId = OGF::SceneController::getSingletonPtr()->addChild(_shotBuffer);
}

void
PlayerCpu::exit()
{

}

bool
PlayerCpu::frameStarted(const Ogre::FrameEvent &event)
{
	return true;
}

bool
PlayerCpu::keyPressed(const OIS::KeyEvent &event)
{
	return true;
}
