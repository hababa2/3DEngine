#include "LightComponent.h"
#include "Engine.h"

namespace nh
{
	void LightComponent::Update()
	{
		glm::vec4 position{ 1 };

		auto actor = owner->scene->FindActor("camera");
		if (actor != nullptr)
		{
			position = actor->GetComponent<CameraComponent>()->view * glm::vec4{ owner->transform.position, 1 };
			auto shaders = actor->scene->engine->Get<nh::ResourceSystem>()->Get<nh::Program>();

			for (auto& shader : shaders)
			{
				shader->SetUniform("light.ambient", ambient);
				shader->SetUniform("light.diffuse", diffuse);
				shader->SetUniform("light.specular", specular);
				shader->SetUniform("light.position", position);
			}
		}
	}

	bool LightComponent::Write(const rapidjson::Value& value) const
	{
		return false;
	}

	bool LightComponent::Read(const rapidjson::Value& value)
	{
		return false;
	}
}