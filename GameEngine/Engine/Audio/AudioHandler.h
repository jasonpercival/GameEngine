#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <string>
#include <map>
#include <memory>
#include <fmod.hpp>
#include <glm/glm.hpp>

#include "../Core/Debug.h"

class AudioHandler
{
public:
	// disable copy/move constructors
	AudioHandler(const AudioHandler&) = delete;
	AudioHandler(AudioHandler&&) = delete;
	AudioHandler& operator=(const AudioHandler&) = delete;
	AudioHandler& operator=(AudioHandler&&) = delete;

	static AudioHandler* GetInstance();
	bool Initialize(glm::vec3 position_ = glm::vec3(0), glm::vec3 velocity_ = glm::vec3(0),
		glm::vec3 forwardDir_ = glm::vec3(0, 0, -1), glm::vec3 upDir_ = glm::vec3(0, 1, 0));
	void Update();
	void OnDestroy();

	friend class AudioSource;

private:
	AudioHandler();
	~AudioHandler();
	static std::unique_ptr<AudioHandler> audioInstance;
	friend std::default_delete<AudioHandler>;

	FMOD_VECTOR glmToFMOD(glm::vec3 v);
	void LoadSound(const std::string& fileName, bool isLooping, bool is3D, bool isStreaming);
	FMOD::Sound* GetSound(const std::string& name);
	int PlaySound(const std::string& fileName, glm::vec3 position, glm::vec3 velocity = glm::vec3(0), float volume = 1.0f);
	void UpdateChannelPositionVelocity(int channelID, glm::vec3 position = glm::vec3(0), glm::vec3 velocity = glm::vec3(0));
	bool IsPlaying(int channelID);

	FMOD::System* fmodSystem;
	static std::map<std::string, FMOD::Sound*> sounds;
	static std::map<int, FMOD::Channel*> channels;
	int channelCount;
};

#endif

