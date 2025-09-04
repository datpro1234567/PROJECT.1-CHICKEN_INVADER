#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Bullet.h"
#include "Sound.h"

#include <vector>
#include <iostream>

class Player
{
public:
	sf::Keyboard::Key up, down, left, right, fire, ult;

	sf::SoundBuffer shootBF, reloadBF;
	sf::Sound shootS, reloadS;

	Player(sf::Vector2f pos);
	Player(sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key left, sf::Keyboard::Key right,
		sf::Keyboard::Key fire, sf::Keyboard::Key ult, sf::Color color,sf::Vector2f pos);
	
	sf::RectangleShape getShape();
	void setShape(sf::RectangleShape shape);
	void setAlive(bool alive);
	bool getAlive();
	void setHealth(float health);
	float getHealth();

	void move(float dt);
	void reachBorder(float width, float height);
	void fireBullet(float dt, std::vector<Bullet>& bullets, sf::Window& window);
	void reloadAmmo(float dt, int& ammo);
	void checkReadyToFire(sf::Event& ev);

protected:
	sf::RectangleShape shape;

	float health, range, damage, speed, reload, attack_speed
		, reload_count//use for support reload
		, attack_speed_count;//use for support attack_speed
	int ammo, ammo_holder;//ammo_holder use for support ammo
	bool isReload;// use for support reload sound
	bool alive;

	bool readyFire;//use to support function fireBullet
};