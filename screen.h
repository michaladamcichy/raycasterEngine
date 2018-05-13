#pragma once
class Screen
{
private:
	Stripe * * stripes;
	int* stripesCount;
	double skyTranslation = 0;
	bool blur = false;
	MyTexture* renderedImages;

	Player& player;
	GameWorld& gameWorld;
	Config& cfg;

	int renderedImage = 0;
	MyTexture sky;
	MyTexture snow;
	MyTexture floor;

	AnimatedTexture* objectsTextures;
	int wallsTexCount;
	int objectsTexturesCount;
	int independentTexCount;
	int allTexturesCount;
	void loadTextures(SDL_Renderer* renderer)
	{
		std::cout << "\nStarting loading textures.";
		wallsTexCount = gameWorld.getWallTextures().size();
		int spritesTexCount = gameWorld.getSpriteTextures().size();
		independentTexCount = gameWorld.getIndependentTextures().size();
		objectsTexturesCount = wallsTexCount + spritesTexCount;
		allTexturesCount = objectsTexturesCount + independentTexCount;
		objectsTextures = new AnimatedTexture[allTexturesCount];
		std::string path;
		std::cout << "\nNumber of wall textures: " << wallsTexCount;
		std::cout << "\nNumber of sprite textures: " << spritesTexCount;
		std::cout << "\nNumber of all textures: " << allTexturesCount;
		for (int i = 0; i< wallsTexCount; i++)
		{
			path = "textures/";
			path.append(gameWorld.getWallTextures()[i]);
			path.append(".png");
			objectsTextures[i].createFromFile(
				renderer, 
				path, 
				1, 
				1, 
				DEFAULT_ANIMATED_TEXTURE, 
				SWITCH_FRAMES_MANUALLY
			);
			std::cout << "\n" << path << " loaded.";
		}
		for (int i = wallsTexCount; i< objectsTexturesCount; i++)
		{
			path = "textures/";
			path.append(gameWorld.getSpriteTextures()[i - wallsTexCount]);
			path.append(".png");
			std::cout << "\n" << path;
			if (path == "textures/bonfire.png")
			{
				objectsTextures[i].createFromFile(
					renderer,
					path, 
					13, 
					2, 
					BIG_ANIMATED_TEXTURE, 
					SWITCH_FRAMES_MANUALLY
				);
			}
			else
			{
				objectsTextures[i].createFromFile(renderer, path, 1, 1, DEFAULT_ANIMATED_TEXTURE);
			}
			std::cout << "\n" << path << " loaded.";
		}
		int frames[] = {67, 2, 1};
		int speed[] = {1, 1, 1};
		for (int i = objectsTexturesCount ; i < allTexturesCount; i++)
		{
		path = "textures/";
		path.append(gameWorld.getIndependentTextures()[i-objectsTexturesCount]);
		path.append(".png");
		objectsTextures[i].createFromFile(
			renderer, 
			path, 
			frames[i-objectsTexturesCount], 
			speed[i - objectsTexturesCount], 
			BIG_ANIMATED_TEXTURE, 
			SWITCH_FRAMES_MANUALLY
		);
		std::cout << "\n" << path << " loaded.";
		}
		sky.createFromFile(renderer, "textures/sky.png");
		sky.setAlpha(100);
		floor.createFromFile(renderer, "textures/floor.png");
		std::cout << "\nTextures loaded :)";
	}
	void sortStripes(int partOfScreen, int stripesCount)
	{
		int index, i, j;
		int n = stripesCount;

		for (i = 0; i < n - 1; i++)
		{
			index = i;
			j = i + 1;
			while (stripes[partOfScreen][j].getDistance() < stripes[partOfScreen][index].getDistance())
			{
				std::swap(stripes[partOfScreen][j], stripes[partOfScreen][index]);
				index--;
				j--;
				if (index < 0)
				{
					break;
				}
			}
		}
	}
public:
	Screen(
		SDL_Renderer* renderer, 
		GameWorld &gameWorldRef, 
		Player& playerRef, Config& cfgRef
	) : gameWorld(gameWorldRef), player(playerRef), cfg(cfgRef)
	{
		loadTextures(renderer);
		stripes = new Stripe*[cfg.getThreadsCount()];
		for (int i = 0; i<cfg.getThreadsCount(); i++)
		{
			stripes[i] = new Stripe[cfg.getStripesForThreadLimit()];
		}
		stripesCount = new int[cfg.getThreadsCount()]{ 0 };

		renderedImages = new MyTexture[cfg.getRenderedImagesLimit()];
		for (int i = 0; i < cfg.getRenderedImagesLimit(); i++)
		{
			renderedImages[i].createBlank(
				renderer, 
				cfg.getScreenWidth(), 
				cfg.getScreenHeight(), 
				SDL_TEXTUREACCESS_TARGET
			);
		}
	}
	void raycastThread(int partOfScreen, int threadsNumber)
	{
		stripesCount[partOfScreen] = 0;
		MyPoint currentScreenPoint, crossing, currentObserver;
		currentObserver = player.getObserver();
		Wektor screenStep;
		AlternativeLine viewLine;
		viewLine.define(player.getA(), player.getB());
		screenStep.defineFromPoints(player.getA(), player.getB());
		screenStep = screenStep.getVersor();
		Wektor initialTrans;
		initialTrans.defineFromPoints(player.getA(), player.getB());
		initialTrans.scale(double(1) / cfg.getThreadsCount());
		currentScreenPoint.define(
			player.getA().x + partOfScreen * initialTrans.x, 
			player.getA().y + partOfScreen * initialTrans.y
		);
		bool existence = false;
		Stripe currentStripe;
		GameObject* object;
		//
		double fullDistance;
		double percentageOfRealSize;
		double stripeVisualHeight;
		double objectVisualWidth;
		double stripePositionInTexture;
		double stripeWidth;
		int alpha;
		//
		int crossingFound = false;
		int rayPenetration = 1;
		char continueRaycasting = rayPenetration;
		for (
			int stripeIndex = partOfScreen * cfg.getScreenWidth() / threadsNumber;
			stripeIndex < (partOfScreen + 1)*cfg.getScreenWidth() / threadsNumber;
			stripeIndex++
		)
		{
			continueRaycasting = rayPenetration;
			for (int objectIndex = 0; objectIndex < gameWorld.getObjectsCount(); objectIndex++)
			{
				if (stripesCount[partOfScreen] == cfg.getStripesForThreadLimit())
				{
					objectIndex = gameWorld.getObjectsCount();
					stripeIndex = (partOfScreen + 1)*cfg.getScreenWidth() / threadsNumber;
				}
				else
				{
					object = gameWorld.getObject(objectIndex);
					if (object->rayCrossing(player, currentScreenPoint, crossing))
					{
						stripes[partOfScreen][stripesCount[partOfScreen]++] =
							object->createStripe(cfg, player, currentScreenPoint, stripeIndex, crossing, objectsTextures);
						if (object->stoppingRays() || pointPointDistance(crossing, currentObserver) > cfg.getVisibleDistance())
						{
							continueRaycasting--;
							if (!continueRaycasting)
							{
								objectIndex = gameWorld.getObjectsCount();
							}
						}
					}
				}
			}
			currentScreenPoint.x += screenStep.x;
			currentScreenPoint.y += screenStep.y;
		}
	}
	void raycast(TimeManager& timeManager, int threadsNumber)
	{
		gameWorld.clearSpottings();
		MyPoint currentObserver = player.getObserver();
		for (int objectIndex = 0; objectIndex < gameWorld.getObjectsCount(); objectIndex++)
		{
			gameWorld.getObject(objectIndex)->calcDistanceFromObserverAndHandleCollisions(timeManager, player);
		}
		gameWorld.sortObjects();
		std::thread * threads;
		threads = new std::thread[cfg.getThreadsCount()];

		for (int i = 0; i<cfg.getThreadsCount(); i++)
		{
			threads[i] = std::thread([=] { raycastThread(i, cfg.getThreadsCount()); });
		}
		for (int i = 0; i<cfg.getThreadsCount(); i++)
		{
			threads[i].join();
		}
		if (cfg.isStripesPostSortingEnabled())
		{
			Uint32 time = SDL_GetTicks();
			for (int i = 0; i < cfg.getThreadsCount(); i++)
			{
				threads[i] = std::thread([=] { sortStripes(i, stripesCount[i]); });
			}
			for (int i = 0; i < cfg.getThreadsCount(); i++)
			{
				threads[i].join();
			}
		}
		delete[] threads;
		/*for (int i = 0; i < cfg.getThreadsCount(); i++)
		{
		std::sort(stripes[i], stripes[i] + stripesCount[i]);
		}
		putSpritesWhereTheyBelong();*/
	}
	void drawObjects(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		for (int thread = 0; thread < cfg.getThreadsCount(); thread++)
		{
			for (int i = stripesCount[thread] - 1; i >= 0; i--)
			{
				stripes[thread][i].render(renderer, objectsTextures);
			}
		}
	}
	void drawSky(SDL_Renderer* renderer)
	{
		if (skyTranslation < 0)
		{
			skyTranslation = cfg.getScreenWidth() + skyTranslation;
		}
		skyTranslation = int(skyTranslation) % cfg.getScreenWidth();
		SDL_Rect rectIn = {
			int(skyTranslation),
			0,
			cfg.getScreenWidth() - int(skyTranslation), 
			cfg.getScreenHeight() / 2 
		};
		SDL_Rect rectOut = { 
			0,
			0, 
			cfg.getScreenWidth() - int(skyTranslation), 
			cfg.getScreenHeight() / 2 
		};
		sky.render(renderer, &rectIn, &rectOut);

		rectIn = { 
			0,
			0,
			int(skyTranslation), cfg.getScreenHeight() / 2 
		};
		rectOut = { cfg.getScreenWidth() - int(skyTranslation),
			0, 
			int(skyTranslation), 
			cfg.getScreenHeight() / 2 };
		sky.render(renderer, &rectIn, &rectOut);
	}
	void handleSkyTranslation(TimeManager& timeManager, double degrees, int mouseTransX, int mouseSensitivity)
	{
		skyTranslation += 
			degrees * 
			mouseTransX / mouseSensitivity / 2 / PI * 2*cfg.getScreenWidth() *
			timeManager.getLastFrame() / timeManager.getFrameAssumedTime();
	}
	void drawFloor(SDL_Renderer* renderer)
	{
		SDL_Rect out = { 0, cfg.getScreenHeight() / 2, cfg.getScreenWidth(), cfg.getScreenHeight() / 2 };
		floor.render(renderer, NULL, &out);
	}
	void prepareImage(SDL_Renderer* renderer)
	{
		//renderedImages[renderedImage].setAsRenderrayPeTarget(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		drawSky(renderer);
		drawFloor(renderer);
		drawObjects(renderer);
		//SDL_SetRenderTarget(renderer, NULL);
	}
	void render(SDL_Renderer* renderer, GameWorld gameWorld)
	{
		prepareImage(renderer);
	}
	void switchTexturesFrame()
	{
		for (int i = 0; i < allTexturesCount; i++)
		{
			objectsTextures[i].nextFrame();
		}
	}
	void enableBlur()
	{
		blur = true;
	}
	void disableBlur()
	{
		blur = false;
	}
	AnimatedTexture* getObjectsTextures()
	{
		return objectsTextures;
	}
	int getWallTexturesCount()
	{
		return wallsTexCount;
	}
	int getObjectsTexturesCount()
	{
		return objectsTexturesCount;
	}
	int getIndependentTexturesCount()
	{
		return independentTexCount;
	}
	int getAllTexturesCount()
	{
		return allTexturesCount;
	}
};