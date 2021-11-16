#include "Audio/AudioChannel.h"

namespace nh
{
	bool AudioChannel::IsPlaying()
	{
		if (channel)
		{
			bool playing;
			channel->isPlaying(&playing);
			return playing;
		}

		return false;
	}

	void AudioChannel::Stop()
	{
		if (IsPlaying())
		{
			channel->stop();
		}
	}

	void AudioChannel::SetPitch(float pitch)
	{
		if (IsPlaying())
		{
			channel->setPitch(pitch);
		}
	}

	float AudioChannel::GetPitch()
	{
		float pitch{ 0.0f };

		if (IsPlaying())
		{
			channel->getPitch(&pitch);
		}

		return pitch;
	}

	void AudioChannel::SetVolume(float volume)
	{
		if (IsPlaying())
		{
			channel->setPitch(volume);
		}
	}

	float AudioChannel::GetVolume()
	{
		float volume{ 0.0f };

		if (IsPlaying())
		{
			channel->getPitch(&volume);
		}

		return volume;
	}
}