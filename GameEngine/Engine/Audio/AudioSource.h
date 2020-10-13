#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <string>
#include "../Rendering/3D/Component.h"
#include "../Rendering/3D/GameObject.h"
#include "../Audio/AudioHandler.h"

class AudioSource : public Component
{
public:
	// Inherited via Component
	AudioSource(const std::string& fileName_, bool isLooping_, bool is3D_, bool isStreaming_);
	bool OnCreate(GameObject* parent_) override;
	void Update(float deltaTime_) override;
	bool IsPlaying();
	void PlaySound(float volume = 1.0f);
private:
	std::string fileName;
	int channelID;
};

#endif

