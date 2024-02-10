#ifndef _AUDIO_INCLUDE
#define _AUDIO_INCLUDE

#include <string>

static class Audio
{
public:
	static Audio& instance()
	{
		static Audio A;

		return A;
	}
	Audio();
	~Audio();
	void playMusic(char* url);
	void playMenu();
	void playLevel(int l) ;
	void playWin();
	void playGameOver();
	void playStar();

	void resumeMusic();
	void pauseMusic();
	void stopMusic();

	void squishEffect();
	void kickEffect();
	void jumpEffect();
	void itemEffect();
	void powerupEffect();
	void dieEffect();
	void coinEffect();
	void flagpoleEffect();
	void breakEffect();
	void warpEffect();

private:
	void init();
};

#endif // _AUDIO_INCLUDE