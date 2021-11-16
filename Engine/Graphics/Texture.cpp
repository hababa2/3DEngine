#include "Texture.h"
#include "Graphics/Renderer.h"

#include <iostream>
#include <cassert>

namespace nh
{
	Texture::Texture(Renderer* renderer)
	{
		assert(renderer);
		this->renderer = renderer->renderer;
	}

	bool Texture::Load(const std::string& filename, void* data)
	{
		renderer = reinterpret_cast<Renderer*>(data)->renderer;

		SDL_Surface* surface = IMG_Load(filename.c_str());

		//assert(surface);
		if (!surface)
		{
			std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
			return false;
		}

		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		if (!texture)
		{
			std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
			return false;
		}

		return true;
	}

	bool Texture::Create(SDL_Surface* surface)
	{
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		if (!texture)
		{
			std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
			return false;
		}

		return true;
	}

	glm::vec2 Texture::GetSize() const
	{
		SDL_Point p;
		SDL_QueryTexture(texture, nullptr, nullptr, &p.x, &p.y);
		return { p.x, p.y };
	}
}