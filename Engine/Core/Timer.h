#pragma once

#include <chrono>

namespace nh
{
	class Timer
	{
	public:
		using clock = std::chrono::high_resolution_clock;
		using clock_duration = clock::duration;
		using clock_tick_type = clock::rep;

	public:
		Timer() : timePoint{ clock::now() } {}

		void Reset() { timePoint = clock::now(); }
		clock_tick_type ElapsedTicks();
		float ElapsedSeconds();

	protected:
		clock::time_point timePoint;
	};

	class FrameTimer : public Timer
	{
	public:
		FrameTimer() : frameTimePoint{ clock::now() }, startTimePoint{ clock::now() } {}
		void Tick();

	public:
		float timeScale{ 1.0f };
		float deltaTime{ 0.0f };
		float time{ 0.0f };

	private:
		clock::time_point frameTimePoint;
		clock::time_point startTimePoint;
	};
}