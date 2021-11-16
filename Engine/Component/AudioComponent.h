#pragma once

#include "Component/Component.h"
#include "Audio/AudioChannel.h"

namespace nh
{
	class AudioComponent : public Component
	{
	public:
		std::unique_ptr<Object> Clone() const { return std::make_unique<AudioComponent>(*this); }

		virtual void Update() override;

		void Play();
		void Stop();

		virtual bool Write(const rapidjson::Value& value) const override;

		virtual bool Read(const rapidjson::Value& value) override;

	public:
		std::string name;
		float volume{ 1.0f };
		float pitch{ 1.0f };
		bool loop{ false };
		bool playOnAwake;

	private:
		AudioChannel channel;
		bool played{ false };
	};
}