#include "Audio.h"
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <irrKlang.h>
#include <vector>
using namespace irrklang;



ISoundEngine* engineMusic;
ISoundEngine* engineEffects;
ISound* music;
ISound* effect;

Audio::Audio()
{
	init();
}


Audio::~Audio()
{
	music->drop();
	engineEffects->drop();
	engineMusic->drop();

}

void Audio::init() {
	engineMusic = createIrrKlangDevice();
	engineEffects = createIrrKlangDevice();
}

void Audio::playMusic(char* url)
{
	engineMusic->stopAllSounds();
	music = engineMusic->play2D(url, true, false, true);
	music->setVolume(0.1f);
}

void Audio::playMenu()
{
	engineMusic->stopAllSounds();
	music = engineMusic->play2D("sound/01. Title Screen.ogg", true, false, true);
	music->setVolume(0.1f);
}

void Audio::playLevel(int level) {
	switch (level)
	{
	case 1:
		music = engineMusic->play2D("sound/level01.ogg", true, false, true);
		break;
	case 2:
		music = engineMusic->play2D("sound/level02.ogg", true, false, true);
		break;
	default:
		break;
	}
	music->setVolume(0.1f);
}

void Audio::playWin()
{
	engineMusic->stopAllSounds();
	music = engineMusic->play2D("sound/Stage Win (Super Mario) - QuickSounds.com.ogg", false, false, true);
	music->setVolume(0.2f);
}

void Audio::playGameOver()
{
	engineMusic->stopAllSounds();
	music = engineMusic->play2D("sound/GameOver.wav", false, false, true);
	music->setVolume(0.2f);
}


void Audio::playStar() {
	engineMusic->stopAllSounds();
	music = engineMusic->play2D("sound/super-mario-bros-nes-music-star-theme-cut-mp3.ogg", false, false, true);
	music->setVolume(0.2f);
}



void Audio::pauseMusic() {
	engineMusic->setAllSoundsPaused();
}

void Audio::stopMusic() {
	engineMusic->stopAllSounds();
}

void Audio::squishEffect()
{
	effect = engineEffects->play2D("sound/Squish.wav", false, false, true);
	effect->setVolume(0.2f);
}

void Audio::kickEffect()
{
	effect = engineEffects->play2D("sound/Kick.wav", false, false, true);
	effect->setVolume(0.2f);
}

void Audio::warpEffect() {
	effect = engineEffects->play2D("sound/Warp.wav", false, false, true);
	effect->setVolume(0.2f);
}

void Audio::jumpEffect()
{
	effect = engineEffects->play2D("sound/Jump.wav", false, false, true);
	effect->setVolume(0.2f);
}

void Audio::itemEffect()
{
	effect = engineEffects->play2D("sound/Item.wav", false, false, true);
	effect->setVolume(0.15f);
}

void Audio::powerupEffect()
{
	effect = engineEffects->play2D("sound/Powerup.wav", false, false, true);
	effect->setVolume(0.2f);
}

void Audio::dieEffect()
{
	effect = engineEffects->play2D("sound/Die.wav", false, false, true);
	effect->setVolume(0.2f);
}

void Audio::coinEffect()
{
	effect = engineEffects->play2D("sound/Coin.wav", false, false, true);
	effect->setVolume(0.2f);
}

void Audio::flagpoleEffect()
{
	effect = engineEffects->play2D("sound/Flagpole.wav", false, false, true);
	effect->setVolume(0.2f);
}

void Audio::breakEffect()
{
	effect = engineEffects->play2D("sound/BreaK.wav", false, false, true);
	effect->setVolume(0.15f);
}