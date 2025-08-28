#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.h"

class Player
{
public:
	sf::RectangleShape shape;
	float speed;
	float timeCounter;
	float timeShooter;
	int lives;
	
	Player(sf::Vector2f pos, float speed)
	{
		shape.setPosition(pos);
		shape.setSize(sf::Vector2f(12.f, 12.f));
		shape.setOrigin(sf::Vector2f(shape.getSize().x / 2, shape.getSize().y / 2));
		shape.setFillColor(sf::Color::Cyan);
		this->speed = speed;
		timeCounter = 0.f;
		timeShooter = 0.2f;
		lives = 3;
	}

	void update(float dt, std::vector<Bullet>& bullets, float windowheight, float windowwidth)
	{
		auto p = shape.getPosition();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			shape.move(sf::Vector2f(-speed * dt, 0.f));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			shape.move(sf::Vector2f(speed * dt, 0.f));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			shape.move(sf::Vector2f(0.f, -speed * dt));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			shape.move(sf::Vector2f(0.f, speed * dt));

		p=shape.getPosition();
		if (p.x < 10.f)
			p.x = 10.f;
		if (p.x > windowwidth - 10.f)
			p.x = windowwidth - 10.f;
		if (p.y < 10.f)
			p.y = 10.f;
		if (p.y > windowheight - 10.f)
			p.y = windowheight- 10.f;
		shape.setPosition(p);

		timeCounter += dt;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && timeCounter >= timeShooter)
		{
			bullets.emplace_back(Bullet(shape.getPosition(), -300.f, true));
			timeCounter = 0.f;
		}
	}
};