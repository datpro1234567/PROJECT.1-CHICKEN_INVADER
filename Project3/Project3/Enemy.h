#pragma once
#include <SFML/Graphics.hpp>

#include <random>

#include "Bullet.h"

class Enemy
{
public:
	Enemy(sf::Vector2f pos);

	sf::RectangleShape getShape();
	bool getAlive();
	void setAlive(bool alive);
	float getHealth();
	void setHealth(float health);

	void fireBullet(std::vector<Bullet>& bullets, float dt, std::mt19937& gen);
	void reachBorder(float width, Enemy& enemy);
	void move(float dt, float width, std::vector<Enemy>& enemys);
private:
	sf::RectangleShape shape;
	float health, damage, speed, attack_speed;
	bool alive;
};