/* Copyright by Michał Cichy
michal.adam.cichy@gmail.com
2018
*/

/*
Balans rozgrywki
Plansze
Pierwszy dostaje
*/

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<thread>
#include<mutex>
#include<future>
#include<iostream>
  #include<cstdio>
#include<string>
#include<vector>
#include<string>
#include<cstdlib>
#include<fstream>
#include<algorithm>
#include<utility>
#include<ctime>
#include<cmath>
#include<time.h>
#include<windows.h>
//My libraries:
#include<myMath.h>
#include<timeManager.h>
#include<utils.h>
#include<progressVisualiser.h>
#include<sdlUtils.h>
#include<config.h>
#include<myGeometry.h>
#include<drawing.h>
#include<view.h>
#include<player.h>
#include<stripe.h>
#include<gameObject.h>
#include<wall.h>
#include<sprite.h>
#include<gameWorld.h>
#include<enemy.h>
#include<enemiesGarrison.h>
#include<sound.h>
#include<input.h>
#include<screen.h>
#include<overlayManager.h>

SDL_Renderer* myRenderer;
SDL_Window* myWindow;

int main(int argc, char* argv[])
{
	srand(time(NULL));
	Config cfg;
	if (argc > 2)
	{
		cfg.setEnemiesAutoSpawn(2);
	}
	int objectsCount;
	long long int frameCount = 0;
	if (!init(cfg.getScreenState(), cfg.getScreenWidth(), cfg.getScreenHeight(), myWindow, myRenderer))
	{
		printf("Failed to initialize\n");
	}
	else
	{
		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetRenderDrawColor(myRenderer, 0, 0, 0, 255);
		SDL_RenderClear(myRenderer);
		SDL_RenderPresent(myRenderer);

		Mouse mouse;
		Keyboard keyboard;	
		SoundManager soundManager;
		soundManager.toogleSound();
		GameWorld gameWorld(cfg);
		TimeManager timeManager(cfg.getFrameAssumedTime());
		Player player(cfg.getScreenWidth(), cfg.getScreenHeight(), cfg.getScreenWidth(), cfg.getObserverDistance());
		std::string objectsPath = "objects.txt";
		if (argc > 1)
		{
			objectsPath = argv[1];
		}
		gameWorld.loadObjects(objectsPath);
		objectsCount = gameWorld.getObjectsCount();
		std::cout << '\n' << objectsPath;
		gameWorld.printObjectsData();
		Screen screen(myRenderer, gameWorld, player, cfg);
		gameWorld.updateSpritesWidth(cfg, screen.getObjectsTextures());
		OverlayManager overlayManager(cfg, myRenderer, player);
		EnemiesGarrison enemiesGarrison(
			cfg,
			&gameWorld,
			player,
			screen.getObjectsTextures(),
			screen.getObjectsTexturesCount(),
			screen.getIndependentTexturesCount()
		);
		enemiesGarrison.loadEnemies("enemies.txt");

		for (int i = 0; i < gameWorld.getObjectsCount(); i++)
		{
			gameWorld.getObject(i)->calcDistanceFromObserverAndHandleCollisions(timeManager, player);
		}
		gameWorld.sortObjects();

		SDL_Event event;
		Wektor mouseMovement;
		bool exit = false;

		while (!exit)
		{
			timeManager.startFrame();
			while (SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_QUIT)
				{
					exit = true;
				}
				else
				{
					keyboard.handleInput(event);
					mouse.handleButtons(event);
				}
			}
			mouse.recordMovement(myWindow, cfg.getScreenWidth() / 2, cfg.getScreenHeight() / 2);
			mouseMovement = mouse.getTransXY();

			player.clearEvents();
			if (mouseMovement.x < 0)
			{
				player.rotate(timeManager, cfg.getRotateUnit()*mouseMovement.x / cfg.getMouseSensitivity());
				screen.handleSkyTranslation(timeManager, cfg.getRotateUnit(), mouseMovement.x, cfg.getMouseSensitivity());
				player.setMovement(WALKING);
			}
			if (mouseMovement.x > 0)
			{
				player.rotate(timeManager, cfg.getRotateUnit()*mouseMovement.x / cfg.getMouseSensitivity());
				screen.handleSkyTranslation(timeManager, cfg.getRotateUnit(), mouseMovement.x, cfg.getMouseSensitivity());
				player.setMovement(WALKING);
			}
			if ((keyboard.up || keyboard.down) && (keyboard.left || keyboard.right))
			{
				player.setDiagonal(SQRT_2);
			}
			else
			{
				player.setDiagonal(1);
			}

			if (keyboard.up)
			{
				player.translate(timeManager, FORWARD, cfg.getPlayerSpeed() / player.getDiagonal()*player.getBoost());
				player.setMovement(WALKING);
			}
			if (keyboard.down)
			{
				player.translate(timeManager, BACKWARDS, cfg.getPlayerSpeed() / player.getDiagonal()*player.getBoost());
				player.setMovement(WALKING);
			}
			if (keyboard.left)
			{
				player.translate(timeManager, LEFT, cfg.getPlayerSpeed() / player.getDiagonal());
				player.setMovement(WALKING);
			}
			if (keyboard.right)
			{
				player.translate(timeManager, RIGHT, cfg.getPlayerSpeed() / player.getDiagonal());
				player.setMovement(WALKING);
			}
			if (keyboard.tab)
			{
				soundManager.toogleSound();
				keyboard.tab = false;
			}
			if (mouse.right)
			{
				player.changePosition();
				mouse.right = false;
			}
			if (mouse.left)
			{
				player.recordFire();
				mouse.left = false;
			}
			if (keyboard.leftShift && keyboard.up)
			{
				player.setBoost(cfg.getMaxBoost());
				player.setMovement(RUNNING);
				player.stopFiring();
			}
			else
			{
				player.setBoost(1);
			}
			player.updateDirection();
			enemiesGarrison.AI(timeManager);
			screen.raycast(timeManager, cfg.getThreadsCount());

			if (cfg.motionBlurEnabled())
			{
				if ( keyboard.up && keyboard.leftShift)
				{
					screen.enableBlur();
				}
				else
				{
					screen.disableBlur();
				}
			}
			screen.render(myRenderer, gameWorld);
			overlayManager.renderOverlay(myRenderer, timeManager);
			SDL_RenderPresent(myRenderer);
			if (player.isFiring())
			{
				soundManager.playShot();
				enemiesGarrison.handleShots(cfg, player);
			}
			screen.switchTexturesFrame();
			overlayManager.switchTexturesFrame();

			if (player.getHP() == 0)
			{
				exit = true;
			}
			timeManager.endFrame();
		}
		if (player.getHP() == 0)
		{
			overlayManager.deathScreen(myRenderer);
		}
		gameWorld.freeResources();
	}
	quit();
	return 0;
}