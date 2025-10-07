#pragma once
#include <SFML/Graphics.hpp>

#include <random>

#include "Bullet.h"

class Enemy
{
public:
	Enemy(sf::Vector2f pos, std::mt19937& rd);

	sf::RectangleShape getShape();
	bool getAlive();
	void setAlive(bool alive);
	float getHealth();
	void setHealth(float health);
	float getHealthHolder();
	void setHealthHolder(float healthHolder);
	void setSpeed(float speed)
	{
		this->speed = speed;
	}
	float getSpeed()
	{
		return speed;
	}
	float getSpeedHolder()
	{
		return speedHolder;
	}
	auto getPosition()
	{
		return shape.getPosition();
	}

	void fireBullet(std::vector<Bullet>& bullets, float dt, std::mt19937& gen);
	void reachBorder(float width);
	void move(float dt, float width);//auto moving
	void moveSpecific(float dt, float& distance);
private:
	sf::RectangleShape shape;
	float health, damage, speed, bulletSpeed, attack_speed, healthHolder, speedHolder;//healthHolder used to support health
	bool alive;
};