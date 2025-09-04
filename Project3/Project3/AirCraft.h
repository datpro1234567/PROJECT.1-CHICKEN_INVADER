#pragma once

#include "Player.h"

enum class aircraftType
{
	sniper, riffle, rocket
};

class Aircraft : public Player
{
	Aircraft(sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key left, sf::Keyboard::Key right,
		sf::Keyboard::Key fire, sf::Keyboard::Key ult, sf::Color color, sf::Vector2f pos,
		aircraftType type);
private:
	aircraftType type;
};