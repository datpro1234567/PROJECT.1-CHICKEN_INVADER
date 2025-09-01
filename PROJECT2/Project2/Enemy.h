#pragma once
#include <SFML/graphics.hpp>
#include <random>
#include "Bullet.h"
#include <vector>

class Enemy
{
public:
	sf::CircleShape shape;
	float attackSpeed;
	bool isAlived;

	Enemy(float x, float y)
	{
		shape.setRadius(12.f);
		shape.setOrigin(6.f, 6.f);
		shape.setFillColor(sf::Color::Yellow);
		shape.setPosition(x, y);
		attackSpeed = 1.f;
		isAlived = true;
	}

	void update(float dt, std::vector<Bullet>& bullets, std::mt19937& gen)
	{
		if (!isAlived)
			return;
		attackSpeed -= dt;
		if (attackSpeed <= 0)
		{
			std::uniform_real_distribution<float> domain(0.00f, 1.f);
			if (domain(gen) <= 0.5)
			{
				bullets.emplace_back(Bullet(shape.getPosition(), 150.f, false));
			}
			attackSpeed = domain(gen) * 3;
		}

	}
};