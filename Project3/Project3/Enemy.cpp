#include "Enemy.h"

Enemy::Enemy(sf::Vector2f pos)
{
	shape.setSize(sf::Vector2f(60.f, 60.f));
	shape.setOrigin(sf::Vector2f(shape.getSize().x / 2, shape.getSize().y / 2));
	shape.setPosition(pos);
	shape.setFillColor(sf::Color(204, 204, 0));
	health = 30;
	damage = 20;
	speed = 1;
	attack_speed = 3;
	alive = true;
}

sf::RectangleShape Enemy:: getShape()
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

void Enemy::fireBullet(std::vector<Bullet>& bullets, float dt, std::mt19937& gen)
{
	std::uniform_real_distribution<float> dom(0.00f, 1.f);
	attack_speed -= dt;
	if (attack_speed <= 0)
	{
		if (dom(gen) <= 0.02)
		{
			bullets.emplace_back(Bullet(false, damage, shape.getPosition()));
		}
		attack_speed = 1 + dom(gen) * 2;
	}
}
void Enemy::reachBorder(float width,Enemy& enemy)
{
	auto p = enemy.getShape().getPosition();
	auto s = enemy.getShape().getSize();
	if (p.x <= 0 + s.x / 2 || p.x >= width - s.x / 2)
	{
		enemy.speed = -enemy.speed;
	}
}
void Enemy::move(float dt, float width, std::vector<Enemy>& enemys)
{

	for (Enemy& enemy : enemys)
	{
		enemy.shape.move(sf::Vector2f(dt * enemy.speed, 0.f));
		reachBorder(width,enemy);
	}
}