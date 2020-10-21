#include "AudioHandler.h"

std::unique_ptr<AudioHandler> AudioHandler::audioInstance = nullptr;
std::map<std::string, FMOD::Sound*> AudioHandler::sounds = std::map<std::string, FMOD::Sound*>();
std::map<int, FMOD::Channel*> AudioHandler::channels = std::map<int, FMOD::Channel*>();

AudioHandler::AudioHandler() : channelCount(0), fmodSystem(nullptr)
{
}

void AudioHandler::OnDestroy()
{
	if (sounds.size() > 0)
	{
		for (auto s : sounds)
		{
			s.second->release();
			s.second = nullptr;
		}
		sounds.clear();
	}

	if (channels.size() > 0)
	{
		for (auto c : channels)
		{
			c.second->stop();
			c.second = nullptr;
		}
		channels.clear();
	}

	fmodSystem->release();
	fmodSystem = nullptr;
}

AudioHandler::~AudioHandler()
{
	OnDestroy();
}

FMOD_VECTOR AudioHandler::glmToFMOD(glm::vec3 v)
{
	return FMOD_VECTOR{ v.x, v.y, v.z };
}

void AudioHandler::LoadSound(const std::string& fileName, bool isLooping, bool is3D, bool isStreaming)
{
	// check if sound is already loaded
	if (GetSound(fileName) != nullptr)
		return;

	// set sound modes
	FMOD_MODE mode = FMOD_DEFAULT;
	mode |= isLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	mode |= is3D ? FMOD_3D : FMOD_2D;
	mode |= isStreaming ? FMOD_CREATESTREAM : FMOD_CREATESAMPLE;

	FMOD::Sound* sound = nullptr;

	// create the sound
	std::string fullPath = "Resources/Audio/" + fileName;
	FMOD_RESULT result;

	if ((result = fmodSystem->createSound(fullPath.c_str(), mode, nullptr, &sound)) == FMOD_OK)
		sounds.insert({ fileName, sound });
	else
		Debug::Warning("Failed to create sound. Error Code: " + result, "AudioHandler.cpp", __LINE__);
}

FMOD::Sound* AudioHandler::GetSound(const std::string& name)
{
	auto sound = sounds.find(name);
	if (sound != sounds.end())
		return sound->second;
	return nullptr;
}

int AudioHandler::PlaySound(const std::string& fileName, glm::vec3 position, glm::vec3 velocity, float volume)
{
	int channelID = -1;
	FMOD::Sound* sound = GetSound(fileName);
	if (!sound)
	{
		LoadSound(fileName, false, false, false);
		sound = GetSound(fileName);
	}

	FMOD::Channel* channel = nullptr;
	if (fmodSystem->playSound(sound, nullptr, true, &channel) != FMOD_OK)
	{
		Debug::Warning("Failed to play sound.", "AudioHandler.cpp", __LINE__);
	}

	// set 3D attributes if 3D sound
	FMOD_MODE mode;
	sound->getMode(&mode);

	if (mode & FMOD_3D)
		channel->set3DAttributes(&glmToFMOD(position), &glmToFMOD(velocity));

	channel->setVolume(volume);
	channel->setPaused(false);
	channelID = channelCount + 1;

	channels.insert({ channelID, channel });

	return channelID;
}

void AudioHandler::UpdateChannelPositionVelocity(int channelID, glm::vec3 position, glm::vec3 velocity)
{
	auto iterator = channels.find(channelID);
	if (iterator != channels.end())
	{
		iterator->second->set3DAttributes(&glmToFMOD(position), &glmToFMOD(velocity));
	}
}

bool AudioHandler::IsPlaying(int channelID)
{
	bool isPlaying = false;
	auto iterator = channels.find(channelID);
	if (iterator != channels.end())
	{
		iterator->second->isPlaying(&isPlaying);
	}
	return isPlaying;
}

AudioHandler* AudioHandler::GetInstance()
{
	if (audioInstance.get() == nullptr)
	{
		audioInstance.reset(new AudioHandler);
	}
	return audioInstance.get();
}

bool AudioHandler::Initialize(glm::vec3 position_, glm::vec3 velocity_, glm::vec3 forwardDir_, glm::vec3 upDir_)
{
	if (FMOD::System_Create(&fmodSystem) != FMOD_OK)
	{
		Debug::FatalError("Failed to create FMOD.", "AudioHandler.cpp", __LINE__);
		return false;
	}

	int numDrivers;
	fmodSystem->getNumDrivers(&numDrivers);
	if (numDrivers == 0)
	{
		Debug::FatalError("No audio drivers available.", "AudioHandler.cpp", __LINE__);
		return false;
	}

	if (fmodSystem->init(10, FMOD_INIT_NORMAL, nullptr) != FMOD_OK)
	{
		Debug::FatalError("Failed to initialize FMOD.", "AudioHandler.cpp", __LINE__);
		return false;
	}

	if (fmodSystem->set3DListenerAttributes(0, &glmToFMOD(position_), &glmToFMOD(velocity_),
		&glmToFMOD(forwardDir_), &glmToFMOD(upDir_)) != FMOD_OK)
	{
		Debug::FatalError("Failed to set 3D listener attributes.", "AudioHandler.cpp", __LINE__);
		return false;
	}

	return true;
}

void AudioHandler::Update()
{
	fmodSystem->update();
}

