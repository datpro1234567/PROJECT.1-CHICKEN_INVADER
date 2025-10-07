#pragma once
#include <SFML/Graphics.hpp>

#include <random>

#include "Player.h"

class Bullet
{
public:
	Bullet(bool fromplayer,float damage,float speed, sf::Vector2f pos);

	void setFromPlayer(bool fromplayer);
	bool getFromPlayer();
	int getAlive();
	sf::CircleShape& getShape();
	float getDamage();
	void setAlive(int alive);
	void setOwnwer(Player* owener);
	Player* getOwner();
	float getRange();
	void setRange(float range);
	float getRangeHolder();
	void setRangeHolder(float rangeHolder);
	void setColor(sf::Color color);
	void setSplash(bool splash);
	bool getSplash();
	void setSplashArea(float splashArea)
	{
		this->splashArea = splashArea;
	}
	float getSplashArea()
	{
		return splashArea;
	}

	void move(float dt);
	void reachBorder(float width);
	void reachRange(Player*& player, float dt);

private:
	sf::CircleShape shape;
	Player* owener;
	float damage, speed,range, rangeHolder;
	bool fromPlayer;
	int alive;
	bool splash;
	float splashArea;
};

struct Splash
{
private:
	sf::CircleShape shape;
	float damage;
	float splashTime = 0.2;
	float splashTimeCounter = 0;
	float splashArea;
	bool alive = true;
public:
	Splash(Bullet bullet,float splashArea);

	bool getAlive()
	{
		return alive;
	}
	void setAlive(bool alive)
	{
		this->alive = alive;
	}
	void setBase(float base);
	float getBase();
	void setDamage(float damage);
	float getDamage();
	void setShape(sf::CircleShape shape);
	sf::CircleShape& getShape();
	float getSplashTime();
	float getSplashTimeCounter();
	void setsplashTimeCounter(float dt);
};