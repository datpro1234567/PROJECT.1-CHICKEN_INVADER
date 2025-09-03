#include "Game.h"

std::vector<Enemy> ChickenInvader::createArrayEnemy(float height, float width, int row, int col)
{

	float spaceX, spaceY;
	spaceX = width / (10 + 4);
	spaceY = height / (6 + 7);
	std::vector<Enemy> enemys;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			enemys.emplace_back(Enemy(sf::Vector2f(spaceX + spaceX * j, spaceY + spaceY * i)));
		}
	}
	return enemys;
}

void ChickenInvader::playerBehavior(std::vector<Player>& players, float dt, float width, float height, std::vector<Bullet>& bullets)
{
	for (Player& player : players)
	{
		player.move(dt);
		player.reachBorder(width, height);
		player.fireBullet(dt, bullets);
	}
}
void ChickenInvader::bulletBehavior(std::vector<Bullet>& bullets, float dt, float width)
{
	for (Bullet& bullet : bullets)
	{
		bullet.move(dt);
		bullet.reachBorder(width);
	}
}
void ChickenInvader::enemyBehavior(std::vector<Enemy>& enemys,std::vector<Bullet>& bullets, float dt, std::mt19937& gen,float width)
{
	for (Enemy& enemy : enemys)
	{
		enemy.fireBullet(bullets, dt, gen);
		enemy.move(dt, width, enemys);
	}
}

void ChickenInvader::drawPlayer(std::vector<Player>& players, sf::RenderWindow& window)
{
	for (Player& player : players)
	{
		window.draw(player.getShape());
	}
}
void ChickenInvader::drawEnemy(std::vector<Enemy>& enemys, sf::RenderWindow& window)
{
	for (Enemy& enemy : enemys)
	{
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

void ChickenInvader::bulletTouchEnemy(std::vector<Bullet>& bullets, std::vector<Enemy>& enemys)
{
	for (Bullet& bullet : bullets)
	{
		if (bullet.getAlive() && bullet.getFromPlayer())
		{
			for (Enemy& enemy : enemys)
			{
				if (enemy.getAlive())
				{
					if (bullet.getShape().getGlobalBounds().intersects(enemy.getShape().getGlobalBounds()))
					{
						enemy.setHealth(enemy.getHealth() - bullet.getDamage());
						if (enemy.getHealth() <= 0)
						{
							enemy.setAlive(false);
						}
						bullet.setAlive(false);
						break;
					}
				}
			}
		}
	}
}
void ChickenInvader::bulletTouchPlayer(std::vector<Bullet>& bullets, std::vector<Player>& players)
{
	for (Bullet& bullet : bullets)
	{
		if (!bullet.getFromPlayer() && bullet.getAlive())
		{
			for (Player& player : players)
			{
				if (player.getAlive())
				{
					if (bullet.getShape().getGlobalBounds().intersects(player.getShape().getGlobalBounds()))
					{
						player.setHealth(player.getHealth() - bullet.getDamage());
						if (player.getHealth() <= 0)
						{
							player.setAlive(false);
							std::cout << player.getAlive();
						}
						bullet.setAlive(false);
						break;	
					}
				}
			}
		}
	}
}
void ChickenInvader::playerTouchEnemy(std::vector<Player>& players, std::vector<Enemy>& enemys)
{
	for (Player& player : players)
	{
		if (player.getAlive())
			for (Enemy& enemy : enemys)
			{
				if (enemy.getAlive())
				{
					if (player.getShape().getGlobalBounds().intersects(enemy.getShape().getGlobalBounds()))
					{
						player.setAlive(false);
						enemy.setAlive(false);
					}
				}
			}
	}
}

void ChickenInvader::deleteDeadBullet(std::vector<Bullet>& bullets)
{
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& bullet) { return !bullet.getAlive(); }), bullets.end());
}
void ChickenInvader::deleteDeadEnemy(std::vector<Enemy>& enemys)
{
	enemys.erase(std::remove_if(enemys.begin(), enemys.end(), [](Enemy& enemy) { return !enemy.getAlive(); }), enemys.end());
}
void ChickenInvader::deleteDeadPlayer(std::vector<Player>& players)
{
	players.erase(std::remove_if(players.begin(), players.end(), [](Player& player) {return !player.getAlive(); }), players.end());
}

void ChickenInvader::closeGame(std::vector<Player>& players, sf::RenderWindow& window)
{
	bool anyAlive = false;
	for (Player& player : players)
	{
		if (player.getAlive())
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
	Player player1(sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A
		, sf::Keyboard::D, sf::Keyboard::F, sf::Keyboard::G, sf::Color::Red, shape.getPosition());
	Player player2(sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left
		, sf::Keyboard::Right, sf::Keyboard::Numpad1, sf::Keyboard::Numpad2, sf::Color::Magenta, shape.getPosition());
	std::vector<Player> players;
	players.push_back(player1);
	players.push_back(player2);

	int row = 6, col = 10;
	std::vector<Enemy> enemys = createArrayEnemy(height, width, row, col);

	std::vector<Bullet> bullets;

	std::random_device rd;
	std::mt19937 gen(rd());

	sf::Clock clock;

	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();

		sf::Event ev;
		while (window.pollEvent(ev))
		{
			if (ev.key.code == sf::Keyboard::Escape)
				window.close();
		}

		playerBehavior(players, dt, width, height, bullets);
		bulletBehavior(bullets, dt, width);
		enemyBehavior(enemys, bullets, dt, gen,width);

		bulletTouchEnemy(bullets, enemys);
		bulletTouchPlayer(bullets, players);
		playerTouchEnemy(players, enemys);

		deleteDeadBullet(bullets);
		deleteDeadEnemy(enemys);
		deleteDeadPlayer(players);

		closeGame(players, window);

		window.clear(sf::Color(30,30,40));
		drawPlayer(players, window);
		drawEnemy(enemys, window);
		drawBullet(bullets, window);
		window.display();
	}
}
