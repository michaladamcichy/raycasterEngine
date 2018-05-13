#pragma once
class Config	
{
private:
	int screenState = SDL_WINDOW_SHOWN;
	int screenWidth = 1920;
	int screenHeight = 1080;
	bool motionBlur = false;
	int renderedImagesLimit = 5;
	int threadsCount = 10; //MAX 12
	int stripesForThreadLimit = 50000 / threadsCount;
	bool stripesPostSortingEnabled = false;
	int gameObjectsLimit = 10000;
	int observerDistance = 1000;
	double playerSpeed = 180;
	double globalSpeed = 100;
	double maxBoost = 3;
	double rotateUnit = PI / 60;
	int mouseSensitivity = 70;
	double defaultObjectsHeight = 5; //object's height is represented as multiplication of cfg::screenHeight
	double visibleDistance = 100000;
	double brightness = 3;
	double frameAssumedTime = 0.01666;
	double optimalScrW = 1920;
	double optimalScrH = 1080;
	double playerVisibilityDistance = 100000;
	double enemiesSpeed = 200;// 200;
	int enemiesStrength = 30;
	double enemiesFragilePercentage = 0.5;
	int shotPower = 10;
	int enemyHurtTextureDuration = 2;
	bool enemiesAutoSpawn = true;
public:
	Config()
	{
		observerDistance *= screenWidth / optimalScrW;
	}
	int getScreenState()
	{
		return screenState;
	}
	int getScreenWidth()
	{
		return screenWidth;
	}
	int getScreenHeight()
	{
		return screenHeight;
	}
	bool motionBlurEnabled()
	{
		return motionBlur;
	}
	int getRenderedImagesLimit()
	{
		return renderedImagesLimit;
	}
	int getThreadsCount()
	{
		return threadsCount;
	}
	int getStripesForThreadLimit()
	{
		return stripesForThreadLimit;
	}
	int getGameObjectsLimit()
	{
		return gameObjectsLimit;
	}
	int getObserverDistance()
	{
		return observerDistance;
	}
	double getPlayerSpeed()
	{
		return playerSpeed;
	}
	double getMaxBoost()
	{
		return maxBoost;
	}
	double getRotateUnit()
	{
		return rotateUnit;
	}
	int getMouseSensitivity()
	{
		return mouseSensitivity;
	}
	double getDefaultObjectsHeight()
	{
		return defaultObjectsHeight;
	}
	double getVisibleDistance()
	{
		return visibleDistance;
	}
	double getBrightness()
	{
		return brightness;
	}
	double getGlobalSpeed()
	{
		return globalSpeed;
	}
	double getFrameAssumedTime()
	{
		return frameAssumedTime;
	}
	double getOptimalScrW()
	{
		return optimalScrW;
	}
	double getOptimalScrH()
	{
		return optimalScrH;
	}
	bool isStripesPostSortingEnabled()
	{
		return stripesPostSortingEnabled;
	}
	double getPlayerVisibilityDistance()
	{
		return playerVisibilityDistance;
	}
	double getEnemiesSpeed()
	{
		return enemiesSpeed;
	}
	int getEnemiesStrength()
	{
		return enemiesStrength;
	}
	double getEnemiesFragilePercentage()
	{
		return enemiesFragilePercentage;
	}
	int getShotPower()
	{
		return shotPower;
	}
	int getEnemyHurtTextureDuration()
	{
		return enemyHurtTextureDuration;
	}
	bool enemiesAutoSpawnEnabled() 
	{
		return enemiesAutoSpawn;
	}
	void setEnemiesAutoSpawn(bool spawn)
	{
		enemiesAutoSpawn = spawn;
	}
};