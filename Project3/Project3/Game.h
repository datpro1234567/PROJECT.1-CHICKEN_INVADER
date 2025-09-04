#pragma once
#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Enemy.h"
#include "AirCraft.h"

#include <vector>
#include <iostream>


class ChickenInvader
{
public:
	std::vector<Enemy> createArrayEnemy(float height, float width, int row, int col);

	void playerBehavior(std::vector<Player>& players, float dt, float width, float height, std::vector<Bullet>& bullets, sf::Window& window);
	void bulletBehavior(std::vector<Bullet>& bullets, float dt, float width);
	void enemyBehavior(std::vector<Enemy>& enemys, std::vector<Bullet>& bullets, float dt,std::mt19937& gen, float width);

	void drawPlayer(std::vector<Player>& players, sf::RenderWindow& window);
	void drawEnemy(std::vector<Enemy>& enemys, sf::RenderWindow& window);
	void drawBullet(std::vector<Bullet>& bullets, sf::RenderWindow& window);

	void bulletTouchEnemy(std::vector<Bullet>& bullets, std::vector<Enemy>& enemys);
	void bulletTouchPlayer(std::vector<Bullet>& bullets, std::vector<Player>& players);
	void playerTouchEnemy(std::vector<Player>& players, std::vector<Enemy>& enemys);

	void deleteDeadBullet(std::vector<Bullet>& bullets);
	void deleteDeadEnemy(std::vector<Enemy>& enemys);
	void deleteDeadPlayer(std::vector<Player>& players);

	void closeGame(std::vector<Player>& players, sf::RenderWindow& window);

	void game1();
};