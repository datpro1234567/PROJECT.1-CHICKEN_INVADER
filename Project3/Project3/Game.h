#pragma once
#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Enemy.h"
#include "AirCraft.h"
#include "Upgrading.h"

#include <vector>
#include <iostream>

class ChickenInvader
{
public:
	ChickenInvader();
	std::vector<Enemy> createEnemy(float height, float width, int row, int col,float initialSpace, std::mt19937& rd);
	std::vector<float> createInitialSpaceArr(int row, int col, float initialSpace);
	void prepareEnemy(int row, int col,std::vector<Enemy>& enemys, float dt, std::vector<float>& initialSpaceArr);
	void spawnWave(std::vector<Enemy>& enemys, float dt, int numOfWave, float height, float width, int& row, int& col
		,std::vector<float>& initialSpaceArr, float initialSpace, std::mt19937& rd);

	void createUpgradingOfEnemyArr(std::vector<Enemy>& enemys);
	void createUpgrading(std::vector<Upgrading>& upgradings, std::vector<Enemy>& enemys, std::mt19937& rd);

	void playerBehavior(std::vector<Player*>& players, float dt, float width, float height, std::vector<Bullet>& bullets);
	void bulletBehavior(std::vector<Bullet>& bullets,std::vector<Player*>& players, float dt, float width, std::mt19937& rd);
	void enemyBehavior(std::vector<Enemy>& enemys, std::vector<Bullet>& bullets, float dt, std::mt19937& gen, float width);
	void upgradingBehavior(std::vector<Upgrading>& upgradings, float dt);

	void drawPlayer(std::vector<Player*>& players, sf::RenderWindow& window);
	void drawEnemy(std::vector<Enemy>& enemys, sf::RenderWindow& window);
	void drawBullet(std::vector<Bullet>& bullets, sf::RenderWindow& window);
	void drawUpgrading(std::vector<Upgrading>& upgradings, sf::RenderWindow& window);
	void drawEnemyHealthBar(std::vector<Enemy>& enemys, sf::RenderWindow& window);
	void drawPlayerHealthBar(std::vector<Player*>& players, sf::RenderWindow& window);
	void drawPlayerCD(std::vector<Player*>& players, sf::RenderWindow& window);
	void drawPlayerAmmo(std::vector<Player*>& players, sf::RenderWindow& window);
	void drawSplash(std::vector<Splash>& splashs, sf::RenderWindow& window);

	void bulletTouchEnemy(std::vector<Bullet>& bullets, std::vector<Enemy>& enemys, std::vector<Splash>& splashs);
	void bulletTouchPlayer(std::vector<Bullet>& bullets, std::vector<Player*>& players);
	void playerTouchEnemy(std::vector<Player*>& players, std::vector<Enemy>& enemys);
	void playerTouchUpgrading(std::vector<Player*>& players, std::vector<Upgrading>& upgradings);
	void splashTouchEnemy(std::vector<Splash>& splashs, std::vector<Enemy>& enemys);

	void deleteDeadBullet(std::vector<Bullet>& bullets);
	void deleteDeadEnemy(std::vector<Enemy>& enemys);
	void deleteDeadPlayer(std::vector<Player*>& players);
	void deleteDeadUpgrading(std::vector<Upgrading>& upgradings);
	void deleteDeadSplash(std::vector<Splash>& splashs, float dt);

	void closeGame(std::vector<Player*>& players, sf::RenderWindow& window);

	void game1();
private: 
	int wave;
	bool enemyPrepared;// use for check is enemy ready ?
	float waitTime;
	float countWaitTime;
	std::vector<bool> upgradingOfEnemyArr;// array use for each enemy to check is it valid in prepare phase
};