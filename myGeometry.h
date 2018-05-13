#pragma once
int8_t sgn(double value)
{
	if (value > 0) return 1;
	else if (value == 0) return 0;
	else return -1;
}
class MyPoint
{
public:
	double x;
	double y;
	void define(double inX, double inY)
	{
		x = inX;
		y = inY;
	}
	void trans(double inX, double inY)
	{
		x += inX;
		y += inY;
	}
};

class Wektor
{
public:
	double x;
	double y;
	double length;
	void define(double inX, double inY)
	{
		x = inX;
		y = inY;
		length = sqrt(x*x + y * y);
	}
	void defineFromPoints(MyPoint A, MyPoint B)
	{
		x = B.x - A.x;
		y = B.y - A.y;
		length = sqrt(x*x + y * y);
	}
	double getLength()
	{
		return length;
	}
	Wektor getVersor()
	{
		Wektor wektor = { x, y };
		if (length > 0)
		{
			wektor.x /= length;
			wektor.y /= length;
			wektor.length = 1;
			return wektor;
		}
		else return { 0,0 };
	}
	void scale(double multiplier)
	{
		x *= multiplier;
		y *= multiplier;
		length *= multiplier;
	}
	void setLength(double inLength)
	{
		Wektor versor = getVersor();
		x = versor.x * inLength;
		y = versor.y * inLength;
		length = inLength;
	}
};

double roundCent(double x)
{
	x *= 100;
	x = int(x);
	x /= 100;
	return x;
}
MyPoint roundPoint(MyPoint point)
{
	return { roundCent(point.x), roundCent(point.y) };
}

MyPoint scalePoint(MyPoint point, double scale)
{
	return { point.x * scale, point.y * scale };
}

bool equalPoints(MyPoint A, MyPoint B)
{
	if (A.x == B.x && A.y == B.y)
	{
		return true;
	}
	else
		return false;
}

double pointPointDistance(MyPoint A, MyPoint B)
{
	return sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y));
}

double pointPointDistanceFast(MyPoint A, MyPoint B)
{
	return (A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y);
}

class MyLine
{
private:
	double a;
	double b;
	bool vertical;
	double verticalX;
public:
	void define(MyPoint A, MyPoint B)
	{
		if (A.x == B.x)
		{
			if (A.y == B.y)
			{
				printf("Points %lf %lf and %lf %lf are the same. Line creating error.\n",
					A.x, A.y, B.x, B.y);
			}
			vertical = true;
			verticalX = A.x;
		}
		else
		{
			vertical = false;
			verticalX = NULL;
			a = (A.y - B.y) / (A.x - B.x);
			b = A.y - a * A.x;
		}
	}
	double getA()
	{
		return a;
	}
	double getB()
	{
		return b;
	}
	bool isVertical()
	{
		return vertical;
	}

	double getVerticalX()
	{
		return verticalX;
	}
};


class AlternativeLine
{
public:
	double A;
	double B;
	double C;
	void define(MyPoint pointA, MyPoint pointB)
	{
		MyLine standard;
		standard.define(pointA, pointB);
		if (standard.isVertical())
		{
			A = 1;
			B = 0;
			C = -standard.getVerticalX();
		}
		else
		{
			A = standard.getA();
			B = -1;
			C = standard.getB();
		}
	}
	void define(double inA, double inB, double inC)
	{
		A = inA;
		B = inB;
		C = inC;
	}
	double getA()
	{
		return A;
	}
	double getB()
	{
		return B;
	}
	double getC()
	{
		return C;
	}
};

bool intersection(AlternativeLine line1, AlternativeLine line2, MyPoint& intersect)
{
	return (cramer(line1.getA(), line1.getB(), -1 * line1.getC(),
		line2.getA(), line2.getB(), -1 * line2.getC(),
		intersect.x, intersect.y));
}

double perpendicularDistance(AlternativeLine line, MyPoint inputPoint)
{
	return (abs(line.A*inputPoint.x + line.B*inputPoint.y + line.C) / sqrt(line.A*line.A + line.B*line.B));
}

bool isBetweenPoints(MyPoint point, MyPoint inA, MyPoint inB)
{
	double precision = 0;
	MyPoint A, B;
	A = inA;
	B = inB;
	if (pointPointDistanceFast(point, A) + precision > pointPointDistanceFast(A, B)
		|| pointPointDistanceFast(point, B) + precision > pointPointDistanceFast(A, B))
	{
		return false;
	}
	else return true;
}

bool sameDirection(Wektor inA, Wektor inB)
{
	Wektor A, B;
	A.define(sgn(inA.x), sgn(inA.y));
	B.define(sgn(inB.x), sgn(inB.y));
	if (A.x == B.x && A.y == B.y)
	{
		return true;
	}
	else return false;
}

MyPoint triangleMiddle(MyPoint A, MyPoint B, MyPoint C)
{
	MyPoint middle;

	if (cramer(2 * (B.x - A.x), 2 * (B.y - A.y), B.x*B.x + B.y*B.y - A.x*A.x - A.y*A.y,
		2 * (B.x - C.x), 2 * (B.y - C.y), B.x*B.x + B.y*B.y - C.x*C.x - C.y*C.y,
		middle.x, middle.y))
	{
		return middle;
	}
	else
	{
		printf("\nFailed to calculate middle of the triangle");
		return { 0,0 };
	}
}


AlternativeLine transLine(AlternativeLine line, Wektor vec)
{
	line.C += vec.x * line.A + vec.y *line.B;
	return line;
}