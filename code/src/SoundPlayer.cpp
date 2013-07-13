/* 
 * SoundPlayer.cpp -- Sound player implementation.
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

#include "SoundPlayer.h"

using namespace CrazyTennis;

template<> SoundPlayer * Ogre::Singleton<SoundPlayer>::msSingleton = 0;

SoundPlayer::SoundPlayer()
{
}

SoundPlayer::~SoundPlayer()
{
}

SoundPlayer&
SoundPlayer::getSingleton()
{
	if (!msSingleton) {
		SoundPlayer *instance = new SoundPlayer();
	}

	return *msSingleton;
}

SoundPlayer *
SoundPlayer::getSingletonPtr()
{
	if (!msSingleton) {
		SoundPlayer *instance = new SoundPlayer();
	}

	return msSingleton;
}

void
SoundPlayer::preload()
{
	_tracks[SOUND_AUDIENCE] = OGF::SoundTrackManager::getSingletonPtr()->load("audience.mp3");
	_chunks[SOUND_BALL_SERVE] = OGF::SoundChunkManager::getSingletonPtr()->load("ball_serve.wav");
	_chunks[SOUND_BALL_SHOT] = OGF::SoundChunkManager::getSingletonPtr()->load("ball_shot.wav");
	_chunks[SOUND_MENU_BACK] = OGF::SoundChunkManager::getSingletonPtr()->load("menu_back.wav");
	_tracks[SOUND_MENU_MAIN_TRACK] = OGF::SoundTrackManager::getSingletonPtr()->load("menu_main_track.mp3");
	_tracks[SOUND_MENU_SPLASH] = OGF::SoundTrackManager::getSingletonPtr()->load("menu_splash.mp3");
	_chunks[SOUND_MENU_SELECT] = OGF::SoundChunkManager::getSingletonPtr()->load("menu_select.wav");
	_chunks[SOUND_MENU_SLIDE] = OGF::SoundChunkManager::getSingletonPtr()->load("menu_slide.wav");
}

void
SoundPlayer::play(const Sound &sound, const bool &loop)
{
	switch(sound) {
		case SOUND_BALL_SERVE:
		case SOUND_BALL_SHOT:
		case SOUND_MENU_BACK:
		case SOUND_MENU_SLIDE:
		case SOUND_MENU_SELECT:
			_chunks[sound]->play();
			break;

		case SOUND_AUDIENCE:
		case SOUND_MENU_MAIN_TRACK:
		case SOUND_MENU_SPLASH:
			_tracks[sound]->play(loop);
			break;
	}
}

void
SoundPlayer::stop(const Sound &sound)
{
	switch(sound) {
		case SOUND_BALL_SERVE:
		case SOUND_BALL_SHOT:
		case SOUND_MENU_BACK:
		case SOUND_MENU_SLIDE:
		case SOUND_MENU_SELECT:
			break;

		case SOUND_AUDIENCE:
		case SOUND_MENU_MAIN_TRACK:
		case SOUND_MENU_SPLASH:
			_tracks[sound]->stop();
			break;
	}
}

void
SoundPlayer::stopAll()
{
	for (std::map<Sound, OGF::SoundTrackPtr>::iterator it = _tracks.begin(); it != _tracks.end(); it++) {
		it->second->stop();
	}
}
