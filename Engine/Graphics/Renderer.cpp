#include "Renderer.h"

#include "Math/MathUtils.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>

namespace nh
{
	void Renderer::Startup()
	{
		if (SDL_Init(SDL_INIT_VIDEO))
		{
			std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		}

		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
		TTF_Init();
	}

	void Renderer::Shutdown()
	{
		TTF_Quit();
		IMG_Quit();
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}

	void Renderer::Update(float dt)
	{

	}

	void Renderer::Create(const std::string& name, int width, int height)
	{
		window = SDL_CreateWindow(name.c_str(), 100, 100, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (!window)
		{
			std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
			SDL_Quit();
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	}

	void Renderer::BeginFrame()
	{
		SDL_RenderClear(renderer);
	}

	void Renderer::EndFrame()
	{
		SDL_RenderPresent(renderer);
	}
}