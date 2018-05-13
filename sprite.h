#pragma once
class Sprite : public GameObject
{
private:
	MyPoint center;
public:
	Sprite(MyPoint inCenter, bool stoppingRays, bool genCollisions,
		double inHeight,
		bool inVerticalPosition,
		int texture)
	{
		type = GAME_OBJECT_SPRITE;
		center = inCenter;
		stopsRays = stoppingRays;
		collisions = genCollisions;
		height = inHeight;
		verticalPosition = inVerticalPosition;
		textureIndex = texture;
	}
	void trans(Wektor vector)
	{
		center.trans(vector.x, vector.y);
	}
	MyPoint getCenter()
	{
		return center;
	}
	void calcDistanceFromObserverAndHandleCollisions(TimeManager& timeManager, Player& player)
	{
		if (timeToDestruction > 0)
		{
			timeToDestruction--;
			if (timeToDestruction == 0)
			{
				this->destroy();
				std::cout << "\nObject destroyed!";
			}
		}
		Wektor pushAway;
		AlternativeLine playersView;
		playersView.define(player.getA(), player.getB());
		distanceFromObserver = perpendicularDistance(playersView, center) + player.getObserverDistance();
		if (generatesCollisions() && pointPointDistance(player.getObserver(), center) < player.getCollisionRadius())
		{
			pushAway.defineFromPoints(center, player.getObserver());
			pushAway.setLength((player.getCollisionRadius() - distanceFromObserver));
			player.trans(pushAway);
		}
	}
	bool rayCrossing(Player& player, MyPoint screenPoint, MyPoint& crossing)
	{
		if (width == 0) return false;
		else
		{
			AlternativeLine ray;
			ray.define(player.getObserver(), screenPoint);
			double distanceFromCenter = perpendicularDistance(ray, center);
			if ( distanceFromCenter <= width / 2)
			{
				AlternativeLine AB, view;
				view.define(player.getA(), player.getB());
				AB.define(view.getA(), view.getB(), -view.getA()*center.x - view.getB()*center.y);
				if (intersection(ray, AB, crossing))
				{
					Wektor ObsCross, ObsScreen;
					ObsCross.defineFromPoints(player.getObserver(), crossing);
					ObsScreen.defineFromPoints(player.getObserver(), screenPoint);
					if (sameDirection(ObsCross, ObsScreen))
					{
						if (distanceFromCenter < width / 8)
						{
							recordSpotting();
						}
						return true;
					}
					else
					{
						crossing = { 0, 0 };
						return false;
					}
				}
				else
				{
					if (distanceFromCenter < width / 8)
					{
						recordSpotting();
					}
					return true;
				}
			}
			else
			{
				crossing = { 0,0 };
				return false;
			}
		}
	}
	Stripe createStripe(Config& cfg, Player& player, MyPoint screenPoint, int stripeIndex, MyPoint crossing, AnimatedTexture* textures)
	{
		Stripe finalStripe;
		double fullDistance;
		double percentageOfRealSize;
		int stripeVisualHeight;
		int defaultVisualHeight;
		int objectVisualWidth;
		int stripePositionInTexture;
		int stripeWidth;
		int alpha;

		Wektor OC;
		OC.defineFromPoints(player.getObserver(), crossing);
		AlternativeLine viewLine;
		viewLine.define(player.getA(), player.getB());
		SDL_Rect defaultRectIn, defaultRectOut, rectIn, rectOut;
		fullDistance = perpendicularDistance(viewLine, crossing) + player.getObserverDistance();
		percentageOfRealSize = (cfg.getScreenHeight()) /
			((cfg.getScreenHeight() / cfg.getObserverDistance()) *fullDistance);
		defaultVisualHeight = percentageOfRealSize * cfg.getScreenHeight() * cfg.getDefaultObjectsHeight();
		stripeVisualHeight = defaultVisualHeight * this->height;
		objectVisualWidth = percentageOfRealSize * this->getWidth();
		textures[this->getTextureIndex()].getW();
		stripeWidth = this->getWidth() / objectVisualWidth;
		alpha = percentageOfRealSize * SDL_ALPHA_OPAQUE * cfg.getBrightness();

		MyPoint leftEdgeOfSprite;
		Wektor left;
		left.defineFromPoints(player.getA(), player.getB());
		left.setLength(this->width / 2);
		leftEdgeOfSprite = { this->center.x + left.x, this->center.y + left.y };
		stripePositionInTexture = pointPointDistance(leftEdgeOfSprite, crossing) / this->width * textures[this->getTextureIndex()].getFrameWidth();
		if (alpha > SDL_ALPHA_OPAQUE)
		{
			alpha = SDL_ALPHA_OPAQUE;
		}
		if (stripePositionInTexture < 0 || stripePositionInTexture >  textures[this->getTextureIndex()].getFrameWidth()*cfg.getDefaultObjectsHeight())
		{
			rectIn = { 0,0,0,0 };
		}
		else
		{
			rectIn = { stripePositionInTexture - stripeWidth / 2, 0, stripeWidth, textures[this->getTextureIndex()].getFrameHeight() }; //stripeWidth
		}
		if (this->verticalPosition == ON_GROUND)
		{
			rectOut = { stripeIndex, cfg.getScreenHeight()/2 - defaultVisualHeight/2 + (defaultVisualHeight-stripeVisualHeight) , 1, stripeVisualHeight };
		}
		else
		{
			rectOut = { stripeIndex, cfg.getScreenHeight() / 2 - defaultVisualHeight / 2, 1, stripeVisualHeight };
		}
		finalStripe.create(textureIndex, fullDistance, rectIn, rectOut, alpha);
		return finalStripe;
	}
	double getWidth()
	{
		return width;
	}
	void printInfo()
	{
		printf("\n%lf %lf %d %d %lf %d %d %s",
			center.x, center.y,
			stopsRays, collisions, height, verticalPosition, textureIndex, "SPRITE");
	}
};