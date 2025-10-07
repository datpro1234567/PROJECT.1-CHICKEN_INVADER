#pragma once

#include "Player.h"
struct Splash;

enum class AircraftType
{
	sniper, riffle, rocket
};

class Aircraft : public Player
{
public:
	Aircraft(sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key left, sf::Keyboard::Key right,
		sf::Keyboard::Key fire, sf::Keyboard::Key ult, sf::Color color, sf::Vector2f pos,
		AircraftType type);

	AircraftType getAircraftType();
	void setPierce(int pierce);
	int getPierce();
	int getBarrel();
	void setBarrel(int barrel);
	void setSplash(bool splash)
	{
		this->splash = splash;
	}
	bool getSplash()
	{
		return splash;
	}
	void setSplashArea(float splashArea)
	{
		this->splashArea = splashArea;
	}
	float getSplashArea()
	{
		return splashArea;
	}


	void fireBullet(float dt, std::vector<Bullet>& bullets) override;
private:
	AircraftType type;
	int pierce;
	int barrel;
	bool splash;
	float splashArea;
};