#include "Player.h"

Player::Player(sf::Vector2f pos)
{
	shape.setSize(sf::Vector2f(45.f, 55.f));
	shape.setOrigin(sf::Vector2f(shape.getSize().x / 2, shape.getSize().y / 2));
	health = 100;
	range = 500;
	damage = 15;
	ammo = 5;
	ammo_holder = ammo;
	speed = 500;
	reload = 5;
	reload_count = 0;
	isReload = false;
	attack_speed = 3;
	attack_speed_count = 3;
	shape.setPosition(pos);

	shootBF = makePewSound();
	shootS.setBuffer(shootBF);
	shootS.setVolume(50.f);

	reloadBF = makeReloadSound();
	reloadS.setBuffer(reloadBF);
	reloadS.setVolume(100.f);
}
Player::Player(sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key left, sf::Keyboard::Key right,
	sf::Keyboard::Key fire, sf::Keyboard::Key ult, sf::Color color,sf::Vector2f pos) : Player(pos)
{
	this->up = up;
	this->down = down;
	this->left = left;
	this->right = right;
	this->fire = fire;
	this->ult = ult;
	shape.setFillColor(color);
}

sf::RectangleShape Player::getShape()
{
	return shape;
}
void Player::setShape(sf::RectangleShape shape)
{
	this->shape = shape;
}
void Player::move(float dt)
{
	if (sf::Keyboard::isKeyPressed(up))
		shape.move(sf::Vector2f(0.f, dt * -speed));
	if (sf::Keyboard::isKeyPressed(down))
		shape.move(sf::Vector2f(0.f, dt * speed));
	if (sf::Keyboard::isKeyPressed(left))
		shape.move(sf::Vector2f(dt * -speed, 0.f));
	if (sf::Keyboard::isKeyPressed(right))
		shape.move(sf::Vector2f(dt * speed, 0.f));
}
void Player::reachBorder(float width, float height)
{
	auto p = shape.getPosition();
	auto s = shape.getSize();
	if (p.x < 0 + s.x / 2)
		p.x = 0 + s.x / 2;
	if (p.x > width - s.x / 2)
		p.x = width - s.x / 2;
	if (p.y < 0 + s.y / 2)
		p.y = 0 + s.y / 2;
	if (p.y > height - s.y / 2)
		p.y = height - s.y / 2;
	shape.setPosition(p);
}
void Player::fireBullet(float dt, std::vector<Bullet>& bullets)
{
	if (ammo == 0)
	{
		reloadAmmo(dt, ammo);
		if(ammo==0)
			return;
		attack_speed_count = 3;
	}
	attack_speed_count += dt;
	if (attack_speed_count >= 1.f/attack_speed)
	{
		if (sf::Keyboard::isKeyPressed(fire))
		{
			bullets.emplace_back(Bullet(true,damage, shape.getPosition()));
			ammo -= 1;

			shootS.stop();
			shootS.play();
		}
		attack_speed_count = 0;
	}
}
void Player::reloadAmmo(float dt, int& ammo)
{
	reload_count += dt;
	if (!isReload)
	{
		isReload = true;
		reloadS.stop();
		reloadS.play();
	}
	if (reload_count >= reload)
	{
		reloadS.stop();
		reloadS.play();
		ammo = ammo_holder;
		reload_count = 0;
		isReload = false;
		return;
	}
}
