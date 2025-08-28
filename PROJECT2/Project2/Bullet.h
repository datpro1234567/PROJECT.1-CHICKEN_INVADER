#pragma once

#include <SFML/Graphics.hpp>

class Bullet
{
public:
	sf::CircleShape shape;
	float speed;
	bool isAlive;
	bool isPlayer;

	Bullet(sf::Vector2f pos, float speed, bool isPlayer)
	{
		this->speed = speed;
		this->isAlive = true;
		this->isPlayer = isPlayer;
		shape.setRadius(isPlayer ? 8 : 12);
		shape.setOrigin(sf::Vector2f(shape.getRadius(),shape.getRadius()));
		shape.setPosition(pos);
		shape.setFillColor(isPlayer ? sf::Color::Red : sf::Color::White);
	}

	void update(float deltaTime, float windowheight)
	{
		if (!isAlive)
			return;
		shape.move(sf::Vector2f(0.f, deltaTime * speed));
		if (shape.getPosition().y<0 || shape.getPosition().y>windowheight)
			isAlive = false;
	}

};