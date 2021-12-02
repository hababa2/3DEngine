#include "Material.h"
#include "Engine.h"

namespace nh
{
	bool Material::Load(const std::string& filename, void* data)
	{
		auto engine = static_cast<Engine*>(data);

		rapidjson::Document document;
		bool success = nh::json::Load(filename, document);
		if (!success)
		{
			SDL_Log("Could not load material file (%s).", filename.c_str());
			return false;
		}

		JSON_READ(document, ambient);
		JSON_READ(document, diffuse);
		JSON_READ(document, specular);
		JSON_READ(document, shininess);

		std::string shader_name;
		JSON_READ(document, shader_name);
		shader = engine->Get<nh::ResourceSystem>()->Get<nh::Program>(shader_name, engine);
		
		std::vector<std::string> texture_names;
		JSON_READ(document, texture_names);

		for (auto& name : texture_names)
		{
			auto texture = engine->Get<nh::ResourceSystem>()->Get<nh::Texture>(name);
			if (texture.get())
			{
				AddTexture(texture);
			}
		}

		return true;
	}

	void Material::Set()
	{
		shader->Use();
		shader->SetUniform("material.ambient", ambient);
		shader->SetUniform("material.diffuse", diffuse);
		shader->SetUniform("material.specular", specular);
		shader->SetUniform("material.shininess", shininess);

		for (auto& texture : textures)
		{
			texture->Bind();
		}
	}

	void Material::SetShader(Program& shader)
	{
		shader.Use();
	}
}
