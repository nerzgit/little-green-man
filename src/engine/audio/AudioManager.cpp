#include "AudioManager.hpp"

#include "AudioClip.hpp"

#include <stdexcept>

ALCdevice*  AudioManager::device_              = nullptr;
ALCcontext* AudioManager::context_             = nullptr;
ALuint      AudioManager::sources_[kMaxSources] = {};

void AudioManager::init() {
	device_ = alcOpenDevice(nullptr);
	if (!device_) {
		throw std::runtime_error("AudioManager: failed to open audio device");
	}

	context_ = alcCreateContext(device_, nullptr);
	if (!context_ || !alcMakeContextCurrent(context_)) {
		throw std::runtime_error("AudioManager: failed to create audio context");
	}

	alGenSources(kMaxSources, sources_);
}

void AudioManager::shutdown() {
	alDeleteSources(kMaxSources, sources_);
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context_);
	alcCloseDevice(device_);
	context_ = nullptr;
	device_  = nullptr;
}

void AudioManager::play(const AudioClip& clip, float volume, bool loop) {
	ALuint source = findFreeSource();
	if (source == 0) {
		return; // 空きソースなし
	}

	alSourcei(source, AL_BUFFER, static_cast<ALint>(clip.getBuffer()));
	alSourcef(source, AL_GAIN, volume);
	alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	alSourcePlay(source);
}

void AudioManager::stopAll() {
	alSourceStopv(kMaxSources, sources_);
}

ALuint AudioManager::findFreeSource() {
	for (int i = 0; i < kMaxSources; ++i) {
		ALint state;
		alGetSourcei(sources_[i], AL_SOURCE_STATE, &state);
		if (state != AL_PLAYING) {
			return sources_[i];
		}
	}
	return 0;
}
