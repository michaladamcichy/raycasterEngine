#pragma once
class OverlayManager
{
private:
	Config & cfg;
	Player & player;
	MyTexture gunStandard;
	AnimatedTexture* shiningGunStandard;
	AnimatedTexture gunRunning;
	AnimatedTexture fire;
	MyTexture gunAimed;
	MyTexture gunAimedFire;
	MyTexture gunStandardFire;
	MyTexture death;
	MyText fps;
	MyText HP;
	int lastHP;

	void loadTextures(SDL_Renderer* renderer)
	{
		gunStandard.createFromFile(renderer, "textures/standard.png");
		gunRunning.createFromFile(renderer, "textures/running.png", 8, 3, DEFAULT_ANIMATED_TEXTURE, SWITCH_FRAMES_MANUALLY);
		gunAimed.createFromFile(renderer, "textures/aimed.png");
		gunAimedFire.createFromFile(renderer, "textures/aimedfireblurred.png");
		gunStandardFire.createFromFile(renderer, "textures/firestandardblow.png");
		fire.createFromFile(renderer, "textures/fire.png", 3, 3, DEFAULT_ANIMATED_TEXTURE, SWITCH_FRAMES_MANUALLY);
		death.createFromFile(renderer, "textures/death.png");
		shiningGunStandard = new AnimatedTexture[player.getShiningGunStandardAnimationCount()];
		std::string path;
		for (int i = 0; i<player.getShiningGunStandardAnimationCount(); i++)
		{
			path = "textures/";
			path.append("shine");
			path += char(i + 48);
			path.append(".png");
			shiningGunStandard[i].createFromFile(
				renderer, 
				path.c_str(), 
				player.getShiningGunStandardFramesCount(), 
				5, 
				DEFAULT_ANIMATED_TEXTURE, SWITCH_FRAMES_MANUALLY
			);
		}
	}
	void drawGun(SDL_Renderer* renderer)
	{
		if (player.getMovement() == RUNNING)
		{
			gunRunning.render(renderer, NULL, player.getRunning());
		}
		else
		{
			switch (player.getPosition())
			{
			case STANDARD:
			{
				if (player.isFiring())
				{
					fire.render(renderer, NULL, player.getFiring());
					gunStandardFire.render(renderer, NULL, player.getStandard());
				}
				else if (player.getMovement() != MOVEMENT_NONE)
				{
					shiningGunStandard[player.getCurrentAnimation()].render(renderer, NULL, player.getStandard());
					player.handleAnimationSwitching();
				}
				else
				{
					gunStandard.render(renderer, NULL, player.getStandard());
				}
			}
			break;
			case AIMED:
			{
				if (player.isFiring())
				{
					fire.render(renderer, NULL, player.getFiringZoom());
					gunAimedFire.render(renderer, NULL, player.getAimed());
				}
				else
				{
					gunAimed.render(renderer, NULL, player.getAimed());
				}
			}
			break;
			default: break;
			}
		}
	}
	void drawFps(SDL_Renderer* renderer, TimeManager& timeManager)
	{
		fps.update(renderer, std::to_string(
			int(round(double(1)/timeManager.getLastAverage())))
		);
		SDL_Rect out = { 0,0,100,100 };
		fps.render(renderer, &out);
	}
	void drawHP(SDL_Renderer* renderer)
	{
		SDL_Rect out;
		SDL_Color color;
		if (player.getHP() < lastHP)
		{
			out = { 0,cfg.getScreenHeight() - 150,150,150 };
			color = { 255, 0, 0, 255 };
		}
		else
		{
			out = { 0,cfg.getScreenHeight() - 100,100,100 };
			color = { 255, 255, 255, 255 };
		}
		HP.update(renderer, std::to_string( player.getHP() ), color);
		HP.render(renderer, &out);
		lastHP = player.getHP();
	}
public:
	OverlayManager(Config& cfgRef, SDL_Renderer* renderer, Player& playerRef) : player(playerRef), cfg(cfgRef)
	{
		loadTextures(renderer);
		fps.create("font.ttf", 100);
		HP.create("font.ttf", 100);
		player.prepareTextureRects(cfg, 
			&gunStandard, 
			&gunAimed, 
			&gunRunning, 
			&fire, 
			cfg.getScreenWidth(), 
			cfg.getScreenHeight()
		);
		player.setFavorizedAnimation(4);
		lastHP = player.getHP();
	}
	void renderOverlay(SDL_Renderer* renderer, TimeManager& timeManager)
	{
		drawGun(renderer);
		drawFps(renderer, timeManager);
		drawHP(renderer);
	}
	void switchTexturesFrame()
	{
		gunRunning.nextFrame();
		fire.nextFrame();
		for (int i = 0; i < player.getShiningGunStandardAnimationCount(); i++)
		{
			shiningGunStandard[i].nextFrame();
		}
	}
	void deathScreen(SDL_Renderer* renderer)
	{	
		unsigned char brightness = SDL_ALPHA_TRANSPARENT;

		while (brightness < SDL_ALPHA_OPAQUE)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);
			death.setAlpha(brightness);
			death.render(renderer);
			SDL_RenderPresent(renderer);
			Sleep(10);
			brightness++;
		}
		SDL_Delay(2000);
	}
};