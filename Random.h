#pragma once
#include <random>
#include <ctime>

class Random
{
	static std::mt19937 random;
	static int x;

public:
	static int Int(int min, int max);
	static float Float(float min, float max);
};

