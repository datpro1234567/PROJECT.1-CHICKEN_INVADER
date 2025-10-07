#pragma once
#include "AirCraft.h"

class Upgrading
{
public:
	Upgrading(sf::Vector2f pos, AircraftType type);

	sf::RectangleShape getShape();
	bool getAlive();
	void setAlive(bool alive);

	void upgrade(AircraftType type, Player*& player);
	void move(float dt);

private:
	sf::RectangleShape shape;
	bool alive;
	float speed;
};