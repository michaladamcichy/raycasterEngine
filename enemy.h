#pragma once
enum EnemiesTextures
{
	ENEMY_STANDARD_TEXTURE,
	ENEMY_HURT_TEXTURE,
	ENEMY_DEAD_TEXTURE
};

class Enemy
{
public:
	GameWorld* gameWorld;
	GameObject * visibleObject;
	MyPoint currentPos;
	bool startedChasing = false;
	MyPoint lastSeenPlayerPos;
	int health = 100;
	std::vector <int> textures;
	int textureTimer = 0;
public:
	Enemy(GameWorld* gameWorld, MyPoint position, int firstTextIndex, int texCount, AnimatedTexture* aTex);
	~Enemy();
	void trans(Wektor vector);
	bool hurt(Config& cfg, int damage);
	MyPoint getCurrentPos();
	void setPos(MyPoint point);
	void AI(Config& cfg, TimeManager& timeManager, Player& player);
	bool isHit(Config& cfg, AlternativeLine& line, Player& player);
	void switchTexture();
	double getDistanceFromObserver();
	bool operator < (Enemy& enemy);
	
};

Enemy::Enemy(GameWorld* gameWorldRef, MyPoint position, int firstTextIndex, int texCount, AnimatedTexture* aTex)
{
	gameWorld = gameWorldRef;
	currentPos = position;
	for (int i = 0; i < texCount; i++)
	{
		textures.push_back(firstTextIndex + i);
	}
	visibleObject = gameWorld->addNewEnemy(position, textures[ENEMY_STANDARD_TEXTURE], aTex);
	lastSeenPlayerPos = currentPos;
	printf("\nNew enemy created");
}
Enemy::~Enemy() {}

void Enemy::trans(Wektor vector)
{
	visibleObject->trans(vector);
	currentPos.trans(vector.x, vector.y);
}

void Enemy::AI(Config& cfg, TimeManager &timeManager, Player& player)
{
	if (textureTimer > 0)
	{
		textureTimer--;
		if (textureTimer == 0)
		{
			visibleObject->setTextureIndex(textures[ENEMY_STANDARD_TEXTURE]);
		}
	}
	Wektor move;
	double distanceFromPlayer;
	distanceFromPlayer = pointPointDistance(player.getObserver(), this->currentPos);
	if (!startedChasing &&
		visibleObject->wasSpotted() &&
		distanceFromPlayer < cfg.getPlayerVisibilityDistance())
	{
		startedChasing = true;
	}
	if (startedChasing == true)
	{
		lastSeenPlayerPos = player.getObserver();
		if (distanceFromPlayer > player.getCollisionRadius() * 0.8)
		{
			move.defineFromPoints(this->currentPos, lastSeenPlayerPos);
			move.setLength(cfg.getEnemiesSpeed()* timeManager.getLastFrame() / timeManager.getFrameAssumedTime());
			this->trans(move);
		}
		if (distanceFromPlayer < player.getCollisionRadius())
		{
			player.hurt(cfg.getEnemiesStrength());
		}
	}
}

bool Enemy::hurt(Config& cfg, int damage)
{
	health -= cfg.getShotPower();
	if (health <= 0)
	{
		visibleObject->setTextureIndex(textures[ENEMY_DEAD_TEXTURE]);
		std::cout << "\nEnemy died";
		visibleObject->orderDestruction();
		return JUST_DIED;
	}
	visibleObject->setTextureIndex(textures[ENEMY_HURT_TEXTURE]);
	textureTimer = cfg.getEnemyHurtTextureDuration();
	return STILL_ALIVE;
}

bool Enemy::isHit(Config& cfg, AlternativeLine& shootingLine, Player& player)
{
	if ( perpendicularDistance(shootingLine, this->currentPos) < 
		visibleObject->getWidth()/2 * cfg.getEnemiesFragilePercentage() )
	{
		AlternativeLine playerEnemy;
		playerEnemy.define(player.getObserver(), visibleObject->getCenter());
		MyPoint intersect;
		if ( intersection(player.getViewLine(), playerEnemy, intersect) ) 
		{
			if ( isBetweenPoints(intersect, player.getObserver(), visibleObject->getCenter()) )
			{
				return true;
			}
		}
	}
	return false;
}
double Enemy::getDistanceFromObserver()
{
	return visibleObject->getDistanceFromObserver();
}

bool Enemy::operator < (Enemy& enemy)
{
	return this->getDistanceFromObserver() < enemy.getDistanceFromObserver();
}