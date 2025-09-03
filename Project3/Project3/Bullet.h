#pragma once
#include <SFML/Graphics.hpp>

class Bullet
{
public:
	sf::CircleShape shape;

	Bullet(bool fromplayer,float damage, sf::Vector2f pos);

	void setFromPlayer(bool fromplayer);
	bool getFromPlayer();
	bool getAlive();
	sf::CircleShape getShape();
	float getDamage();
	void setAlive(bool alive);	
	void move(float dt);
	void reachBorder(float width);

private:
	float damage, speed;
	bool fromPlayer;
	bool alive;
};