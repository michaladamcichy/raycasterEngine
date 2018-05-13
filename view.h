#pragma once
enum ViewDirections
{
	FORWARD,
	BACKWARDS,
	LEFT,
	RIGHT
};

class View
{
private:
	MyPoint pointA;
	MyPoint pointB;
	MyPoint observer;
	MyPoint center;

	Wektor frontDirection;
	Wektor sideDirection;
	double deltaAngle;
	double angle;

	int screenWidth;
	int screenHeight;
	int observerDistance;
protected:
	double radius;
public:
	View(int inScreenWidth, int inScreenHeight, int AB_DISTANCE, int inObserverDistance)
	{
		screenWidth = inScreenWidth;
		screenHeight = inScreenHeight;
		observerDistance = inObserverDistance;
		observer.define(0, 0);
		pointA.define(AB_DISTANCE / 2, observerDistance);
		pointB.define(-AB_DISTANCE / 2, observerDistance);
		radius = pointPointDistance(pointA, observer);
		deltaAngle = (acos((2 * radius*radius - screenWidth * screenWidth) / (2 * radius*radius)));
		angle = 0.376991;
	}
	void updateDirection()
	{
		MyPoint middle;
		middle.define((pointA.x + pointB.x) / 2, (pointA.y + pointB.y) / 2);
		frontDirection.define((middle.x - observer.x) / observerDistance,
			(middle.y - observer.y) / observerDistance);
		frontDirection =
		{
			frontDirection.x / frontDirection.getLength(),
			frontDirection.y / frontDirection.getLength()
		};
		sideDirection.define((pointA.x - pointB.x) / screenWidth,
			(pointA.y - pointB.y) / screenWidth);
		sideDirection =
		{
			sideDirection.x / sideDirection.getLength(),
			sideDirection.y / sideDirection.getLength()
		};
	}

	void translate(TimeManager& timeManager, char dir, double howMuch)
	{
		howMuch *= timeManager.getLastFrame() / timeManager.getFrameAssumedTime();
		char sign;
		Wektor direction;
		switch (dir)
		{
		case FORWARD: {sign = 1; direction = frontDirection; }; break;
		case BACKWARDS: {sign = -1; direction = frontDirection; }; break;
		case LEFT: {sign = 1; direction = sideDirection; }; break;
		case RIGHT: {sign = -1; direction = sideDirection; }; break;
		}
		Wektor value = { sign * howMuch * direction.x , sign * howMuch * direction.y };
		observer.x += value.x;
		observer.y += value.y;
		pointA.x += value.x;
		pointA.y += value.y;
		pointB.x += value.x;
		pointB.y += value.y;
	}
	Wektor getTranslationVector(char dir, double howMuch)
	{
		Wektor translationVector;
		char sign;
		Wektor direction;
		switch (dir)
		{
		case FORWARD: {sign = 1; direction = frontDirection; }; break;
		case BACKWARDS: {sign = -1; direction = frontDirection; }; break;
		case LEFT: {sign = 1; direction = sideDirection; }; break;
		case RIGHT: {sign = -1; direction = sideDirection; }; break;
		}
		translationVector.x = sign * howMuch * direction.x;
		translationVector.y = sign * howMuch * direction.y;
		return translationVector;
	}
	void rotate(TimeManager& timeManager, double inputAngle)
	{
		angle += inputAngle * timeManager.getLastFrame() / timeManager.getFrameAssumedTime();
		if (angle > 2 * PI) angle -= 2 * PI;
		if (angle <-2 * PI) angle += 2 * PI;


		MyPoint abstractA, abstractB, abstractObserver;
		abstractA.define(pointA.x - observer.x, pointA.y - observer.y);
		abstractB.define(pointA.x - observer.x, pointA.y - observer.y);
		abstractObserver.define(0, 0);

		pointA.x = cos(angle)*radius + observer.x;
		pointA.y = sin(angle)*radius + observer.y;
		pointB.x = cos(angle + deltaAngle)*radius + observer.x;
		pointB.y = sin(angle + deltaAngle)*radius + observer.y;
		center = triangleMiddle(pointA, pointB, observer);
	}
	void trans(Wektor move)
	{
		observer.x += move.x;
		observer.y += move.y;
		pointA.x += move.x;
		pointA.y += move.y;
		pointB.x += move.x;
		pointB.y += move.y;
		center.x += move.x;
		center.y += move.y;
	}
	MyPoint getObserver()
	{
		return observer;
	}
	MyPoint getA()
	{
		return pointA;
	}
	MyPoint getB()
	{
		return pointB;
	}
	MyPoint getCenter()
	{
		return center;
	}
	double getAngle()
	{
		return angle;
	}
	double getRadius()
	{
		return radius;
	}
	double getObserverDistance()
	{
		return observerDistance;
	}
	AlternativeLine getViewLine()
	{
		AlternativeLine line;
		line.define(this->pointA, this->pointB);
		return line;
	}
};