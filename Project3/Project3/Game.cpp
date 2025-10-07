#include "Game.h"

ChickenInvader::ChickenInvader()
{
	enemyPrepared = false;
	wave = 1;
	waitTime = 3;
	countWaitTime = waitTime;
}

std::vector<Enemy> ChickenInvader::createEnemy(float height, float width, int row, int col, float initialSpace,std::mt19937& rd)
{

	float spaceX, spaceY;
	spaceX = width / (10 + 4);
	spaceY = height / (6 + 7);

	std::vector<Enemy> enemys;
	for (int i = 0; i < row; i++)
	{
		for (int j = 3; j < col+3; j++)
		{
		enemys.emplace_back(Enemy(sf::Vector2f((spaceX + spaceX * j) - initialSpace *j, spaceY + spaceY * i), rd));
		}
	}

	return enemys;
}
std::vector<float> ChickenInvader::createInitialSpaceArr(int row, int col, float initialSpace)
{
	std::vector<float> initialSpaceArr;
	for (int i = 0; i < row; i++)
	{
		for (int j = 1; j < col+1; j++)
		{
			initialSpaceArr.emplace_back(initialSpace * j);
		}
	}
	return initialSpaceArr;
}
void ChickenInvader::prepareEnemy(int row, int col,std::vector<Enemy>& enemys, float dt, std::vector<float>& initialSpaceArr)
{
	if (enemyPrepared == false)
	{
		int i = 0;//index
		int lastIndexArr = initialSpaceArr.size() - 1;//last index of initialspacearr
		for (Enemy& enemy : enemys)
		{
			if(enemy.getSpeed()==enemy.getSpeedHolder())
				enemy.setSpeed(enemy.getSpeed() * 3.9);
			enemy.moveSpecific(dt, initialSpaceArr[i]);
			i++;
		}

		if (initialSpaceArr[lastIndexArr] <= 0)
		{
			enemyPrepared = true;
			for (Enemy& enemy : enemys)
			{
				enemy.setSpeed(enemy.getSpeedHolder());
			}
		}
	}
}
void ChickenInvader::spawnWave(std::vector<Enemy>& enemys,float dt,int numOfWave, float height, float width, int& row, int& col, std::vector<float>& initialSpaceArr,float initialSpace, std::mt19937& rd)
{
	if (enemys.empty())
	{
		if (countWaitTime >= waitTime && wave<numOfWave)
		{
			initialSpaceArr = createInitialSpaceArr(row, col, initialSpace);
			enemys = createEnemy(height, width, row, col, initialSpace,rd);
			row += wave;
			col += wave;
			wave++;
			countWaitTime = 0;
			enemyPrepared = false;
			createUpgradingOfEnemyArr(enemys);
		}
		countWaitTime += dt;
	}
	prepareEnemy(row, col, enemys, dt, initialSpaceArr);
}

void ChickenInvader::createUpgradingOfEnemyArr(std::vector<Enemy>& enemys)
{
	upgradingOfEnemyArr = std::vector<bool>(enemys.size(), true);
}
void ChickenInvader::createUpgrading(std::vector<Upgrading>& upgradings, std::vector<Enemy>& enemys, std::mt19937& rd)
{
	int i = 0;
	int size = enemys.size() - 1;
	for (Enemy& enemy : enemys)
	{
		if (!enemy.getAlive()&&upgradingOfEnemyArr[i])
		{
			std::uniform_real_distribution<float> dom(1, 100);
			if (dom(rd) <= 100)
				upgradings.emplace_back(Upgrading(enemy.getPosition(), AircraftType::sniper));
			upgradingOfEnemyArr[i] = false;
		}
		i++;
	}
	if (i == size)
	{
		for (int i = 0; i < size; i++)
		{
			upgradingOfEnemyArr[i] = true;
		}
	}
}

void ChickenInvader::playerBehavior(std::vector<Player*>& players, float dt, float width, float height, std::vector<Bullet>& bullets)
{
	for (Player*& player : players)
	{
		player->move(dt);
		player->reachBorder(width, height);
		player->fireBullet(dt, bullets);
	}
}
void ChickenInvader::bulletBehavior(std::vector<Bullet>& bullets,std::vector<Player*>& players, float dt, float width,std::mt19937& rd)
{
	for (Bullet& bullet : bullets)
	{
		bullet.move(dt);
		bullet.reachBorder(width);
	}
		
	for (Player*& player : players)
	{
		for (Bullet& bullet : bullets)
		{
			if (bullet.getFromPlayer()&&bullet.getOwner() == player)
			{
				bullet.reachRange(player, dt);
			}
		}
	}
}
void ChickenInvader::enemyBehavior(std::vector<Enemy>& enemys,std::vector<Bullet>& bullets, float dt, std::mt19937& gen,float width)
{
	for (Enemy& enemy : enemys)
	{
		if (enemyPrepared)
		{
			enemy.fireBullet(bullets, dt, gen);
			enemy.move(dt, width);
		}
	}
}
void ChickenInvader::upgradingBehavior(std::vector<Upgrading>& upgradings, float dt)
{
	for (Upgrading& upgrading : upgradings)
	{
		upgrading.move(dt);
	}
}

void ChickenInvader::drawPlayer(std::vector<Player*>& players, sf::RenderWindow& window)
{
	for (Player*& player : players)
	{
		window.draw(player->getShape());
	}
}
void ChickenInvader::drawEnemy(std::vector<Enemy>& enemys, sf::RenderWindow& window)
{
	for (Enemy& enemy : enemys)
	{
		if(enemy.getAlive())
			window.draw(enemy.getShape());
	}
}
void ChickenInvader::drawBullet(std::vector<Bullet>& bullets, sf::RenderWindow& window)
{
	for (Bullet& bullet : bullets)
	{
		window.draw(bullet.getShape());
	}
}
void ChickenInvader::drawEnemyHealthBar(std::vector<Enemy>& enemys, sf::RenderWindow& window)
{
	for (Enemy& enemy : enemys)
	{
		if (enemy.getAlive() && enemy.getHealth() < enemy.getHealthHolder())
		{
			sf::RectangleShape shape;

			sf::Vector2f enemyPos = enemy.getShape().getPosition();
			sf::Vector2f enemySize = enemy.getShape().getSize();
			float fullWidth = enemySize.x;
			float percent = static_cast<float>(enemy.getHealth()) / enemy.getHealthHolder();
			float G = 250;//value in RGB
			G = G * percent;
			float R = 250 - G;

			shape.setSize(sf::Vector2f(fullWidth * percent, 10.f));
			shape.setFillColor(sf::Color(R, G, 0));
			shape.setPosition(sf::Vector2f(enemyPos.x - enemySize.x / 2, (enemyPos.y + enemySize.y / 2) + 2));
			window.draw(shape);
		}
	}
}
void ChickenInvader::drawPlayerHealthBar(std::vector<Player*>& players, sf::RenderWindow& window)
{
	for (Player*& player : players)
	{
		if (player->getAlive() > 0 && player->getHealth() <= player->getHealthHolder())
		{
			sf::RectangleShape shape;

			auto playerSize = player->getShape().getLocalBounds();
			sf::Vector2f playerPos = player->getShape().getPosition();
			float fullWidth = playerSize.width;
			float percent = static_cast<float>(player->getHealth()) / player->getHealthHolder();
			float G = 250;//value in RGB
			G = G * percent;
			float R = 250 - G;

			shape.setSize(sf::Vector2f(fullWidth * percent, 14.f));
			shape.setFillColor(sf::Color(R, G, 0));
			shape.setPosition(sf::Vector2f(playerPos.x - playerSize.width / 2, (playerPos.y + playerSize.height / 2) + 2));
			window.draw(shape);
		}
	}
}
void ChickenInvader::drawPlayerCD(std::vector<Player*>& players, sf::RenderWindow& window)
{
	for (Player*& player : players)
	{
		if (player->getAlive() > 0 && player->getReloading())
		{
			sf::RectangleShape shape;
			sf::Vector2f playerPos = player->getShape().getPosition();
			auto playerSize = player->getShape().getLocalBounds();

			float fullHeight = playerSize.height;
			float percent = (float)player->getReloadCount() / player->getReload();
			float R = 250;//Red value in RGB
			float G = R * percent;//Green value
 
			shape.setSize(sf::Vector2f(14.f, fullHeight * percent));
			shape.setFillColor(sf::Color(R, G, 0));
			shape.setOrigin(0.f, shape.getSize().y);
			shape.setPosition(sf::Vector2f(playerPos.x + playerSize.width / 2 + 2, playerPos.y + playerSize.height / 2));
			window.draw(shape);
		}
	}
}
void ChickenInvader::drawPlayerAmmo(std::vector<Player*>& players, sf::RenderWindow& window)
{
	for (Player*& player : players)
	{
		if (!player->getReloading())
		{
			sf::RectangleShape shape;
			auto playerSize = player->getShape().getLocalBounds();
			sf::Vector2f playerPos = player->getShape().getPosition();

			float fullHeight = playerSize.height;
			float percent = (float)player->getAmmo() / player->getAmmoHolder();
			float R = 250;
			float G = 250 * percent;
			shape.setFillColor(sf::Color(R, G, 0));
			shape.setSize(sf::Vector2f(14.f, fullHeight * percent));
			shape.setOrigin(sf::Vector2f(0.f, shape.getSize().y));
			shape.setPosition(sf::Vector2f(playerPos.x + playerSize.width / 2 + 2, playerPos.y + playerSize.height / 2));
			window.draw(shape);
		}
	}
}
void ChickenInvader::drawUpgrading(std::vector<Upgrading>& upgradings, sf::RenderWindow& window)
{
	for (Upgrading& upgrading : upgradings)
	{
		if (upgrading.getAlive())
			window.draw(upgrading.getShape());
	}
}
void ChickenInvader::drawSplash(std::vector<Splash>& splashs, sf::RenderWindow& window)
{
	for (Splash& splash : splashs)
	{
		if (splash.getSplashTimeCounter() < splash.getSplashTime())
			window.draw(splash.getShape());
	}
}

void ChickenInvader::bulletTouchEnemy(std::vector<Bullet>& bullets, std::vector<Enemy>& enemys,std::vector<Splash>& splashs)
{
	for (Bullet& bullet : bullets)
	{
		if (bullet.getAlive() > 0 && bullet.getFromPlayer())
		{
			for (Enemy& enemy : enemys)
			{
				if (enemy.getAlive())
				{
					if (bullet.getShape().getGlobalBounds().intersects(enemy.getShape().getGlobalBounds()))
					{
						if (bullet.getSplash())
						{
							splashs.emplace_back(Splash(bullet,bullet.getSplashArea()));
						}
						bullet.setAlive(bullet.getAlive() - 1);

						enemy.setHealth(enemy.getHealth() - bullet.getDamage());
						if (enemy.getHealth() <= 0)
						{
							enemy.setAlive(false);
						}
					}
				}
			}
		}
	}
}
void ChickenInvader::bulletTouchPlayer(std::vector<Bullet>& bullets, std::vector<Player*>& players)
{
	for (Bullet& bullet : bullets)
	{
		if (!bullet.getFromPlayer() && bullet.getAlive() > 0)
		{
			for (Player*& player : players)
			{
				if (player->getAlive() > 0)
				{
					if (bullet.getShape().getGlobalBounds().intersects(player->getShape().getGlobalBounds()))
					{
						player->setHealth(player->getHealth() - bullet.getDamage());
						if (player->getHealth() <= 0)
						{
							player->setAlive(player->getAlive()-1);
						}
						bullet.setAlive(bullet.getAlive() - 1);
					}
				}
			}
		}
	}
}
void ChickenInvader::playerTouchEnemy(std::vector<Player*>& players, std::vector<Enemy>& enemys)
{
	for (Player*& player : players)
	{
		if (player->getAlive() > 0)
			for (Enemy& enemy : enemys)
			{
				if (enemy.getAlive())
				{
					if (player->getShape().getGlobalBounds().intersects(enemy.getShape().getGlobalBounds()))
					{
						player->setAlive(player->getAlive() - 1);
						enemy.setAlive(false);
					}
				}
			}
	}
}
void ChickenInvader::playerTouchUpgrading(std::vector<Player*>& players, std::vector<Upgrading>& upgradings)
{
	for (Player*& player : players)
	{
		if (player->getAlive() > 0)
		{
			for (Upgrading& upgrading : upgradings)
			{
				if (upgrading.getAlive())
				{
					if (upgrading.getShape().getGlobalBounds().intersects(player->getShape().getGlobalBounds()))
					{
						if (auto p = dynamic_cast<Aircraft*>(player))
						{
							upgrading.upgrade(p->getAircraftType(), player);
							upgrading.setAlive(false);
						}
					}
				}
			}
		}
	}
}
void ChickenInvader::splashTouchEnemy(std::vector<Splash>& splashs, std::vector<Enemy>& enemys)
{
	for (Splash& splash : splashs)
	{
		if (splash.getAlive())
		{
			for (Enemy& enemy : enemys)
			{
				if (enemy.getAlive())
				{
					if (splash.getShape().getGlobalBounds().intersects(enemy.getShape().getGlobalBounds()))
					{
						enemy.setHealth(enemy.getHealth() - splash.getDamage());
						if (enemy.getHealth() <= 0)
						{
							enemy.setAlive(false);
						}
					}
				}
			}
			splash.setAlive(false);
		}
	}
}

void ChickenInvader::deleteDeadBullet(std::vector<Bullet>& bullets)
{
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& bullet) { return !(bullet.getAlive() > 0); }), bullets.end());
}
void ChickenInvader::deleteDeadEnemy(std::vector<Enemy>& enemys)
{
	if (enemyPrepared)
	{
		enemys.erase(std::remove_if(enemys.begin(), enemys.end(), [](Enemy& enemy) { return !enemy.getAlive(); }), enemys.end());
	}
}
void ChickenInvader::deleteDeadPlayer(std::vector<Player*>& players)
{
	players.erase(std::remove_if(players.begin(), players.end(), [](Player*& player) {return !(player->getAlive() > 0); }), players.end());
}
void ChickenInvader::deleteDeadUpgrading(std::vector<Upgrading>& upgradings)
{
	int size = upgradings.size();
	for (int i = 0; i < size; i++)
	{
		if (!upgradings[i].getAlive())
		{
			upgradings.erase(upgradings.begin() + i);
			i -= 1;
			size -= 1;
		}
	}
}
void ChickenInvader::deleteDeadSplash(std::vector<Splash>& splashs, float dt)
{
	int i = 0;
	for (Splash& splash : splashs)
	{
		splash.setsplashTimeCounter(dt);
		if (splash.getSplashTimeCounter() >= splash.getSplashTime())
		{
			splashs.erase(splashs.begin() + i);
			i--;
		}
		i++;
	}
}

void ChickenInvader::closeGame(std::vector<Player*>& players, sf::RenderWindow& window)
{
	bool anyAlive = false;
	for (Player*& player : players)
	{
		if (player->getAlive() > 0)
		{	
			anyAlive = true;
			break;
		}
	}
	if (!anyAlive)
	{
		window.close();
	}
}

void ChickenInvader::game1()
{
	auto mode = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(mode, "chicken_invader", sf::Style::Fullscreen);
	float width = mode.width; 
	float height = mode.height;

	sf::RectangleShape shape;
	shape.setPosition(width / 2, height - 100);

	std::vector<Player*> players;
	players.emplace_back(new Aircraft(sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A
		, sf::Keyboard::D, sf::Keyboard::F, sf::Keyboard::G, sf::Color::Red, shape.getPosition(), AircraftType::sniper));
	players.emplace_back(new Aircraft(sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left
		, sf::Keyboard::Right, sf::Keyboard::Numpad1, sf::Keyboard::Numpad2, sf::Color::Magenta, shape.getPosition(), AircraftType::riffle));
	players.emplace_back(new Aircraft(sf::Keyboard::I, sf::Keyboard::K, sf::Keyboard::J, sf::Keyboard::L, sf::Keyboard::SemiColon
		, sf::Keyboard::Quote, sf::Color::Cyan, shape.getPosition(), AircraftType::rocket));

	int row = 1, col = 1;
	float initialSpace=250;// use for move the enemy beyond the left border
	int numOfWave = 5;
	std::vector<Enemy> enemys;
	std::vector<float> initialSpaceArr;

	std::vector<Bullet> bullets;
	std::vector<Splash> splashs;

	std::vector<Upgrading> upgradings;

	std::random_device rdNum;
	std::mt19937 rd(rdNum());

	sf::Clock clock;

	//window.setKeyRepeatEnabled(false);
	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();

		sf::Event ev;
		while (window.pollEvent(ev))
		{
			if (ev.key.code == sf::Keyboard::Escape)
				window.close();
			for (Player*& player : players)
			{
				player->checkReadyToFire(ev);
			}
		}

		spawnWave(enemys, dt, numOfWave, height, width, row, col, initialSpaceArr, initialSpace, rd);

		playerBehavior(players, dt, width, height, bullets);
		bulletBehavior(bullets,players, dt, width, rd);
		enemyBehavior(enemys, bullets, dt, rd,width);

		bulletTouchEnemy(bullets, enemys,splashs);
		bulletTouchPlayer(bullets, players);
		playerTouchEnemy(players, enemys);
		splashTouchEnemy(splashs, enemys);

		createUpgrading(upgradings, enemys, rd);
		upgradingBehavior(upgradings, dt);
		playerTouchUpgrading(players, upgradings);

		deleteDeadBullet(bullets);
		deleteDeadEnemy(enemys);
		deleteDeadPlayer(players);
		deleteDeadUpgrading(upgradings);
		deleteDeadSplash(splashs, dt);

		closeGame(players, window);

		window.clear(sf::Color(30,30,40));
		drawPlayer(players, window);
		drawPlayerHealthBar(players, window);
		drawPlayerCD(players, window);
		drawPlayerAmmo(players, window);
		drawEnemy(enemys, window);
		drawEnemyHealthBar(enemys, window);
		drawBullet(bullets, window);
		drawUpgrading(upgradings, window);
		drawSplash(splashs, window);
		drawSplash(splashs, window);
		window.display();
	}
} 