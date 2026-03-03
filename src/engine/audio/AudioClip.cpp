#include "AudioClip.hpp"

#include <AL/al.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

// 標準的なPCM WAVファイルを読み込むためのRIFFヘッダ構造
#pragma pack(push, 1)
struct WavHeader {
	char     riff[4];
	uint32_t fileSize;
	char     wave[4];
	char     fmt[4];
	uint32_t fmtSize;
	uint16_t audioFormat;
	uint16_t channels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
};
#pragma pack(pop)

AudioClip::AudioClip(const std::string& path) {
	alGenBuffers(1, &buffer_);
	if (!loadWav(path, buffer_)) {
		alDeleteBuffers(1, &buffer_);
		throw std::runtime_error("AudioClip: failed to load " + path);
	}
}

AudioClip::~AudioClip() {
	alDeleteBuffers(1, &buffer_);
}

bool AudioClip::loadWav(const std::string& path, ALuint buffer) {
	FILE* file = fopen(path.c_str(), "rb");
	if (!file) {
		return false;
	}

	WavHeader header;
	if (fread(&header, sizeof(WavHeader), 1, file) != 1) {
		fclose(file);
		return false;
	}

	if (strncmp(header.riff, "RIFF", 4) != 0 ||
	    strncmp(header.wave, "WAVE", 4) != 0 ||
	    strncmp(header.fmt, "fmt ", 4) != 0 ||
	    header.audioFormat != 1) { // 1 = PCM
		fclose(file);
		return false;
	}

	// fmtチャンクが16バイト以外の場合は余分なバイトをスキップ
	if (header.fmtSize > 16) {
		fseek(file, header.fmtSize - 16, SEEK_CUR);
	}

	// dataチャンクを探す
	char    chunkId[4];
	uint32_t chunkSize = 0;
	while (fread(chunkId, 1, 4, file) == 4) {
		if (fread(&chunkSize, 4, 1, file) != 1) {
			break;
		}
		if (strncmp(chunkId, "data", 4) == 0) {
			break;
		}
		fseek(file, chunkSize, SEEK_CUR);
		chunkSize = 0;
	}

	if (chunkSize == 0) {
		fclose(file);
		return false;
	}

	std::vector<uint8_t> data(chunkSize);
	if (fread(data.data(), 1, chunkSize, file) != chunkSize) {
		fclose(file);
		return false;
	}
	fclose(file);

	ALenum format = AL_NONE;
	if (header.channels == 1) {
		format = (header.bitsPerSample == 16) ? AL_FORMAT_MONO16 : AL_FORMAT_MONO8;
	} else if (header.channels == 2) {
		format = (header.bitsPerSample == 16) ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8;
	}

	if (format == AL_NONE) {
		return false;
	}

	alBufferData(buffer, format, data.data(), static_cast<ALsizei>(chunkSize),
	             static_cast<ALsizei>(header.sampleRate));
	return alGetError() == AL_NO_ERROR;
}
