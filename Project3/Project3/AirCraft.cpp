#include "AirCraft.h"
#include "Bullet.h"

Aircraft::Aircraft(sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key left, sf::Keyboard::Key right,
	sf::Keyboard::Key fire, sf::Keyboard::Key ult, sf::Color color, sf::Vector2f pos,
	AircraftType type) : Player(up, down, left, right, fire, ult, color, pos), type(type)
{
	pierce = 1;
	barrel = 1;
	splashArea = 0;
	splash = false;
	switch (type)
	{
	case AircraftType::sniper:
	{
		health = 60;
		damage = 20;
		ammo = 3;
		attack_speed = 2;
		reload = 6;
		range = 2000;
		break;
	}
	case AircraftType::riffle:
	{
		health = 80;
		damage = 10;
		ammo = 10;
		attack_speed = 6;
		reload = 6;
		range = 500;
		break;
	}
	case AircraftType::rocket:
	{
		health = 100;
		damage = 3;
		ammo = 100;
		attack_speed = 10;
		reload = 8;
		range = 500;
		break;
	}
	}
	health_holder = health;
	ammoHolder = ammo;
	rangeHolder = range;
}

AircraftType Aircraft::getAircraftType()
{
	return type;
}
void Aircraft::setPierce(int pierce)
{
	this->pierce = pierce;
}
int Aircraft::getPierce()
{
	return pierce;
}
int Aircraft::getBarrel()
{
	return barrel;
}
void Aircraft::setBarrel(int barrel)
{
	this->barrel = barrel;
}

void Aircraft::fireBullet(float dt, std::vector<Bullet>& bullets)
{
	if (ammo <= 0)
	{
		reloadAmmo(dt, ammo);
		readyFire = false;
		if (ammo <= 0)
			return;
		attack_speed_count = 3;
	}
	attack_speed_count += dt;

	if (readyFire && attack_speed_count >= 1.f / attack_speed)
	{
		switch (getAircraftType())
		{
		case AircraftType::sniper:
		{
			bullets.emplace_back(Bullet(true, damage, bulletSpeed, shape.getPosition()));
			bullets.back().setAlive(this->getPierce());
			break;
		}
		case AircraftType::riffle:
		{
			if (barrel == 1)
			{
				bullets.emplace_back(Bullet(true, damage, bulletSpeed, shape.getPosition()));
			}
			else if (barrel % 2 == 0)
			{
				int distance = 8;
				for (int i = 0; i < barrel; i++)
				{
					bullets.emplace_back(Bullet(true, damage, bulletSpeed, sf::Vector2f(shape.getPosition().x - distance, shape.getPosition().y)));
					distance *= -1;
					if (i != 0 && i % 2 != 0)
					{
						distance <= 0 ? distance -= 14 : distance += 14;
					}
				}
			}
			else
			{
				int distance = 14;
				bullets.emplace_back(Bullet(true, damage, bulletSpeed, shape.getPosition()));
				for (int i = 0; i < barrel - 1; i++)
				{
					bullets.emplace_back(Bullet(true, damage, bulletSpeed, sf::Vector2f(shape.getPosition().x - distance, shape.getPosition().y)));
					distance *= -1;
					if (i != 0 && i % 2 != 0)
					{
						distance <= 0 ? distance -= 14 : distance += 14;
					}
				}
			}
			break;
		}
		case AircraftType::rocket:
		{
			bullets.emplace_back(Bullet(true, damage, bulletSpeed, shape.getPosition()));
			if (getSplash())
			{
				bullets.back().setSplashArea(getSplashArea());
				bullets.back().setSplash(getSplash());
			}
			break;
		}
		}
		int tailBullets = bullets.size() - 1;
		for (int i = 0; i < barrel; i++)
		{
			bullets[tailBullets - i].setOwnwer(this);
			bullets[tailBullets - i].setRange(this->getRange());
			bullets[tailBullets - i].setRangeHolder(this->getRange());
			bullets[tailBullets - i].setColor(this->shape.getFillColor());
			ammo -= 1;
		}
		shootS.stop();
		shootS.play();

		attack_speed_count = 0;
		readyFire = false;
	}
}