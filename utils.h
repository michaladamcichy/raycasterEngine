#pragma once
bool equal(double a, double b)
{
	a *= 100;
	b *= 100;
	a = int(a);
	b = int(b);
	a /= 100;
	b /= 100;

	return (a == b);
}

double max3(double a, double b, double c)
{
	if (a > b)
	{
		if (a > c) return a;
		else return c;
	}
	else
	{
		if (b> c) return b;
		else return c;
	}
}
double min3(double a, double b, double c, int& smallestID)
{
	if (a < b)
	{
		if (a < c)
		{
			smallestID = 0;
			return a;
		}
		else
		{
			smallestID = 2;
			return c;
		}
	}
	else
	{
		if (b < c)
		{
			smallestID = 1;
			return b;
		}
		else
		{
			smallestID = 2;
			return c;
		}
	}
}
bool isFileEmpty(std::string path)
{
	std::ifstream in;
	in.open(path.c_str(), std::ios::ate | std::ios::binary);
	return !in.tellg();
}

std::string lastWordFromPath(std::string path)
{
	if (path.size() == 0)
	{
		return "";
	}
	else
	{
		std::string last;
		int index = path.size() - 1;

		while (path[index] != '/' && index >= 0)
		{
			index--;
		}
		if (index + 1 < path.size())
		{
			index++;
		}
		last = path.substr(index, path.size() - index);
		return last;
	}
}


