#include "Upgrading.h"
#include "Bullet.h"

Upgrading::Upgrading(sf::Vector2f pos, AircraftType type)
{
	shape.setSize(sf::Vector2f(30.f, 30.f));
	shape.setFillColor(sf::Color::Cyan);
	shape.setPosition(pos);
	speed = 100;
	alive = true; 
}

sf::RectangleShape Upgrading::getShape()
{
	return shape;
}
bool Upgrading::getAlive()
{
	return alive;
}
void Upgrading::setAlive(bool alive)
{
	this->alive = alive;
}

void Upgrading::upgrade(AircraftType type, Player*& player)
{
	switch (type)
	{
	case AircraftType::sniper:
	{
		Aircraft* playerD = dynamic_cast<Aircraft*>(player);
		switch (playerD->getLevel())
		{
		case 1:
		{
			playerD->setBulletSpeed(playerD->getBulletSpeed() * 3);
			playerD->setDamage(playerD->getDamage() * 1.5);
			playerD->setLevel(playerD->getLevel() + 1);
			break;
		}
		case 2:
		{

			playerD->setPierce(playerD->getPierce() + 1);
			playerD->setLevel(playerD->getLevel() + 1);
			break;
		}
		case 3:
			playerD->setPierce(playerD->getPierce() + 2);
			playerD->setDamage(playerD->getDamage() * 1.5);
			playerD->setBulletSpeed(playerD->getBulletSpeed() * 2);
			playerD->setLevel(playerD->getLevel() + 1);
			break;
		}
		break;
	}
	case AircraftType::riffle:
	{
		Aircraft* playerD = dynamic_cast<Aircraft*>(player);
		switch (playerD->getLevel())
		{
		case 1:
		{
			playerD->setBarrel(playerD->getBarrel() + 1);
			playerD->setLevel(playerD->getLevel() + 1);
			break;
		}
		case 2:
		{
			playerD->setAmmoHolder(playerD->getAmmoHolder() * 2);
			playerD->setLevel(playerD->getLevel() + 1);
			break;
		}
		case 3:
		{
			playerD->setBarrel(playerD->getBarrel() + 1);
			playerD->setAmmoHolder(playerD->getAmmoHolder() * 1.5);
			playerD->setLevel(playerD->getLevel() + 1);
			break;
		}
		}
		break;
	}
	case AircraftType::rocket:
		Aircraft* playerD = dynamic_cast<Aircraft*>(player);
		switch (playerD->getLevel())
		{
		case 1:
			playerD->setHealthHolder(playerD->getHealthHolder() * 1.5);
			playerD->setHealth(playerD->getHealthHolder());
			playerD->setLevel(playerD->getLevel() + 1);
			break;
		case 2:
			playerD->setSplash(true);
			playerD->setSplashArea(6.5);
			playerD->setLevel(playerD->getLevel() + 1);
			break;
		case 3:
			playerD->setHealthHolder(playerD->getHealthHolder() * 2);
			playerD->setHealth(playerD->getHealthHolder());
			playerD->setSplashArea(10.5);
			playerD->setLevel(playerD->getLevel() + 1);
			break;
		}
		break;
	}
}
void Upgrading::move(float dt)
{
	shape.move(sf::Vector2f(0.f, speed * dt));
}
