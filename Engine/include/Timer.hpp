#pragma once
#include <chrono>
#include <iostream>

//1 / 120 = 0.008333
//1 / 60 = 0.016667

enum class FrameRate
{
	FPS_30 = 30,
	FPS_60 = 60,
	FPS_120 = 120,
	FPS_144 = 144,
	FPS_240 = 240,
	FPS_300 = 300
};

class Timer
{
public:
	void Init(FrameRate rate = FrameRate::FPS_60)
	{
		frame = rate;
		timeStamp = clock::now();
		lastTimeStamp = clock::now();
		fpsCalcTime = lastTimeStamp;
	}

	void Update() { timeStamp = clock::now();}

	void ResetLastTimeStamp() noexcept
	{
		lastTimeStamp = timeStamp;
	}

	void ResetFPSCalculateTime() noexcept
	{
		fpsCalcTime = timeStamp;
	}

	float GetDeltaTime() const noexcept
	{
		return std::chrono::duration_cast<second>(timeStamp - lastTimeStamp).count();
	}

	float GetFrameRateCalculateTime() const noexcept
	{
		return std::chrono::duration_cast<second>(timeStamp - fpsCalcTime).count();
	}

	FrameRate GetFrameRate() { return frame; }
private:
	using clock = std::chrono::system_clock;
	using second = std::chrono::duration <float>;

	std::chrono::system_clock::time_point timeStamp;
	std::chrono::system_clock::time_point lastTimeStamp;
	std::chrono::system_clock::time_point fpsCalcTime;

	FrameRate frame = FrameRate::FPS_60;
};
