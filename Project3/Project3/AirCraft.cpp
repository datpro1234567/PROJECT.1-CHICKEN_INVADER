#include "AirCraft.h"

Aircraft::Aircraft(sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key left, sf::Keyboard::Key right,
	sf::Keyboard::Key fire, sf::Keyboard::Key ult, sf::Color color, sf::Vector2f pos,
	aircraftType type) : Player(up, down, left, right, fire, ult, color, pos), type(type)
{
	switch (type)
	{
	case aircraftType::sniper:
	{
		damage = 30;
		ammo = 5;
		attack_speed = 2;
		reload = 6;
		break;
	}
	case aircraftType::riffle:
	{
		damage = 10;
		ammo = 15;
		attack_speed = 6;
		reload = 6;
		break;
	}
	case aircraftType::rocket:
	{
		damage = 30;
		ammo = 2;
		attack_speed = 0.5;
		reload = 8;
		break;
	}

	}
}