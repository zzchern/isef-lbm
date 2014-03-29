#pragma once

class Timer
{
public:
	void start_timer();
	double time_passed();
private:
	double start;
};

