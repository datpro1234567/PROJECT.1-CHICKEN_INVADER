#include "Bullet.h"

Bullet::Bullet(bool fromplayer, float damage, float speed, sf::Vector2f pos)
{
	this->damage = damage;
	this->fromPlayer = fromplayer;
	this->speed = speed;
	owener = NULL;
	range = 0;
	shape.setRadius(10.f);
	shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));
	shape.setPosition(pos);
	shape.setFillColor(fromplayer ? sf::Color::Red : sf::Color::White);
	alive = 1;
	splash = false;
	splashArea = 0;
}

bool Bullet::getFromPlayer()
{
	return fromPlayer;
}
void Bullet::setFromPlayer(bool fromplayer)
{
	this->fromPlayer = fromplayer;
}
int Bullet::getAlive()
{
	return alive;
}
sf::CircleShape& Bullet::getShape()
{
	return shape;
}
float Bullet::getDamage()
{
	return damage;
}
void Bullet::setAlive(int alive)
{
	this->alive = alive;
}
void Bullet::setOwnwer(Player* owener)
{
	this->owener = owener;
}
Player* Bullet::getOwner()
{
	return owener;
}
float Bullet::getRange()
{
	return range;
}
void Bullet::setRange(float range)
{
	this->range = range;
}
float Bullet::getRangeHolder()
{
	return rangeHolder;
}
void Bullet::setRangeHolder(float rangeHolder)
{
	this->rangeHolder = rangeHolder;
}
void Bullet::setColor(sf::Color color)
{
	shape.setFillColor(color);
}
void Bullet::setSplash(bool splash)
{
	this->splash = splash;
}
bool Bullet::getSplash()
{
	return splash;
}

void Bullet::move(float dt)
{
	shape.move(fromPlayer ? sf::Vector2f(0.f, -speed * dt) : sf::Vector2f(0.f, (speed) * dt));
}
void Bullet::reachBorder(float width)
{
	auto p = shape.getPosition();
	if (p.y < 0 || p.y > width)
		alive = false;
}
void Bullet::reachRange(Player*& player, float dt)
{
	setRange(getRange() - dt * player->getBulletSpeed());
	if (getRange() <= 0)
	{
		alive = false;
	}
}

//------------------------------------------------------------------------------------------

Splash::Splash(Bullet bullet,float splashArea)
{
	shape.setRadius(bullet.getShape().getRadius() * splashArea);
	shape.setFillColor(bullet.getShape().getFillColor());
	shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));
	shape.setPosition(bullet.getShape().getPosition());
	damage = bullet.getDamage() / 2;
}

void Splash::setDamage(float damage)
{
	this->damage = damage;
}
float Splash::getDamage()
{
	return damage;
}
void Splash::setShape(sf::CircleShape shape)
{
	this->shape = shape;
}
sf::CircleShape& Splash::getShape()
{
	return shape;
}
float Splash::getSplashTime()
{
	return splashTime;
}
float Splash::getSplashTimeCounter()
{
	return this->splashTimeCounter;
}
void Splash::setsplashTimeCounter(float dt)
{
	splashTimeCounter += dt;
}
void Splash::setBase(float splashArea)
{
	this->splashArea = splashArea;
}
float Splash::getBase()
{
	return splashArea;
}