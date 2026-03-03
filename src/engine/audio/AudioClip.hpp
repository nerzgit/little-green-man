#ifndef AUDIO_CLIP_HPP
#define AUDIO_CLIP_HPP

#include <AL/al.h>
#include <string>

class AudioClip {
public:
	AudioClip(const std::string& path);
	~AudioClip();

	AudioClip(const AudioClip&)            = delete;
	AudioClip& operator=(const AudioClip&) = delete;

	ALuint getBuffer() const { return buffer_; }

private:
	ALuint buffer_;

	static bool loadWav(const std::string& path, ALuint buffer);
};

#endif // AUDIO_CLIP_HPP
