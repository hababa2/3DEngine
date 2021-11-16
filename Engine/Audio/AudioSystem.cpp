#include "AudioSystem.h"
#include "Core/Utilities.h"

namespace nh
{
	void AudioSystem::Startup()
	{
		FMOD::System_Create(&fmodSystem);

		void* extradriverdata = nullptr;
		fmodSystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
	}

	void AudioSystem::Shutdown()
	{
		for (auto& sound : sounds)
		{
			sound.second->release();
		}
		sounds.clear();

		fmodSystem->close();
		fmodSystem->release();
	}

	void AudioSystem::Update(float dt)
	{
		fmodSystem->update();
	}

	void AudioSystem::AddAudio(const std::string& name, const std::string& filename)
	{
		std::string n = nh::ToLower(name);
		if (sounds.find(n) == sounds.end())
		{
			FMOD::Sound* sound{ nullptr };
			fmodSystem->createSound(filename.c_str(), FMOD_DEFAULT, 0, &sound);
			sounds[n] = sound;
		}
	}

	AudioChannel AudioSystem::PlayAudio(const std::string& name, float volume, float pitch, bool loop)
	{
		auto iter = sounds.find(nh::ToLower(name));
		if (iter != sounds.end())
		{
			FMOD::Sound* sound = iter->second;
			sound->setMode(FMOD_LOOP_OFF + loop);
			FMOD::Channel* channel;
			fmodSystem->playSound(sound, 0, true, &channel);
			channel->setVolume(volume);
			channel->setPitch(pitch);
			channel->setPaused(false);

			return AudioChannel{ channel };
		}

		return AudioChannel{};
	}
}
