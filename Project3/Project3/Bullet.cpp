#include "Bullet.h"

Bullet::Bullet(bool fromplayer, float damage, sf::Vector2f pos)
{
	this->damage = damage;
	this->fromPlayer = fromplayer;
	shape.setRadius(10.f);
	shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));
	shape.setPosition(pos);
	shape.setFillColor(fromplayer ? sf::Color::Red : sf::Color::White);
	speed = 300;
	alive = true;
}

bool Bullet::getFromPlayer()
{
	return fromPlayer;
}
void Bullet::setFromPlayer(bool fromplayer)
{
	this->fromPlayer = fromplayer;
}
bool Bullet::getAlive()
{
	return alive;
}
sf::CircleShape Bullet::getShape()
{
	return shape;
}
float Bullet::getDamage()
{
	return damage;
}
void Bullet::setAlive(bool alive)
{
	this->alive = alive;
}

void Bullet::move(float dt)
{
	shape.move(fromPlayer ? sf::Vector2f(0.f, -speed * dt) : sf::Vector2f(0.f, (speed - 50) * dt));
}
void Bullet::reachBorder(float width)
{
	auto p = shape.getPosition();
	if (p.y < 0 || p.y > width)
		alive = false;
}

