#include "Player.h"
#include "Bullet.h"


Player::Player(sf::Vector2f pos)
{
	shape.setPointCount(11);
	shape.setPoint(0, sf::Vector2f(-5.f, -20.f));
	shape.setPoint(1, sf::Vector2f(0, -24));
	shape.setPoint(2, sf::Vector2f(5.f, -20.f));
	shape.setPoint(3, sf::Vector2f(5.f, 0.f));
	shape.setPoint(4, sf::Vector2f(25.f, 15.f));
	shape.setPoint(5, sf::Vector2f(15.f, 15.f));
	shape.setPoint(6, sf::Vector2f(15.f, 20.f));
	shape.setPoint(7, sf::Vector2f(-15.f, 20.f));
	shape.setPoint(8, sf::Vector2f(-15.f, 15.f));
	shape.setPoint(9, sf::Vector2f(-25.f, 15.f));
	shape.setPoint(10, sf::Vector2f(-5.f, 0.f));
	auto bound = shape.getLocalBounds();
	shape.setOrigin(bound.left + bound.width / 2, bound.top + bound.height / 2);
	speed = 450;
	bulletSpeed = 300;
	reload_count = 0;
	reloading = false;
	attack_speed_count = 100;//intial shoot is immediately
	alive = 1;
	shape.setPosition(pos);
	readyFire = false;
	level = 1;

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

sf::ConvexShape Player::getShape()
{
	return shape;
}
void Player::setShape(sf::ConvexShape shape)
{
	this->shape = shape;
}
void Player::setAlive(int alive)
{
	this->alive = alive;
}
int Player::getAlive()
{
	return alive;
}
void Player::setHealth(float health)
{
	this->health = health;
}
float Player::getHealth()
{
	return health;
}
float Player::getHealthHolder()
{
	return health_holder;
}
void Player::setHealthHolder(float health_holder)
{
	this->health_holder = health_holder;
}
bool Player::getReloading()
{
	return reloading;
}
float Player::getReload()
{
	return reload;
}
float Player::getReloadCount()
{
	return reload_count;
}
float Player::getAmmo()
{
	return ammo;
}
float Player::getAmmoHolder()
{
	return ammoHolder;
}
float Player::getRange()
{
	return range;
}
void Player::setRange(float range)
{
	this->range = range;
}
float Player::getRangeHolder()
{
	return rangeHolder;
}
float Player::getBulletSpeed()
{
	return bulletSpeed;
}
void Player::setBulletSpeed(float bulletSpeed)
{
	this->bulletSpeed = bulletSpeed;
}
void Player::setAttackSpeed(float attackSpeed)
{
	this->attack_speed = attackSpeed;
}
float Player::getAttackSpeed()
{
	return attack_speed;
}
void Player::setLevel(int level)
{
	this->level = level;
}
int Player::getLevel()
{
	return level;
}
float Player::getDamage()
{
	return damage;
}
void Player::setDamage(float damage)
{
	this->damage = damage;
}
void Player::setAmmo(int ammo)
{
	this->ammo = ammo;
}
void Player::setAmmoHolder(int ammoHolder)
{
	this->ammoHolder = ammoHolder;
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
	auto b = shape.getLocalBounds();

	if (p.x < 0 +  b.width/ 2)
		p.x = 0 + b.width / 2;
	if (p.x > width - b.width / 2)
		p.x = width - b.width / 2;
	if (p.y < 0 + b.height / 2)
		p.y = 0 + b.height / 2;
	if (p.y > height - b.height / 2)
		p.y = height - b.height / 2;
	shape.setPosition(p);
}
void Player::fireBullet(float dt, std::vector<Bullet>& bullets) {}
void Player::reloadAmmo(float dt, int& ammo)
{
	reload_count += dt;
	if (!reloading)
	{
		reloading = true;
		reloadS.stop();
		reloadS.play();
	}
	if (reload_count >= reload)
	{
		reloadS.stop();
		reloadS.play();
		ammo = ammoHolder;
		reload_count = 0;
		reloading = false;
		return;
	}
}
void Player::checkReadyToFire(sf::Event& ev)
{
	if (ev.type == sf::Event::KeyPressed && ev.key.code == fire)
		readyFire = true;
}