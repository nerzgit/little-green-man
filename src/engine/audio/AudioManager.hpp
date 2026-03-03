#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP

#include <AL/al.h>
#include <AL/alc.h>

class AudioClip;

class AudioManager {
public:
	static void init();
	static void shutdown();

	static void play(const AudioClip& clip, float volume = 1.0f, bool loop = false);
	static void stopAll();

private:
	static constexpr int kMaxSources = 16;

	static ALCdevice*  device_;
	static ALCcontext* context_;
	static ALuint      sources_[kMaxSources];

	static ALuint findFreeSource();
};

#endif // AUDIO_MANAGER_HPP
