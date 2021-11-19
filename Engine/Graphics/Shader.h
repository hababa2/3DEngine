#pragma once

#include "Renderer.h"
#include "Resource/Resource.h"

namespace nh
{
	class Shader : public Resource
	{
	public:
		~Shader();

		bool Load(const std::string& name, void* data) override;

	public:
		GLuint shader = 0;
	};
}
