#pragma once
#include <SFML/Graphics.hpp>
#include <random>

class Enemy
{
public:
	Enemy(sf::Vector2f pos)
	{
		shape.setSize(sf::Vector2f(60.f, 60.f));
		shape.setOrigin(sf::Vector2f(shape.getSize().x / 2, shape.getSize().y / 2));
		shape.setPosition(pos);
		shape.setFillColor(sf::Color(204,204,0));
		health = 30;
		damage = 20;
		speed = 150;
		attack_speed = 3;
		alive = true;
	}

	sf::RectangleShape getShape()
	{
		return shape;	
	}
	bool getAlive()
	{
		return alive;
	}
	void setAlive(bool alive)
	{
		this->alive = alive;
	}
	float getHealth()
	{
		return health;
	}
	void setHealth(float health)
	{
		this->health = health;
	}

	void fireBullet(std::vector<Bullet>& bullets, float dt,std::mt19937& gen)
	{
		std::uniform_real_distribution<float> dom(0.00f, 1.f);
		attack_speed -= dt;
		if (attack_speed <= 0)
		{
			if (dom(gen) <= 0.08)
			{
				bullets.emplace_back(Bullet(false,damage, shape.getPosition()));
			}
			attack_speed = 1 + dom(gen)*2;
		}
	}
private:
	sf::RectangleShape shape;
	float health, damage, speed, attack_speed;
	bool alive;
};