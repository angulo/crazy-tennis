/* 
 * SoundPlayer.h -- Header file for the sound player.
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

#ifndef _SOUND_PLAYER_H_
#define _SOUND_PLAYER_H_

#include <map>

#include <OGF/OGF.h>

namespace CrazyTennis {
	
	enum Sound {
		SOUND_AUDIENCE,
		SOUND_BALL_SERVE,
		SOUND_BALL_SHOT,
		SOUND_MENU_MAIN_TRACK,
		SOUND_MENU_SPLASH
	};
	
	class SoundPlayer : public Ogre::Singleton<SoundPlayer> {
		
		protected:
			
			std::map<Sound, OGF::SoundTrackPtr> _tracks;
			std::map<Sound, OGF::SoundChunkPtr> _chunks;
		
		public:
			
			SoundPlayer();
			~SoundPlayer();

			static SoundPlayer & getSingleton();
			static SoundPlayer * getSingletonPtr();

			/**
			 * Preload all the sounds.
			 */
			void preload();

			/**
			 * Play a sound.
			 *
			 * @param sound Sound id.
			 */
			void play(const Sound &sound, const bool &loop = false);

			/**
			 * Stop a sound.
			 *
			 * @param sound Sound id.
			 */
			void stop(const Sound &sound);

			/**
			 * Stop all the sounds being played.
			 */
			void stopAll();
	};
};

#endif
