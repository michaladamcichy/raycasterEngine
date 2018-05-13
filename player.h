#pragma once
enum PlayerPosition
{
	STANDARD = 0,
	AIMED = 1
};
enum PlayerMovement
{
	MOVEMENT_NONE,
	WALKING,
	RUNNING
};
enum HelthManagement
{
	STILL_ALIVE,
	JUST_DIED
};

class Player : public View
{
private:
	int movement = MOVEMENT_NONE;
	int position = STANDARD;
	int fire = 0;
	int shiningGunStandardAnimationCount = 5;
	int shiningGunStandardFramesCount = 5;
	int favorizedAnimation;
	int animationChange = 0;
	int currentAnimation = 0;
	SDL_Rect standard, aimed, running, firing, firingZoom;
	float fireZoom = 1.5;
	double boost = 1;
	double diagonalWalking = 1;
	double collisionRadius;
	bool collision = false;
	int hurtTimer;
	int hurtDuration = 30;
	int HP = 100;
public:
	Player(int inScreenWidth, int inScreenHeight, int AB_DISTANCE, int inObserverDistance)
		: View(inScreenWidth, inScreenHeight, AB_DISTANCE, inObserverDistance)
	{
		View(inScreenWidth, inScreenHeight, AB_DISTANCE, inObserverDistance);
		collisionRadius = radius * 1.8;
	}

	void prepareTextureRects(
		Config& cfg, 
		MyTexture* gunStandard, 
		MyTexture* gunAimed, 
		AnimatedTexture* gunRunning, 
		AnimatedTexture* fire,
		int screenWidth, 
		int screenHeight
	)
	{
		double ratioX = screenWidth / cfg.getOptimalScrW();
		double ratioY = screenHeight / cfg.getOptimalScrH();
		standard = {
			(screenWidth / 2 - int(gunStandard->getW()*ratioX) / 2) + int(gunStandard->getW()*ratioX) / 2,
			screenHeight - int(gunStandard->getH()*ratioX),
			int(gunStandard->getW()*ratioX),
			int(gunStandard->getH()*ratioX)
		};
		aimed = {
			screenWidth / 2 - int(gunAimed->getW()*ratioX) / 2,
			screenHeight - int(gunAimed->getH()*ratioX),
			int(gunAimed->getW()*ratioX),
			int(gunAimed->getH()*ratioX)
		};
		running = {
			0,
			screenHeight - int(gunRunning->getFrameHeight()*ratioX),
			int(gunRunning->getFrameWidth()*ratioX),
			int(gunRunning->getFrameHeight()*ratioX)
		};
		firing = {
			screenWidth / 2 - int(fire->getFrameWidth()*ratioX) / 2 + int(screenWidth/14*ratioX),
			screenHeight / 2 - int(fire->getFrameHeight() / 2 * ratioY),
			int(fire->getFrameWidth() * ratioX),
			int(fire->getFrameHeight() * ratioX)
		};
		firingZoom = {
			int(screenWidth / 2 - int(fire->getFrameWidth()*ratioX) / 2 * fireZoom),
			int(screenHeight / 2 - int(fire->getFrameHeight()*ratioX) / 2 * fireZoom),
			int(fire->getFrameWidth() * fireZoom * ratioX),
			int(fire->getFrameHeight() * fireZoom * ratioX)
		};

	}
	int getPosition()
	{
		return position;
	}
	void setPosition(int inPosition)
	{
		position = inPosition;
	}
	int getMovement()
	{
		return movement;
	}
	void setMovement(int inMovement)
	{
		movement = inMovement;
	}
	bool isFiring()
	{
		return fire;
	}
	void recordFire()
	{
		fire = 3;
	}
	void stopFiring()
	{
		fire = false;
	}
	void setFavorizedAnimation(int animation)
	{
		favorizedAnimation = animation;
	}
	void handleAnimationSwitching()
	{
		srand(time(NULL));
		if (animationChange <= 0)
		{
			animationChange = shiningGunStandardFramesCount;
			if (rand() % 2)
			{
				currentAnimation = favorizedAnimation;
			}
			else
			{
				currentAnimation = rand() % shiningGunStandardAnimationCount;
			}
		}
		animationChange--;
	}
	int getShiningGunStandardAnimationCount()
	{
		return shiningGunStandardAnimationCount;
	}
	int getShiningGunStandardFramesCount()
	{
		return shiningGunStandardFramesCount;
	}
	int getCurrentAnimation()
	{
		return currentAnimation;
	}
	void changePosition()
	{
		position = !position;
	}
	double getBoost()
	{
		return boost;
	}
	void setBoost(double inBoost)
	{
		boost = inBoost;
	}
	double getDiagonal()
	{
		return diagonalWalking;
	}
	void setDiagonal(double inDiagonal)
	{
		diagonalWalking = inDiagonal;
	}
	double getCollisionRadius()
	{
		return collisionRadius;
	}
	SDL_Rect* getStandard()
	{
		return &standard;
	}
	SDL_Rect* getAimed()
	{
		return &aimed;
	}
	SDL_Rect* getRunning()
	{
		return &running;
	}
	SDL_Rect* getFiring()
	{
		return &firing;
	}
	SDL_Rect* getFiringZoom()
	{
		return &firingZoom;
	}
	void recordCollision()
	{
		collision = true;
	}
	bool collisionDetected()
	{
		return collision;
	}
	void clearEvents()
	{
		movement = MOVEMENT_NONE;
		if (fire > 0) fire--;
		collision = false;
		if (hurtTimer > 0) hurtTimer--;
	}
	void hurt(int HPloss)
	{
		if (hurtTimer == 0)
		{
			HP -= HPloss;
			if (HP < 0)
			{
				HP = 0;
			}
			hurtTimer = hurtDuration;
		}
	
	}
	int getHP()
	{
		return HP;
	}
};