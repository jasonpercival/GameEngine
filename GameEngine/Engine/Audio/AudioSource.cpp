#include "AudioSource.h"

AudioSource::AudioSource(const std::string& fileName_, bool isLooping_, bool is3D_, bool isStreaming_) :
	channelID(-1), fileName(fileName_)
{
	AudioHandler::GetInstance()->LoadSound(fileName_, isLooping_, is3D_, isStreaming_);
}

bool AudioSource::OnCreate(GameObject* parent_)
{
	parent = parent_;
	return true;
}

bool AudioSource::IsPlaying()
{
	return AudioHandler::GetInstance()->IsPlaying(channelID);
}

void AudioSource::PlaySound(float volume)
{
	channelID = AudioHandler::GetInstance()->PlaySound(fileName,
		parent ? parent->GetPosition() : glm::vec3(0), glm::vec3(0), volume);
}

void AudioSource::Update(float deltaTime_)
{
	AudioHandler::GetInstance()->Update();
	// UpdateChannelPositionVelocity()
}