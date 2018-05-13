#pragma once
class Stripe
{
private:
	int texture;
	SDL_Rect rectIn;
	SDL_Rect rectOut;
	double distance;
	unsigned char alpha;

public:
	Stripe()
	{
	}
	Stripe(
		int inTexture, 
		double inDistance, 
		SDL_Rect in = { 0,0,0,0 }, 
		SDL_Rect out = { 0,0,0,0 }, 
		unsigned char inAlpha = SDL_ALPHA_OPAQUE
	)
	{
		texture = inTexture;
		rectIn = in;
		rectOut = out;
		distance = inDistance;
		alpha = inAlpha;
	}
	void create(
		int inTexture, 
		double inDistance, 
		SDL_Rect in = { 0,0,0,0 }, 
		SDL_Rect out = { 0,0,0,0 }, 
		unsigned char inAlpha = SDL_ALPHA_OPAQUE
	)
	{
		texture = inTexture;
		rectIn = in;
		rectOut = out;
		distance = inDistance;
		alpha = inAlpha;
	}
	void render(SDL_Renderer* renderer, AnimatedTexture* textures)
	{
		textures[texture].setBrightness(alpha);
		textures[texture].render(renderer, &rectIn, &rectOut);
	}
	SDL_Rect getRectIn()
	{
		return rectIn;
	}
	SDL_Rect getRectOut()
	{
		return rectOut;
	}
	unsigned char getAlpha()
	{
		return alpha;
	}
	int getTexture()
	{
		return texture;
	}
	double getDistance()
	{
		return distance;
	}
	bool operator < (Stripe& stripe)
	{
		return distance < stripe.getDistance();
	}
};