#pragma once
class EnemiesGarrison
{
private:
	std::vector <Enemy> enemies;
	Config& cfg;
	Player& player;
	GameWorld* gameWorld;
	AnimatedTexture* textures;
	int firstTextureIndex;
	int texturesCount;
	int timer = 10;
public:
	EnemiesGarrison(Config& cfg, GameWorld* gameWorld, Player& player, AnimatedTexture* textures, int firstTextureIndex, int texturesCount);
	void loadEnemies(std::string path);
	void addEnemy(MyPoint initialPos);
	void AI(TimeManager& timeManager);
	void handleShots(Config& cfg, Player& player);
};

EnemiesGarrison::EnemiesGarrison(
	Config& cfgRef,
	GameWorld* gameWorldRef,
	Player& playerRef,
	AnimatedTexture* textures,
	int firstTextureIndex,
	int texturesCount) :
	cfg(cfgRef), player(playerRef)
{
	gameWorld = gameWorldRef;
	this->textures = textures;
	this->firstTextureIndex = firstTextureIndex;
	this->texturesCount = texturesCount;
}

void EnemiesGarrison::loadEnemies(std::string path)
{
	std::cout << "\nLoading enemies...";
	std::ifstream input;
	input.open(path);
	if (!input.good())
	{
		printf("\nUnable to open %s", path.c_str());
	}
	else
	{
		if (!isFileEmpty(path))
		{
			MyPoint temp;
			while (!input.eof())
			{
				input >> temp.x >> temp.y;
				this->addEnemy(temp);
				std::cout << "\nEnemies' garrison consists of " << enemies.size() << "enemies now.";
			}
		}
		else
		{
			std::cout << "\nThere are no enemies defined in "<<path;
		}
	}
	input.close();
}

void EnemiesGarrison::addEnemy(MyPoint initialPos)
{
	Enemy enemy(gameWorld, initialPos, firstTextureIndex, texturesCount, textures);
	enemies.push_back(enemy);
}

void EnemiesGarrison::AI(TimeManager& timeManager)
{
	for (auto& enemy : enemies)
	{
		enemy.AI(cfg, timeManager, player);
	}
}

void EnemiesGarrison::handleShots(Config& cfg, Player& player)
{
	AlternativeLine shootingLine;
	MyPoint viewLineMiddle = { (player.getA().x + player.getB().x) / 2, (player.getA().y + player.getB().y) / 2 };
	shootingLine.define(player.getObserver(), viewLineMiddle);

	std::vector<Enemy>::iterator it = enemies.begin();
	std::vector<std::vector<Enemy>::iterator> enemiesHit;

	while (it != enemies.end())
	{
		if (it->isHit(cfg, shootingLine, player) )
		{
			enemiesHit.push_back( it );
		}
		it++;
	}
	if (!enemiesHit.empty())
	{
		std::vector<Enemy>::iterator closestEnemyHit = enemiesHit[0];
		for (std::vector<Enemy>::iterator compared : enemiesHit)
		{
			if (compared->getDistanceFromObserver() < closestEnemyHit->getDistanceFromObserver())
			{
				closestEnemyHit = compared;
			}
		}
		if (closestEnemyHit->hurt(cfg, cfg.getShotPower()) == JUST_DIED)
		{
			enemies.erase(closestEnemyHit);
		}
	}

	if (enemies.size() < 10 && cfg.enemiesAutoSpawnEnabled())
	{
		timer--;
		if (timer <= 0)
		{
			MyPoint randomPoint;
			Wektor pushAway;
			for (int i = 0; i < 5; i++)
			{
				randomPoint = { double(rand() % 100000 - 50000), double(rand() % 100000 - 50000) };
				if (pointPointDistance(randomPoint, player.getObserver()) < cfg.getPlayerVisibilityDistance() / 15)
				{
					pushAway.defineFromPoints(player.getObserver(), randomPoint);
					pushAway.setLength(cfg.getPlayerVisibilityDistance() / 15);
					randomPoint.trans(pushAway.x, pushAway.y);
				}
				addEnemy(randomPoint);
				std::cout << "\nEnemy spawned at " << randomPoint.x << " " << randomPoint.y;
			}
			timer = 500;
		}
	}
};