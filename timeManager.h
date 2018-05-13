#pragma once
class TimeManager
{
private:
	double lastFrame = 0;
	Uint32 currentFrame = 0;
	double frameAssumedTime = 0.016666;
	bool tick = false;
	double lastAverage = 0;
	double workingAverage = 0;
	int counter = 0;
public:
	TimeManager(double frameAssumedTime);
	~TimeManager();
	void startFrame();
	void endFrame();
	double getLastFrame();
	double getFrameAssumedTime();
	double getLastAverage();
};

TimeManager::TimeManager(double frameAssumedTime = 0.016666)
{
this->frameAssumedTime = frameAssumedTime;
}
TimeManager::~TimeManager() {}

void TimeManager::startFrame()
{
	currentFrame = SDL_GetTicks();
	tick = false;
}
void TimeManager::endFrame()
{
	if (counter >= 20)
	{
		counter = 0;
		lastAverage = workingAverage / 20;
		workingAverage = 0;
	}
	lastFrame = double(SDL_GetTicks() - currentFrame) / 1000;
	workingAverage += lastFrame;
	counter++;
}
double TimeManager::getLastFrame()
{
	return lastFrame;
}
double TimeManager::getFrameAssumedTime()
{
	return frameAssumedTime;
}
double TimeManager::getLastAverage()
{
	return lastAverage;
}