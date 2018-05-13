#pragma once
enum GAME_OBJECT_TYPES
{
	GAME_OBJECT_EMPTY,
	GAME_OBJECT_WALL,
	GAME_OBJECT_SPRITE
};

enum NearestPoint
{
	POINT_A,
	POINT_B,
	WALL_CENTER
};

enum VERTICAL_POSITION
{
	ON_GROUND,
	HIGH
};

class GameObject
{
protected:
	int type;
	bool stopsRays = false;
	bool collisions = false;
	double height = 0; //Multiplication of the default height;
	double width = 0;
	bool verticalPosition = false;
	int textureIndex = NULL;
	double distanceFromObserver = 0;
	bool spotted = false;
	int timeToDestruction = -1;
public:
	int getType()
	{
		return type;
	}
	virtual bool perpHitsObject(Player& player, MyPoint* crossing = nullptr)
	{
		std::cout << "\nPolimorphism alert.";
		return false;
	}
	virtual void trans(Wektor wektor)
	{
		std::cout << "\nPolimorphism alert.";
	}
	virtual void calcDistanceFromObserverAndHandleCollisions(TimeManager& timeManager, Player& player)
	{
		std::cout << "\nPolimorphism alert.";
	}
	virtual bool rayCrossing(Player& player, MyPoint screenPoint, MyPoint& crossing)
	{
		return false;
	}
	virtual MyPoint getCenter()
	{
		std::cout << "\nPolimorphism alert.";
		return { 0,0 };
	}
	Stripe createDefaultStripe(
		Config& cfg,
		Player& player,
		MyPoint screenPoint,
		int stripeIndex,
		MyPoint crossing,
		AnimatedTexture* textures
	)
	{
		Stripe defaultStripe;
		double fullDistance;
		double percentageOfRealSize;
		int stripeVisualHeight;
		int objectVisualWidth;
		int stripePositionInTexture;
		int stripeWidth;
		int alpha;
		AlternativeLine viewLine;
		viewLine.define(player.getA(), player.getB());
		SDL_Rect defaultRectIn, defaultRectOut, rectIn, rectOut;
		fullDistance = perpendicularDistance(viewLine, crossing) + player.getObserverDistance();
		percentageOfRealSize = (cfg.getScreenHeight()) /
			((cfg.getScreenHeight() / cfg.getObserverDistance()) *fullDistance);
		stripeVisualHeight = percentageOfRealSize * cfg.getScreenHeight() * cfg.getDefaultObjectsHeight();
		objectVisualWidth = percentageOfRealSize * this->getWidth();
		stripePositionInTexture = pointPointDistance(crossing, this->getPointA()) / this->getWidth() *
			textures[this->getTextureIndex()].getW();
		stripeWidth = this->getWidth() / objectVisualWidth;
		alpha = percentageOfRealSize * SDL_ALPHA_OPAQUE * 3;
		if (alpha > SDL_ALPHA_OPAQUE)
		{
			alpha = SDL_ALPHA_OPAQUE;
		}
		defaultRectIn = { stripePositionInTexture - stripeWidth / 2, 0, stripeWidth, textures[this->getTextureIndex()].getH() };
		defaultRectOut = { stripeIndex, cfg.getScreenHeight() / 2 - stripeVisualHeight / 2, 1, stripeVisualHeight };
		defaultStripe.create(this->getTextureIndex(), fullDistance, defaultRectIn, defaultRectOut, alpha);
		return defaultStripe;
	}
	virtual Stripe createStripe(
		Config& cfg,
		Player& player,
		MyPoint screenPoint, 
		int stripeIndex, 
		MyPoint crossing, 
		AnimatedTexture* textures
	)
	{
		std::cout << "\nPolimorphism alert.";
		Stripe empty;
		return empty;
	}
	virtual MyPoint getPointA()
	{
		std::cout << "\nPolimorphism alert.";
		return { 0,0 };
	}
	double getDistanceFromObserver()
	{
		return distanceFromObserver;
	}
	bool stoppingRays()
	{
		return stopsRays;
	}
	bool generatesCollisions()
	{
		return collisions;
	}
	double getHeight()
	{
		return height;
	}
	double getWidth()
	{
		return width;
	}
	bool getVerticalPosition()
	{
		return verticalPosition;
	}
	void setTextureIndex(int index)
	{
		textureIndex = index;
	}
	int getTextureIndex()
	{
		return textureIndex;
	}
	void recordSpotting()
	{
		spotted = true;
	}
	bool wasSpotted()
	{
		return spotted;
	}
	void clearSpottings()
	{
		spotted = false;
	}
	virtual void printInfo()
	{
		std::cout << "\nPolimorphism alert.";
	}
	void setWidth(double inWidth)
	{
		width = inWidth;
	}
	void destroy()
	{
		width = 0;
		height = 0;
		collisions = false;
		stopsRays = false;
	}
	void orderDestruction()
	{
		timeToDestruction = 500;
	}
};