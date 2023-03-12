#pragma once
#include <chrono>



class Timer
{
public:
	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	Timer(Timer& t)
	{
		start = t.start;
	}
	Timer operator=(Timer& t)
	{
		start = t.start;
	}

	void reset()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	float getPassedSeconds()
	{		
		return std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - start).count();
	}


private:
	std::chrono::high_resolution_clock::time_point start;
};

