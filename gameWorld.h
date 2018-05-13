#pragma once
class GameWorld
{
private:
	GameObject * objects[10000];
	Config& cfg;
	int objectsCount = 0;
	std::vector <std::string> wallTextures;
	std::vector <std::string> spriteTextures;
	std::vector <std::string> independentTextures;
public:
	GameWorld(Config& cfgRef) : cfg(cfgRef)
	{
	}
	void freeResources()
	{
		for (int i = 0; i < objectsCount; i++)
		{
			delete objects[i];
		}
	}
	int getObjectsCount()
	{
		return objectsCount;
	}
	GameObject* getObject(int index)
	{
		return objects[index];
	}
	GameObject* addNewEnemy(MyPoint center, int texture, AnimatedTexture* textures)
	{
		if (objectsCount >= cfg.getGameObjectsLimit())
		{
			std::cout << "\nObjects limit exceeded!!!";
			return nullptr;
		}
		else
		{
			objects[objectsCount++] = new Sprite(center, false, false, 0.8, ON_GROUND, texture);
			objects[objectsCount - 1]->setWidth(
				textures[objects[objectsCount - 1]->getTextureIndex()].getFrameWidth() *
				cfg.getDefaultObjectsHeight() *
				objects[objectsCount - 1]->getHeight()
			);
			return objects[objectsCount - 1];
		}
	}

	void loadObjects(std::string path)
	{
		std::ifstream input;
		input.open(path.c_str());

		if (!input.good())
		{
			printf("Failed to open file with objects");
		}
		else
		{
			if (!isFileEmpty(path))
			{
				GameObject* object;
				std::string type;
				MyPoint A, B;
				MyPoint center;
				bool flags[2];
				bool vertPos;
				double h;
				std::string textureName;
				int textureIndex;
				std::vector <std::string> ::iterator it;
				while (!input.eof())
				{
					input >> type;

					if (type == "WALL")
					{
						std::cout << "\nWall object loaded ";
						input >> A.x >> A.y >> B.x >> B.y >> flags[0] >> flags[1] >> h >> vertPos >> textureName;
						it = find(wallTextures.begin(), wallTextures.end(), textureName);
						if (it == wallTextures.end())
						{
							wallTextures.push_back(textureName);
							textureIndex = wallTextures.size() - 1;
						}
						else
						{
							textureIndex = std::distance(wallTextures.begin(), it);
						}
						object = new Wall(A, B, flags[0], flags[1], h, vertPos, textureIndex);
						objects[objectsCount++] = object;
						std::cout << textureName << " \n";
					}
					else if (type == "SPRITE")
					{
						std::cout << "\nSprite object loaded ";
						input >> center.x >> center.y >> flags[0] >> flags[1] >> h >> vertPos >> textureName;
						it = find(spriteTextures.begin(), spriteTextures.end(), textureName);
						if (it == spriteTextures.end())
						{
							spriteTextures.push_back(textureName);
							textureIndex = spriteTextures.size() - 1;
						}
						else
						{
							textureIndex = std::distance(spriteTextures.begin(), it);
						}
						objects[objectsCount++] = new Sprite(center, flags[0], flags[1], h, vertPos, textureIndex);
						std::cout << textureName << " \n";
					}
					else break;
				}
				for (int i = 0; i < objectsCount; i++)
				{
					if (objects[i]->getType() == GAME_OBJECT_SPRITE)
					{
						objects[i]->setTextureIndex(objects[i]->getTextureIndex() + wallTextures.size());
					}
				}
			}
			input.close();
		}
		input.open("enemiesTextures.txt");
		if (!input.good())
		{
			printf("\nCannot open enemiesTextures.txt");
		}
		else
		{
			std::string temp;
			while (!input.eof())
			{
				input >> temp;
				independentTextures.push_back(temp);
			}
			input.close();
		}
	}
	void sortObjects()
	{
		int index, i, j;
		int n = objectsCount;

		for (i = 0; i < n - 1; i++)
		{
			index = i;
			j = i + 1;
			while (objects[j]->getDistanceFromObserver() < objects[index]->getDistanceFromObserver())
			{
				std::swap(objects[j], objects[index]);
				index--;
				j--;
				if (index<0)
				{
					break;
				}
			}
		}
	}
	std::vector <std::string> getWallTextures()
	{
		return wallTextures;
	}
	std::vector <std::string> getSpriteTextures()
	{
		return spriteTextures;
	}
	std::vector <std::string> getIndependentTextures()
	{
		return independentTextures;
	}
	void printObjectsData()
	{
		std::cout << "\nOBJECTS' COUNT: " << objectsCount << std::endl;
		for (int i = 0; i< objectsCount; i++)
		{
			objects[i]->printInfo();
		}
	}
	void updateSpritesWidth(Config& cfg, AnimatedTexture* textures)
	{
		for (int i = 0; i < objectsCount; i++)
		{
			if (objects[i]->getType() == GAME_OBJECT_SPRITE)
			{
				objects[i]->setWidth(
					textures[objects[i]->getTextureIndex()].getFrameWidth() * 
					cfg.getDefaultObjectsHeight() * 
					objects[i]->getHeight() * 2
				);
				//Not a mistake. DefaultObjectsHeight is how much is everything stretched vertically.
				//So sprites should be widened, and now they look normally.
			}
		}
	}
	void clearSpottings()
	{
		for (int i = 0; i < objectsCount; i++)
		{
			objects[i]->clearSpottings();
		}
	}
};