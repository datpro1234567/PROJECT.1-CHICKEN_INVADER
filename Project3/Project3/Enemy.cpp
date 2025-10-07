#include "Enemy.h"

Enemy::Enemy(sf::Vector2f pos,std::mt19937& rd)
{
	shape.setSize(sf::Vector2f(60.f, 60.f));
	shape.setOrigin(sf::Vector2f(shape.getSize().x / 2, shape.getSize().y / 2));
	shape.setPosition(pos);
	shape.setFillColor(sf::Color(204, 204, 0));
	health = 30;
	healthHolder = health;
	damage = 20;
	speed = 100;

	std::uniform_real_distribution<float> bulletSpeedDom(1.0, 3.0);
	bulletSpeed = bulletSpeedDom(rd);

	speedHolder = speed;
	attack_speed = 2;
	alive = true;
}

sf::RectangleShape Enemy::getShape()
{
	return shape;
}
bool Enemy::getAlive()
{
	return alive;
}
void Enemy::setAlive(bool alive)
{
	this->alive = alive;
}
float Enemy::getHealth()
{
	return health;
}
void Enemy::setHealth(float health)
{
	this->health = health;
}
float Enemy::getHealthHolder()
{
	return healthHolder;
}
void Enemy::setHealthHolder(float healthHolder)
{
	this->healthHolder = healthHolder;
}

void Enemy::fireBullet(std::vector<Bullet>& bullets, float dt, std::mt19937& gen)
{
	std::uniform_real_distribution<float> dom(0.00f, 1.f);
	attack_speed -= dt;
	if (attack_speed <= 0)
	{
		if (dom(gen) <= 1 )//0.02
		{
			bullets.emplace_back(Bullet(false, damage, bulletSpeed* 90 , shape.getPosition()));
		}
		attack_speed = 1 + dom(gen) * 2;
	}
}
void Enemy::reachBorder(float width)
{
	auto p = getShape().getPosition();
	auto s = getShape().getSize();
	if (p.x <= 0 + s.x / 2 || p.x >= width - s.x / 2)
	{
		speed = -speed;
	}
}
void Enemy::move(float dt, float width)
{
	shape.move(sf::Vector2f(dt * speed, 0.f));
	reachBorder(width);
}
void Enemy::moveSpecific(float dt, float& distance)
{
	if (distance <= 0)
	{
		return;
	}
	distance -= dt * speed;
	shape.move(sf::Vector2f(dt * speed, 0.f));
}