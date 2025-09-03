#pragma once
#include "Player.h"

enum class AircraftType { Sniper, Rifle, Rocket };

class AirCraft : public Player {
public:
    AirCraft(AircraftType type,
        sf::Keyboard::Key up, sf::Keyboard::Key down,
        sf::Keyboard::Key left, sf::Keyboard::Key right,
        sf::Keyboard::Key fire, sf::Keyboard::Key ult,
        sf::Color color, sf::Vector2f pos)
        : Player(up, down, left, right, fire, ult, color, pos), type(type)
    {
        switch (type) {
        case AircraftType::Sniper:
            ammo = 5;
            damage = 30;
            attack_speed = 5;
            reload = 6;
            break;
        case AircraftType::Rifle:
            ammo = 15;
            damage = 10;
            attack_speed = 3;
            reload = 5;
            break;
        case AircraftType::Rocket:
            ammo = 3;
            damage = 50;
            attack_speed = 1;
            reload = 8;
            break;
        }
    }
private:
    AircraftType type;
};
