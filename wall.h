#pragma once
class Wall : public GameObject
{
private:
	MyPoint center;
	MyPoint pointA;
	MyPoint pointB;
public:
	Wall(MyPoint A = { 0,0 }, MyPoint B = { 0,0 },
		bool stoppingRays = false,
		bool genCollisions = false,
		double inHeight = 1,
		bool inVerticalPosition = ON_GROUND,
		int texture = 0)
	{
		type = GAME_OBJECT_WALL;
		pointA = A;
		pointB = B;
		stopsRays = stoppingRays;
		collisions = genCollisions;
		height = inHeight;
		setWidth(pointPointDistance(A, B));
		verticalPosition = inVerticalPosition;
		textureIndex = texture;
	}
	MyPoint getPointA()
	{
		return pointA;
	}
	MyPoint getPointB()
	{
		return pointB;
	}
	bool perpHitsWall(Player& player, MyPoint* crossing = nullptr)
	{
		AlternativeLine wallLine;
		wallLine.define(pointA, pointB);
		double perpDist = perpendicularDistance(wallLine, player.getObserver());
		double OA = pointPointDistance(player.getObserver(), pointA);
		if (perpDist == OA)
		{
			*crossing = pointA;
			return true;
		}
		else
		{
			double AX = sqrt(OA*OA - perpDist * perpDist);
			Wektor wektorAX;
			wektorAX.defineFromPoints(pointA, pointB);
			wektorAX.x *= AX / pointPointDistance(pointA, pointB);
			wektorAX.y *= AX / pointPointDistance(pointA, pointB);

			MyPoint potCrossing = { pointA.x + wektorAX.x, pointA.y + wektorAX.y };
			if (pointPointDistance(player.getObserver(), potCrossing) > OA)
			{
				potCrossing = { pointA.x - wektorAX.x, pointA.y - wektorAX.y };
			}
			Wektor toleranceVector = { 0,0 };

			if (
				isBetweenPoints(potCrossing, { pointA.x - toleranceVector.x, pointA.y - toleranceVector.y },
				{ pointB.x + toleranceVector.x, pointB.y + toleranceVector.y })
			)
			{
				*crossing = potCrossing;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	void calcDistanceFromObserverAndHandleCollisions(TimeManager& timeManager, Player& player)
	{
		Wektor pushAway;
		AlternativeLine AB;
		AB.define(pointA, pointB);
		double perpDist = perpendicularDistance(AB, player.getObserver());
		MyPoint wallCenter = { (pointA.x + pointB.x) / 2, (pointA.y + pointB.y) / 2 };
		MyPoint crossing;
		if (perpHitsWall(player, &crossing))
		{
			distanceFromObserver = perpDist;
			if (generatesCollisions() && distanceFromObserver < player.getCollisionRadius())
			{
				player.recordCollision();
				pushAway.defineFromPoints(crossing, player.getObserver());
				pushAway.setLength((player.getCollisionRadius() - distanceFromObserver));
				player.trans(pushAway);
			}
		}
		else
		{
			int nearestPoint;
			distanceFromObserver = min3(pointPointDistance(player.getObserver(), wallCenter),
				pointPointDistance(player.getObserver(), pointA),
				pointPointDistance(player.getObserver(), pointB), nearestPoint);
			if (generatesCollisions() && distanceFromObserver < player.getCollisionRadius())
			{
				player.recordCollision();
				switch (nearestPoint)
				{
				case POINT_A: crossing = pointA; break;
				case POINT_B: crossing = pointB; break;
				case WALL_CENTER: crossing = wallCenter; break;
				default: break;
				}
				pushAway.defineFromPoints(crossing, player.getObserver());
				pushAway.setLength( (player.getCollisionRadius() - distanceFromObserver));
				player.trans(pushAway);
			}
		}
	}
	bool rayCrossing(Player& player, MyPoint screenPoint, MyPoint& crossing)
	{
		MyPoint crossingTemp;
		Wektor ObScr, ObCross;
		AlternativeLine segment;
		segment.define(pointA, pointB);
		MyPoint rayStart = player.getObserver();
		AlternativeLine ray;
		ray.define(rayStart, screenPoint);

		if (!cramer(ray.A, ray.B, -ray.C, segment.A, segment.B, -segment.C, crossingTemp.x, crossingTemp.y))
		{
			return false;
		}
		else
		{
			ObScr.defineFromPoints(rayStart, crossingTemp);
			ObCross.defineFromPoints(rayStart, screenPoint);
			if (sameDirection(ObScr, ObCross))
			{
				if (isBetweenPoints(crossingTemp, pointA, pointB))
				{
					crossing = crossingTemp;
					return true;
				}
				else
				{
					crossing = { 0,0 };
					return false;
				}
			}
			else
			{
				crossing = { 0,0 };
				return false;
			}
		}
	}
	Stripe createStripe(
		Config& cfg, 
		Player& player, 
		MyPoint screenPoint, 
		int stripeIndex, 
		MyPoint crossing, 
		AnimatedTexture* textures
	)
	{
		Stripe defaultStripe, finalStripe;
		defaultStripe = createDefaultStripe(cfg, player, screenPoint, stripeIndex, crossing, textures);
		SDL_Rect rectIn = defaultStripe.getRectIn();
		SDL_Rect rectOut = defaultStripe.getRectOut();

		rectOut.h *= this->height;

		if (this->verticalPosition == ON_GROUND)
		{
			rectOut.y = defaultStripe.getRectOut().y - (rectOut.h - defaultStripe.getRectOut().h);
		}
		else
		{
			rectOut.y = defaultStripe.getRectOut().y;
		}
		finalStripe.create(textureIndex, defaultStripe.getDistance(), rectIn, rectOut, defaultStripe.getAlpha());
		return finalStripe;
	}

	void printInfo()
	{
		printf("\n%lf %lf %lf %lf %d %d %lf %d %d %s",
			pointA.x, pointA.y, pointB.x, pointB.y,
			stopsRays, collisions, height, verticalPosition, textureIndex, "WALL");
	}
};
