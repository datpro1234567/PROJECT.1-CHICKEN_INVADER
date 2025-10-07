#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Sound.h"
class Bullet;

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
	
	virtual void setHealthHolder(float health_holder);
	virtual void setRange(float range);
	virtual void setHealth(float health);
	virtual void setShape(sf::ConvexShape shape);
	virtual void setAlive(int alive);
	virtual void setBulletSpeed(float bulletSpeed);
	virtual void setAttackSpeed(float attackSpeed);
	virtual void setLevel(int level);
	virtual void setDamage(float damage);
	virtual void setAmmo(int ammo);
	virtual void setAmmoHolder(int ammoHolder);

	virtual sf::ConvexShape getShape();
	virtual int getAlive();
	virtual float getHealth();
	virtual float getHealthHolder();
	virtual bool getReloading();
	virtual float getReload();
	virtual float getReloadCount();
	virtual float getAmmo();
	virtual float getAmmoHolder();
	virtual float getRange();
	virtual float getRangeHolder();
	virtual float getBulletSpeed();
	virtual float getAttackSpeed();
	virtual int getLevel();
	virtual float getDamage();

	virtual void move(float dt);
	virtual void reachBorder(float width, float height);
	virtual void fireBullet(float dt, std::vector<Bullet>& bullets);
	virtual void reloadAmmo(float dt, int& ammo);
	virtual void checkReadyToFire(sf::Event& ev);
protected:
	sf::ConvexShape shape;

	float health, range, damage, speed, reload, attack_speed, bulletSpeed
		, rangeHolder
		,health_holder//use for support health
		, reload_count//use for support reload
		, attack_speed_count;//use for support attack_speed
	int ammo, ammoHolder;//ammo_holder use for support ammo
	bool reloading;// use for support reload sound
	int alive;
	bool readyFire;//use to support function fireBullet
	int level;
};